/* 
 * PPGLTK
 *
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

namespace pp{

Color::Color(const float red, const float green, const float blue, const float alpha)
 : r(red), g(green), b(blue), a(alpha)
{
}

Color::Color(const float *color)
 : r(color[0]), g(color[1]), b(color[2]), a(1.0)
{
}

Color::Color(const double *color)
 : r(color[0]), g(color[1]), b(color[2]), a(1.0)
{
}

void
Color::set(const float *color)
{
	r=color[0];
	g=color[1];
	b=color[2];
	a=color[3];
}

void
Color::set(const double *color)
{
	r=color[0];
	g=color[1];
	b=color[2];
	a=color[3];
}

const Color Color::black(0.0f,0.0f,0.0f);
const Color Color::white(1.0f,1.0f,1.0f);

} //namespace pp
