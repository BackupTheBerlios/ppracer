/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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

#include "paused.h"

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
#include "part_sys.h"
#include "joystick.h"
#include "course_mgr.h"
#include "lights.h"

#include "game_mgr.h"

#include "bench.h"

Paused::Paused()
 : m_backgroundFrm(240,260),
   m_pausedLbl(_("Paused"),"paused"),
   m_configBtn(_("Configuration")),
   m_resumeBtn(_("Resume")),
   m_quitBtn(_("Quit"))
{
	ppogl::UIManager::getInstance().setBoxDimension(ppogl::Vec2d(640,480));	

	m_backgroundFrm.setPosition(ppogl::Vec2d(320,250));
	m_backgroundFrm.alignment.set(0.5, 0.5);
	
	m_pausedLbl.setPosition(ppogl::Vec2d(320,350));
	m_pausedLbl.alignment.set(0.5,0.5);

	m_configBtn.setPosition(ppogl::Vec2d(320,245));
	m_configBtn.alignment.center();
	m_configBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Paused::configuration));
	
	m_resumeBtn.setPosition(ppogl::Vec2d(320,205));
	m_resumeBtn.alignment.center();
	m_resumeBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Paused::resume));

	m_quitBtn.setPosition(ppogl::Vec2d(320,165));
    m_quitBtn.alignment.center();
	m_quitBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Paused::quit));

	m_paused=true;
	
	//ppogl::AudioMgr::getInstance().playMusic("paused");
	ppogl::AudioMgr::getInstance().stopAllMusic();
}

void
Paused::preDisplay(float timeStep)
{
    // check joystick 
    if(is_joystick_active()){
		update_joystick();

		if(is_joystick_continue_button_down()){
			setMode(RACING);
		    return;
		}
    }
}

void
Paused::postDisplay(float timestep)
{
    reshape(resolutionX, resolutionY);
	set_gl_options(GUI);
	
   	if(Benchmark::getMode()!=Benchmark::PAUSED){
		ppogl::UIManager::getInstance().draw(resolutionX, resolutionY,
											 false); // no decoration
	}    	
}

bool
Paused::keyPressEvent(SDLKey key)
{
	// continue race if pause key is pressed	
	if(key==PPConfig.getInt("pause_key")){
		if(Benchmark::getMode() == Benchmark::PAUSED){
			setMode( GAME_OVER );
		}else{
			setMode( RACING );
		}
		return true;
	}else{
		return false;
	}
}

void
Paused::resume()
{
	setMode( RACING );
}

void
Paused::quit()
{
	GameMgr::getInstance().abortRace();
    setMode( GAME_OVER );
}

void
Paused::configuration()
{
	setMode( CONFIGURATION );
}
