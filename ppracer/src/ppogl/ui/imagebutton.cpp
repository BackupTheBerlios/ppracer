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

#include "imagebutton.h"

#include "../base/glstuff.h"

namespace ppogl{
	
ImageButton::ImageButton(ppogl::Vec2d size)
 : Widget(),
   m_mode(MODE_NONE),
   m_hilitState(-1),
   m_border(0)
{
	m_size=size;
}
	
void
ImageButton::addState(int state, ppogl::Vec4f texcoords)
{
	if(m_states.empty()){
		m_states[state]=texcoords;
		m_it=m_states.begin();
	}else{
		m_states[state]=texcoords;
	}
}

void
ImageButton::setTexture(ppogl::TextureRef texture)
{
	m_texture=texture;
}

void
ImageButton::setTexture(const std::string& binding)
{
	ppogl::TextureRef texture = 
		ppogl::TextureMgr::getInstance().get(binding);
	
	if(!texture){
		PP_WARNING("Unable to find texture for binding: " << binding);
	}else{
		m_texture=texture;		
	}
}

int
ImageButton::getState()
{
	if(m_states.empty() || (m_it == m_states.end())){
		return -1;
	}else{
		return (*m_it).first;	
	}
}

void
ImageButton::setState(int state)
{
	PP_REQUIRE(m_states.empty() != true,"Cannot set state because of empty map");
	
	std::map<int, ppogl::Vec4f>::iterator it;
	if((it = m_states.find(state))!=m_states.end()){
		m_it=it;		
	}else{
		PP_WARNING("Unable to find state: " << state);		
	}
}

void
ImageButton::setHilitState(int state)
{
	m_hilitState = state;	
}

void
ImageButton::draw(const ppogl::Vec2d& position)
{	
	if(m_border>0){
		ppogl::drawRectangle(position-ppogl::Vec2d(m_border,m_border),
							 m_size+Vec2d(m_border*2,m_border*2));				
	}
	
	if(m_states.empty() || m_it==m_states.end()){
		PP_WARNING("Unable to draw ImageButton: No states set");
	}else{
		std::map<int, ppogl::Vec4f>::iterator it;
		
		if(m_focused && (m_mode&MODE_HILIT)){
			if((it = m_states.find(m_hilitState))==m_states.end()){
				PP_WARNING("Unable to set highlight Texture: State " << m_hilitState);
				it=m_it;
			}			
		}else{
			it = m_it;
		}
		ppogl::drawRectangle(position, m_size, m_texture, (*it).second);		
	}
}
	
void
ImageButton::setClicked(bool clicked)
{
	if(clicked && (m_mode & MODE_CYCLE)){
		if(m_it == --m_states.end()){
			m_it=m_states.begin();			
		}else{
			m_it++;
		}	
	}
	Widget::setClicked(clicked);
}

void
ImageButton::setBorder(unsigned int size)
{
	m_border=size;
}



} //namepsace ppogl
