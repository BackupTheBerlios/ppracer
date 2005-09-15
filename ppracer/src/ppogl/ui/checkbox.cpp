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

#include "checkbox.h"

#include "uitheme.h"
#include "../base/glstuff.h"

namespace ppogl{
	
CheckBox::CheckBox()
 : Widget(),
   m_selected(false)
{
	m_size.x()=32;
	m_size.y()=32;
	signalClicked.Connect(ppogl::CreateSlot(this,&CheckBox::clicked));	
}

void
CheckBox::draw(ppogl::Vec2d position)
{
	ppogl::TextureRef texture =
		ppogl::UITheme::getInstance().checkBoxTex;
	
	if(m_selected){
		ppogl::drawRectangle(position, m_size, texture,
					ppogl::Vec4f(0.0,0.0,0.5,0.5));
	}else{
		ppogl::drawRectangle(position, m_size, texture,
					ppogl::Vec4f(0.5,0.5,1.0,1.0));
	}	
}

void
CheckBox::clicked()
{
	m_selected=!m_selected;	
}

} //namepsace ppogl
