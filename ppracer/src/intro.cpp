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

#include "intro.h"

#include "ppogl/base/defs.h"

#include "keyframe.h"
#include "course_render.h"
#include "gl_util.h"
#include "fps.h"
#include "loop.h"
#include "render_util.h"
#include "view.h"
#include "tux.h"
#include "tux_shadow.h"
#include "fog.h"
#include "hud.h"
#include "viewfrustum.h"
#include "phys_sim.h"
#include "part_sys.h"
#include "course_load.h"
#include "joystick.h"
#include "winsys.h"
#include "course_mgr.h"
#include "lights.h"

#include "game_mgr.h"

#include "bench.h"
#include "snow.h"

Intro::Intro()
{
	ppogl::Vec2d start_pt = Course::getStartPt();

    GameMgr::getInstance().time = 0.0;
    
	if(Benchmark::getMode()==Benchmark::PAUSED){
		Course::setStartPt(Benchmark::getPosition());
	}else{
		for(int i=0; i<GameMgr::getInstance().numPlayers; i++){		
			players[i].pos.x() = start_pt.x();
    		players[i].pos.z() = start_pt.y();
			
			// move second player to a different position
			players[i].pos.x() += i*2;
			
		}
	}
    init_physical_simulation();
	
	clear_particles();
	
	for(int i=0; i<GameMgr::getInstance().numPlayers; i++){		
		keyFrames[i].init(i);	    
		players[i].vel = ppogl::Vec3d(0,0,0);
		players[i].herring = 0;
    	players[i].score = 0;
    	players[i].airbornetime = 0.0;
		players[i].orientation_initialized = false;
    	players[i].view.initialized = false;
		set_view_mode(players[i], ABOVE);
    	update_view(players[i], EPS);
	}
	
	fpsCounter.setMaxFPS(PPConfig.getInt("max_fps"));
	ppogl::AudioMgr::getInstance().playMusic("intro");
}

void
Intro::preDisplay(float timeStep)
{
    if(PPConfig.getBool("do_intro_animation") == false){
		setMode(RACING);
		return;
    }

    // Check joystick
    if(joystick.isActive()){
		joystick.update();
		if(joystick.isContinueButtonDown()){
	    	abort(players[0]);
	    	return;
		}
    }	
	
	for(int i=0; i<GameMgr::getInstance().numPlayers; i++){		
		keyFrames[i].update(players[i], timeStep);
	}
}

void
Intro::postDisplay(float timestep)
{
	if( Benchmark::getMode() != Benchmark::NONE){
		abort(players[0]);
	}
}

void
Intro::abort(Player& plyr)
{
    const ppogl::Vec2d start_pt = Course::getStartPt();

    setMode(RACING);

	plyr.orientation_initialized = false;
    plyr.view.initialized = true;

	plyr.pos.x() = start_pt.x();
    plyr.pos.z() = start_pt.y();
}

bool
Intro::keyPressEvent(SDLKey key)
{ 
	abort(players[0]);
	return true;
}
