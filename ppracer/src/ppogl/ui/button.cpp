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

#include "button.h"
#include "uitheme.h"

namespace ppogl{
	
Button::Button()
 : Label()
///construct plain button object
{
	m_hilitFont = ppogl::UITheme::getInstance().hilitLabelFont;		
}
	
Button::Button(const std::string& text, const std::string& binding)
 : Label(text,binding)
///construct button object with the given text and binding
{	
	m_hilitFont = ppogl::UITheme::getInstance().hilitLabelFont;
}

void
Button::draw(const ppogl::Vec2d& position)
///draw button at the specified position
{
	if(m_insensitive){
		if(m_insensitiveFont){
			m_insensitiveFont->draw(m_text,position);				
		}else if(m_font){
			m_font->draw(m_text,position);				
		}
	}else if(m_focused){
		if(m_hilitFont){
			m_hilitFont->draw(m_text,position);				
		}else if(m_font){
			m_font->draw(m_text,position);				
		}
	}else if(m_font){
		m_font->draw(m_text,position);				
	}	
}

} //namepsace ppogl
