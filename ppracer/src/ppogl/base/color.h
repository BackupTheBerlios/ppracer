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

#ifndef _PPOGL_COLOR_H_
#define _PPOGL_COLOR_H_

#include "assert.h"

namespace ppogl {

/// float based RGBA color class
class Color
{
public: 
	float values[4];
		
	Color(){
		values[0]=1.0f;
		values[1]=1.0f;
		values[2]=1.0f;
		values[3]=1.0f;
	}

	Color(const float red, const float green, const float blue, const float alpha=1.0f)
	{
		values[0]=red;
		values[1]=green;
		values[2]=blue;
		values[3]=alpha;
	}


	float& r(){return values[0];}
	float& g(){return values[1];}
	float& b(){return values[2];}
	float& a(){return values[3];}
	
	float r() const {return values[0];}
	float g() const {return values[1];}
	float b() const {return values[2];}
	float a() const {return values[3];}

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

	static const ppogl::Color black;
	static const ppogl::Color white;
	
	/// print formated values to an std::ostream
	friend std::ostream& operator << (std::ostream& output, const Color& color);
};

// unsigned char based RGBA color class
class Color4c
{
public: 
	unsigned char values[4];
	
	Color4c()
	{
		values[0]=255;
		values[1]=255;
		values[2]=255;
		values[3]=255;
	}

	Color4c(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha = 255)
	{
		values[0]=red;
		values[1]=green;
		values[2]=blue;
		values[3]=alpha;
	}
	
	unsigned char& r(){return values[0];}
	unsigned char& g(){return values[1];}
	unsigned char& b(){return values[2];}
	unsigned char& a(){return values[3];}
	
	unsigned char r() const {return values[0];}
	unsigned char g() const {return values[1];}
	unsigned char b() const {return values[2];}
	unsigned char a() const {return values[3];}
	
	unsigned char& operator[](const int position)
	{
		PP_REQUIRE(position>=0 && position<=3,"Position exceeds size of array: " << position);
		return values[position];
	}
	
	unsigned char operator[](const int position) const 
	{
		PP_REQUIRE(position>=0 && position<=3,"Position exceeds size of array: " << position);
		return values[position];
	}
	
	///returns integer for the RGB part of the color
	inline unsigned int getRGB(){return r() + (g()<<8) + (b()<<16);}
	
	///returns integer value for this color
	inline unsigned int getRGBA(){return r() + (g()<<8) + (b()<<16) + (a()<<24);}
	
	bool compareRGB(const Color4c& color)
	///compares two colors without alpha
	{
		return (r()==color.r()) && (g()==color.g()) && (b()==color.b());
	}
		
	static const ppogl::Color4c black;
	static const ppogl::Color4c white;
		
	/// print formated values to an std::ostream
	friend std::ostream& operator << (std::ostream& output, const Color4c& color);
};

} // namespace ppogl

#endif // _PPOGL_COLOR_H_
