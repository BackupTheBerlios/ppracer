/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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

#include "gl_util.h"
#include "render_util.h"
#include "player.h"
#include "gameconfig.h"

#include "stuff.h"

#include "ppogl/base/glwrappers.h"

/*
 * Constants 
 */

/** Distance by which to push back far clip plane, to ensure that the
   fogging plane is drawn (m) */
#define FAR_CLIP_FUDGE_AMOUNT 5

void
reshape(const ppogl::Vec2i& resolution, int multiscreen)
{
	PP_REQUIRE(multiscreen <= 1,  "Screen " << multiscreen << " is not supported in multiscreen mode");
	
	if(multiscreen<0){
		//not in multiscreen mode
    	gl::Viewport(0, 0, resolution.x(), resolution.y());
    	gl::MatrixMode( GL_PROJECTION );
    	gl::LoadIdentity();
    	const double far_clip_dist = GameConfig::forwardClipDistance + FAR_CLIP_FUDGE_AMOUNT;
    	glu::Perspective( GameConfig::fov, double(resolution.x())/resolution.y(), NEAR_CLIP_DIST, far_clip_dist );
    	gl::MatrixMode( GL_MODELVIEW );
	}else if(multiscreen==0){
		// multiscreen 0 (top)
		gl::Viewport(0, resolution.y()/2, resolution.x(), resolution.y()/2);
    	gl::MatrixMode(GL_PROJECTION);
    	gl::LoadIdentity();
    	const double far_clip_dist = GameConfig::forwardClipDistance + FAR_CLIP_FUDGE_AMOUNT;
    	glu::Perspective( GameConfig::fov, double(resolution.x()/2)/(resolution.y()/4), NEAR_CLIP_DIST, far_clip_dist);
    	gl::MatrixMode( GL_MODELVIEW );
	}else if(multiscreen==1){
		// multiscreen 1 (bottom)
		gl::Viewport(0, 0, resolution.x(), resolution.y()/2);
    	gl::MatrixMode(GL_PROJECTION);
    	gl::LoadIdentity();
    	const double far_clip_dist = GameConfig::forwardClipDistance + FAR_CLIP_FUDGE_AMOUNT;
    	glu::Perspective( GameConfig::fov, double(resolution.x()/2)/(resolution.y()/4), NEAR_CLIP_DIST, far_clip_dist);
    	gl::MatrixMode( GL_MODELVIEW );
	}
} 

void
clear_rendering_context()
{
    gl::DepthMask( GL_TRUE );
    gl::ClearColor(ppogl::Color(0.5, 0.6, 0.9));
    gl::ClearStencil(0);
    gl::Clear( GL_COLOR_BUFFER_BIT 
	     | GL_DEPTH_BUFFER_BIT 
	     | GL_STENCIL_BUFFER_BIT );
}

void
set_material(const ppogl::Color& diffuse, const ppogl::Color& specular,
			 const float specular_exp)
///Sets the material properties
{
  // Set material color (used when lighting is on)
  gl::Material(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse);
  gl::Material(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  gl::Material(GL_FRONT_AND_BACK, GL_SHININESS, specular_exp);
  // Set standard color
  gl::Color(diffuse);
} 

void
draw_billboard(const Player& plyr, 
		     const ppogl::Vec3d& center_pt, float width, float height, 
		     const ppogl::Vec2d& min_tex_coord, const ppogl::Vec2d& max_tex_coord )
{
	const ppogl::Vec3d x_vec(plyr.view.inv_view_mat.data[0][0],
    		plyr.view.inv_view_mat.data[0][1],
    		plyr.view.inv_view_mat.data[0][2]
	);

	const ppogl::Vec3d y_vec(plyr.view.inv_view_mat.data[1][0],
			plyr.view.inv_view_mat.data[1][1],
			plyr.view.inv_view_mat.data[1][2]
	);
	
	const ppogl::Vec3d z_vec(plyr.view.inv_view_mat.data[2][0],
			plyr.view.inv_view_mat.data[2][1],
			plyr.view.inv_view_mat.data[2][2]
	);

    gl::Begin(GL_QUADS);
    {
		ppogl::Vec3d pt = center_pt+((-width/2.0)*x_vec);
		pt = pt+((-height/2.0)*y_vec);
		gl::Normal(z_vec);
		gl::TexCoord(min_tex_coord);
		gl::Vertex(pt);

		pt = pt + (width*x_vec);
		gl::TexCoord(max_tex_coord.x(), min_tex_coord.y());
		gl::Vertex(pt);

		pt = pt - (height*y_vec);
		gl::TexCoord(max_tex_coord);
		gl::Vertex(pt);

		pt = pt - (-width *x_vec);
		gl::TexCoord(min_tex_coord.x(), max_tex_coord.y());
		gl::Vertex(pt);
	}
    gl::End();
}
