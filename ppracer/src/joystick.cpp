/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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

#if defined(HAVE_SDL_JOYSTICKOPEN)

#include "SDL.h"
#include "SDL_joystick.h"

static SDL_Joystick *joystick = NULL;
static int num_buttons = 0;
static int num_axes = 0;

static bool disabled=false;


void init_joystick()
{
    if(PPConfig.getBool("disable_joystick")){
		disabled=true;
		return;
	}
		
	int num_joysticks = 0;
    char *js_name;

    /* Initialize SDL SDL joystick module */
    if ( SDL_Init( SDL_INIT_JOYSTICK ) < 0 ) {
		PP_ERROR("Couldn't initialize SDL: %s", SDL_GetError() );
    }

    num_joysticks = SDL_NumJoysticks();

    PP_LOG( DEBUG_JOYSTICK, "Found %d joysticks", num_joysticks );

    if ( num_joysticks == 0 ) {
		joystick = NULL;
		return;
    }

    js_name = const_cast<char*>(SDL_JoystickName(0));

    PP_LOG( DEBUG_JOYSTICK, "Using joystick `%s'", js_name );

    joystick = SDL_JoystickOpen( 0 );

    if ( joystick == NULL ) {
		PP_LOG( DEBUG_JOYSTICK, "Cannot open joystick" );
		return;
    }

    /* Get number of buttons */
    num_buttons = SDL_JoystickNumButtons( joystick );
    PP_LOG( DEBUG_JOYSTICK, "Joystick has %d button%s", 
		 num_buttons, num_buttons == 1 ? "" : "s" );

    /* Get number of axes */
    num_axes = SDL_JoystickNumAxes( joystick );
    PP_LOG( DEBUG_JOYSTICK, "Joystick has %d ax%ss", 
		 num_axes, num_axes == 1 ? "i" : "e" );

}

bool is_joystick_active()
{
	if(disabled) return false;

	return ( joystick != NULL );
	
}

void update_joystick()
{
	if(disabled) return;
    SDL_JoystickUpdate();
}

double get_joystick_x_axis()
{
    if(disabled) return 0.0;

	
	static bool warning_given = false;
    int axis;

    PP_CHECK_POINTER(joystick);

    axis = PPConfig.getInt("joystick_x_axis");

    /* Make sure axis is valid */
    if ( axis >= num_axes || axis < 0 ) {

	if ( !warning_given ) {
	    PP_WARNING( "joystick x axis mapped to axis %d "
			   "but joystick only has %d axes", axis, num_axes );
	    warning_given = true;
	}

	return 0.0;
    }

    return SDL_JoystickGetAxis( joystick, axis )/32768.0;
}

double get_joystick_y_axis()
{
	if(disabled) return 0.0;
	
    static bool warning_given = false;
    int axis;

    PP_CHECK_POINTER( joystick );

    axis = PPConfig.getInt("joystick_y_axis");

    /* Make sure axis is valid */
    if ( axis >= num_axes || axis < 0 ) {

	if ( !warning_given ) {
	    PP_WARNING( "joystick y axis mapped to axis %d "
			   "but joystick only has %d axes", axis, num_axes );
	    warning_given = true;
	}

	return 0.0;
    }

    return SDL_JoystickGetAxis( joystick, PPConfig.getInt("joystick_y_axis") )/32768.0;
}

bool is_joystick_button_down( int button ) 
{
    if(disabled) return false;
	
	static bool warning_given = false;

    PP_CHECK_POINTER( joystick );
    PP_REQUIRE( button >= 0, "button is negative" );

    if ( button >= num_buttons ) {
	if ( !warning_given ) {
	    PP_WARNING( "state of button %d requested, but "
			   "joystick only has %d buttons.  Further warnings "
			   "of this type will be suppressed", 
			   button, num_buttons );
	    warning_given = true;
	}
	return false;
    }

    return SDL_JoystickGetButton( joystick, button );
}

bool is_joystick_continue_button_down()
{
	if(disabled) return false;
	
    if ( joystick == NULL ) {
		return false;
    }

    if(PPConfig.getInt("joystick_continue_button") < 0 ) {
		return false;
    }

    return is_joystick_button_down( PPConfig.getInt("joystick_continue_button") );
}

int get_joystick_down_button()
{
    for(int i=0; i<num_buttons; i++){
		if(is_joystick_button_down(i)){
			return i;			
		}	
	}
	return -1;
}

#else

// Stub functions

void init_joystick()
{
}

bool is_joystick_active()
{
    return false;
}

void update_joystick()
{
}

float get_joystick_x_axis()
{
    return 0.0;
}

float get_joystick_y_axis()
{
    return 0.0;
}

bool is_joystick_button_down( int button ) 
{
    return false;
}

bool is_joystick_continue_button_down()
{
    return false;
}

int get_joystick_down_button()
{
    return -1;
}


#endif

/* EOF */
