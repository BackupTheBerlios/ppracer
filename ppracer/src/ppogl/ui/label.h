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

#ifndef _PPOGL_LABEL_H_
#define _PPOGL_LABEL_H_

#include "widget.h"

#include "../font.h"

namespace ppogl{
	
/// A simple text label widget
class Label : public Widget
{
protected:
	ppogl::FontRef m_font;
	ppogl::FontRef m_insensitiveFont;
	std::string m_text;
	
public:
	Label();
	Label(const std::string& text, const std::string binding="");

	virtual void draw(const ppogl::Vec2d& position);

	void setText(const std::string& text);
	std::string& getText(){return m_text;};	

	void setFont(const std::string& binding);
	void setFont(ppogl::FontRef font);
};


} //namepsace ppogl

#endif // _PPOGL_LABEL_H_
