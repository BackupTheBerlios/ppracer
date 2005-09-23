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

#ifndef _PPOGL_IMAGEBUTTON_H_
#define _PPOGL_IMAGEBUTTON_H_

#include "widget.h"

#include "../textures.h"
#include "../base/vec4f.h"

#include <map>

namespace ppogl{
	
/// A widget displaying an image and with support for state switch and highlighting
class ImageButton : public Widget
{
public:
	enum Mode{
		MODE_NONE  = 0 << 0,
		MODE_CYCLE = 1 << 1,
		MODE_HILIT = 2 << 2		
	}m_mode;	
	
protected:
	ppogl::TextureRef m_texture;
		
	std::map<int, ppogl::Vec4f> m_states;	
	std::map<int, ppogl::Vec4f>::iterator m_it;
	
	int m_hilitState;
	unsigned int m_border;
	
public:
	ImageButton(ppogl::Vec2d size = ppogl::Vec2d(32,32));

	void addState(int state, ppogl::Vec4f texcoords);
	void setTexture(ppogl::TextureRef texture);
	void setTexture(const std::string& binding);

	int getState();
	void setState(int state);
	void setHilitState(int state);

	void setBorder(unsigned int size=0);

	inline void setMode(Mode mode){m_mode=mode;};

	virtual void draw(const ppogl::Vec2d& position);
	virtual void setClicked(bool clicked=true);
};

} //namepsace ppogl

#endif // _PPOGL_IMAGEBUTTON_H_
