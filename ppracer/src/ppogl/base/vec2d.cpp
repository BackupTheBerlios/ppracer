/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
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

#include "vec2d.h"

#include <math.h>

namespace ppogl {

Vec2d::Vec2d()
{
	values[0]=0.0;
	values[1]=0.0;
}
	
Vec2d::Vec2d(const double x, const double y)
{
	values[0]=x;
	values[1]=y;
}

Vec2d::Vec2d(const double *v)
{
	values[0]=v[0];
	values[1]=v[1];
}

Vec2d
operator+(const Vec2d& vec1,const Vec2d& vec2)
{
	return Vec2d(vec1.x()+vec2.x(),vec1.y()+vec2.y());
}

Vec2d
operator+(const Vec2d& vec,const double value)
{
	return Vec2d(vec.x()+value, vec.y()+value);
}

Vec2d
operator-(const Vec2d& vec1,const Vec2d& vec2)
{
	return Vec2d(vec1.x()-vec2.x(),vec1.y()-vec2.y());
}

Vec2d
operator-(const Vec2d& vec,const double value)
{
	return Vec2d(vec.x()-value, vec.y()-value);
}

} //namespace ppogl
