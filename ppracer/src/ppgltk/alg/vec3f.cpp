/* 
 * PPGLTK
 *
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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

#include "vec3f.h"

#include <math.h>

namespace pp{

Vec3f::Vec3f()
 : x(0.0f), y(0.0f), z(0.0f)
{
}
	
Vec3f::Vec3f(const float x, const float y, const float z)
 : x(x), y(y), z(z)
{
}

Vec3f::Vec3f(const float *v)
 : x(v[0]), y(v[1]), z(v[2])
{
}

void
Vec3f::set(const float *v)
{
	x=v[0];
	y=v[1];
	z=v[2];
}

void
Vec3f::set(const double *v)
{
	x=v[0];
	y=v[1];
	z=v[2];
}

} //namespace pp