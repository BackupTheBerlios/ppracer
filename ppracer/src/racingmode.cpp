/*  
 * PlanetPenguin Racer 
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
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
 
#include "racingmode.h"

#include "gameconfig.h"
#include "track_marks.h"
#include "game_mgr.h"
#include "hud.h"
#include "fog.h"
#include "viewfrustum.h"
#include "part_sys.h"
#include "render_util.h"
#include "view.h"
#include "course_render.h"
#include "lights.h"
#include "tux.h"
#include "tux_shadow.h"
#include "fps.h"

RacingMode::RacingMode()
 : m_paused(false)
{}

void
RacingMode::loop(float timestep)
{
	//update the FPS counter for this frame
    fpsCounter.update();
	
	// the multiplayer stuff is experimental
	if(GameMode::exp_multiscreen<0){
		// player 0 (first player)
		prePlayer(0,timestep);
		
		//things to do before displaying
		preDisplay(timestep);
		
		setup_view_frustum( players[0], NEAR_CLIP_DIST, 
				GameConfig::forwardClipDistance);
		reshape(resolutionX, resolutionY);
		
		renderCourse(0, timestep);
		postPlayer(0,timestep);
				
	}else{
		prePlayer(0,timestep);
		prePlayer(1,timestep);
		
		//things to do before displaying
		preDisplay(timestep);
				
		// player 0 (first player)
				
		setup_view_frustum( players[0], NEAR_CLIP_DIST, 
							GameConfig::forwardClipDistance,0);
		reshape(resolutionX, resolutionY, 0);
		renderCourse(0, timestep);
				
		// player 1 (second player)
		setup_view_frustum( players[1], NEAR_CLIP_DIST, 
							GameConfig::forwardClipDistance,0);
		reshape(resolutionX, resolutionY, 1);
		renderCourse(1, timestep);	
					
		postPlayer(0,timestep);
		postPlayer(1,timestep);
	}
	
	//additional things after displaying
	postDisplay(timestep);	
}

void
RacingMode::renderCourse(int player, float timestep)
{
	//setup the fog plane (is this necessary for every player?)	
	fogPlane.setup();
	
	//track marks
    trackMarks[player].update();	
	
	if(m_paused==false){	
		//only update the players view if game is not paused
    	update_view(players[player], timestep);
	}
	
    courseRenderer.setClipping(true);
    setup_course_lighting();
	
	//draw course
	courseRenderer.render(players[player].view.pos);

	// draw trackmarks for all players
	TrackMarks::drawAllPlayers();
		
    if(GameConfig::drawParticles){
		if(m_paused==false){
			//update particles if game is not paused
			update_particles(timestep);
		}
		draw_particles(players[player]);
    }

	for(int i=0; i<GameMgr::getInstance().numPlayers; i++){
		tux[i].draw();
		draw_tux_shadow(i);
	}
	
	if(player==0){
    	HUD1.draw(players[0], resolutionX, resolutionY);
	}else if(player==1){
		HUD2.draw(players[1], resolutionX, resolutionY);
	}else{
		PP_WARNING("Player " << player << " is not supported");		
	}
}
