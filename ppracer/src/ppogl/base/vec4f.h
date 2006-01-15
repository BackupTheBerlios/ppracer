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
 
#ifndef _PPOGL_VEC4F_H_
#define _PPOGL_VEC4F_H_

#include "assert.h"

namespace ppogl {

///A 4D vector class
class Vec4f
{
public:
	float values[4];
	
	Vec4f(){
		values[0]=0.0f;
		values[1]=0.0f;
		values[2]=0.0f;
		values[3]=0.0f;
	}
	
	Vec4f(const float x, const float y, const float z, const float w)
	{
		values[0]=x;
		values[1]=y;
		values[2]=z;
		values[3]=w;	
	}

	float& x(){return values[0];}
	float& y(){return values[1];}
	float& z(){return values[2];}
	float& w(){return values[3];}

	float x() const {return values[0];}
	float y() const {return values[1];}
	float z() const {return values[2];}	
	float w() const {return values[3];}

	float& operator[](const int position)
	{
		PP_REQUIRE(position>=0 && position<=3,"Position exceeds size of array: " << position);
		return values[position];
	}
	
	float operator[](const int position) const 
	{
		PP_REQUIRE(position>=0 && position<=3,"Position exceeds size of array: " << position);
		return values[position];
	}
	
	/// print formated values to an std::ostream
	friend std::ostream& operator << (std::ostream& output, const Vec4f& vec);	
};


} //namespace ppogl

#endif // _PPOGL_VEC4F_H_
