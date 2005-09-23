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

#ifndef _PPOGL_UIMGR_H_
#define _PPOGL_UIMGR_H_

#include "../base/singleton.h"
#include "../base/vec2d.h"

#include <SDL.h>
#include <list>

namespace ppogl{

//no need to include widget.h
class Widget;	
	
/// A singleton managing the ui widgets
class UIManager : public Singleton<UIManager>
{
	std::list<ppogl::Widget*> m_widgets;	
	ppogl::Widget* mp_focusedWidget;
	
	ppogl::Vec2d m_cursorPosition;
	ppogl::Vec2d m_boxDimension;
	ppogl::Vec2d m_boxStart;
	ppogl::Vec2d m_boxEnd;
		
	void drawCursor();
	
	ppogl::Vec2d getWidgetPosition(ppogl::Vec2d position);	
	
public:
	UIManager();
	~UIManager();

	void draw(int width, int height, bool decoration=true);	

	void add(ppogl::Widget *widget);
	void remove(ppogl::Widget *widget);

	void mouseEvent(int x, int y);
	void mouseButtonEvent(int button, int state);
	bool keyboardEvent(SDLKey key, SDLMod mod, bool release);

	/// set the dimension of the box in which the ui gets drawn
	inline void setBoxDimension(const ppogl::Vec2d& dimension){m_boxDimension=dimension;};

	void grabFocus(ppogl::Widget* widget, bool focus);
	
	const ppogl::Vec2d& getCursorPosition(){return m_cursorPosition;};
};	

} //namepsace ppogl

#endif // _PPOGL_UIMGR_H_
