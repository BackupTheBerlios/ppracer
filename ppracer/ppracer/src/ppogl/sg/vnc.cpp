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
 
#include "vnc.h"

namespace ppogl{
	
VNCArray::VNCArray(int size)
{
	mp_vertices = new GLfloat[size*3];
	mp_normals = new GLfloat[size*3];
	mp_colors = new GLubyte[size*4];
}
	
VNCArray::~VNCArray()
{
	disable();
	
	if(mp_vertices){
		delete [] mp_vertices;
	}
	
	if(mp_normals){
		delete [] mp_normals;
	}
		
	if(mp_colors){
		delete [] mp_colors;
	}
}

void
VNCArray::enable()
{
   	gl::EnableClientState(GL_VERTEX_ARRAY);
   	gl::VertexPointer(3, GL_FLOAT, 0, mp_vertices);
		
	gl::EnableClientState(GL_NORMAL_ARRAY);
   	gl::NormalPointer(GL_FLOAT, 0, mp_normals);
		
   	gl::EnableClientState(GL_COLOR_ARRAY);
   	gl::ColorPointer(4, GL_UNSIGNED_BYTE, 0, mp_colors);
}

void
VNCArray::disable()
{
	gl::DisableClientState(GL_VERTEX_ARRAY);
   	gl::DisableClientState(GL_NORMAL_ARRAY);
   	gl::DisableClientState(GL_COLOR_ARRAY);
}

} // namespace ppogl
