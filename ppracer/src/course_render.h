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

#ifndef _COURSE_RENDER_H_
#define _COURSE_RENDER_H_

#include "ppogl/base/vec3d.h"
#include "ppogl/textures.h"
#include "quadtree.h"

class CourseRenderer
{
protected:
	/// array of teytures for the sky box
	ppogl::TextureRef m_skyTexture[6];

	/// the course normal vectors
	ppogl::Vec3d* mp_nmls;

	quadsquare* mp_root;
	quadcornerdata m_rootCornerData;

	/// is clipping activiated
	bool m_clip;

	void calcNormals();
	void setupTexGen();

	void drawSky(const ppogl::Vec3d& pos);
	void drawFogPlane(const ppogl::Vec3d& pos);
	void drawElements(const ppogl::Vec3d& pos);

	void updateQuadtree(const ppogl::Vec3d& view_pos);
	void renderQuadtree();
	int getRootLevel(int nx, int nz);

public:
	CourseRenderer();

	void init();

	void render(const ppogl::Vec3d& pos);

	void setClipping(bool state){m_clip = state;}
	void setFog(bool state);
	
	ppogl::Vec3d* getNormals();
	
	void initQuadtree(const float *elevation, int nx, int nz, 
			   double scalex, double scalez,
			   const ppogl::Vec3d& view_pos);
	void resetQuadtree();
};

/// global instance for the course renderer
// note: this is only be a temporal sollution that will
//       be replaced with the sg
extern CourseRenderer courseRenderer;

#endif // _COURSE_RENDER_H_
