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
 
#ifndef _PPOGL_VEC3F_H
#define _PPOGL_VEC3F_H

#include "assert.h"

namespace ppogl {

///A 3D vector class
class Vec3f
{
public:
	float values[3];	

	float& x(){return values[0];};
	float& y(){return values[1];};
	float& z(){return values[2];};
	
	float x() const {return values[0];};
	float y() const {return values[1];};	
	float z() const {return values[2];};	
	
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
	
	Vec3f();
	Vec3f(const float x, const float y, const float z);
	Vec3f(const float *v);	
	
	void set(const float *v);
	void set(const double *v);
	
	
	/// print formated values to an std::ostream
	friend inline std::ostream& operator << (std::ostream& output, const Vec3f& vec)
	{
		output 	<< vec.values[0] << ":"
				<< vec.values[1] << ":"
				<< vec.values[2];		
		return output;
	}	
};


} //namespace ppogl

#endif // _PPOGL_VEC3F_H
