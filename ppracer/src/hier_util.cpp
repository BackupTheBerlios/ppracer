/* 
 * PPRacer 
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

#include "render_util.h"
#include "hier.h"

#include "ppgltk/alg/defs.h"
#include "ppgltk/alg/glwrappers.h"

#include "game_config.h"


#define USE_GLUSPHERE 0

#if USE_GLUSPHERE

// Draws a sphere using gluSphere
void draw_sphere( int num_divisions )
{
    GLUquadricObj *qobj;

    qobj = gluNewQuadric();
    gluQuadricDrawStyle( qobj, GLU_FILL );
    gluQuadricOrientation( qobj, GLU_OUTSIDE );
    gluQuadricNormals( qobj, GLU_SMOOTH );

    gluSphere( qobj, 1.0, 2.0 * num_divisions, num_divisions );

    gluDeleteQuadric( qobj );
    
}

#else 

void draw_sphere( int num_divisions )
{
    double theta, phi, d_theta, d_phi, eps, twopi;
    double x, y, z;
    int div = num_divisions;

    eps = 1e-15;
    twopi = M_PI * 2.0;

    d_theta = d_phi = M_PI / div;

    for ( phi = 0.0; phi + eps < M_PI; phi += d_phi ) {
	double cos_theta, sin_theta;
	double sin_phi, cos_phi;
	double sin_phi_d_phi, cos_phi_d_phi;

	sin_phi = sin( phi );
	cos_phi = cos( phi );
	sin_phi_d_phi = sin( phi + d_phi );
	cos_phi_d_phi = cos( phi + d_phi );
      
	if ( phi <= eps ) {
		gl::Begin( GL_TRIANGLE_FAN );
		gl::Normal(0.0f, 0.0f, 1.0f);
		gl::Vertex(0.0f, 0.0f, 1.0f);

		for ( theta = 0.0; theta + eps < twopi; theta += d_theta ) {
			sin_theta = sin( theta );
			cos_theta = cos( theta );

			x = cos_theta * sin_phi_d_phi;
			y = sin_theta * sin_phi_d_phi;
			z = cos_phi_d_phi;
			gl::Normal(x, y, z);
			gl::Vertex(x, y, z);
		} 

		x = sin_phi_d_phi;
		y = 0.0;
		z = cos_phi_d_phi;
		gl::Normal(x, y, z);
		gl::Vertex(x, y, z);
		gl::End();

        } else if ( phi + d_phi + eps >= M_PI ) {
            
            gl::Begin( GL_TRIANGLE_FAN );
                gl::Normal(0.0f, 0.0f, -1.0f);
                gl::Vertex(0.0f, 0.0f, -1.0f);

                for ( theta = twopi; theta - eps > 0; theta -= d_theta ) {
		    sin_theta = sin( theta );
		    cos_theta = cos( theta );

                    x = cos_theta * sin_phi;
                    y = sin_theta * sin_phi;
                    z = cos_phi;
                    gl::Normal(x, y, z);
                    gl::Vertex(x, y, z);
                } 
                x = sin_phi;
                y = 0.0;
                z = cos_phi;
                gl::Normal(x, y, z);
                gl::Vertex(x, y, z);
            gl::End();

        } else {
            
            gl::Begin( GL_TRIANGLE_STRIP );
                
                for ( theta = 0.0; theta + eps < twopi; theta += d_theta ) {
		    sin_theta = sin( theta );
		    cos_theta = cos( theta );

                    x = cos_theta * sin_phi;
                    y = sin_theta * sin_phi;
                    z = cos_phi;
                    gl::Normal(x, y, z);
                    gl::Vertex(x, y, z);

                    x = cos_theta * sin_phi_d_phi;
                    y = sin_theta * sin_phi_d_phi;
                    z = cos_phi_d_phi;
                    gl::Normal(x, y, z);
                    gl::Vertex(x, y, z);
                } 
                x = sin_phi;
                y = 0.0;
                z = cos_phi;
                gl::Normal(x, y, z);
                gl::Vertex(x, y, z);

                x = sin_phi_d_phi;
                y = 0.0;
                z = cos_phi_d_phi;
                gl::Normal(x, y, z);
                gl::Vertex(x, y, z);

            gl::End();

        } 
    } 
} 

#endif // USE_GLUSPHERE 

static GLuint get_sphere_display_list( int divisions ) {
    static bool initialized = false;
    static int num_display_lists;
    static GLuint *display_lists = NULL;
    int base_divisions;
    int idx;

    if ( !initialized ) {
		initialized = true;
		base_divisions = getparam_tux_sphere_divisions();

		num_display_lists = MAX_SPHERE_DIVISIONS - MIN_SPHERE_DIVISIONS + 1;

		PP_ASSERT( display_lists == NULL, "display_lists not NULL" );
		display_lists = reinterpret_cast<GLuint*>(malloc( sizeof(GLuint) * num_display_lists ));

		for (int i=0; i<num_display_lists; i++) {
			display_lists[i] = 0;
		}
    }

    idx = divisions - MIN_SPHERE_DIVISIONS;

    PP_ENSURE( idx >= 0 &&
		     idx < num_display_lists, 
		     "invalid number of sphere subdivisions" );

    if ( display_lists[idx] == 0 ) {
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
void traverse_dag( scene_node_t *node, material_t *mat )
{
    scene_node_t *child;

    PP_CHECK_POINTER( node );
    gl::PushMatrix();

    gl::MultMatrix(node->trans);

    if ( node->mat != NULL ) {
        mat = node->mat;
    } 

    if ( node->geom == Sphere ) {
        set_material( mat->diffuse, mat->specular, 
                     mat->specular_exp );

	if ( getparam_use_sphere_display_list() ) {
	    gl::CallList( get_sphere_display_list( 
		node->param.sphere.divisions ) );
	} else {
	    draw_sphere( node->param.sphere.divisions );
	}
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
pp::Vec3d make_normal( pp::Polygon p, pp::Vec3d *v )
{
    PP_REQUIRE( p.numVertices > 2, "number of vertices must be > 2" );

	pp::Vec3d normal, v1, v2;
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
bool intersect_polygon( pp::Polygon p, pp::Vec3d *v )
{
    ray_t ray; 
    pp::Vec3d nml, edge_nml, edge_vec;
    pp::Vec3d pt;
    double d, s, nuDotProd, wec;
    double edge_len, t, distsq;
    int i;

    /* create a ray from origin along polygon normal */
    nml = make_normal( p, v );
    ray.pt = pp::Vec3d( 0., 0., 0. );
    ray.vec = nml;

    nuDotProd = nml*ray.vec;
    if ( fabs(nuDotProd) < EPS )
        return false;

    /* determine distance of plane from origin */
    d = -( nml.x * v[p.vertices[0]].x + 
           nml.y * v[p.vertices[0]].y + 
           nml.z * v[p.vertices[0]].z );

    /* if plane's distance to origin > 1, immediately reject */
    if ( fabs( d ) > 1 )
        return false;

    /* check distances of edges from origin */
    for ( i=0; i < p.numVertices; i++ ) {
	pp::Vec3d *v0, *v1;

	v0 = &v[p.vertices[i]];
	v1 = &v[p.vertices[ (i+1) % p.numVertices ]]; 

	edge_vec = (*v1) - (*v0) ;
	edge_len = edge_vec.normalize();

	/* t is the distance from v0 of the closest point on the line
           to the origin */
	t = - ( *(reinterpret_cast<pp::Vec3d *>(v0))* edge_vec );

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
    s = - ( d + ( nml*pp::Vec3d(ray.pt.x, ray.pt.y, ray.pt.z) ) ) /
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
bool intersect_polyhedron( pp::Polyhedron p )
{
    bool hit = false;
    int i;
	
    for (i=0; i<p.num_polygons; i++) {
		
		hit = intersect_polygon( p.polygons[i], p.vertices );
        if ( hit == true ) 
            break;
    } 
    return hit;
} 

/*--------------------------------------------------------------------------*/

pp::Polyhedron copy_polyhedron( pp::Polyhedron ph )
{
    pp::Polyhedron newph = ph;
    newph.vertices = reinterpret_cast<pp::Vec3d *>(malloc( sizeof(pp::Vec3d) * ph.num_vertices));
    for (int i=0; i<ph.num_vertices; i++) {
        newph.vertices[i] = ph.vertices[i];
    } 
    return newph;
} 

/*--------------------------------------------------------------------------*/

void free_polyhedron( pp::Polyhedron ph ) 
{
    free(ph.vertices);
} 

/*--------------------------------------------------------------------------*/

void trans_polyhedron( pp::Matrix mat, pp::Polyhedron ph )
{
    int i;
    for (i=0; i<ph.num_vertices; i++) {
        ph.vertices[i] = mat.transformPoint(ph.vertices[i]);
    }
}

/*--------------------------------------------------------------------------*/

bool check_polyhedron_collision_with_dag( 
    scene_node_t *node, pp::Matrix modelMatrix, pp::Matrix invModelMatrix,
    pp::Polyhedron ph)
{
    pp::Matrix newModelMatrix, newInvModelMatrix;
    scene_node_t *child;
    pp::Polyhedron newph;
    bool hit = false;

    PP_CHECK_POINTER( node );

    newModelMatrix=modelMatrix*node->trans;
    newInvModelMatrix=node->invtrans*invModelMatrix;

    if ( node->geom == Sphere ) {
        newph = copy_polyhedron( ph );
        trans_polyhedron( newInvModelMatrix, newph );

        hit = intersect_polyhedron( newph );
        free_polyhedron( newph );
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
