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

#ifndef _PP_COLOR_H
#define _PP_COLOR_H

namespace pp {

/// Color
class Color
{
public:

	float r,g,b,a;
	
	Color(const float red = 0.0f, const float green = 0.0f, const float blue = 0.0f, const float alpha=1.0f);	
	Color(const float *color);
	Color(const double *color);
		
	void set(const float *color);
	void set(const double *color);
	
	static const pp::Color black;
	static const pp::Color white;
};

} //namespace pp


#endif // _PP_COLOR_H
