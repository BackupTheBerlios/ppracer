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

#ifndef _RENDER_UTIL_H_
#define _RENDER_UTIL_H_

#include "ppogl/base/color.h"
#include "ppogl/base/vec2d.h"
#include "ppogl/base/vec3d.h"

//#include "player.h"
//reduce dependencies
class Player;

#define NEAR_CLIP_DIST 0.1

void reshape( int w, int h, int multiscreen=-1);

void clear_rendering_context();

void set_material(const ppogl::Color& diffuse, const ppogl::Color& specular,
		   const double specular_exp );

void draw_billboard(const Player& plyr,
		     const ppogl::Vec3d& center_pt, double width, double height, 
		     const ppogl::Vec2d& min_tex_coord, const ppogl::Vec2d& max_tex_coord );

#endif // _RENDER_UTIL_H_
