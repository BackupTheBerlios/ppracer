/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#ifndef _CREDITS_H_
#define _CREDITS_H_

#include "loop.h"
#include "ppogl/font.h"

class Credits
 : public GameMode
{	
	struct CLine{
		ppogl::FontRef font;
		std::string text;
	};
	
	CLine *lines;
	double m_yOffset;
	
	void drawText(float timestep);

public:
	Credits();
	~Credits();

	void loop(float timestep);
	bool mouseButtonReleaseEvent(int button, const ppogl::Vec2i& position);
	bool keyPressEvent(SDLKey key);
};

#endif // _CREDITS_H_
