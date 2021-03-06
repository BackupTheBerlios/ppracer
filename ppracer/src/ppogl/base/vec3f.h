/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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
 
#ifndef _PPOGL_VEC3F_H_
#define _PPOGL_VEC3F_H_

#include "assert.h"

namespace ppogl {

///A 3D vector class
class Vec3f
{
public:
	float values[3];	

	Vec3f()
	{
		values[0]=0.0f;
		values[1]=0.0f;
		values[2]=0.0f;
	}

	Vec3f(const float _x, const float _y, const float _z)
	{
		values[0]=_x;
		values[1]=_y;
		values[2]=_z;	
	}

	float& x(){return values[0];}
	float& y(){return values[1];}
	float& z(){return values[2];}
	
	float x() const {return values[0];}
	float y() const {return values[1];}
	float z() const {return values[2];}
	
	float& operator[](const int position)
	{
		PP_REQUIRE(position>=0 && position<=2,"Position exceeds size of array: " << position);
		return values[position];
	}
	
	float operator[](const int position) const 
	{
		PP_REQUIRE(position>=0 && position<=2,"Position exceeds size of array: " << position);
		return values[position];
	}
		
	/// print formated values to an std::ostream
	friend std::ostream& operator << (std::ostream& output, const Vec3f& vec);
};


} //namespace ppogl

#endif // _PPOGL_VEC3F_H_
