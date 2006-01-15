/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
 *
 * Copyright (C) 1999-2001 Jasmin F. Patry
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

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include "ppracer.h"

#ifdef HAVE_SDL_JOYSTICKOPEN

#include "SDL.h"
#include "SDL_joystick.h"

/// Class for handling Joysticks
class Joystick
{
	/// pointer to the used joystick
	SDL_Joystick* mp_joystick;

	/// number of buttons available for the joystick
	int m_numButtons;
	
	/// number of axes available for the joystick
	int m_numAxes;
	
	/// is joystick active
	bool m_active;

public:
	Joystick();

	/// initialize joystick
	void init();
	
	/// returns whether joystick is active
	bool isActive(){return m_active;}
	
	/// updates the state of the joystick
	void update();
	
	/// get horizontal axis
	float getXAxis();
	
	/// get vertical axis
	float getYAxis();
	
	/// returns true if button is down
	bool isButtonDown(int button);
	
	/// returns true if continue button is down
	bool isContinueButtonDown();
	
	/// returns number of pressed button oder -1
	int getDownButton();
};

#else
// stub implementation if joystick support is not available in SDL

class Joystick
{
public:
	Joystick(){};
		
	void init(){};
	bool isActive(){return false;}
	void update(){}
	float getXAxis(){return 0.0f;}
	float getYAxis(){return 0.0f;}
	bool isButtonDown(int button){return false;}
	bool isContinueButtonDown(){return false;}
	int getDownButton(){return -1;}
};

#endif // HAVE_SDL_JOYSTICKOPEN

#endif // _JOYSTICK_H_
