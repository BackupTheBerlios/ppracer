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

#include "frame.h"

#include "../base/glstuff.h"

namespace ppogl{
	
Frame::Frame(int width, int height)
 : Widget()
{
	m_size.x()=width;
	m_size.y()=height;	
}

void
Frame::draw(const ppogl::Vec2d& position)
{
	ppogl::drawRectangle(position,m_size);
  	ppogl::drawRectangle(position+4, m_size-8,
						ppogl::Color(0.5, 0.6, 0.9));
}

} //namepsace ppogl
