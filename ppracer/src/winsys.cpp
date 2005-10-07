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

#include "winsys.h"
#include "loop.h"
#include "render_util.h"
#include "stuff.h"

#include "ppogl/base/glwrappers.h"

void
setup_sdl_video_mode()
{
    Uint32 video_flags = SDL_OPENGL; 
    int bpp = 0;
    int width, height;

    if (ppogl::Config::getInstance().getBool("fullscreen")) {
		video_flags |= SDL_FULLSCREEN;
    } else {
		video_flags |= SDL_RESIZABLE;
    }
	
#if SDL_VERSION_ATLEAST(1,2,6)
	if(ppogl::Config::getInstance().getBool("enable_fsaa")){
		//enable FSAA
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, PPConfig.getInt("multisamples"));
	}
#endif
	
	if(ppogl::Config::getInstance().getBool("stencil_buffer")){
		SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
	}

    switch(PPConfig.getInt("bpp_mode")){
    	case 0:
			bpp = 0;
			break;
    	case 1:
			bpp = 16;
			break;
	    case 2:
			bpp = 32;
			break;
    	default:
			PPConfig.setInt("bpp_mode", 0);
			bpp = 0;
			break;
    }

    width = PPConfig.getInt("x_resolution");
	
	if(PPConfig.getBool("x_resolution_half_width")){
		width /=2;		
	}
	
    height = PPConfig.getInt("y_resolution");

	PP_MESSAGE("Init video: " << width << "x" << height << " bpp:" << bpp);  
	
    if(SDL_SetVideoMode( width, height, bpp, video_flags) == NULL){
		PP_ERROR("Couldn't initialize video: " << SDL_GetError());
    }
		
	GameMode::resolutionX = width;
	GameMode::resolutionY = height;
	
	gl::Viewport(0,0,width,height);
}

void
winsys_init(char *window_title, char *icon_title)
{
	PP_MESSAGE("Init SDL");
	Uint32 sdl_flags = SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE;
	if(SDL_Init(sdl_flags) < 0){
		PP_ERROR("Couldn't initialize SDL: " << SDL_GetError());
	}

	// init video 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	setup_sdl_video_mode();

	SDL_WM_SetCaption(window_title, icon_title);
	
	SDL_EnableKeyRepeat( 0, 0 );
}
 
void
winsys_process_events()
{
	SDL_Event event; 
    int x, y;

    while(true){
		SDL_LockAudio();
		SDL_UnlockAudio();

		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					SDL_GetMouseState(&x, &y);
					loop_keyboard_func(event.key.keysym.sym,
							event.key.keysym.mod, false, x, y);
					break;
	    		case SDL_KEYUP:
					SDL_GetMouseState( &x, &y );
					loop_keyboard_func( event.key.keysym.sym,
							event.key.keysym.mod, true, x, y);
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					loop_mouse_func( event.button.button,
				   			event.button.state,
				   			event.button.x,
				   			event.button.y );
					break;
				case SDL_MOUSEMOTION:
					if(event.motion.state){
		    			// buttons are down
						loop_mouse_motion_func( event.motion.x, event.motion.y );
					}else{
		    			// no buttons are down
						loop_mouse_motion_func( event.motion.x, event.motion.y );
					}
					break;
	    		case SDL_VIDEORESIZE:
					PPConfig.setInt("x_resolution",event.resize.w);
					PPConfig.setInt("y_resolution",event.resize.h);
					setup_sdl_video_mode();
		    		reshape(event.resize.w, event.resize.h);
					break;
				case SDL_QUIT:
					winsys_exit(0);		
					break;
	    	}

	    	SDL_LockAudio();
	    	SDL_UnlockAudio();
		}

		GameMode::mainLoop();

		// delay for 1ms to alow other threads to do some work
		SDL_Delay(1);
    }

    PP_NOT_REACHED();
}

void
winsys_exit(int code)
{
    write_config_file();
	PP_MESSAGE("Quit SDL");
	SDL_Quit();
	exit(code);
}
