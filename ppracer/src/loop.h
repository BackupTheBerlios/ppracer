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

#ifndef _LOOP_H_
#define _LOOP_H_

#include "SDL.h"
#include "ppogl/translation.h"
#include "ppogl/audio.h"

// including this here because this headers
// are used in nearly all modes
#include "gl_util.h"
#include "render_util.h"
#include "winsys.h"
#include "gameconfig.h"
#include "joystick.h"

void loop_mouse_func (int button, int state, int x, int y);
void loop_mouse_motion_func( int x, int y );
void loop_keyboard_func(SDLKey key, SDLMod mod, bool release);

class GameMode
{
public:
	enum Mode{
	    NO_MODE = -1,
		SPLASH = 0,
		GAME_TYPE_SELECT,
		EVENT_SELECT,
		EVENT_RACE_SELECT,
		RACE_SELECT,
		LOADING,
		INTRO,
		RACING,
		GAME_OVER,
		PAUSED,
		RESET,
		CREDITS,
		CONFIGURATION,
		CONFIG_GENERAL,
		CONFIG_VIDEO,
		CONFIG_AUDIO,
		CONFIG_KEYBOARD,
		CONFIG_JOYSTICK,
		CONFIG_GRAPHICS,
		QUIT,
		BENCHMARK,
		NUM_GAME_MODES
	};
	
protected:
	/// custom function for drawing snow in the ui
	void drawSnow( float timeStep, bool windy = false );
	
public:
	GameMode();
	virtual ~GameMode();
	
	virtual void loop(float timeStep) = 0;
	
	virtual bool keyboardEvent(SDLKey key, bool release){return false;};
	virtual bool keyPressEvent(SDLKey key){return false;};
	virtual bool keyReleaseEvent(SDLKey key){return false;};
	
	virtual bool mouseButtonEvent(int button, int x, int y, bool pressed){return false;};
	virtual bool mouseButtonPressEvent(int button, int x, int y){return false;};
	virtual bool mouseButtonReleaseEvent(int button, int x, int y){return false;};
	
	static GameMode* currentMode;
	static GameMode::Mode mode;
	static GameMode::Mode prevmode;
	
	static Joystick joystick;
	
	static int resolutionX;
	static int resolutionY; 	
	
	static void setMode(GameMode::Mode mode);
	static void mainLoop();
	
	// multiscreen is an experimental feature
	enum MultiscreenMode{
		MULTISCREEN_NONE = -1,
		MULTISCREEN_HORIZ = 0,
		MULTISCREEN_VERT = 2,
		MULTISCREEN_BOTH = 4
	};
	static enum MultiscreenMode exp_multiscreen;
	
};

void updateDisplay();

#endif // _LOOP_H_
