/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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

#ifndef _PPOGL_TEXTAREA_H_
#define _PPOGL_TEXTAREA_H_

#include "widget.h"

#include "imagebutton.h"
#include "../font.h"

#include <list>

namespace ppogl{
	
/// A widget displaying muliple Lines of text
class Textarea : public Widget
{	
protected:
	/// the font used for this texture area
	ppogl::FontRef m_font;

	/// the raw text of this object
	std::string m_text;

	/// a list containing the lines of the text
	std::list< std::string > m_lines;

	/// an iterator that points to the first displayed line
	std::list< std::string >::iterator m_top;
	
	/// the up button widget
	ppogl::ImageButton m_upBtn;

	/// the down button widget
	ppogl::ImageButton m_downBtn;
	
public:
	Textarea(int width=256, int height=128);

	virtual void draw(const ppogl::Vec2d& position);
	void setPosition(const ppogl::Vec2d& position);

	void updateButtonStates();
	void updateButtonPositions();

	void upButtonCallback();
	void downButtonCallback();

	void setText(const std::string& text);
	void setFont(const std::string& binding);
};

} //namepsace ppogl

#endif // _PPOGL_TEXTAREA_H_
