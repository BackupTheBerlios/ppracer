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

#ifndef _PPOGL_BUTTON_H_
#define _PPOGL_BUTTON_H_

#include "label.h"

namespace ppogl{
	
/// A button widget
class Button
 : public Label
{
	/// the font used when button is highlighted	
	ppogl::FontRef m_hilitFont;
	
public:
	Button();
	Button(const std::string& text, const std::string& binding="");

	virtual void draw(const ppogl::Vec2d& position);
};

} //namepsace ppogl

#endif // _PPOGL_BUTTON_H_