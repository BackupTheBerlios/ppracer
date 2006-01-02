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
 
#include "alignment.h"

namespace ppogl{

Alignment::Alignment()
 : m_horizontal(0.0f),
   m_vertical(0.0f)
{	
}
	
Alignment::Alignment(float horizontal, float vertical)
 : m_horizontal(horizontal),
   m_vertical(vertical)
{	
	checkHorizontal();
	checkVertical();
}

void
Alignment::checkHorizontal()
///ensure 0.0f <= m_horizontal <= 1.0f 
{
	if(m_horizontal>1.0f){
		m_horizontal = 1.0f;
	}else if(m_horizontal<0.0f){
		m_horizontal = 0.0f;
	}
}
	
void
Alignment::checkVertical()
///ensure 0.0f <= m_vertical <= 1.0f 
{
	if(m_vertical>1.0f){
		m_vertical = 1.0f;
	}else if(m_vertical<0.0f){
		m_vertical = 0.0f;
	}
}

void
Alignment::setHorizontal(float horizontal)
///sets the horizontal alignment
{
	m_horizontal = horizontal;
	checkHorizontal();
	signalChanged.Emit();
}

void
///sets the vertical alignment
Alignment::setVertical(float vertical)
{
	m_vertical = vertical;
	checkVertical();
	signalChanged.Emit();	
}

void
Alignment::set(float horizontal, float vertical)
///set horizontal and vertical alignment
{
	setHorizontal(horizontal);
	setVertical(vertical);	
	signalChanged.Emit();
}	

ppogl::Vec2d
Alignment::alignPosition( const ppogl::Vec2d &position,
						  const ppogl::Vec2d &size) const
///returns the new position in respect to the size and the alignment
{
	return ppogl::Vec2d(
		position.x() - (m_horizontal*size.x()),
		position.y() - (m_vertical*size.y())
	);	
}

} //namepsace ppogl
