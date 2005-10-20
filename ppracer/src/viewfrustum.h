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

#ifndef _VIEWFRUSTUM_H_
#define _VIEWFRUSTUM_H_

#include "player.h"

enum ClipResult{
    NoClip,
    SomeClip,
    NotVisible
};

void setup_view_frustum(const Player& plyr,
			double near_dist, double far_dist,
			int multiscreen=-1);

ClipResult clip_aabb_to_view_frustum(const ppogl::Vec3d& min, const ppogl::Vec3d& max);

const pp::Plane& get_far_clip_plane();
const pp::Plane& get_left_clip_plane();
const pp::Plane& get_right_clip_plane();
const pp::Plane& get_bottom_clip_plane();
 
#endif // _VIEWFRUSTUM_H_
