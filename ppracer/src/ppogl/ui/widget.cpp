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

#include "widget.h"

#include "uimgr.h"

namespace ppogl{
	
Widget::Widget()
 : m_focused(false),
   m_clicked(false),
   m_hasFocus(false),
   m_insensitive(false)
{
	//add self to UIManager to receive events and get drawn
	ppogl::UIManager::getInstance().add(this);
}
	
Widget::~Widget()
{
	//remove self from UIManager
	ppogl::UIManager::getInstance().remove(this);
}
	
void
Widget::setPosition(const ppogl::Vec2d& position)
///set the position of the widget
{
	m_position = position;	
}

void
Widget::setClicked(bool clicked)
///set the state of the mouse button and emit click signal if necessary
{
	/*
	 * if the widget is clicked, the button is released,
	 * the widget has the focus and is sensitive
	 * we emit the clicked signal.
	 * A little bit complicated but it seems to work :)
	 */	 
	if(m_clicked && !clicked && m_focused && !m_insensitive){
		signalClicked.Emit();
	}
	
	m_clicked=clicked;
}

void
Widget::wheelUpEvent()
///emit mousewheelup signal
{
    signalMouseWheelUp.Emit();
}

void
Widget::wheelDownEvent()
///emit mousewheeldown signal
{
    signalMouseWheelDown.Emit();
}

void
Widget::removeFocus()
///removes the focus from the widget
{
	m_hasFocus=false;	
}

void
Widget::setFocus(bool focus)
///set or unsets the focus if necessary
{
	if(focus && !m_hasFocus){
		m_hasFocus=true;
		ppogl::UIManager::getInstance().grabFocus(this,true);
	}else if(!focus && m_hasFocus){
		ppogl::UIManager::getInstance().grabFocus(this,false);
	}	
}

} //namepsace ppogl
