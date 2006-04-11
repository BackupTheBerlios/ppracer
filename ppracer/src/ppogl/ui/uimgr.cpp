/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005-2006 Volker Stroebel <volker@planetpenguin.de>
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
 
#include "uimgr.h"

#include "ui/widget.h"
#include "ui/uitheme.h"

#include "../textures.h"
#include "../base/glstuff.h"

#include <SDL.h>

namespace ppogl{
	
PPOGL_SINGLETON(UIManager);	
	
UIManager::UIManager()
 : mp_focusedWidget(NULL),
   m_boxDimension(-1.0, -1.0)
{
}

UIManager::~UIManager()
{
}
	
void
UIManager::draw(const ppogl::Vec2i& resolution, bool decoration)
/// Initializes the screen and draw widget, mouse cursor and decoration
{
	
	gl::PushMatrix();
	{
	gl::MatrixMode(GL_PROJECTION);
    gl::LoadIdentity();
    gl::Ortho(0.0, resolution.x(), 0.0, resolution.y(), -1.0, 1.0);
    gl::MatrixMode(GL_MODELVIEW);
    gl::LoadIdentity();
    gl::Translate(0.0, 0.0, -1.0);
    gl::Color(ppogl::Color::white);
			
	if(decoration){
		ppogl::UITheme::getInstance().drawDecoration(resolution);
	}
	
	if(m_boxDimension.x()<=0.0){
		m_boxStart.x() = 0;
		m_boxEnd.x()   = resolution.x();
	}else{
		m_boxStart.x() = (resolution.x()-m_boxDimension.x())/2;
		m_boxEnd.x()   = m_boxStart.x() + m_boxDimension.x();
	}
	
	if(m_boxDimension.y()<=0.0){
		m_boxStart.y() = 0;
		m_boxEnd.y()   = resolution.y();
	}else{
		m_boxStart.y() = (resolution.y()-m_boxDimension.y())/2;
		m_boxEnd.y()   = m_boxStart.y() + m_boxDimension.y();
	}
		
	std::list<ppogl::Widget*>::iterator it;
	for(it=m_widgets.begin();it!=m_widgets.end();it++){
		ppogl::Vec2d position = 
			(*it)->alignment.alignPosition((*it)->getPosition(),
										   (*it)->getSize());
		
		(*it)->draw(getWidgetPosition(position));
	}
	drawCursor();	
	
	}
	gl::PopMatrix();
}
	
void 
UIManager::add(ppogl::Widget *widget)
/// add widget to the ui manager
{
	m_widgets.push_back(widget);
}

void
UIManager::remove(ppogl::Widget *widget)
/// remove image from the ui manager
{	
	if(mp_focusedWidget==widget){
		mp_focusedWidget=NULL;
	}	
	m_widgets.remove(widget);
}

void
UIManager::mouseEvent(const ppogl::Vec2i& position)
/// update mouse position and the focus state of the widgets
{
	m_cursorPosition = position;
	
	std::list<ppogl::Widget*>::iterator it;
	for(it=m_widgets.begin();it!=m_widgets.end();it++){
		const ppogl::Vec2d &size = (*it)->getSize();
		ppogl::Vec2d position = 
			getWidgetPosition((*it)->alignment.alignPosition(
								(*it)->getPosition(),
								(*it)->getSize()));
		
		(*it)->setFocused(
		    ( m_cursorPosition.x() >= position.x() ) &&
			( m_cursorPosition.x() <= position.x() + size.x() ) &&
			( m_cursorPosition.y() >= position.y() ) &&
			( m_cursorPosition.y() <= position.y() + size.y() ));
	}
}

void
UIManager::mouseButtonEvent(int button, int state)
/// handels mouse button and scrollwheel events
{
	std::list<ppogl::Widget*>::iterator it;
	for(it=m_widgets.begin();it!=m_widgets.end();it++){
		if(button==SDL_BUTTON_WHEELUP && state == SDL_PRESSED && (*it)->isFocused()){
			(*it)->wheelUpEvent();
		}else if(button==SDL_BUTTON_WHEELDOWN && state == SDL_PRESSED && (*it)->isFocused()){
			(*it)->wheelDownEvent();
		}else if(button==SDL_BUTTON_LEFT){		
			(*it)->setClicked(state==SDL_PRESSED && (*it)->isFocused());
		}	
	}
}

void
UIManager::drawCursor()
/// draws the mouse cursor
{
    ppogl::TextureRef texture =
		ppogl::TextureMgr::getInstance().get("mouse_cursor");

	ppogl::drawRectangle(ppogl::Vec2d(m_cursorPosition.x(),m_cursorPosition.y()-32),
						 ppogl::Vec2d(32,32),texture);
}
	
ppogl::Vec2d
UIManager::getWidgetPosition(ppogl::Vec2d position)
/// return position of the widget on the screen in respect to the ui box
{
	if(position.x()>=0.0){
		position.x() = m_boxStart.x() + position.x();			
	}else{
		position.x() = m_boxEnd.x() + 1 + position.x(); 	
	}
		
	if(position.y()>=0.0){
		position.y() = m_boxStart.y() + position.y();			
	}else{
		position.y() = m_boxEnd.y() + 1 + position.y(); 	
	}
	return position;	
}

void
UIManager::grabFocus(ppogl::Widget *widget, bool focus)
/// gives widget the focus for keyboard events
{
	if(focus){
		if(mp_focusedWidget!=NULL){
			mp_focusedWidget->removeFocus();
		}
		mp_focusedWidget=widget;
	}else{
		if(mp_focusedWidget==widget){
			mp_focusedWidget=NULL;
		}
	}
}

bool
UIManager::keyboardEvent(SDLKey key, SDLMod mod, bool release)
/// handels keyboard events and returns falls if event is not handeled
{
	if(mp_focusedWidget!=NULL){
		return mp_focusedWidget->keyboardEvent(key, mod, release);	
	}
	return false;
}

} //namepsace ppogl
