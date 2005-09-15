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

#include "vec4f.h"

#include <math.h>

namespace ppogl{

Vec4f::Vec4f()
{
	values[0]=0.0;
	values[1]=0.0;
	values[2]=0.0;
	values[3]=0.0;
}
	
Vec4f::Vec4f(const float x, const float y, const float z, const float w)
{
	values[0]=x;
	values[1]=y;
	values[2]=z;
	values[3]=w;	
}

Vec4f::Vec4f(const float *v)
{
	values[0]=v[0];
	values[1]=v[1];
	values[2]=v[2];
	values[3]=v[3];
}

void
Vec4f::set(const float *v)
{
	values[0]=v[0];
	values[1]=v[1];
	values[2]=v[2];
	values[3]=v[3];
}

void
Vec4f::set(const double *v)
{
	values[0]=v[0];
	values[1]=v[1];
	values[2]=v[2];
	values[3]=v[3];
}

} //namespace ppogl
