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
 
#ifndef _PPOGL_VEC3D_H_
#define _PPOGL_VEC3D_H_

#include "assert.h"
#include "defs.h"

namespace ppogl {

///A 3D vector class
class Vec3d
{
public:
	double values[3];
		
	Vec3d()
	{
		values[0]=0.0;
		values[1]=0.0;
		values[2]=0.0;
	}
	
	Vec3d(const double x, const double y, const double z)
	{
		values[0]=x;
		values[1]=y;
		values[2]=z;	
	}	

	double& x(){return values[0];}
	double& y(){return values[1];}
	double& z(){return values[2];}
	
	double x() const {return values[0];}
	double y() const {return values[1];}
	double z() const {return values[2];}

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
		
	double normalize()
	{
    	const double len = length();
		if(len>0.0){
			x() /= len;
			y() /= len;
			z() /= len;
		}                
		return len;
	}
	
	double length2() const
	{
		return x()*x()+y()*y()+z()*z();
	}
	
	double length() const
	{
		return sqrt(length2());
	}

	friend Vec3d operator+(const Vec3d& vec1,const Vec3d& vec2)
	{
		return Vec3d(vec1.x()+vec2.x(),vec1.y()+vec2.y(),vec1.z()+vec2.z());
	}
	
	Vec3d& operator+=(const Vec3d& vec)
    {
		values[0] += vec.values[0];
		values[1] += vec.values[1];
 		values[2] += vec.values[2];
		return *this;
	}
	
	friend Vec3d operator-(const Vec3d& vec1,const Vec3d& vec2)
	{
		return Vec3d(vec1.x()-vec2.x(),vec1.y()-vec2.y(),vec1.z()-vec2.z());
	}
	
	Vec3d& operator-=(const Vec3d& vec)
    {
		values[0] -= vec.values[0];
		values[1] -= vec.values[1];
 		values[2] -= vec.values[2];
		return *this;
	}
	
	friend Vec3d operator*(const Vec3d& vec, const double scalar)
	{
		return Vec3d(vec.x()*scalar,vec.y()*scalar,vec.z()*scalar);
	}
	
	friend Vec3d operator*(const double scalar, const Vec3d& vec)
	{
		return Vec3d(vec.x()*scalar,vec.y()*scalar,vec.z()*scalar);
	}	
	
	Vec3d& operator*=(const double scalar)
    {
		values[0] *= scalar;
		values[1] *= scalar;
 		values[2] *= scalar;
		return *this;
	}
		
	friend double operator*(const Vec3d& vec1,const Vec3d& vec2)
	{
		return vec1.x()*vec2.x()+vec1.y()*vec2.y()+vec1.z()*vec2.z();
	}

	friend Vec3d operator^(const Vec3d& vec1,const Vec3d& vec2)
	{
		return Vec3d(
			vec1.y() * vec2.z() - vec1.z() * vec2.y(),
			vec1.z() * vec2.x() - vec1.x() * vec2.z(),
			vec1.x() * vec2.y() - vec1.y() * vec2.x()
		);
	}
	
	/// print formated values to an std::ostream
	friend std::ostream& operator << (std::ostream& output, const Vec3d& vec);
	
	static inline const Vec3d origin(){return Vec3d();}
};

} //namespace ppogl

#endif // _PPOGL_VEC3D_H_
