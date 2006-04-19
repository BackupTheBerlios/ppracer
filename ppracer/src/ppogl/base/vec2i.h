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
 
#ifndef _PPOGL_VEC2I_H_
#define _PPOGL_VEC2I_H_

#include "assert.h"

///
namespace ppogl {
	
///
class Vec2i
{
public:
	int values[2];
	
	Vec2i()
	{	
		values[0]=0;
		values[1]=0;
	}

	Vec2i(const int _x, const int _y)
	{
		values[0]=_x;
		values[1]=_y;
	}	

	int& x(){return values[0];}
	int& y(){return values[1];}
	
	int x() const {return values[0];}
	int y() const {return values[1];}	

	int& operator[](const int position)
	{
		PP_REQUIRE(position>=0 && position<=1,"Position exceeds size of array: " << position);
		return values[position];
	}	
	
	int operator[](const int position) const 
	{
		PP_REQUIRE(position>=0 && position<=1,"Position exceeds size of array: " << position);
		return values[position];
	}
		
	friend Vec2i operator+(const Vec2i& vec1,const Vec2i& vec2)
	{
		return Vec2i(vec1.x()+vec2.x(),vec1.y()+vec2.y());
	}
	
	friend Vec2i operator+(const Vec2i& vec,const int value)
	{
		return Vec2i(vec.x()+value, vec.y()+value);
	}
		
	friend Vec2i operator-(const Vec2i& vec1,const Vec2i& vec2)
	{
		return Vec2i(vec1.x()-vec2.x(),vec1.y()-vec2.y());
	}
	
	friend Vec2i operator-(const Vec2i& vec,const int value)
	{
		return Vec2i(vec.x()-value, vec.y()-value);
	}
	
	friend Vec2i operator*(const Vec2i& vec,const int value)
	{
		return Vec2i(vec.x()*value, vec.y()*value);
	}
		
	friend Vec2i operator/(const Vec2i& vec,const int value)
	{
		return Vec2i(vec.x()/value, vec.y()/value);
	}
	
	friend Vec2i operator/(const int value, const Vec2i& vec)
	{
		return Vec2i(value/vec.x(), value/vec.y());
	}
	
	friend bool operator==(const Vec2i& vec1, const Vec2i& vec2)
	{
		return (vec1.x() == vec2.x()) && (vec1.y() == vec2.y());
	}
	
	friend bool operator!=(const Vec2i& vec1, const Vec2i& vec2)
	{
		return (vec1.x() != vec2.x()) || (vec1.y() != vec2.y());
	}
	
	/// print formated values to an std::ostream
	friend std::ostream& operator << (std::ostream& output, const Vec2i& vec);	
};

} //namespace ppogl

#endif // _PPOGL_VEC2D_H_
