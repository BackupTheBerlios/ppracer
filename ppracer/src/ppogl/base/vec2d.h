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
 
#ifndef _PPOGL_VEC2D_H_
#define _PPOGL_VEC2D_H_

#include "assert.h"

///
namespace ppogl {
	
///
class Vec2d
{
public:
	double values[2];
	
	Vec2d();
	Vec2d(const double x, const double y);

	double& x(){return values[0];};
	double& y(){return values[1];};
	
	double x() const {return values[0];};
	double y() const {return values[1];};	

	double& operator[](const int position)
	{
		PP_REQUIRE(position>=0 && position<=1,"Position exceeds size of array: " << position);
		return values[position];
	}	
	
	double operator[](const int position) const 
	{
		PP_REQUIRE(position>=0 && position<=1,"Position exceeds size of array: " << position);
		return values[position];
	}
		
	friend Vec2d operator+(const Vec2d& vec1,const Vec2d& vec2);
	friend Vec2d operator+(const Vec2d& vec1,const double value);
	friend Vec2d operator-(const Vec2d& vec1,const Vec2d& vec2);
	friend Vec2d operator-(const Vec2d& vec1,const double value);

	/// print formated values to an std::ostream
	friend std::ostream& operator << (std::ostream& output, const Vec2d& vec);	
};

} //namespace ppogl

#endif // _PPOGL_VEC2D_H_
