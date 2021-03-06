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

#include "reset.h"

#include "ppogl/base/defs.h"

#include "gl_util.h"
#include "fps.h"
#include "render_util.h"
#include "phys_sim.h"
#include "view.h"
#include "course_render.h"
#include "tux.h"
#include "tux_shadow.h"
#include "loop.h"
#include "fog.h"
#include "viewfrustum.h"
#include "hud.h"
#include "course_load.h"
#include "winsys.h"
#include "lights.h"
#include "elements.h"

#include "stuff.h"

#include "game_mgr.h"


#define BLINK_IN_PLACE_TIME 0.5
#define TOTAL_RESET_TIME 1.0

Reset::Reset()
 : m_textColor(0.0, 0.0, 0.0),
   m_resetStartTime(SDL_GetTicks()/1000.0),
   m_positionReset(false)
{
}

void
Reset::loop(float timeStep)
{
	float elapsed_time = SDL_GetTicks()/1000.0 - m_resetStartTime;
    static bool tux_visible = true; 
    static int tux_visible_count = 0;

    fogPlane.setup();

    update_player_pos(EPS);
	
    update_view( players[0], EPS );

    viewFrustum.setup( players[0], NEAR_CLIP_DIST, 
			PPConfig.getInt("forward_clip_distance") );

    courseRenderer.setClipping(true);

	Light::setup();
	
	courseRenderer.render(players[0].view.pos);
	
    if((elapsed_time > BLINK_IN_PLACE_TIME) && (!m_positionReset)){
			
		std::list<ppogl::Vec2d>::iterator bestit = resetLocs.end();
		
		if(resetLocs.empty()){
			// no reset point
			const ppogl::Vec2d& courseDim = Course::getDimensions();
	    	players[0].pos.x() = courseDim.x()/2.0;
	    	players[0].pos.z() = MIN(players[0].pos.z() + 10, -1.0);
		}else{
			// iterate through list...
			std::list<ppogl::Vec2d>::iterator it;
			for(it=resetLocs.begin(); it!=resetLocs.end();it++){
				if((*it).y() > players[0].pos.y() ){ 
					if(bestit==resetLocs.end() || (*it).y() < (*bestit).y()){
						bestit = it;
					}
				}
			}
			
			if(bestit==resetLocs.end()){
				const ppogl::Vec2d& courseDim = Course::getDimensions();
				players[0].pos.x() = courseDim.x()/2.0;
				players[0].pos.z() = MIN(players[0].pos.z() + 10, -1.0);
			}else{
				players[0].pos.x() = (*bestit).x();
				players[0].pos.z() = (*bestit).y();
			}
		}

	// Re-initialize the camera 
	players[0].view.initialized = false;

	init_physical_simulation();
	m_positionReset = true;
    }

    if (tux_visible) { 
		tux[0].draw();
		draw_tux_shadow(0);
    } 
    if (++tux_visible_count > 3) {
		tux_visible = !tux_visible;
		tux_visible_count = 0;
    }

    HUD1.draw(players[0], resolution);

    reshape(resolution);

    GameMgr::getInstance().time += timeStep;

    if (elapsed_time > TOTAL_RESET_TIME) {
		setMode( RACING );
    }
}
