/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * Copyright (C) 1999-2001 Jasmin F. Patry
 * 
 * This proigram is free software; you can redistribute it and/or
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

#include "course_load.h"
#include "course_render.h"
#include "phys_sim.h"
#include "hier_util.h"
#include "gl_util.h"
#include "render_util.h"
#include "fog.h"
#include "course_quad.h"
#include "viewfrustum.h"
#include "track_marks.h"
#include "gameconfig.h"

#include "elements.h"

#include "ppogl/base/defs.h"
#include "ppogl/base/vec4f.h"
#include "ppogl/base/glwrappers.h"

#include <iostream>

/* 
 *  Constants 
 */
 

/* How long to make the flat part at the bottom of the course, as a
   fraction of the total length of the course */
#define FLAT_SEGMENT_FRACTION 0.2

/* Aspect ratio of background texture */
#define BACKGROUND_TEXTURE_ASPECT 3.0


/*
 * Statics 
 */

/* The course normal vectors */
static ppogl::Vec3d *nmls = NULL;

/* Should we activate clipping when drawing the course? */
static bool clip_course = false;

/* If clipping is active, it will be based on a camera located here */
static ppogl::Vec3d eye_pt;


/* Macros for converting indices in height map to world coordinates */
#define XCD(x) (  double(x) / (nx-1.) * courseWidth )
#define ZCD(y) ( -double(y) / (ny-1.) * courseLength )

#define NORMAL(x, y) ( nmls[ (x) + nx * (y) ] )


/*
 * Function definitions
 */

void
set_course_clipping(bool state)
{
	clip_course = state;
}

void
set_course_eye_point(const ppogl::Vec3d& pt)
{
	eye_pt = pt;
}

ppogl::Vec3d*
get_course_normals()
{
	return nmls;
} 

void
calc_normals()
{
    float *elevation;
    float courseWidth, courseLength;
    int nx, ny;
    int x,y;
    ppogl::Vec3d p0, p1, p2;
    ppogl::Vec3d n, nml, v1, v2;

    elevation = Course::getElevData();
    Course::getDimensions( &courseWidth, &courseLength );
    Course::getDivisions( &nx, &ny );

    if(nmls!=NULL) delete[] nmls;
      
	nmls = new ppogl::Vec3d[nx*ny]; 
	PP_CHECK_ALLOC(nmls);

    for ( y=0; y<ny; y++) {
        for ( x=0; x<nx; x++) {
            nml = ppogl::Vec3d( 0., 0., 0. );

            p0 = ppogl::Vec3d( XCD(x), ELEV(x,y), ZCD(y) );

	    /* The terrain is meshed as follows:
	             ...
	          +-+-+-+-+            x<---+
	          |\|/|\|/|                 |
	       ...+-+-+-+-+...              V
	          |/|\|/|\|                 y
	          +-+-+-+-+
		     ...

	       So there are two types of vertices: those surrounded by
	       four triangles (x+y is odd), and those surrounded by
	       eight (x+y is even).
	    */

#define POINT(x,y) ppogl::Vec3d( XCD(x), ELEV(x,y), ZCD(y) )

	    if ( (x + y) % 2 == 0 ) {
		if ( x > 0 && y > 0 ) {
		    p1 = POINT(x,  y-1);
		    p2 = POINT(x-1,y-1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;

		    p1 = POINT(x-1,y-1);
		    p2 = POINT(x-1,y  );
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;
		} 
		if ( x > 0 && y < ny-1 ) {
		    p1 = POINT(x-1,y  );
		    p2 = POINT(x-1,y+1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;

		    p1 = POINT(x-1,y+1);
		    p2 = POINT(x  ,y+1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;
		} 
		if ( x < nx-1 && y > 0 ) {
		    p1 = POINT(x+1,y  );
		    p2 = POINT(x+1,y-1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;

		    p1 = POINT(x+1,y-1);
		    p2 = POINT(x  ,y-1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;
		} 
		if ( x < nx-1 && y < ny-1 ) {
		    p1 = POINT(x+1,y  );
		    p2 = POINT(x+1,y+1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v1^v2;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;

		    p1 = POINT(x+1,y+1);
		    p2 = POINT(x  ,y+1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v1^v2;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;

		} 
	    } else {
		/* x + y is odd */
		if ( x > 0 && y > 0 ) {
		    p1 = POINT(x,  y-1);
		    p2 = POINT(x-1,y  );
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;
		} 
		if ( x > 0 && y < ny-1 ) {
		    p1 = POINT(x-1,y  );
		    p2 = POINT(x  ,y+1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;
		} 
		if ( x < nx-1 && y > 0 ) {
		    p1 = POINT(x+1,y  );
		    p2 = POINT(x  ,y-1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;
		} 
		if ( x < nx-1 && y < ny-1 ) {
		    p1 = POINT(x+1,y  );
		    p2 = POINT(x  ,y+1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v1^v2;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;
		} 
	    }

            nml.normalize();
            NORMAL(x,y) = nml;
            continue;
        } 
#undef POINT
    } 
} 

void
setup_course_tex_gen()
{
    static ppogl::Vec4f xplane(1.0 / TEX_SCALE, 0.0, 0.0, 0.0);
    static ppogl::Vec4f zplane(0.0, 0.0, 1.0 / TEX_SCALE, 0.0);
    gl::TexGen( GL_S, GL_OBJECT_PLANE, xplane );
    gl::TexGen( GL_T, GL_OBJECT_PLANE, zplane );
}

void
render_course()
{
    int nx, ny;

    Course::getDivisions(&nx, &ny);
    set_gl_options( COURSE );

    setup_course_tex_gen();

    gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    set_material( ppogl::Color::white, ppogl::Color::black, 1.0 );
    
    update_course_quadtree(eye_pt);

    render_course_quadtree();
}

void
draw_sky(const ppogl::Vec3d& pos)
{
  ppogl::TextureRef texture[6];

  set_gl_options( SKY );

	texture[0] =
		ppogl::TextureMgr::getInstance().get("sky_front");
	texture[1] =
		ppogl::TextureMgr::getInstance().get("sky_top");
	texture[2] =
		ppogl::TextureMgr::getInstance().get("sky_bottom");
	texture[3] =
		ppogl::TextureMgr::getInstance().get("sky_left");
	texture[4] =
		ppogl::TextureMgr::getInstance().get("sky_right");
	texture[5] =
		ppogl::TextureMgr::getInstance().get("sky_back");
	
  gl::Color(ppogl::Color::white);

  gl::PushMatrix();

  gl::Translate(pos);

  gl::BindTexture( GL_TEXTURE_2D, texture[0] );
  gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

  gl::Begin(GL_QUADS);
  gl::TexCoord( 0.0, 0.0 );
  gl::Vertex( -1, -1, -1);
  gl::TexCoord( 1.0, 0.0 );
  gl::Vertex(  1, -1, -1);
  gl::TexCoord( 1.0, 1.0 );
  gl::Vertex(  1,  1, -1);
  gl::TexCoord( 0.0, 1.0 );
  gl::Vertex( -1,  1, -1);
  gl::End();

  gl::BindTexture( GL_TEXTURE_2D, texture[1] );
  gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

  gl::Begin(GL_QUADS);
  gl::TexCoord( 0.0, 0.0 );
  gl::Vertex( -1,  1, -1);
  gl::TexCoord( 1.0, 0.0 );
  gl::Vertex(  1,  1, -1);
  gl::TexCoord( 1.0, 1.0 );
  gl::Vertex(  1,  1,  1);
  gl::TexCoord( 0.0, 1.0 );
  gl::Vertex( -1,  1,  1);
  gl::End();

  gl::BindTexture( GL_TEXTURE_2D, texture[2] );
  gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

  gl::Begin(GL_QUADS);
  gl::TexCoord( 0.0, 0.0 );
  gl::Vertex( -1, -1,  1);
  gl::TexCoord( 1.0, 0.0 );
  gl::Vertex(  1, -1,  1);
  gl::TexCoord( 1.0, 1.0 );
  gl::Vertex(  1, -1, -1);
  gl::TexCoord( 0.0, 1.0 );
  gl::Vertex( -1, -1, -1);
  gl::End();


  gl::BindTexture( GL_TEXTURE_2D, texture[3] );
  gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

  gl::Begin(GL_QUADS);
  gl::TexCoord( 0.0, 0.0 );
  gl::Vertex( -1, -1,  1);
  gl::TexCoord( 1.0, 0.0 );
  gl::Vertex( -1, -1, -1);
  gl::TexCoord( 1.0, 1.0 );
  gl::Vertex( -1,  1, -1);
  gl::TexCoord( 0.0, 1.0 );
  gl::Vertex( -1,  1,  1);
  gl::End();


  gl::BindTexture( GL_TEXTURE_2D, texture[4] );
  gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

  gl::Begin(GL_QUADS);
  gl::TexCoord( 0.0, 0.0 );
  gl::Vertex(  1, -1, -1);
  gl::TexCoord( 1.0, 0.0 );
  gl::Vertex(  1, -1,  1);
  gl::TexCoord( 1.0, 1.0 );
  gl::Vertex(  1,  1,  1);
  gl::TexCoord( 0.0, 1.0 );
  gl::Vertex(  1,  1, -1);
  gl::End();

  gl::BindTexture( GL_TEXTURE_2D, texture[5] );
  gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

  gl::Begin(GL_QUADS);
  gl::TexCoord( 0.0, 0.0 );
  gl::Vertex(  1, -1,  1);
  gl::TexCoord( 1.0, 0.0 );
  gl::Vertex( -1, -1,  1);
  gl::TexCoord( 1.0, 1.0 );
  gl::Vertex( -1,  1,  1);
  gl::TexCoord( 0.0, 1.0 );
  gl::Vertex(  1,  1,  1);
  gl::End();

  gl::PopMatrix();

}

void
draw_elements() 
{
    ppogl::Vec3d  normal;
    double  fwd_clip_limit, bwd_clip_limit;

    fwd_clip_limit = GameConfig::forwardClipDistance;
    bwd_clip_limit = GameConfig::backwardClipDistance;

    set_gl_options( TREES );

    gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    set_material( ppogl::Color::white, ppogl::Color::black, 1.0 );
    
	{
	std::list<Model>::iterator it;
	for(it=modelLocs.begin();it!=modelLocs.end();it++){
		if(clip_course){
	   		if ( eye_pt.z() - (*it).getPosition().z() > fwd_clip_limit ) 
			continue;
	    
	    	if ( (*it).getPosition().z() - eye_pt.z() > bwd_clip_limit )
			continue;
		}
		normal = eye_pt - (*it).getPosition();
		normal.normalize();
		(*it).draw(normal);
    }
	}
    
	{

	std::list<Item>::iterator it;
    for(it=itemLocs.begin();it!=itemLocs.end();it++){
		if(!(*it).isDrawable()){
			continue;
		}
	
		if(clip_course){
	    	if( eye_pt.z() - (*it).getPosition().z() > fwd_clip_limit ) 
				continue;
	    	if( (*it).getPosition().z() - eye_pt.z() > bwd_clip_limit )
				continue;
		}

		normal = eye_pt - (*it).getPosition();
		normal.normalize();
	   	if (normal.y() == 1.0) {
			continue;
	   	}
		normal.y() = 0.0;
	    normal.normalize();
		(*it).draw(normal);	
	} 
	}
}

/*! 
  Draws a fog plane at the far clipping plane to mask out clipping of terrain.

  \return  none
  \author  jfpatry
  \date    Created:  2000-08-31
  \date    Modified: 2000-08-31
*/

void
draw_fog_plane()
{
    if(fogPlane.isEnabled()==false){
		return;
    }   

	pp::Plane left_edge_plane, right_edge_plane;
    pp::Plane left_clip_plane, right_clip_plane;
    pp::Plane far_clip_plane;
    pp::Plane bottom_clip_plane;
    pp::Plane bottom_plane, top_plane;

    float course_width, course_length;
    double course_angle, slope;

    ppogl::Vec3d left_pt, right_pt, pt;
    ppogl::Vec3d top_left_pt, top_right_pt;
    ppogl::Vec3d bottom_left_pt, bottom_right_pt;
    ppogl::Vec3d left_vec, right_vec;
    double height;

	Course::getDimensions( &course_width, &course_length );
    course_angle = Course::getAngle();
    slope = tan( ANGLES_TO_RADIANS( course_angle ) );

    left_edge_plane = pp::Plane( 1.0, 0.0, 0.0, 0.0 );

    right_edge_plane = pp::Plane( -1.0, 0.0, 0.0, course_width );

    far_clip_plane = get_far_clip_plane();
    left_clip_plane = get_left_clip_plane();
    right_clip_plane = get_right_clip_plane();
    bottom_clip_plane = get_bottom_clip_plane();


    // Find the bottom plane 
    bottom_plane.nml = ppogl::Vec3d( 0.0, 1, -slope );
    height = Course::getTerrainBaseHeight( 0 );

    bottom_plane.d = -height * bottom_plane.nml.y();

    // Find the top plane 
    top_plane.nml = bottom_plane.nml;
    height = Course::getTerrainMaxHeight( 0 );
    top_plane.d = -height * top_plane.nml.y();

    // Now find the bottom left and right points of the fog plane 
    if ( !pp::Plane::intersect( bottom_plane, far_clip_plane, left_clip_plane,
			    &left_pt ) )
    {
	return;
    }

    if ( !pp::Plane::intersect( bottom_plane, far_clip_plane, right_clip_plane,
			    &right_pt ) )
    {
		return;
    }

    if ( !pp::Plane::intersect( top_plane, far_clip_plane, left_clip_plane,
			    &top_left_pt ) )
    {
		return;
    }

    if ( !pp::Plane::intersect( top_plane, far_clip_plane, right_clip_plane,
			    &top_right_pt ) )
    {
		return;
    }

    if ( !pp::Plane::intersect( bottom_clip_plane, far_clip_plane, 
			    left_clip_plane, &bottom_left_pt ) )
    {
		return;
    }

    if ( !pp::Plane::intersect( bottom_clip_plane, far_clip_plane, 
			    right_clip_plane, &bottom_right_pt ) )
    {
		return;
    }

    left_vec = top_left_pt - left_pt;
    right_vec = top_right_pt - right_pt;


    // Now draw the fog plane 

    set_gl_options( FOG_PLANE );

    const ppogl::Color &fogColor = fogPlane.getColor();

    gl::Color(fogColor);

    gl::Begin(GL_QUAD_STRIP);

    gl::Vertex(bottom_left_pt);
    gl::Vertex(bottom_right_pt);
    gl::Vertex(left_pt);
    gl::Vertex(right_pt);

    gl::Color(fogColor, 0.9);
    gl::Vertex(top_left_pt);
    gl::Vertex(top_right_pt);

    gl::Color(fogColor, 0.3);
    pt = top_left_pt + left_vec ;
    gl::Vertex(pt);
    pt = top_right_pt + right_vec;
    gl::Vertex(pt);
		
    gl::Color(fogColor, 0.0 );
    pt = top_left_pt + left_vec*3.0;
    gl::Vertex(pt);
    pt = top_right_pt + right_vec*3.0;
    gl::Vertex( pt.x(), pt.y(), pt.z() );

    gl::End();
}
