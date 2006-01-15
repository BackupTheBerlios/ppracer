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

#include "joystick.h"

#include "ppogl/base/errors.h"
#include "ppracer.h"

#ifdef HAVE_SDL_JOYSTICKOPEN

#include "SDL.h"
#include "SDL_joystick.h"

Joystick::Joystick()
 : mp_joystick(NULL),
   m_numButtons(0),
   m_numAxes(0),
   m_active(false)
{
}

void
Joystick::init()
{	
	if(PPConfig.getBool("disable_joystick")){
		PP_MESSAGE("Disabling joysticks");
		m_active=false;
		return;
	}

    // Initialize SDL joystick module
    if ( SDL_Init( SDL_INIT_JOYSTICK ) < 0 ) {
		PP_ERROR("Couldn't initialize SDL: " << SDL_GetError());
    }

	const int num_joysticks = SDL_NumJoysticks();

    PP_MESSAGE("Found " << num_joysticks << " joysticks");

    if(num_joysticks == 0){
		m_active=false;
		return;
    }

    PP_LOG(DEBUG_JOYSTICK, "Using joystick " << SDL_JoystickName(0));

    mp_joystick = SDL_JoystickOpen(0);

    if(mp_joystick == NULL){
		PP_LOG(DEBUG_JOYSTICK, "Cannot open joystick");
		m_active=false;
		return;
    }

    // Get number of buttons
	m_numButtons = SDL_JoystickNumButtons(mp_joystick);
    PP_LOG(DEBUG_JOYSTICK, "Joystick has " << m_numButtons
			<< " button"<< (m_numButtons == 1 ? "" : "s"));

    // Get number of axes
    m_numAxes = SDL_JoystickNumAxes(mp_joystick);
    PP_LOG(DEBUG_JOYSTICK, "Joystick has " << m_numAxes 
			<< " ax" << (m_numAxes == 1 ? "i" : "e") <<"s");
	m_active=true;
}

void
Joystick::update()
{
	if(!m_active) return;
    SDL_JoystickUpdate();
}

float
Joystick::getXAxis()
{
    if(!m_active) return 0.0;
	PP_CHECK_POINTER(mp_joystick);
	
	static bool warning_given = false;
    const int axis = PPConfig.getInt("joystick_x_axis");

    // Make sure axis is valid
    if(axis >= m_numAxes || axis < 0 ){
		if(!warning_given){
	    	PP_WARNING("joystick x axis mapped to axis " << axis
					<< " but joystick only has " << m_numAxes
					<< " axes");
	    	warning_given = true;
		}
		return 0.0;
    }

    return SDL_JoystickGetAxis(mp_joystick, axis)/32768.0;
}

float
Joystick::getYAxis()
{
	if(!m_active) return 0.0;
	PP_CHECK_POINTER(mp_joystick);
	
    static bool warning_given = false;
    const int axis = PPConfig.getInt("joystick_y_axis");

    // Make sure axis is valid
    if(axis >= m_numAxes || axis < 0){
		if(!warning_given){
	    	PP_WARNING("joystick y axis mapped to axis " << axis
					<< " but joystick only has " << m_numAxes
					<< " axes");
	    	warning_given = true;
		}
		return 0.0;
    }

    return SDL_JoystickGetAxis(mp_joystick, axis)/32768.0;
}

bool
Joystick::isButtonDown(int button) 
{
    if(!m_active) return false;
	PP_CHECK_POINTER(mp_joystick);
    PP_REQUIRE(button >= 0, "button is negative");
	
	static bool warning_given = false;

    if(button >= m_numButtons){
		if(!warning_given){
	    	PP_WARNING( "state of button " << button
					<< " requested, but joystick only has "
					<< m_numButtons << " buttons.  Further warnings "
			   		"of this type will be suppressed");
		    warning_given = true;
		}
		return false;
    }

    return SDL_JoystickGetButton(mp_joystick, button);
}

bool
Joystick::isContinueButtonDown()
{
	if(!m_active) return false;
	PP_CHECK_POINTER(mp_joystick);
	
	const int button = PPConfig.getInt("joystick_continue_button");

    if(button<0){
		return false;
    }

    return isButtonDown(button);
}

int
Joystick::getDownButton()
{
    if(!m_active) return -1;
	for(int i=0; i<m_numButtons; i++){
		if(isButtonDown(i)){
			return i;		
		}	
	}
	return -1;
}

#endif // HAVE_SDL_JOYSTICKOPEN
