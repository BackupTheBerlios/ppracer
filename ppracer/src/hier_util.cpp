/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * Copyright (C) 1999-2001 Jasmin F. Patry
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "hier_util.h"

#include "render_util.h"
#include "hier.h"
#include "ppracer.h"

#include "ppogl/base/defs.h"
#include "ppogl/base/glwrappers.h"

/// Ray (half-line)
struct Ray
{ 
    ppogl::Vec3d pt;
    ppogl::Vec3d vec;
};

/// Draws a sphere using glu
void
draw_sphere(int num_divisions)
{
    glu::Quadric quad;
	quad.DrawStyle(GLU_FILL);
    quad.Orientation(GLU_OUTSIDE);
    quad.Normals(GLU_SMOOTH);
	quad.Sphere(1.0, 2*num_divisions, num_divisions);
}

static GLuint
get_sphere_display_list(int divisions)
{
    static bool initialized = false;
    static int num_display_lists = MAX_SPHERE_DIVISIONS - MIN_SPHERE_DIVISIONS + 1;
    static GLuint *display_lists = NULL;

    if(!initialized){
		initialized = true;

		PP_ASSERT(display_lists == NULL, "display_lists not NULL");
		display_lists = new GLuint[num_display_lists];
		PP_CHECK_ALLOC(display_lists);		
		
		for(int i=0; i<num_display_lists; i++){
			display_lists[i] = 0;
		}
    }

    const int idx = divisions - MIN_SPHERE_DIVISIONS;

    PP_ENSURE(idx >= 0 && idx < num_display_lists, 
		     "invalid number of sphere subdivisions" );

    if(display_lists[idx]==0){
		// Initialize the sphere display list 
		display_lists[idx] = gl::GenLists(1);
		gl::NewList( display_lists[idx], GL_COMPILE );
		draw_sphere( divisions );
		gl::EndList();
    }

    return display_lists[idx];
}

/*--------------------------------------------------------------------------*/

/* Traverses the DAG structure and draws the nodes
 */
void
traverse_dag(SceneNode *node, ppogl::Material *mat)
{
    SceneNode *child;

    PP_CHECK_POINTER( node );
    gl::PushMatrix();

    gl::MultMatrix(node->trans);

    if ( node->mat != NULL ) {
        mat = node->mat;
    } 

    if ( node->isSphere == true ) {
        set_material( mat->diffuse, mat->specular, mat->getSpecularExponent() );

	    gl::CallList( get_sphere_display_list( 
		node->sphere.divisions ) );
    } 

    child = node->child;
    while (child != NULL) {
        traverse_dag( child, mat );
        child = child->next;
    } 

    gl::PopMatrix();
} 

/*--------------------------------------------------------------------------*/

/*
 * make_normal - creates the normal vector for the surface defined by a convex
 * polygon; points in polygon must be specifed in counter-clockwise direction
 * when viewed from outside the shape for the normal to be outward-facing
 */
ppogl::Vec3d
make_normal(const ppogl::Polygon& p, const ppogl::Vec3d *v)
{
    PP_REQUIRE( p.numVertices > 2, "number of vertices must be > 2" );
	
	ppogl::Vec3d normal, v1, v2;
    double old_len;

    v1 = v[p.vertices[1]] - v[p.vertices[0]];
    v2 = v[p.vertices[p.numVertices-1]] - v[p.vertices[0]];
    normal = v1^v2;

    old_len = normal.normalize();
    
	PP_ENSURE( old_len > 0, "normal vector has length 0" );
	
    return normal;
} 

/*--------------------------------------------------------------------------*/

/* Returns true iff the specified polygon intersections a unit-radius sphere
 * centered at the origin.  */
bool
intersect_polygon(const ppogl::Polygon& p, ppogl::Vec3d *v)
{
    Ray ray; 
    ppogl::Vec3d nml, edge_nml, edge_vec;
    ppogl::Vec3d pt;
    double d, s, nuDotProd, wec;
    double edge_len, t, distsq;
    int i;

    /* create a ray from origin along polygon normal */
    nml = make_normal( p, v );
    ray.pt = ppogl::Vec3d( 0., 0., 0. );
    ray.vec = nml;

    nuDotProd = nml*ray.vec;
    if ( fabs(nuDotProd) < EPS )
        return false;

    /* determine distance of plane from origin */
    d = -( nml.x() * v[p.vertices[0]].x() + 
           nml.y() * v[p.vertices[0]].y() + 
           nml.z() * v[p.vertices[0]].z() );

    /* if plane's distance to origin > 1, immediately reject */
    if ( fabs( d ) > 1 )
        return false;

    /* check distances of edges from origin */
    for ( i=0; i < p.numVertices; i++ ) {
	ppogl::Vec3d *v0, *v1;

	v0 = &v[p.vertices[i]];
	v1 = &v[p.vertices[ (i+1) % p.numVertices ]]; 

	edge_vec = (*v1) - (*v0) ;
	edge_len = edge_vec.normalize();

	/* t is the distance from v0 of the closest point on the line
           to the origin */
	t = - ( *(reinterpret_cast<ppogl::Vec3d *>(v0))* edge_vec );

	if ( t < 0 ) {
	    /* use distance from v0 */
	    distsq = v0->length2();
	} else if ( t > edge_len ) {
	    /* use distance from v1 */
	    distsq = v1->length2();
	} else {
	    /* closest point to origin is on the line segment */
	    *v0 = (*v0)+(t*edge_vec);
	    distsq = v0->length2();
	}

	if ( distsq <= 1 ) {
	    return true;
	}
    }

    /* find intersection point of ray and plane */
    s = - ( d + ( nml*ppogl::Vec3d(ray.pt.x(), ray.pt.y(), ray.pt.z()) ) ) /
        nuDotProd;

    pt = ray.pt+(s*ray.vec);

    /* test if intersection point is in polygon by clipping against it 
     * (we are assuming that polygon is convex) */
    for ( i=0; i < p.numVertices; i++ ) {
        edge_nml = nml^( (v[p.vertices[ (i+1) % p.numVertices ]]) - v[p.vertices[i]] );

        wec = (pt - v[p.vertices[i]] )*edge_nml;
        if (wec < 0)
            return false;
    } 

    return true;
} 

/*--------------------------------------------------------------------------*/

/* returns true iff polyhedron intersects unit-radius sphere centered
   at origin */
bool
intersect_polyhedron(const ppogl::Polyhedron& p)
{
    bool hit = false;
		
    for(int i=0; i<p.num_polygons; i++){
		hit = intersect_polygon( p.polygons[i], p.vertices );
		if ( hit == true ) 
            break;
    } 
    return hit;
} 

void
trans_polyhedron(const pp::Matrix& mat, const ppogl::Polyhedron& ph)
{
	for(int i=0; i<ph.num_vertices; i++) {
		ph.vertices[i] = mat.transformPoint(ph.vertices[i]);
	}
}

bool
check_polyhedron_collision_with_dag(SceneNode *node,
	const pp::Matrix& modelMatrix, const pp::Matrix& invModelMatrix,
    const ppogl::Polyhedron& ph)
{
    pp::Matrix newModelMatrix, newInvModelMatrix;
    SceneNode *child;
    bool hit = false;

    PP_CHECK_POINTER( node );

    newModelMatrix=modelMatrix*node->trans;
    newInvModelMatrix=node->invtrans*invModelMatrix;

    if(node->isSphere == true){
        ppogl::Polyhedron newph(ph);
		trans_polyhedron( newInvModelMatrix, newph );
		hit = intersect_polyhedron( newph );
	}

    if (hit == true) return hit;

    child = node->child;
    while (child != NULL) {

        hit = check_polyhedron_collision_with_dag( 
	    child, newModelMatrix, newInvModelMatrix, ph );

        if ( hit == true ) {
            return hit;
        }

        child = child->next;
    } 

    return false;
}
