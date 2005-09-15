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

#ifndef _PPOGL_WIDGET_H_
#define _PPOGL_WIDGET_H_

#include "../base/vec2d.h"
#include "../base/signal.h"
#include "../base/assert.h"
#include "alignment.h"

#include <SDL.h>

namespace ppogl{
	
/// The base class for all widgets
class Widget
{
protected:
	///the position of the Widget
	ppogl::Vec2d m_position;

	///the size of the widget
	ppogl::Vec2d m_size;

	///Whether the widget is focused
	bool m_focused;

	///Whether the widget is clicked
	bool m_clicked;

	///Whether the widget has graped the focus
	bool m_hasFocus;

	///Whether the widget is insensitive
	bool m_insensitive;

public:
	Widget();
	virtual ~Widget();

	void setPosition(ppogl::Vec2d position);

	virtual void draw(ppogl::Vec2d position) = 0;

	inline ppogl::Vec2d& getSize(){return m_size;};
	inline ppogl::Vec2d& getPosition(){return m_position;};
	
	void removeFocus();
	inline bool hasFocus(){return m_hasFocus;};
	void setFocus(bool focus=true);
	
	bool isInsensitive(){return m_insensitive;};
	void setInsensitive(bool insensitive=true){m_insensitive=insensitive;};
	
	inline void setFocused(bool focused=true){m_focused=focused;};
	inline bool isFocused(){return m_focused;};
	
	virtual void setClicked(bool clicked=true);

	virtual void wheelUpEvent();
	virtual void wheelDownEvent();
	
	///default handlet for keyboard events
	virtual bool keyboardEvent(SDLKey key, SDLMod mod, bool release){return false;};
	
	///controls the alignment of the widget
	ppogl::Alignment alignment;

	//signals
	ppogl::Signal0 signalClicked;
	ppogl::Signal0 signalMouseWheelUp;
	ppogl::Signal0 signalMouseWheelDown;
};


} //namepsace ppogl

#endif // _PPOGL_WIDGET_H_
