/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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

#include "course_render.h"

#include "render_util.h"
#include "gl_util.h"
#include "gameconfig.h"
#include "fog.h"
#include "track_marks.h"
#include "viewfrustum.h"
#include "course_load.h"
#include "elements.h"

#include "ppogl/base/defs.h"
#include "ppogl/base/vec4f.h"
#include "ppogl/base/glwrappers.h"

/// convert x indices in height map to world coordinate
inline double
getXCD(double x)
{
	return(x / (Course::nx-1.0) * Course::dimension.x());
}

/// convert y indices in height map to world coordinate
inline double
getZCD(double y)
{
	return (-y / (Course::ny-1.0) * Course::dimension.y());
}

#define CULL_DETAIL_FACTOR 25

CourseRenderer courseRenderer;

CourseRenderer::CourseRenderer()
 : mp_nmls(NULL),
   mp_root(NULL),
   m_clip(false)
{
}

CourseRenderer::~CourseRenderer()
{
	if(mp_nmls) delete[] mp_nmls;
	if(mp_root) delete mp_root;
}

void
CourseRenderer::cleanup()
{
	if(mp_root){
		delete mp_root;
		mp_root=NULL;
	}
	quadsquare::cleanup();
}

void
CourseRenderer::calcNormals()
{
    ppogl::Vec3d p0, p1, p2;
    ppogl::Vec3d n, nml, v1, v2;

    if(mp_nmls!=NULL) delete[] mp_nmls;
      
	mp_nmls = new ppogl::Vec3d[Course::nx*Course::ny]; 

    for(int y=0; y<Course::ny; y++) {
        for(int x=0; x<Course::nx; x++) {
            nml = ppogl::Vec3d( 0., 0., 0. );

            p0 = ppogl::Vec3d( getXCD(x), Course::getElevation(x,y), getZCD(y) );

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

#define POINT(x,y) ppogl::Vec3d(getXCD(x), Course::getElevation(x,y), getZCD(y))

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
		if ( x > 0 && y < Course::ny-1 ) {
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
		if ( x < Course::nx-1 && y > 0 ) {
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
		if ( x < Course::nx-1 && y < Course::ny-1 ) {
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
		if ( x > 0 && y < Course::ny-1 ) {
		    p1 = POINT(x-1,y  );
		    p2 = POINT(x  ,y+1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;
		} 
		if ( x < Course::nx-1 && y > 0 ) {
		    p1 = POINT(x+1,y  );
		    p2 = POINT(x  ,y-1);
		    v1 = p1-p0;
		    v2 = p2-p0;
		    n = v2^v1;

		    PP_ASSERT( n.y() > 0, "course normal points down" );

		    n.normalize();
		    nml = nml+n;
		} 
		if ( x < Course::nx-1 && y < Course::ny-1 ) {
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
			mp_nmls[x + Course::nx*y] = nml;
            continue;
        } 
#undef POINT
    } 
}

void
CourseRenderer::setupTexGen()
{
    static const ppogl::Vec4f xplane(1.0 / TEX_SCALE, 0.0, 0.0, 0.0);
    static const ppogl::Vec4f zplane(0.0, 0.0, 1.0 / TEX_SCALE, 0.0);
    gl::TexGen( GL_S, GL_OBJECT_PLANE, xplane );
    gl::TexGen( GL_T, GL_OBJECT_PLANE, zplane );
}

void
CourseRenderer::init()
{
	// calculate the normals for the course	
	calcNormals();
		
	// get textures for drawing the sky	
	m_skyTexture[0] =
		ppogl::TextureMgr::getInstance().get("sky_front");
	m_skyTexture[1] =
		ppogl::TextureMgr::getInstance().get("sky_top");
	m_skyTexture[2] =
		ppogl::TextureMgr::getInstance().get("sky_bottom");
	m_skyTexture[3] =
		ppogl::TextureMgr::getInstance().get("sky_left");
	m_skyTexture[4] =
		ppogl::TextureMgr::getInstance().get("sky_right");
	m_skyTexture[5] =
		ppogl::TextureMgr::getInstance().get("sky_back");
}

void
CourseRenderer::drawSky(const ppogl::Vec3d& pos)
{
	set_gl_options( SKY );
	
	gl::Color(ppogl::Color::white);

	gl::PushMatrix();

	gl::Translate(pos);

	gl::BindTexture( GL_TEXTURE_2D, m_skyTexture[0] );
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

	gl::BindTexture( GL_TEXTURE_2D, m_skyTexture[1] );
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

	gl::BindTexture( GL_TEXTURE_2D, m_skyTexture[2] );
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

	gl::BindTexture( GL_TEXTURE_2D, m_skyTexture[3] );
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

	gl::BindTexture( GL_TEXTURE_2D, m_skyTexture[4] );
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

	gl::BindTexture( GL_TEXTURE_2D, m_skyTexture[5] );
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
CourseRenderer::drawFogPlane()
{
    if(fogPlane.isEnabled()==false){
		return;
    }   
   
    const double course_angle = Course::getAngle();
    const double slope = tan( ANGLES_TO_RADIANS( course_angle ) );

    const pp::Plane& far_clip_plane = viewFrustum.getFarClipPlane();
    const pp::Plane& left_clip_plane = viewFrustum.getLeftClipPlane();
    const pp::Plane& right_clip_plane = viewFrustum.getRightClipPlane();
    const pp::Plane& bottom_clip_plane = viewFrustum.getBottomClipPlane();

    // Find the bottom plane 
	pp::Plane bottom_plane;
    bottom_plane.nml = ppogl::Vec3d( 0.0, 1, -slope );
    bottom_plane.d = -Course::getTerrainBaseHeight(0) * bottom_plane.nml.y();

    // Find the top plane
	pp::Plane  top_plane;
    top_plane.nml = bottom_plane.nml;
    top_plane.d = -Course::getTerrainMaxHeight(0) * top_plane.nml.y();

	ppogl::Vec3d left_pt, right_pt, pt;
    ppogl::Vec3d top_left_pt, top_right_pt;
    ppogl::Vec3d bottom_left_pt, bottom_right_pt;

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
	
    const ppogl::Vec3d left_vec = top_left_pt - left_pt;
    const ppogl::Vec3d right_vec = top_right_pt - right_pt;

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

void
CourseRenderer::drawElements(const ppogl::Vec3d& pos) 
{
    const double fwd_clip_limit = GameConfig::forwardClipDistance;
    const double bwd_clip_limit = GameConfig::backwardClipDistance;

    set_gl_options(TREES);

    gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    set_material( ppogl::Color::white, ppogl::Color::black, 1.0 );
    
	{
	std::list<Model>::iterator it;
	for(it=modelLocs.begin();it!=modelLocs.end();it++){
		if(m_clip){
	   		if ( pos.z() - (*it).getPosition().z() > fwd_clip_limit ) 
			continue;
	    
	    	if ( (*it).getPosition().z() - pos.z() > bwd_clip_limit )
			continue;
		}
		ppogl::Vec3d normal = pos - (*it).getPosition();
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
	
		if(m_clip){
	    	if( pos.z() - (*it).getPosition().z() > fwd_clip_limit ) 
				continue;
	    	if( (*it).getPosition().z() - pos.z() > bwd_clip_limit )
				continue;
		}

		ppogl::Vec3d normal = pos - (*it).getPosition();
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

void
CourseRenderer::render(const ppogl::Vec3d& pos)
{
	//draw the sky	
	drawSky(pos);
    
	//draw fog
	drawFogPlane();

    set_gl_options( COURSE );

    setupTexGen();

    gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    set_material( ppogl::Color::white, ppogl::Color::black, 1.0 );
    
    updateQuadtree(pos);

    renderQuadtree();
	
	//draw elements
	drawElements(pos);
}

ppogl::Vec3d*
CourseRenderer::getNormals()
{
	PP_REQUIRE(mp_nmls!=NULL,"Array for course normals is not valid");
	return mp_nmls;
} 

void
CourseRenderer::resetQuadtree()
{
    PP_REQUIRE(mp_root!=NULL,"root of quadtree is NULL pointer");
	delete mp_root;
	mp_root = NULL;
}

void
CourseRenderer::initQuadtree(const float *elevation, int nx, int nz, 
			   double scalex, double scalez,
			   const ppogl::Vec3d& view_pos)
{
	HeightMapInfo hm;

	hm.data = elevation;
	hm.xOrigin = 0;
	hm.zOrigin = 0;
	hm.xSize = nx;
	hm.zSize = nz;
	hm.rowWidth = hm.xSize;
	hm.scale = 0;

    m_rootCornerData.square = NULL;
    m_rootCornerData.childIndex = 0;
    m_rootCornerData.level = getRootLevel(nx, nz);
    m_rootCornerData.xorg = 0;
    m_rootCornerData.zorg = 0;

    for(int i=0; i<4; i++){
		m_rootCornerData.verts[i] = 0;
		m_rootCornerData.verts[i] = 0;
    }
	
	mp_root = new quadsquare(&m_rootCornerData);		
    mp_root->addHeightMap(m_rootCornerData, hm);
    mp_root->setScale(scalex, scalez);
    mp_root->setTerrain(Course::getTerrainData());
		
	//update static configuration
	GameConfig::update();
	
    // Debug info.
    PP_LOG( DEBUG_QUADTREE, "max error = " <<
		 mp_root->recomputeError(m_rootCornerData));

    // Get rid of unnecessary nodes in flat-ish areas.
    PP_LOG( DEBUG_QUADTREE, 
		 "Culling unnecessary nodes (detail factor = " << CULL_DETAIL_FACTOR << ")...");
    mp_root->staticCullData(m_rootCornerData, CULL_DETAIL_FACTOR);

    // Post-cull debug info.
	//    PP_LOG( DEBUG_QUADTREE, "nodes = " << mp_root->countNodes());
	//    PP_LOG( DEBUG_QUADTREE, "max error = " << mp_root->recomputeError(m_rootCornerData));

    // Run the update function a few times before we start rendering
    // to disable unnecessary quadsquares, so the first frame won't
    // be overloaded with tons of triangles.
    for(int i=0; i<5; i++){
    	mp_root->update(m_rootCornerData, view_pos);
	}
}

void
CourseRenderer::updateQuadtree(const ppogl::Vec3d& view_pos)
{
	//PP_PERFTEST_BEGIN(update_quadtree);
	mp_root->update(m_rootCornerData, view_pos);
	//PP_PERFTEST_END(update_quadtree);
}

void
CourseRenderer::renderQuadtree()
{
	//PP_PERFTEST_BEGIN(render_quadtree);
    mp_root->render(m_rootCornerData, Course::getGLArrays());
	//PP_PERFTEST_END(render_quadtree);
}

int
CourseRenderer::getRootLevel(int nx, int nz)
{
    PP_REQUIRE( nx > 0, "heightmap has x dimension of 0 size" );
    PP_REQUIRE( nz > 0, "heightmap has z dimension of 0 size" );
	
    const int xlev = int( log(double(nx) ) / log (double(2.0) ) );
    const int zlev = int( log(double(nz) ) / log (double(2.0) ) );

    // check to see if nx, nz are powers of 2
    if((nx >> xlev) << xlev == nx){
		// do nothing
    }else{
		nx += 1;
    }

    if((nz >> zlev) << zlev == nz){
		// do nothing 
    }else{
		nz += 1;
    }

    return MAX(xlev, zlev);
}
