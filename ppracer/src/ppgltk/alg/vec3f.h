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
 
#ifndef _PP_VEC3F_H
#define _PP_VEC3F_H

///
namespace pp {

///A 3D vector class
class Vec3f
{
public:
	Vec3f();
	Vec3f(const float x, const float y, const float z);
	Vec3f(const float *v);	

	union {
		struct{
			float x,y,z;
		};
		float values[3];
	};
	
	void set(const float *v);
	void set(const double *v);
};


} //namespace pp

#endif // _PP_VEC3F_H