/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
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

#ifndef _PPOGL_SG_VNC_H_
#define _PPOGL_SG_VNC_H_

#include "../base/glwrappers.h"

namespace ppogl{
	
/// class for managing a set of vertex, color and normal arrays
class VNCArray
{
private:	
	
	/// float array for storing the vertixes
	GLfloat* mp_vertices;
	
	/// float array for storing the normals
	GLfloat* mp_normals;
	
	/// byte array for storing the color values
	GLubyte* mp_colors;

public:
	VNCArray(int size);
	~VNCArray();
		
	/// set vnc pointers and enable the client states
	void enable();

	/// disable the client states
	static void disable();

	GLubyte& colorValue(int index, int offset)
	{
		return mp_colors[index*4+offset];
	}
		
	void setVertex(int index, float x, float y, float z)
	{
		const int base=index*3;
		mp_vertices[base]=x;
		mp_vertices[base+1]=y;
		mp_vertices[base+2]=z;
	}
	
	void setNormal(int index, const ppogl::Vec3d& normal)
	{
		const int base=index*3;
		mp_normals[base]=normal.x();
		mp_normals[base+1]=normal.y();
		mp_normals[base+2]=normal.z();
	}

	void setColor(int index, int red, int green, int blue, int alpha)
	{
		const int base=index*4;
		mp_colors[base]=red;
		mp_colors[base+1]=green;
		mp_colors[base+2]=blue;
		mp_colors[base+3]=alpha;
	}
};

} // namespace ppogl

#endif // _PPOGL_SG_VNC_H_
