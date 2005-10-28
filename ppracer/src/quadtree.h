/*
 * quadtree.hpp	-thatcher 9/15/1999 Copyright 1999-2000 Thatcher Ulrich
 *
 * Data structures for quadtree terrain storage.
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

#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "viewfrustum.h"
#include "ppogl/sg/vnc.h"

enum VertexLoc
{
    East,
    South,
    Center
};

struct HeightMapInfo
{
	const float *data;
    int	xOrigin, zOrigin;
    int	xSize, zSize;
    int	rowWidth;
    int	scale;

   	inline float sample(int x, int z) const
	{
		if(x>=xSize){
			x=xSize-1;
		}
		if(z>=zSize){
			z=zSize-1;
		}
		return data[x+z*rowWidth];
	}
};

class quadsquare;

// A structure used during recursive traversal of the tree to hold
// relevant but transitory data.
class quadcornerdata
{
public:
	quadcornerdata():parent(NULL),square(NULL){}
	const quadcornerdata* parent;
    quadsquare*	square;
    int	childIndex;
    int	level;
    int	xorg, zorg;
	float verts[4];
};


class quadsquare
{
    quadsquare*	Child[4];

    float Vertex[5];	// center, e, n, w, s
    float Error[6];	// e, s, children: ne, nw, sw, se
    float MinY, MaxY;	// Bounds for frustum culling and error testing.
    unsigned char EnabledFlags;	// bits 0-7: e, n, w, s, ne, nw, sw, se
    unsigned char SubEnabledCount[2];	// e, s enabled reference counts.
    bool Static;
    bool Dirty;	// Set when vertex data has changed, but error/enabled data has not been recalculated.

    bool ForceEastVert;
    bool ForceSouthVert;
	
    static float ScaleX, ScaleZ;
    static int RowSize, NumRows;
    static int* Terrain;

    static GLuint *VertexArrayIndices[NUM_TERRAIN_TYPES];
    static GLuint VertexArrayCounter[NUM_TERRAIN_TYPES];
    static GLuint VertexArrayMinIdx[NUM_TERRAIN_TYPES];
    static GLuint VertexArrayMaxIdx[NUM_TERRAIN_TYPES];

    static void makeTri( int a, int b, int c, int terrain );
    static void makeSpecialTri( int a, int b, int c);

    static void drawTris(int terrain);
    static void drawEnvmapTris(GLuint MapTexId, int terrain);
    static void initArrayCounters();

public:
    quadsquare(quadcornerdata* pcd);
    ~quadsquare();

    void addHeightMap(const quadcornerdata& cd, const HeightMapInfo& hm);
    void staticCullData(const quadcornerdata& cd, const float ThresholdDetail);	
    float recomputeError(const quadcornerdata& cd);
	
    void update(const quadcornerdata& cd, const ppogl::Vec3d& viewerLocation);
    void render(const quadcornerdata& cd, ppogl::VNCArray* vnc_array);

	inline void setScale(const float x, const float z)
	{
    	ScaleX = x;
    	ScaleZ = z;
	};

	inline void	setTerrain(int *terrain){Terrain = terrain;};
	
private:
    void enableEdgeVertex(int index, const bool IncrementCount, const quadcornerdata& cd);
    quadsquare*	enableDescendant(int count, int stack[], const quadcornerdata& cd);
    void enableChild(int index, const quadcornerdata& cd);
    void notifyChildDisable(const quadcornerdata& cd, int index);

    void resetTree();
    void staticCullAux(const quadcornerdata& cd, const float ThresholdDetail, const int TargetLevel);

    quadsquare*	getNeighbor(const int dir, const quadcornerdata& cd) const;
    void createChild(int index, const quadcornerdata& cd);
    void setupCornerData(quadcornerdata* q, const quadcornerdata& pd, const int ChildIndex);

    void updateAux(const quadcornerdata& cd, const float ViewerLocation[3], const float CenterError, ClipResult vis);
    void renderAux(const quadcornerdata& cd, ClipResult vi);
    void renderAuxSpezial(const quadcornerdata& cd, ClipResult vis);
    void setStatic(const quadcornerdata& cd);
	int	initVert(const int i, const int x, const int z);

    bool vertexTest(int x, float y, int z, float error, const float Viewer[3], VertexLoc vertex_loc );
    bool boxTest(int x, int z, float size, float miny, float maxy, float error, const float Viewer[3]);
    ClipResult clipSquare(const quadcornerdata& cd);
};

#endif // _QUADTREE_H_
