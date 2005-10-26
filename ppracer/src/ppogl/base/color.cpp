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

#include "color.h"

namespace ppogl{

	
Color::Color()
{
	values[0]=1.0f;
	values[1]=1.0f;
	values[2]=1.0f;
	values[3]=1.0f;
}
	
Color::Color(const float red, const float green, const float blue, const float alpha)
{
	values[0]=red;
	values[1]=green;
	values[2]=blue;
	values[3]=alpha;
}

Color::Color(const float *color)
{
	values[0]=color[0];
	values[1]=color[1];
	values[2]=color[2];
	values[3]=1.0;
}

Color::Color(const double *color)
{
	values[0]=color[0];
	values[1]=color[1];
	values[2]=color[2];
	values[3]=1.0;	
}

void
Color::set(const float *color)
{
	values[0]=color[0];
	values[1]=color[1];
	values[2]=color[2];
	values[3]=color[3];	
}

void
Color::set(const double *color)
{
	values[0]=color[0];
	values[1]=color[1];
	values[2]=color[2];
	values[3]=color[3];	
}

std::ostream& operator << (std::ostream& output, const Color& color)
{
	output 	<< color.values[0] << ":"
			<< color.values[1] << ":"
			<< color.values[2] << ":"
			<< color.values[3];		
	return output;
}

const Color Color::black(0.0f,0.0f,0.0f);
const Color Color::white(1.0f,1.0f,1.0f);

Color4c::Color4c()
{
	values[0]=255;
	values[1]=255;
	values[2]=255;
	values[3]=255;
}

Color4c::Color4c(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha)
{
	values[0]=red;
	values[1]=green;
	values[2]=blue;
	values[3]=alpha;
}

Color4c::Color4c(const unsigned char *color)
{	
	values[0]=color[0];
	values[1]=color[1];
	values[2]=color[2];
	values[3]=255;
}

void
Color4c::set(const unsigned char *color)
{
	values[0]=color[0];
	values[1]=color[1];
	values[2]=color[2];
	values[3]=color[3];
}

std::ostream& operator << (std::ostream& output, const Color4c& color)
{
	output 	<< int(color.values[0]) << ":"
			<< int(color.values[1]) << ":"
			<< int(color.values[2]) << ":"
			<< int(color.values[3]);		
	return output;
}

const Color4c Color4c::black(0,0,0);
const Color4c Color4c::white(255,255,255);

} //namespace ppogl
