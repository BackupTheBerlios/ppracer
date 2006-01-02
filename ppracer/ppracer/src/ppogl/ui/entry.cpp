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

#include "entry.h"

#include "uitheme.h"
#include "uimgr.h"

#include "../base/glstuff.h"

namespace ppogl{
	
Entry::Entry(int width)
 : Widget()
{
	m_size.x()=width;
	m_size.y()=32;
	m_font = ppogl::UITheme::getInstance().entryFont;
}
	
void
Entry::setFont(const std::string& binding)
{
	m_font = ppogl::FontMgr::getInstance().get(binding);
	if(!m_font){
		PP_WARNING("Unable to set font for binding: " << binding);
		m_font = ppogl::UITheme::getInstance().entryFont;	
	}
}

void
Entry::setText(const std::string& text)
{
	m_text = text;
}

void
Entry::draw(const ppogl::Vec2d& position)
{
	if(m_hasFocus){
		ppogl::drawRectangle(position,m_size,ppogl::Color(1.0, 0.9, 0.0));
	}else{	
		ppogl::drawRectangle(position,m_size,ppogl::Color(1.0, 1.0, 1.0));
	}
	ppogl::drawRectangle(position+ppogl::Vec2d(4,4),
						 m_size-ppogl::Vec2d(8,8),
						 ppogl::Color(0.5, 0.6, 0.9));
	if(m_font){
		m_font->draw(m_text, position+ppogl::Vec2d(8,8));
	}
}

void
Entry::setClicked(bool clicked)
{
	if(clicked && !m_hasFocus){
		m_hasFocus=true;
		ppogl::UIManager::getInstance().grabFocus(this,true);
	}
	Widget::setClicked(clicked);
}

bool
Entry::keyboardEvent(SDLKey key, SDLMod mod, bool release)
{
	if(!release){
		if(m_editable){
			if( islower(key) ){
				if(m_text.size()<m_maxchars){
					if(mod & KMOD_SHIFT){
						m_text+=toupper(key);
					}else{
						m_text+=key;
					}
				}
			}else if( isprint(key) ){
				if(m_text.size()<m_maxchars){
					m_text+=key;
				}
			}else if( key == SDLK_BACKSPACE){
				if(!m_text.empty()){
					std::string::iterator it = m_text.end();
					it--;
					m_text.erase(it);	
				}
			}
		}else{
			signalKeyPressed.Emit(this,key);
		}
	}
	return true;
}

} //namepsace ppogl
