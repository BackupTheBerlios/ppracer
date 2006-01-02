/*
 * quadtree.cpp	-thatcher 9/15/1999 Copyright 1999-2000 Thatcher Ulrich
 *
 * Code for quadtree terrain manipulation, meshing, and display.
 *
 * This code may be freely modified and redistributed.  I make no
 * warrantees about it; use at your own risk.  If you do incorporate
 * this code into a project, I'd appreciate a mention in the credits.
 *
 * Thatcher Ulrich <tu@tulrich.com>
 *
 * Modified for use in Tux Racer by Jasmin Patry <jfpatry@cgl.uwaterloo.ca>
 *
 * Modifications for use in ppracer by Volker Stroebel <volker@planetpenguin.de>
 */

#include "course_load.h"
#include "fog.h"
#include "gl_util.h"

#include "course_render.h"

#include "ppogl/base/defs.h"
#include "ppogl/base/glwrappers.h"
#include "ppogl/base/glextensions.h"

#include "ppogl/sg/vnc.h"

#include "quadtree.h"
#include "gameconfig.h"

/** Amount to scale terrain errors by in order to be comparable to
   height errors */
#define TERRAIN_ERROR_SCALE 0.15

/** Distance at which we force the activation of vertices that lie on
   texture edges */
#define VERTEX_FORCE_THRESHOLD 110

/** Maximum Distance at which we magnify the error term (to minimize
   popping near the camera */
#define ERROR_MAGNIFICATION_THRESHOLD 25

/** Amount to magnify errors by within ERROR_MAGNIFICATION_THRESHOLD */
#define ERROR_MAGNIFICATION_AMOUNT 3

/** Environment map alpha value, integer from 0-255 */ 
#define ENV_MAP_ALPHA 50

extern TerrainTex terrain_texture[NUM_TERRAIN_TYPES];
extern unsigned int num_terrains;

int terrain_count[NUM_TERRAIN_TYPES];

//
// quadsquare functions.
//

GLuint *quadsquare::VertexArrayIndices[NUM_TERRAIN_TYPES]; 
GLuint quadsquare::VertexArrayCounter[NUM_TERRAIN_TYPES];
GLuint quadsquare::VertexArrayMinIdx[NUM_TERRAIN_TYPES];
GLuint quadsquare::VertexArrayMaxIdx[NUM_TERRAIN_TYPES];

quadsquare::quadsquare(quadcornerdata* pcd)
/// Constructor.
 : EnabledFlags(0),
   Static(false),
   Dirty(false),
   ForceEastVert(false),
   ForceSouthVert(false)
{
    pcd->square = this;

	Child[0] = NULL;
	Child[1] = NULL;
	Child[2] = NULL;	
	Child[3] = NULL;

	SubEnabledCount[0] = 0;
	SubEnabledCount[1] = 0;
	
    // Set default vertex positions by interpolating from given corners.
    // Just bilinear interpolation.
    Vertex[0] = 0.25 * (pcd->verts[0] + pcd->verts[1] + pcd->verts[2] + pcd->verts[3]);
    Vertex[1] = 0.5 * (pcd->verts[3] + pcd->verts[0]);
    Vertex[2] = 0.5 * (pcd->verts[0] + pcd->verts[1]);
    Vertex[3] = 0.5 * (pcd->verts[1] + pcd->verts[2]);
    Vertex[4] = 0.5 * (pcd->verts[2] + pcd->verts[3]);

    for(int i=0; i<2; i++){
		Error[i] = 0;
    }
    for(int i=0; i<4; i++){
		Error[i+2] = fabs((Vertex[0] + pcd->verts[i]) - (Vertex[i+1] + Vertex[((i+1)&3) + 1])) * 0.25;
    }

    // Compute MinY/MaxY based on corner verts.
    MinY = MaxY = pcd->verts[0];
    for(int i=1; i<4; i++){
		float y = pcd->verts[i];
		if(y<MinY) MinY=y;
		if(y>MaxY)MaxY=y;
    }

    if(pcd->parent==NULL){
		PP_LOG( DEBUG_QUADTREE, "initializing root node" );
		for(int i=0; i< NUM_TERRAIN_TYPES; i++){
			VertexArrayIndices[0] = NULL;		
		}
		Terrain = Course::getTerrainData();
    }
}

quadsquare::~quadsquare()
/// Destructor.
{
    // Recursively delete sub-trees.
    for(int i=0; i<4; i++){
		if(Child[i]!=NULL){
			delete Child[i];
		}
    }
}

void
quadsquare::setStatic(const quadcornerdata& cd)
/// Sets this node's static flag to true.  If static == true, then the
/// node or its children is considered to contain significant height data
/// and shouldn't be deleted.
{
    if(Static==false){
		Static = true;
		
		// Propagate static status to ancestor nodes.
		if(cd.parent && cd.parent->square){
		    cd.parent->square->setStatic(*cd.parent);
		}
    }
}

quadsquare*
quadsquare::getNeighbor(const int dir, const quadcornerdata& cd) const
/// Traverses the tree in search of the quadsquare neighboring this square to the
/// specified direction.  0-3 --> { E, N, W, S }.
/// Returns NULL if the neighbor is outside the bounds of the tree.
{
    // If we don't have a parent, then we don't have a neighbor.
    // (Actually, we could have inter-tree connectivity at this level
    // for connecting separate trees together.)
    if (cd.parent == 0) return 0;
	
    // Find the parent and the child-index of the square we want to locate or create.
    quadsquare*	p = 0;
	
    int	index = cd.childIndex ^ 1 ^ ((dir & 1) << 1);
    bool	SameParent = ((dir - cd.childIndex) & 2) ? true : false;
	
    if (SameParent) {
		p = cd.parent->square;
    } else {
		p = cd.parent->square->getNeighbor(dir, *cd.parent);
		
		if (p == 0) return 0;
    }
	
    quadsquare*	n = p->Child[index];
	
    return n;
}

float
quadsquare::recomputeError(const quadcornerdata& cd)
/// Recomputes the error values for this tree.  Returns the
/// max error.
/// Also updates MinY & MaxY.
{
    unsigned int t;
    int	half = 1 << cd.level;
    int	whole = half << 1;
    float terrain_error;
	
    // Measure error of center and edge vertices.
    float maxerror = 0;

    // Compute error of center vert.
    float	e;
    if (cd.childIndex & 1) {
		e = fabs(Vertex[0] - (cd.verts[1] + cd.verts[3]) * 0.5);
    } else {
		e = fabs(Vertex[0] - (cd.verts[0] + cd.verts[2]) * 0.5);
    }
    if (e > maxerror) maxerror = e;

    // Initial min/max.
    MaxY = Vertex[0];
    MinY = Vertex[0];

    // Check min/max of corners.
    for(int i=0; i<4; i++){
		float y = cd.verts[i];
		if (y < MinY) MinY = y;
		if (y > MaxY) MaxY = y;
    }
	
    // Edge verts.
    e = fabs(Vertex[1] - (cd.verts[0] + cd.verts[3]) * 0.5);
    if (e > maxerror) maxerror = e;
    Error[0] = e;
	
    e = fabs(Vertex[4] - (cd.verts[2] + cd.verts[3]) * 0.5);
    if (e > maxerror) maxerror = e;
    Error[1] = e;

    // Terrain edge checks
    if ( cd.level == 0 && cd.xorg <= RowSize-1 && cd.zorg <= NumRows-1 ) {

		// Check South vertex
		int x = cd.xorg + half;
		int z = cd.zorg + whole;
		int idx = x  + z * RowSize;
		bool different_terrains = false;
	
		terrain_error = 0.f;

		PP_ASSERT( x >= 0, "x coordinate is negative" );
		PP_ASSERT( z >= 0, "z coordinate is negative" );

		if ( x < RowSize && z < NumRows ) {
			if ( x < RowSize - 1 ) {
				if ( Terrain[idx] != Terrain[idx+1] ) {
					different_terrains = true;
				}
			}
			if ( z >= 1 ) {
				idx -= RowSize;
				if ( Terrain[idx] != Terrain[idx+1] ) {
					different_terrains = true;
				}
			}

			if ( different_terrains ) {
				ForceSouthVert = true;
				terrain_error = TERRAIN_ERROR_SCALE * whole * whole;
			} else {
				ForceSouthVert = false;
			}

			if ( terrain_error > Error[0] ) {
				Error[0] = terrain_error;
			}
			if ( Error[0] > maxerror ) {
				maxerror = Error[0];
			}
		}
	
		// Check East vertex
		x = cd.xorg + whole;
		z = cd.zorg + half;
		idx = x  + z * RowSize;
		terrain_error = 0;
		different_terrains = false;

		if ( x < RowSize && z < NumRows ) {	
			if ( Terrain[idx] != Terrain[idx-RowSize] ) {
			    different_terrains = true;
			}
		}
		if ( z >= 1 && x < RowSize - 1 ) {
			idx += 1;
			if ( Terrain[idx] != Terrain[idx-RowSize] ) {
					different_terrains = true;
			}
		}

		if ( different_terrains ) {	
			ForceEastVert = true;
			terrain_error = TERRAIN_ERROR_SCALE * whole * whole;
		} else {
			ForceEastVert = false;
		}

		if ( terrain_error > Error[1] ) {
			Error[1] = terrain_error;
		}
		if ( Error[1] > maxerror ) {
			maxerror = Error[1];
		}
	}
    
    // Min/max of edge verts.
    for(int i=0; i<4; i++){
		float y = Vertex[1 + i];
		if (y < MinY) MinY = y;
		if (y > MaxY) MaxY = y;
    }
	
    // Check child squares.
    for (int i=0; i<4; i++){
		quadcornerdata	q;
		if (Child[i]) {
		    setupCornerData(&q, cd, i);
	 	   Error[i+2] = Child[i]->recomputeError(q);

	 	   if (Child[i]->MinY < MinY) MinY = Child[i]->MinY;
	 	   if (Child[i]->MaxY > MaxY) MaxY = Child[i]->MaxY;
		} else {
		    // Compute difference between bilinear average at child center, and diagonal edge approximation.
		    Error[i+2] = fabs((Vertex[0] + cd.verts[i]) - (Vertex[i+1] + Vertex[((i+1)&3) + 1])) * 0.25;
		}
		if (Error[i+2] > maxerror) maxerror = Error[i+2];
    }

    //
    // Compute terrain_error
    //
    int terrain;

    for(t=0; t<num_terrains; t++){
		terrain_count[t] = 0;
    }
	
	
	int i_min = (cd.xorg<0) ? 0 : cd.xorg;
	
	int i_max = ( (cd.xorg+whole) >= RowSize) ? (RowSize-1) : cd.xorg+whole;
	
	int j_min = (cd.zorg<0) ? 0 : cd.zorg;
	
	int j_max = ( (cd.zorg+whole) >= NumRows) ? (NumRows-1) : cd.zorg+whole;

	for(int i=i_min; i<=i_max; i++){
	for(int j=j_min; j<=j_max; j++){

	    terrain = int(Terrain[ i + RowSize*j ]);
	    PP_ASSERT( terrain >= 0 && 
			     terrain < int(num_terrains),
			     "Invalid terrain type" );
	    terrain_count[ terrain ] += 1;
	}
    }

    int max_count = 0;
    int max_type = 0;
    int total = 0;
    for (t=0; t<num_terrains; t++) {
	if ( terrain_count[t] > max_count ) {
	    max_count = terrain_count[t];
	    max_type = t;
	}
	total += terrain_count[t];
    }

    // Calculate a terrain error that varies between 0 and 1
    if ( total > 0 ) {
	terrain_error = (1.0 - max_count / total);  
	if ( num_terrains > 1 ) {
	    terrain_error *= num_terrains / ( num_terrains - 1.0 );
	}
    } else {
	terrain_error = 0;
    }

    /* and scale it by the square area */
    terrain_error *= whole * whole;

    /* and finally scale it so that it's comparable to height error */
    terrain_error *= TERRAIN_ERROR_SCALE;

    if ( terrain_error > maxerror ) {
	maxerror = terrain_error;
    }

    if ( terrain_error > Error[0] ) {
	Error[0] = terrain_error;
    }
    if ( terrain_error > Error[1] ) {
	Error[1] = terrain_error;
    }


    // The error  and MinY/MaxY values for this node and descendants are correct now.
    Dirty = false;
	
    return maxerror;
}

void
quadsquare::resetTree()
/// Clear all enabled flags, and delete all non-static child nodes.
{
	for(int i=0; i<4; i++){
		if(Child[i]) {
			Child[i]->resetTree();
			if(Child[i]->Static == false){
				delete Child[i];
				Child[i] = NULL;
	    	}
		}
    }
    EnabledFlags = 0;
    SubEnabledCount[0] = 0;
    SubEnabledCount[1] = 0;
    Dirty = true;
}


void
quadsquare::staticCullData(const quadcornerdata& cd, const float ThresholdDetail)
/// Examine the tree and remove nodes which don't contain necessary
/// detail.  Necessary detail is defined as vertex data with a
/// edge-length to height ratio less than ThresholdDetail.
{
    // First, clean non-static nodes out of the tree.
    resetTree();

    // Make sure error values are up-to-date.
    if(Dirty) recomputeError(cd);
	
    // Recursively check all the nodes and do necessary removal.
    // We must start at the bottom of the tree, and do one level of
    // the tree at a time, to ensure the dependencies are accounted
    // for properly.
    for(int level=0; level<=cd.level; level++){
		staticCullAux(cd, ThresholdDetail, level);
    }
}

void
quadsquare::staticCullAux(const quadcornerdata& cd, const float ThresholdDetail, const int TargetLevel)
/// Check this node and its descendents, and remove nodes which don't contain
/// necessary detail.
{
    int	i;
    quadcornerdata	q;

    if(cd.level>TargetLevel){
		// Just recurse to child nodes.
		for(int j= 0; j<4; j++){
			if (j < 2) i = 1 - j;
			else i = j;

			if (Child[i]) {
				setupCornerData(&q, cd, i);
				Child[i]->staticCullAux(q, ThresholdDetail, TargetLevel);
			}
		}
		return;
    }

    // We're at the target level.  Check this node to see if it's OK to delete it.

    // Check edge vertices to see if they're necessary.
    float size = 2 << cd.level;	// Edge length.
    if (Child[0] == NULL && Child[3] == NULL && Error[0] * ThresholdDetail < size) {
		quadsquare*	s = getNeighbor(0, cd);
		if (s == NULL || (s->Child[1] == NULL && s->Child[2] == NULL)){

			// Force vertex height to the edge value.
			float	y = (cd.verts[0] + cd.verts[3]) * 0.5;
			Vertex[1] = y;
			Error[0] = 0;
			
			// Force alias vertex to match.
			if (s) s->Vertex[3] = y;
			
			Dirty = true;
		}
    }

    if (Child[2] == NULL && Child[3] == NULL && Error[1] * ThresholdDetail < size) {
	quadsquare*	s = getNeighbor(3, cd);
	if (s == NULL || (s->Child[0] == NULL && s->Child[1] == NULL)) {
	    float	y = (cd.verts[2] + cd.verts[3]) * 0.5;
	    Vertex[4] = y;
	    Error[1] = 0;
			
	    if (s) s->Vertex[2] = y;
			
	    Dirty = true;
	}
    }

    // See if we have child nodes.
    bool	StaticChildren = false;
    for (i = 0; i < 4; i++) {
	if (Child[i]) {
	    StaticChildren = true;
	    if (Child[i]->Dirty) Dirty = true;
	}
    }

    // If we have no children and no necessary edges, then see if we can delete ourself.
    if (StaticChildren == false && cd.parent != NULL) {
	bool	NecessaryEdges = false;
	for (i = 0; i < 4; i++) {
	    // See if vertex deviates from edge between corners.
	    float	diff = fabs(Vertex[i+1] - (cd.verts[i] + cd.verts[(i+3)&3]) * 0.5);
	    if (diff > 0.00001) {
		NecessaryEdges = true;
	    }
	}

	if (!NecessaryEdges) {
	    size *= 1.414213562;	// sqrt(2), because diagonal is longer than side.
	    if (cd.parent->square->Error[2 + cd.childIndex] * ThresholdDetail < size) {
		delete cd.parent->square->Child[cd.childIndex];	// Delete this.			
		cd.parent->square->Child[cd.childIndex] = NULL;	// Clear the pointer.
	    }
	}
    }
}

//static int MaxCreateDepth = 0;

void
quadsquare::enableEdgeVertex(int index, const bool IncrementCount, const quadcornerdata& cd)
/// Enable the specified edge vertex.  Indices go { e, n, w, s }.
/// Increments the appropriate reference-count if IncrementCount is true.
{
    if ((EnabledFlags & (1 << index)) && IncrementCount == false) return;
	
    // Turn on flag and deal with reference count.
    EnabledFlags |= 1 << index;
    if (IncrementCount == true && (index == 0 || index == 3)) {
		SubEnabledCount[index & 1]++;
    }

    // Now we need to enable the opposite edge vertex of the adjacent square (i.e. the alias vertex).

    // This is a little tricky, since the desired neighbor node may not exist, in which
    // case we have to create it, in order to prevent cracks.  Creating it may in turn cause
    // further edge vertices to be enabled, propagating updates through the tree.

    // The sticking point is the quadcornerdata list, which
    // conceptually is just a linked list of activation structures.
    // In this function, however, we will introduce branching into
    // the "list", making it in actuality a tree.  This is all kind
    // of obscure and hard to explain in words, but basically what
    // it means is that our implementation has to be properly
    // recursive.

    // Travel upwards through the tree, looking for the parent in common with our desired neighbor.
    // Remember the path through the tree, so we can travel down the complementary path to get to the neighbor.
    quadsquare*	p = this;
    const quadcornerdata* pcd = &cd;
    int	ct = 0;
    int	stack[32];
    for (;;) {
		int	ci = pcd->childIndex;

		if (pcd->parent == NULL || pcd->parent->square == NULL) {
		    // Neighbor doesn't exist (it's outside the tree), so there's no alias vertex to enable.
		    return;
		}
		p = pcd->parent->square;
		pcd = pcd->parent;

		bool SameParent = ((index - ci) & 2) ? true : false;
		
		ci = ci ^ 1 ^ ((index & 1) << 1);	// Child index of neighbor node.

		stack[ct] = ci;
		ct++;
		
		if (SameParent) break;
    }

    // Get a pointer to our neighbor (create if necessary), by walking down
    // the quadtree from our shared ancestor.
    p = p->enableDescendant(ct, stack, *pcd);
	
	// Finally: enable the vertex on the opposite edge of our neighbor, the alias of the original vertex.
    index ^= 2;
    p->EnabledFlags |= (1 << index);
    if (IncrementCount == true && (index == 0 || index == 3)) {
		p->SubEnabledCount[index & 1]++;
    }
}


quadsquare*
quadsquare::enableDescendant(int count, int path[], const quadcornerdata& cd)
/// This function enables the descendant node 'count' generations below
/// us, located by following the list of child indices in path[].
/// Creates the node if necessary, and returns a pointer to it.
{
    count--;
    int	ChildIndex = path[count];

    if ((EnabledFlags & (16 << ChildIndex)) == 0) {
		enableChild(ChildIndex, cd);
    }
	
    if (count > 0) {
		quadcornerdata q;
		setupCornerData(&q, cd, ChildIndex);
		return Child[ChildIndex]->enableDescendant(count, path, q);
    } else {
		return Child[ChildIndex];
    }
}

void
quadsquare::createChild(int index, const quadcornerdata& cd)
/// Creates a child square at the specified index.
{
    if (Child[index] == 0) {
		quadcornerdata	q;
		setupCornerData(&q, cd, index);
		Child[index] = new quadsquare(&q);
    }
}

void
quadsquare::enableChild(int index, const quadcornerdata& cd)
/// Enable the indexed child node.  { ne, nw, sw, se }
/// Causes dependent edge vertices to be enabled.
{
    if ((EnabledFlags & (16 << index)) == 0) {
		EnabledFlags |= (16 << index);
		enableEdgeVertex(index, true, cd);
		enableEdgeVertex((index + 1) & 3, true, cd);
		if (Child[index] == 0) {
		    createChild(index, cd);
		}
    }
}

static int BlockUpdateCount = 0;

void
quadsquare::notifyChildDisable(const quadcornerdata& cd, int index)
/// Marks the indexed child quadrant as disabled.  Deletes the child node
/// if it isn't static.
{
    // Clear enabled flag for the child.
    EnabledFlags &= ~(16 << index);
	
	// Update child enabled counts for the affected edge verts.
    quadsquare*	s;
	
    if (index & 2) s = this;
    else s = getNeighbor(1, cd);
    if (s) {
		s->SubEnabledCount[1]--;
    }
	
    if (index == 1 || index == 2) s = getNeighbor(2, cd);
    else s = this;
    if (s) {
		s->SubEnabledCount[0]--;
    }
	
    /* We don't really want to delete nodes when they're disabled, do we?     
	if (Child[index]->Static == false) {
       delete Child[index];
       Child[index] = 0;

       BlockDeleteCount++;//xxxxx
       }
     */
}

bool
quadsquare::vertexTest(int x, float y, int z, float error, const float Viewer[3], VertexLoc vertex_loc )
/// Returns true if the vertex at (x,z) with the given world-space error between
/// its interpolated location and its true location, should be enabled, given that
/// the viewpoint is located at Viewer[].
{
    float	dx = fabs(x - Viewer[0]) * fabs( ScaleX );
    float	dy = fabs(y - Viewer[1]);
    float	dz = fabs(z - Viewer[2]) * fabs( ScaleZ );
    float	d = MAX( dx, MAX( dy, dz ) );


    if ( vertex_loc == South && ForceSouthVert && d < VERTEX_FORCE_THRESHOLD ) 
    {
		return true;
    }
    if ( vertex_loc == East && ForceEastVert && d < VERTEX_FORCE_THRESHOLD ) 
    {
		return true;
    }
    if ( d < ERROR_MAGNIFICATION_THRESHOLD ) {
		error *= ERROR_MAGNIFICATION_AMOUNT;
    }
	
    return (error * GameConfig::courseDetails) > d;
}


bool
quadsquare::boxTest(int x, int z, float size, float miny, float maxy, float error, const float Viewer[3])
/// Returns true if any vertex within the specified box (origin at x,z,
/// edges of length size) with the given error value could be enabled
/// based on the given viewer location.
{
    // Find the minimum distance to the box.
    float	half = size * 0.5;
    float	dx = ( fabs(x + half - Viewer[0]) - half ) * fabs(ScaleX);
    float	dy = fabs((miny + maxy) * 0.5 - Viewer[1]) - (maxy - miny) * 0.5;
    float	dz = ( fabs(z + half - Viewer[2]) - half ) * fabs(ScaleZ);
    float	d = MAX( dx, MAX( dy , dz ) );

    if ( d < ERROR_MAGNIFICATION_THRESHOLD ) {
		error *= ERROR_MAGNIFICATION_AMOUNT;
    }

    if ( (error * GameConfig::courseDetails) > d ) {
		return true;
    }

    // Check to see if this box crosses the heightmap boundary
    if ( ( x < RowSize-1 && x+size >= RowSize ) ||
	 ( z < NumRows-1 && z+size >= NumRows ) ) 
    {
		return true;
    }

    return false;
}

void
quadsquare::update(const quadcornerdata& cd, const ppogl::Vec3d& viewerLocation)
/// Refresh the vertex enabled states in the tree, according to the
/// location of the viewer.  May force creation or deletion of qsquares
/// in areas which need to be interpolated.
{

    float Viewer[3];

    Viewer[0] = viewerLocation[0] / ScaleX;
    Viewer[1] = viewerLocation[1];
    Viewer[2] = viewerLocation[2] / ScaleZ;
	
    updateAux(cd, Viewer, 0, ViewFrustum::SomeClip);
}


void
quadsquare::updateAux(const quadcornerdata& cd, const float ViewerLocation[3], const float CenterError, ViewFrustum::ClipResult vis )
/// Does the actual work of updating enabled states and tree growing/shrinking.
{    
	PP_REQUIRE( vis != ViewFrustum::NotVisible, "Invalid visibility value" );
    
	BlockUpdateCount++;	//xxxxx

    if ( vis != ViewFrustum::NoClip ) {
		vis = clipSquare( cd );
		if ( vis == ViewFrustum::NotVisible ) {
	    	return;
		}
    }
	
    // Make sure error values are current.
    if (Dirty) {
		recomputeError(cd);
    }

    const int half = 1 << cd.level;
    const int whole = half << 1;

    // See about enabling child verts.

    // East vert.
    if ( (EnabledFlags & 1) == 0 && 
	 vertexTest(cd.xorg + whole, Vertex[1], cd.zorg + half, 
		    Error[0], ViewerLocation, East) == true ) 
    {
		enableEdgeVertex(0, false, cd);	
    }

    // South vert.
    if ( (EnabledFlags & 8) == 0 && 
	 vertexTest(cd.xorg + half, Vertex[4], cd.zorg + whole, 
		    Error[1], ViewerLocation, South) == true ) 
    {
		enableEdgeVertex(3, false, cd);	
    }

    if (cd.level > 0) {
	if ((EnabledFlags & 32) == 0) {
	    if (boxTest(cd.xorg, cd.zorg, half, MinY, MaxY, Error[3], ViewerLocation) == true) enableChild(1, cd);	// nw child.er
	}
	if ((EnabledFlags & 16) == 0) {
	    if (boxTest(cd.xorg + half, cd.zorg, half, MinY, MaxY, Error[2], ViewerLocation) == true) enableChild(0, cd);	// ne child.
	}
	if ((EnabledFlags & 64) == 0) {
	    if (boxTest(cd.xorg, cd.zorg + half, half, MinY, MaxY, Error[4], ViewerLocation) == true) enableChild(2, cd);	// sw child.
	}
	if ((EnabledFlags & 128) == 0) {
	    if (boxTest(cd.xorg + half, cd.zorg + half, half, MinY, MaxY, Error[5], ViewerLocation) == true) enableChild(3, cd);	// se child.
	}
		
	// Recurse into child quadrants as necessary.
	quadcornerdata	q;
		
	if (EnabledFlags & 32) {
	    setupCornerData(&q, cd, 1);
	    Child[1]->updateAux(q, ViewerLocation, Error[3], vis);
	}
	if (EnabledFlags & 16) {
	    setupCornerData(&q, cd, 0);
	    Child[0]->updateAux(q, ViewerLocation, Error[2], vis);
	}
	if (EnabledFlags & 64) {
	    setupCornerData(&q, cd, 2);
	    Child[2]->updateAux(q, ViewerLocation, Error[4], vis);
	}
	if (EnabledFlags & 128) {
	    setupCornerData(&q, cd, 3);
	    Child[3]->updateAux(q, ViewerLocation, Error[5], vis);
	}
    }
	
    // Test for disabling.  East, South, and center.
    if ( (EnabledFlags & 1) && 
	 SubEnabledCount[0] == 0 && 
	 vertexTest(cd.xorg + whole, Vertex[1], cd.zorg + half, 
		    Error[0], ViewerLocation, East) == false) 
    {
		EnabledFlags &= ~1;
		quadsquare*	s = getNeighbor(0, cd);
		if (s) s->EnabledFlags &= ~4;
    }

    if ( (EnabledFlags & 8) && 
	 SubEnabledCount[1] == 0 && 
	 vertexTest(cd.xorg + half, Vertex[4], cd.zorg + whole, 
		    Error[1], ViewerLocation, South) == false) 
    {
		EnabledFlags &= ~8;
		quadsquare*	s = getNeighbor(3, cd);
		if (s) s->EnabledFlags &= ~2;
    }

    if (EnabledFlags == 0 &&
	cd.parent != NULL &&
	boxTest(cd.xorg, cd.zorg, whole, MinY, MaxY, CenterError, 
		ViewerLocation) == false)
    {
	// Disable ourself.
		cd.parent->square->notifyChildDisable(*cd.parent, cd.childIndex);	// nb: possibly deletes 'this'.
    }
}

static GLuint VertexIndices[9];
static int VertexTerrains[9];

inline int
quadsquare::initVert(const int i, const int x,const int z)
/// Initializes the indexed vertex of VertexArray[] with the
/// given values.
{
    const int idx = (x >= RowSize ? (RowSize - 1) : x) + RowSize * (  z >= NumRows ? (NumRows - 1)  : z);

    VertexIndices[i] = idx;

	return (VertexTerrains[i] = Terrain[idx]);
}

static ppogl::VNCArray* s_VNCArray;

void
quadsquare::drawTris(int terrain)
{
	if(gl::EXTcompiledVertexArray() && GameConfig::useCVA){
		const int tmp_min_idx = VertexArrayMinIdx[terrain];
		gl::LockArraysEXT(tmp_min_idx, 
			 VertexArrayMaxIdx[terrain] - tmp_min_idx + 1); 
   	}

   	gl::DrawElements(GL_TRIANGLES, VertexArrayCounter[terrain],
	    	GL_UNSIGNED_INT, VertexArrayIndices[terrain]);

   	if(gl::EXTcompiledVertexArray() && GameConfig::useCVA){
		gl::UnlockArraysEXT();
   	}
}

void
quadsquare::drawEnvmapTris(GLuint MapTexId, int terrain) 
{
    if ( VertexArrayCounter[terrain] > 0 ) {
	
		gl::TexGen(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		gl::TexGen(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

		gl::BindTexture(GL_TEXTURE_2D, MapTexId);
		drawTris(terrain);

		gl::TexGen(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		gl::TexGen(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	} 
}

void
quadsquare::initArrayCounters()
{
    for (unsigned int i=0; i<num_terrains ; i++){
		VertexArrayCounter[i] = 0;
		VertexArrayMinIdx[i] = INT_MAX;
		VertexArrayMaxIdx[i] = 0;
	}
}

void
quadsquare::render(const quadcornerdata& cd, ppogl::VNCArray* vnc_array)
/// Draws the heightfield represented by this tree.
{
    s_VNCArray = vnc_array;
    const bool fog_on=fogPlane.isEnabled();
    int i,idx;

    //Draw the "normal" blended triangles

	initArrayCounters();
		
	renderAux(cd, ViewFrustum::SomeClip);
		
	std::list<int>::iterator it;
	for (it=usedTerrains.begin(); it!=usedTerrains.end(); it++) {
		
		if ( VertexArrayCounter[(*it)] == 0 ) {
			continue;
		} 

		for (i=0; i<int(VertexArrayCounter[(*it)]);i++){
			idx = VertexArrayIndices[(*it)][i];
			s_VNCArray->colorValue(idx, 3) =  ( terrain_texture[(*it)].wheight<= terrain_texture[Terrain[idx]].wheight ) ? 255 : 0;
		}

		gl::BindTexture(GL_TEXTURE_2D, terrain_texture[(*it)].texture);
		drawTris((*it));

		if ( terrain_texture[(*it)].envmap  && GameConfig::useTerrainEnvmap) {
		    /* Render Ice with environment map */
		    gl::DisableClientState(GL_COLOR_ARRAY);
		    gl::Color(1.0f, 1.0f, 1.0f, ENV_MAP_ALPHA / 255.0f );

		    drawEnvmapTris(terrain_texture[(*it)].envmap->getID(), (*it));	

		    gl::EnableClientState(GL_COLOR_ARRAY);
		}
    }

    /*
     * Draw the "special" triangles that have different terrain types
     * at each of the corners 
     */

	/*
	 * Get the "special" three-terrain triangles
	 */
	initArrayCounters();
	renderAuxSpezial(cd, ViewFrustum::SomeClip);
	
	if ( VertexArrayCounter[0] != 0 ) {
	    /* Render black triangles */
	    gl::Disable( GL_FOG );
	    
	    /* Set triangle vertices to black */
	    for (i=0; i<int(VertexArrayCounter[0]); i++) {
			s_VNCArray->colorValue(VertexArrayIndices[0][i], 0) = 0;
			s_VNCArray->colorValue(VertexArrayIndices[0][i], 1) = 0;
			s_VNCArray->colorValue(VertexArrayIndices[0][i], 2) = 0;
			s_VNCArray->colorValue(VertexArrayIndices[0][i], 3) = 255;
	    }
	    
	    /* Draw the black triangles */
	    gl::BindTexture(GL_TEXTURE_2D, terrain_texture[0].texture);
		drawTris(0);
	    
	    /* Now we draw the triangle once for each texture */
	    if (fog_on) {
			gl::Enable(GL_FOG);
	    }

	    /* Use additive blend function */
	    gl::BlendFunc(GL_SRC_ALPHA, GL_ONE);

	    /* First set triangle colors to white */
	    for (i=0; i<int(VertexArrayCounter[0]); i++) {
			s_VNCArray->colorValue(VertexArrayIndices[0][i], 0 )=255;
			s_VNCArray->colorValue(VertexArrayIndices[0][i], 1 )=255;
			s_VNCArray->colorValue(VertexArrayIndices[0][i], 2 )=255;
	    }

		//for (int j=0; j<(int)num_terrains; j++) {
		for(it=usedTerrains.begin(); it != usedTerrains.end(); it++){
			
			gl::BindTexture(GL_TEXTURE_2D, terrain_texture[(*it)].texture);

			/* Set alpha values */
			for (i=0; i<int(VertexArrayCounter[0]); i++) {
				s_VNCArray->colorValue(VertexArrayIndices[0][i], 3) = 
				(Terrain[VertexArrayIndices[0][i]] == int(*it) ) ? 
				255 : 0;
			}

			drawTris(0);
	    }


	    /* Render Ice with environment map */
	    if(GameConfig::useTerrainEnvmap){
		gl::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    
		/* Need to set alpha values for ice */
		
		//the ice stuff should be rewritten...
		for (i=0; i<int(VertexArrayCounter[0]); i++) {
		    s_VNCArray->colorValue(VertexArrayIndices[0][i], 3) = 
			(Terrain[VertexArrayIndices[0][i]] == 0) ? 
			ENV_MAP_ALPHA : 0;
		}			
			
		drawEnvmapTris(GameConfig::useTerrainEnvmap,0);
	    }
	}

    gl::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

ViewFrustum::ClipResult
quadsquare::clipSquare( const quadcornerdata& cd )
{
    if ( cd.xorg >= RowSize-1 ) {
		return ViewFrustum::NotVisible;
    } 

    if ( cd.zorg >= NumRows-1 ) {
		return ViewFrustum::NotVisible;
    }
	
    const int whole = 2 << cd.level;
	
	const ppogl::Vec3d Min(cd.xorg*ScaleX, MinY, (cd.zorg + whole) * ScaleZ);
    const ppogl::Vec3d Max((cd.xorg + whole) * ScaleX, MaxY,cd.zorg*ScaleZ);

    const ViewFrustum::ClipResult clip_result = viewFrustum.clip(Min, Max);

    if ( clip_result == ViewFrustum::NotVisible || clip_result == ViewFrustum::SomeClip ) {
		return clip_result;
    }

    if ( cd.xorg + whole >= RowSize ) {
		return ViewFrustum::SomeClip;
    }
    if ( cd.zorg + whole >= NumRows ) {
		return ViewFrustum::SomeClip;
    }

    return clip_result;
}


#define update_min_max( idx,terrain ) \
    if ( idx > VertexArrayMaxIdx[terrain] ) { \
        VertexArrayMaxIdx[terrain] = idx; \
    } \
    if ( idx < VertexArrayMinIdx[terrain] ) { \
        VertexArrayMinIdx[terrain] = idx; \
    }
	
inline void
quadsquare::makeTri(int a, int b, int c, int terrain)
{
    if((VertexTerrains[a] == terrain || 
	    VertexTerrains[b] == terrain || 
	    VertexTerrains[c] == terrain ))
    { 
	VertexArrayIndices[terrain][VertexArrayCounter[terrain]++] = VertexIndices[a]; 
	update_min_max( VertexIndices[a], terrain );
	VertexArrayIndices[terrain][VertexArrayCounter[terrain]++] = VertexIndices[b]; 
	update_min_max( VertexIndices[b], terrain );
	VertexArrayIndices[terrain][VertexArrayCounter[terrain]++] = VertexIndices[c]; 
	update_min_max( VertexIndices[c], terrain );
    }
}


inline void
quadsquare::makeSpecialTri( int a, int b, int c) 
{
    if ( VertexTerrains[a] != VertexTerrains[b] && 
	 VertexTerrains[a] != VertexTerrains[c] && 
	 VertexTerrains[b] != VertexTerrains[c] ) 
    { 
	VertexArrayIndices[0][VertexArrayCounter[0]++] = VertexIndices[a]; 
	update_min_max( VertexIndices[a],0 );
	VertexArrayIndices[0][VertexArrayCounter[0]++] = VertexIndices[b]; 
	update_min_max( VertexIndices[b],0 );
	VertexArrayIndices[0][VertexArrayCounter[0]++] = VertexIndices[c]; 
	update_min_max( VertexIndices[c],0 );
    }
}

static bool terraintest[NUM_TERRAIN_TYPES];

void
quadsquare::renderAux(const quadcornerdata& cd, ViewFrustum::ClipResult vis)
/// Does the work of rendering this square.  Uses the enabled vertices only.
/// Recurses as necessary.
{
    int	flags = 0;
    int	mask = 1;
    quadcornerdata q;
	
    for (int i = 0; i < 4; i++, mask <<= 1) {
		if (EnabledFlags & (16 << i)) {
			setupCornerData(&q, cd, i);
			if (vis != ViewFrustum::NoClip) {
				if ((vis = clipSquare( q ))!= ViewFrustum::NotVisible ) {
		 		   Child[i]->renderAux(q, vis);
				}
   		 	}else{		
		   		Child[i]->renderAux(q, vis);
			}
		} else {
		    flags |= mask;
		}
    }
    if (flags == 0) return;

	
    // Init vertex data.  Here's a diagram of what's going on.
    // Yes, this diagram is fucked, but that's because things are mirrored 
    // in the z axis for us (z coords are actually -z coords).
    //
    // (top of course) 
    //        N                
    //    +---+---+ (xorg, zorg)
    //   2|  3   4|        
    //    |(1) (2)|
    // E  +   +   +  W
    //   1|  0   5|
    //    |(8) (4)|
    //    +---+---+
    //   8   7   6
    //        S
    // (bottom of course)
    //
    // Values in parens are bitmask values for the corresponding child squares
    //

	{
		//bool terraintest[num_terrains];
		for (unsigned int i=0; i<num_terrains; i++) terraintest[i]=false;
		
		const int	half = 1 << cd.level;
    	const int	whole = 2 << cd.level;

    	terraintest[ initVert(0, cd.xorg + half, cd.zorg + half) ] = true;
    	terraintest[ initVert(1, cd.xorg + whole, cd.zorg + half) ] = true;
    	terraintest[ initVert(2, cd.xorg + whole, cd.zorg) ] = true;
    	terraintest[ initVert(3, cd.xorg + half, cd.zorg) ] = true;
    	terraintest[ initVert(4, cd.xorg, cd.zorg) ] = true;
    	terraintest[ initVert(5, cd.xorg, cd.zorg + half) ] = true;
		terraintest[ initVert(6, cd.xorg, cd.zorg + whole) ] = true;
	    terraintest[ initVert(7, cd.xorg + half, cd.zorg + whole) ] = true;
	    terraintest[ initVert(8, cd.xorg + whole, cd.zorg + whole) ] = true;
	
		for(unsigned int j=0; j<num_terrains; j++){
			if(terraintest[j]==true){			 
				// Make the list of triangles to draw
				if ((EnabledFlags & 1) == 0 ) {
					makeTri(0, 2, 8, j);
				} else {
					if (flags & 8) makeTri(0, 1, 8, j);
					if (flags & 1) makeTri(0, 2, 1, j);
				}
 				if ((EnabledFlags & 2) == 0 ) {
					makeTri(0, 4, 2, j);
 				} else {
					if (flags & 1) makeTri(0, 3, 2, j);
					if (flags & 2) makeTri(0, 4, 3, j);
 				}
 				if ((EnabledFlags & 4) == 0 ) {
					makeTri(0, 6, 4, j);
 				} else {
					if (flags & 2) makeTri(0, 5, 4, j);
					if (flags & 4) makeTri(0, 6, 5, j);
 				}
 				if ((EnabledFlags & 8) == 0 ) {
					makeTri(0, 8, 6, j);
 				} else {
					if (flags & 4) makeTri(0, 7, 6, j);
					if (flags & 8) makeTri(0, 8, 7, j);
 				}
			}
		} 
	}
}

void
quadsquare::renderAuxSpezial(const quadcornerdata& cd, ViewFrustum::ClipResult vis)
/// Does the work of rendering this square.  Uses the enabled vertices only.
/// Recurses as necessary.
{
    int	flags = 0;
    int	mask = 1;
    quadcornerdata q;
	
    for(int i = 0; i < 4; i++, mask <<= 1){
		if(EnabledFlags & (16 << i)){
			setupCornerData(&q, cd, i);
			if (vis != ViewFrustum::NoClip) {
				if ((vis = clipSquare( q ))!= ViewFrustum::NotVisible ) {
		 		   Child[i]->renderAuxSpezial(q, vis);
				}
   		 	}else{		
		   		Child[i]->renderAuxSpezial(q, vis);
			}
		}else{
		    flags |= mask;
		}
    }
    if (flags == 0) return;

	
    // Init vertex data.  Here's a diagram of what's going on.
    // Yes, this diagram is fucked, but that's because things are mirrored 
    // in the z axis for us (z coords are actually -z coords).
    //
    // (top of course) 
    //        N                
    //    +---+---+ (xorg, zorg)
    //   2|  3   4|        
    //    |(1) (2)|
    // E  +   +   +  W
    //   1|  0   5|
    //    |(8) (4)|
    //    +---+---+
    //   8   7   6
    //        S
    // (bottom of course)
    //
    // Values in parens are bitmask values for the corresponding child squares
    //

	{
		const int half = 1 << cd.level;
    	const int whole = 2 << cd.level;

    	initVert(0, cd.xorg + half, cd.zorg + half);
    	initVert(1, cd.xorg + whole, cd.zorg + half);
    	initVert(2, cd.xorg + whole, cd.zorg);
    	initVert(3, cd.xorg + half, cd.zorg);
    	initVert(4, cd.xorg, cd.zorg);
    	initVert(5, cd.xorg, cd.zorg + half);
		initVert(6, cd.xorg, cd.zorg + whole);
	    initVert(7, cd.xorg + half, cd.zorg + whole);
	    initVert(8, cd.xorg + whole, cd.zorg + whole);
	}
		
    // Make the list of triangles to draw.
	if((EnabledFlags & 1) == 0){
		makeSpecialTri(0, 2, 8);
    }else{
		if(flags & 8) makeSpecialTri(0, 1, 8);
		if(flags & 1) makeSpecialTri(0, 2, 1);
    }
    if((EnabledFlags & 2) == 0){
		makeSpecialTri(0, 4, 2);
    }else{
		if(flags & 1) makeSpecialTri(0, 3, 2);
		if(flags & 2) makeSpecialTri(0, 4, 3);
    }
    if((EnabledFlags & 4) == 0){
		makeSpecialTri(0, 6, 4);
    }else{
		if(flags & 2) makeSpecialTri(0, 5, 4);
		if(flags & 4) makeSpecialTri(0, 6, 5);
    }
    if((EnabledFlags & 8) == 0){
		makeSpecialTri(0, 8, 6);
    }else{
		if(flags & 4) makeSpecialTri(0, 7, 6);
		if(flags & 8) makeSpecialTri(0, 8, 7);
    }
}

void
quadsquare::setupCornerData(quadcornerdata* q, const quadcornerdata& cd, const int ChildIndex)
/// Fills the given structure with the appropriate corner values for the
/// specified child block, given our own vertex data and our corner
/// vertex data from cd.
//
// ChildIndex mapping:
// +-+-+
// |1|0|
// +-+-+
// |2|3|
// +-+-+
//
// Verts mapping:
// 1-0
// | |
// 2-3
//
// Vertex mapping:
// +-2-+
// | | |
// 3-0-1
// | | |
// +-4-+
{
    const int half = 1 << cd.level;

    q->parent = &cd;
    q->square = Child[ChildIndex];
    q->level = cd.level - 1;
    q->childIndex = ChildIndex;
	
    switch (ChildIndex) {
    default:
    case 0:
	q->xorg = cd.xorg + half;
	q->zorg = cd.zorg;
	q->verts[0] = cd.verts[0];
	q->verts[1] = Vertex[2];
	q->verts[2] = Vertex[0];
	q->verts[3] = Vertex[1];
	break;

    case 1:
	q->xorg = cd.xorg;
	q->zorg = cd.zorg;
	q->verts[0] = Vertex[2];
	q->verts[1] = cd.verts[1];
	q->verts[2] = Vertex[3];
	q->verts[3] = Vertex[0];
	break;

    case 2:
	q->xorg = cd.xorg;
	q->zorg = cd.zorg + half;
	q->verts[0] = Vertex[0];
	q->verts[1] = Vertex[3];
	q->verts[2] = cd.verts[2];
	q->verts[3] = Vertex[4];
	break;

    case 3:
	q->xorg = cd.xorg + half;
	q->zorg = cd.zorg + half;
	q->verts[0] = Vertex[1];
	q->verts[1] = Vertex[0];
	q->verts[2] = Vertex[4];
	q->verts[3] = cd.verts[3];
	break;
    }	
}

int quadsquare::RowSize;
int quadsquare::NumRows;

void
quadsquare::addHeightMap(const quadcornerdata& cd, const HeightMapInfo& hm)
/// Sets the height of all samples within the specified rectangular
/// region using the given array of floats.  Extends the tree to the
/// level of detail defined by (1 << hm.Scale) as necessary.
{
    RowSize = hm.rowWidth;
    NumRows = hm.zSize;

    if ( cd.parent == NULL ) {
		if ( VertexArrayIndices[0] != NULL ) {
			for (int i=0; i< NUM_TERRAIN_TYPES; i++){
				if (VertexArrayIndices[i]!=NULL)
						delete VertexArrayIndices[i];
			}
		}

	/* Maximum number of triangles is 2 * RowSize * NumRows 
	   This uses up a lot of space but it is a *big* performance gain.
	*/
		for (unsigned int i=0; i<num_terrains; i++){	
			VertexArrayIndices[i] = new GLuint[6 * RowSize * NumRows];
		}
    }

    // If block is outside rectangle, then don't bother.
    int	BlockSize = 2 << cd.level;
    if (cd.xorg > hm.xOrigin + ((hm.xSize + 2) << hm.scale) ||
	cd.xorg + BlockSize < hm.xOrigin - (1 << hm.scale) ||
	cd.zorg > hm.zOrigin + ((hm.zSize + 2) << hm.scale) ||
	cd.zorg + BlockSize < hm.zOrigin - (1 << hm.scale))
    {
	// This square does not touch the given height array area; no need to modify this square or descendants.
	return;
    }

    if (cd.parent && cd.parent->square) {
	cd.parent->square->enableChild(cd.childIndex, *cd.parent);	// causes parent edge verts to be enabled, possibly causing neighbor blocks to be created.
    }
	
    int	i;
	
    int	half = 1 << cd.level;

    // Create and update child nodes.
    for (i = 0; i < 4; i++) {
	quadcornerdata	q;
	setupCornerData(&q, cd, i);
				
	if (Child[i] == NULL && cd.level > hm.scale) {
	    // Create child node w/ current (unmodified) values for corner verts.
	    Child[i] = new quadsquare(&q);
	}
		
	// Recurse.
	if (Child[i]) {
	    Child[i]->addHeightMap(q, hm);
	}
    }
	
    // Deviate vertex heights based on data sampled from heightmap.
    float	s[5];
    s[0] = hm.sample(cd.xorg + half, cd.zorg + half);
    s[1] = hm.sample(cd.xorg + half*2, cd.zorg + half);
    s[2] = hm.sample(cd.xorg + half, cd.zorg);
    s[3] = hm.sample(cd.xorg, cd.zorg + half);
    s[4] = hm.sample(cd.xorg + half, cd.zorg + half*2);

    // Modify the vertex heights if necessary, and set the dirty
    // flag if any modifications occur, so that we know we need to
    // recompute error data later.
    for(i = 0; i < 5; i++){
		if (s[i] != 0) {
		    Dirty = true;
		    Vertex[i] += s[i];
		}
    }

    if(!Dirty){
		// Check to see if any child nodes are dirty, and set the dirty flag if so.
		for(i = 0; i < 4; i++){
	    	if(Child[i] && Child[i]->Dirty){
				Dirty = true;
				break;
	    	}
		}
    }

    if (Dirty) setStatic(cd);
}

void
quadsquare::cleanup()
{
// root node should delete VertexArrayIndices
	if(VertexArrayIndices[0]!= NULL){
		for(int i=0; i<NUM_TERRAIN_TYPES; i++){
			if(VertexArrayIndices[i]!=NULL)
				delete [] VertexArrayIndices[i];
				VertexArrayIndices[i]=NULL;
		}
	}
}

float quadsquare::ScaleX;
float quadsquare::ScaleZ;
int* quadsquare::Terrain;
