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

#ifndef _VIEWFRUSTUM_H_
#define _VIEWFRUSTUM_H_

#include "player.h"

class ViewFrustum
{
private:
	static bool p_vertex_code_x[6];
	static bool p_vertex_code_y[6];
	static bool p_vertex_code_z[6];	

	pp::Plane planes[6];

public:
	enum ClipResult{
    	NoClip,
    	SomeClip,
    	NotVisible
	};	

	void setup(	const Player& plyr,
				const double near_dist, const double far_dist,
				const int multiscreen=-1);

	ClipResult clip(const ppogl::Vec3d& min, const ppogl::Vec3d& max);
		
	inline const pp::Plane&
	getFarClipPlane(){return planes[1];}

	inline const pp::Plane&
	getLeftClipPlane(){return planes[2];}

	inline const pp::Plane&
	getRightClipPlane(){return planes[3];}

	inline const pp::Plane&
	getBottomClipPlane(){return planes[5];}	
};

extern ViewFrustum viewFrustum;

#endif // _VIEWFRUSTUM_H_
