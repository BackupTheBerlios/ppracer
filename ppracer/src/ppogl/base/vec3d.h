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
 
#ifndef _PPOGL_VEC3D_H_
#define _PPOGL_VEC3D_H_

#include "assert.h"

namespace ppogl {

///A 3D vector class
class Vec3d
{
public:
	double values[3];
		
	Vec3d();
	Vec3d(const double x, const double y, const double z);

	double& x(){return values[0];};
	double& y(){return values[1];};
	double& z(){return values[2];};
	
	double x() const {return values[0];};
	double y() const {return values[1];};	
	double z() const {return values[2];};	

	double& operator[](const int position)
	{
		PP_REQUIRE(position>=0 && position<=2,"Position exceeds size of array: " << position);
		return values[position];
	}
	
	double operator[](const int position) const 
	{
		PP_REQUIRE(position>=0 && position<=2,"Position exceeds size of array: " << position);
		return values[position];
	}
		
	double normalize();
	double length() const;
	double length2() const;

	friend Vec3d operator+(const Vec3d& vec1,const Vec3d& vec2);
	friend Vec3d operator-(const Vec3d& vec1,const Vec3d& vec2);
	friend Vec3d operator*(const Vec3d& vec, const double scalar);
	friend Vec3d operator*(const double scalar, const Vec3d& vec);
	friend double operator*(const Vec3d& vec1,const Vec3d& vec2);
	friend Vec3d operator^(const Vec3d& vec1,const Vec3d& vec2);

	/// print formated values to an std::ostream
	friend std::ostream& operator << (std::ostream& output, const Vec3d& vec);
};


} //namespace ppogl

#endif // _PPOGL_VEC3D_H_
