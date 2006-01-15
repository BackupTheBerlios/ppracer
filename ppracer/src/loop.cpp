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

#include "loop.h"

#include "ui_snow.h"
#include "ppogl/base/defs.h"

#include "SDL.h"


#include "render_util.h"
#include "splash_screen.h"
#include "game_type_select.h"
#include "credits.h"
#include "race_select.h"
#include "loading.h"
#include "intro.h"
#include "racing.h"
#include "paused.h"
#include "game_over.h"
#include "reset.h"
#include "event_select.h"
#include "event_race_select.h"
#include "bench.h"
#include "fps.h"

#include "configuration.h"
#include "generalconfig.h"
#include "videoconfig.h"
#include "audioconfig.h"
#include "keyboardconfig.h"
#include "joystickconfig.h"
#include "graphicsconfig.h"

#include "game_mgr.h"

#include "ppogl/ui/uimgr.h"

#include "winsys.h"

GameMode* GameMode::currentMode = NULL;

GameMode::Mode GameMode::mode;
GameMode::Mode GameMode::prevmode;

ppogl::Vec2i GameMode::resolution;

/// the joystick
Joystick GameMode::joystick;

static GameMode::Mode new_mode = GameMode::NO_MODE;
	
static ppogl::Vec2i cursorPos;
static bool rightMouseButtonDown  = false;
static bool middleMouseButtonDown = false;

void
updateDisplay()
{
	SDL_GL_SwapBuffers();
}

GameMode::~GameMode()
{
}

void
GameMode::setMode(GameMode::Mode mode) 
{
    new_mode = mode;
}

void
GameMode::mainLoop()
{
	/* Grab mouse pointer
	
	ppogl::Vec2d pos =
		ppogl::UIManager::getInstance().getCursorPosition();
	
	// Flip y coordinates
	pos.y() = GameMode::resolutionY - pos.y();

	if( pos.x() < 0 ){
		pos.x() = 0;
	}
	if( pos.x() > GameMode::resolutionX-1 ){
	    pos.x() = GameMode::resolutionX-1;
	}
	if( pos.y() < 0 ){
	    pos.y() = 0;
	}
	if( pos.y() > GameMode::resolutionY-1 ) {
		pos.y() = GameMode::resolutionY-1;
	}
	
	winsys_warp_pointer( int(pos.x()), int(pos.y()) );
	*/
	if ( GameMode::mode != new_mode ) {
		
		if (GameMode::currentMode!=NULL){
			delete GameMode::currentMode;
		}
		GameMode::prevmode = GameMode::mode;
		PP_MESSAGE("Switching mode: " << new_mode);

		switch(new_mode){
			case SPLASH:
				GameMode::currentMode = new SplashScreen();
				break;			
			case GAME_TYPE_SELECT:
				GameMode::currentMode = new GameTypeSelect();
				break;
			case CREDITS:
				GameMode::currentMode = new Credits();
				break;
			case CONFIGURATION:
				GameMode::currentMode = new Configuration();
				break;
			case RACE_SELECT:
				GameMode::currentMode = new RaceSelect();
				break;
			case LOADING:
				GameMode::currentMode = new Loading();
				break;
			case INTRO:
				GameMode::currentMode = new Intro();
				break;
			case RACING:
				GameMode::currentMode = new Racing();
				break;
			case PAUSED:
				GameMode::currentMode = new Paused();
				break;
			case GAME_OVER:
				GameMode::currentMode = new GameOver();
				break;			
			case RESET:
				GameMode::currentMode = new Reset();
				break;
			case EVENT_SELECT:
				GameMode::currentMode = new EventSelect();
				break;
			case CONFIG_GENERAL:
				GameMode::currentMode = new GeneralConfig();
				break;
			case CONFIG_VIDEO:
				GameMode::currentMode = new VideoConfig();
				break;
			case CONFIG_AUDIO:
				GameMode::currentMode = new AudioConfig();
				break;
			case CONFIG_KEYBOARD:
				GameMode::currentMode = new KeyboardConfig();
				break;
			case CONFIG_JOYSTICK:
				GameMode::currentMode = new JoystickConfig();
				break;
			case CONFIG_GRAPHICS:
				GameMode::currentMode = new GraphicsConfig();
				break;
			case EVENT_RACE_SELECT:
				GameMode::currentMode = new EventRaceSelect();
				break;
			case BENCHMARK:
				GameMode::currentMode = new Benchmark();
				break;
			case QUIT:
				winsys_exit(0);
				break;
			default:
				PP_NOT_REACHED();
		}
		
		GameMode::mode = new_mode;
	
		// Reset time step clock so that there isn't a sudden
		// jump when we start the new mode 
		GameMgr::getInstance().resetTimeStep();
	}
	
	GameMgr::getInstance().updateTimeStep();
	
	clear_rendering_context();	
	
	const float timestep = GameMgr::getInstance().getTimeStep();
		
	if(Benchmark::getTimeStep() >0.0){
		GameMode::currentMode->loop(Benchmark::getTimeStep());
	}else{	
		GameMode::currentMode->loop(timestep);
	}
	
	updateDisplay();
	
	//update the FPS counter for this frame
    fpsCounter.update(timestep);	
}

GameMode::GameMode()
{
}
	
void
GameMode::drawSnow(float timeStep, bool windy)
{
	// check wether ui snow is 
	if(PPConfig.getBool("ui_snow")){
		// update and draw snow
		update_ui_snow(timeStep, windy);
		draw_ui_snow();
    }
}

void
loop_mouse_func(int button, int state,const ppogl::Vec2i& position)
{
    if( button == SDL_BUTTON_MIDDLE ){
		middleMouseButtonDown = ( state == SDL_PRESSED );
    }
    if( button == SDL_BUTTON_RIGHT ){
		rightMouseButtonDown = ( state == SDL_PRESSED );
    }
	
	if(GameMode::currentMode!=NULL){
		bool pressed = state && SDL_PRESSED;
		if(GameMode::currentMode->mouseButtonEvent(button,position,pressed)) return;
		else if(pressed){
			if(GameMode::currentMode->mouseButtonPressEvent(button,position)) return;
		}else{
			if(GameMode::currentMode->mouseButtonReleaseEvent(button,position)) return;
		}	
		ppogl::UIManager::getInstance().mouseButtonEvent(button,state);
	}	
}


void
loop_mouse_motion_func(ppogl::Vec2i position)
{
	if(cursorPos != position){
		// Update UI snow 
		if(PPConfig.getBool("ui_snow")){
			if(rightMouseButtonDown){
				make_ui_snow(cursorPos);
				reset_ui_snow_cursor_pos(cursorPos);
			}else if(middleMouseButtonDown){
				make_ui_snow(cursorPos);
				push_ui_snow(cursorPos);
			}else{
				push_ui_snow(cursorPos);
			}
		}
    }
	cursorPos = position;
	
	// Reverse y coordinate
    position.y() = GameMode::resolution.y() - position.y();
	ppogl::UIManager::getInstance().mouseEvent(position);
}

void
loop_keyboard_func(SDLKey key, SDLMod mod, bool release)
{
	if(GameMode::currentMode!=NULL){		
		if (key < SDLK_UP) {
            if ( isalpha( key ) ) {
                key = SDLKey(tolower( key ));
            }
        }
		
		if(GameMode::currentMode->keyboardEvent(key,release)) return;
		else if(release){
			if(GameMode::currentMode->keyReleaseEvent(key)) return;
		}else{
			if(GameMode::currentMode->keyPressEvent(key)) return;
		}
		ppogl::UIManager::getInstance().keyboardEvent(key,mod,release);
	}
}

// multiscreen is an experimental feature
GameMode::MultiscreenMode GameMode::exp_multiscreen = GameMode::MULTISCREEN_NONE;
