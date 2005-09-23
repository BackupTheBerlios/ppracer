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

#include "label.h"

#include "uitheme.h"

namespace ppogl{
	
Label::Label()
 : Widget()
{
	m_font = ppogl::UITheme::getInstance().labelFont;
	m_insensitiveFont = ppogl::UITheme::getInstance().insensitiveLabelFont;
}

Label::Label(const std::string& text,const std::string binding)
 : Widget(),
   m_text(text)
{
	if(binding!=""){
		m_font = ppogl::FontMgr::getInstance().get(binding);
	}
	
	if(!m_font){
		m_font = ppogl::UITheme::getInstance().labelFont;
	}
	
	if(m_font){
		m_size.x() = m_font->advance(text);
		m_size.y() = m_font->ascender();	
	}

	m_insensitiveFont = ppogl::UITheme::getInstance().insensitiveLabelFont;
}

void
Label::setFont(const std::string& binding)
{
	m_font = ppogl::FontMgr::getInstance().get(binding);
	if(!m_font){
		PP_WARNING("Unable to set font for binding: " << binding);
		m_font = ppogl::UITheme::getInstance().labelFont;	
	}
}

void
Label::setFont(ppogl::FontRef font)
{
	m_font = font;	
}

void
Label::setText(const std::string& text)
{
	m_text = text;
	if(m_font){
		m_size.x() = m_font->advance(text);
		m_size.y() = m_font->ascender();	
	}
}


void
Label::draw(const ppogl::Vec2d& position)
{
	if(m_insensitive && m_insensitiveFont){
		m_insensitiveFont->draw(m_text,position);
	}else if(m_font){
		m_font->draw(m_text,position);				
	}
}

} //namepsace ppogl
