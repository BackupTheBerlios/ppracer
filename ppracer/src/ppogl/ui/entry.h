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

#ifndef _PPOGL_ENTRY_H_
#define _PPOGL_ENTRY_H_

#include "widget.h"
#include "../font.h"

#include <SDL.h>

namespace ppogl{
	
/// A text entry widget
class Entry
 : public Widget
{
protected:
	ppogl::FontRef m_font;
	std::string m_text;
	unsigned int m_maxchars;
	bool m_editable;

public:
	Entry(int width=176);

	bool keyboardEvent(SDLKey key, SDLMod mod, bool release);
	virtual void draw(const ppogl::Vec2d& position);
	void setClicked(bool clicked);


	void setText(const std::string& text);
	inline std::string& getText(){return m_text;};
	
	void setFont(const std::string& binding);

	inline void setMaxChars(unsigned int maxchars){m_maxchars=maxchars;};
	inline void setEditable(bool editable=true){m_editable=editable;};

	//signals
	ppogl::Signal2<ppogl::Entry*,SDLKey> signalKeyPressed;
};

} //namepsace ppogl

#endif // _PPOGL_ENTRY_H_
