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

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "loop.h"

#include "ppogl/ui.h"

class Configuration
 : public GameMode
{
	ppogl::Label m_titleLbl;
	ppogl::Button m_generalBtn;
	ppogl::Button m_graphicsBtn;
	ppogl::Button m_videoBtn;
	ppogl::Button m_audioBtn;
	ppogl::Button m_keyboardBtn;	
	ppogl::Button m_joystickBtn;	

	ppogl::Button m_backBtn;
	
public:
	Configuration();

	void loop(float timeStep);

	bool keyPressEvent(SDLKey key);

	void back();
	void general();
	void graphics();
	void video();
	void audio();
	void keyboard();
	void joystick();
};

#endif // _CONFIGURATION_H_
