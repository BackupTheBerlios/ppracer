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

#include "game_over.h"

#include "ppogl/font.h"

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
#include "joystick.h"
#include "part_sys.h"
#include "lights.h"

#include "stuff.h"
#include "game_mgr.h"

#include "bench.h"

#include "winsys.h"

GameOver::GameOver()
 : m_raceOverLbl("","race_over"),
   m_timeLbl("","race_stats"),
   m_herringLbl("","race_stats"),
   m_scoreLbl("","race_stats"),
   m_maxspeedLbl("","race_stats"),
   m_flyingLbl("","race_stats"),
   m_resultsLbl("","race_stats")
{
	ppogl::AudioMgr::getInstance().stopAllSounds();
	ppogl::UIManager::getInstance().setBoxDimension(ppogl::Vec2d(640,480));	
	
	if( Benchmark::getMode() != Benchmark::NONE ){
		Benchmark::displayState();
		winsys_exit( 0 );
	}
	
    ppogl::AudioMgr::getInstance().playMusic("game_over");

    m_aborted = GameMgr::getInstance().wasRaceAborted();

	if ( !m_aborted ) {
		GameMgr::getInstance().updatePlayersScores();
    }
		
    if ( GameMgr::getInstance().gametype!=GameMgr::PRACTICING ) {
		m_bestScore = GameMgr::getInstance().updateCurrentRaceData();		
		
		if(!GameMgr::getInstance().wasRaceWon()){
			players[0].decLives();
		}
    }else{
		if ( !m_aborted ) {
			m_bestScore = players[0].updateOpenCourseData(
									GameMgr::getInstance().getCurrentRace().getName(),
									GameMgr::getInstance().time,
									players[0].herring,
									players[0].score);
			
			if(m_bestScore) players[0].saveData();
		}
	}
	
	{  
	ppogl::Vec3d dir = players[0].vel;
	int speed = int(dir.normalize());
	//set max_speed
	if (speed > players[0].max_speed) players[0].max_speed=int(speed);
	}
	
	ppogl::Vec2d pos(320, 240+150);
	
	m_raceOverLbl.setPosition(pos);
	m_raceOverLbl.alignment.center();
		
	if ( GameMgr::getInstance().wasRaceAborted() ) {
		m_raceOverLbl.setText(_("Race aborted"));
    }else{	
		m_raceOverLbl.setText(_("Race Over"));
	
		char buff[40];
		int minutes, seconds, hundredths;

		getTimeComponents( GameMgr::getInstance().time, minutes, seconds, hundredths );
		snprintf(buff, 40, _("Time: %02d:%02d.%02d").c_str(), minutes, seconds, hundredths );	
		pos.y()-=100;
		
		m_timeLbl.setPosition(pos);
		m_timeLbl.alignment.center();
		m_timeLbl.setText(buff);
	
		snprintf(buff, 40, _("Herring: %3d").c_str(), players[0].herring );
		pos.y()-=30;
		m_herringLbl.setPosition(pos);
		m_herringLbl.alignment.center();
		m_herringLbl.setText(buff);
	
		snprintf(buff, 40, _("Score: %6d").c_str(), players[0].score );
		pos.y()-=30;
		m_scoreLbl.setPosition(pos);
		m_scoreLbl.alignment.center();
		m_scoreLbl.setText(buff);
	
		int speed = int(double(players[0].max_speed) * M_PER_SEC_TO_KM_PER_H);
		snprintf(buff, 40, _("Max speed: %3d km/h").c_str(), speed);
		pos.y()-=30;
		m_maxspeedLbl.setPosition(pos);
		m_maxspeedLbl.alignment.center();
		m_maxspeedLbl.setText(buff);
	
		double percent = (players[0].airbornetime / GameMgr::getInstance().time) * 100.0;
		snprintf(buff, 40, _("Was flying: %.01f %% of time").c_str(), percent);
		pos.y()-=30;
		m_flyingLbl.setPosition(pos);
		m_flyingLbl.alignment.center();
		m_flyingLbl.setText(buff);
	
		std::string string;
	
		if ( GameMgr::getInstance().gametype==GameMgr::PRACTICING){
			if(m_bestScore){
				string = _("You beat your best score!");
			}
		} else if(GameMgr::getInstance().wasEventWon()){
			string = _("Congratulations! You won the event!");
		} else if(GameMgr::getInstance().wasCupWon()){
			string = _("Congratulations! You won the cup!");
		} else if(GameMgr::getInstance().wasRaceWon()){
			string = _("You advanced to the next race!");
		} else {
			string = _("You didn't advance.");
		}	
	
		pos.y()-=30;
		m_resultsLbl.setPosition(pos);
		m_resultsLbl.alignment.center();
		m_resultsLbl.setText(string);
	}
	//Set the racing mode to paused to disable player updates
	m_paused=true;
	
	fpsCounter.setMaxFPS(PPConfig.getInt("gui_max_fps"));
}

void
GameOver::preDisplay(float timestep)
{
	if(joystick.isActive()){
		joystick.update();

		if(joystick.isContinueButtonDown()){
	    	if(GameMgr::getInstance().gametype != GameMgr::PRACTICING){
				setMode(EVENT_RACE_SELECT);
			}else{
				setMode(RACE_SELECT);
			}
	    	return;
		}
    }
}

void
GameOver::postDisplay(float timestep)
{	
    reshape(resolutionX, resolutionY);

	set_gl_options(GUI);

	gl::PushMatrix();
	{
	gl::MatrixMode(GL_PROJECTION);
    gl::LoadIdentity();
    gl::Ortho(0.0, resolutionX, 0.0, resolutionY, -1.0, 1.0);
    gl::MatrixMode(GL_MODELVIEW);
    gl::LoadIdentity();
    gl::Translate(0.0, 0.0, -1.0);
    gl::Color(ppogl::Color::white);
		
	gl::Disable(GL_TEXTURE_2D);
    gl::Color(0.0,0.0,0.0,0.5);
    gl::Rect(0,0,resolutionX, resolutionY);
    gl::Enable(GL_TEXTURE_2D);
	}
	gl::PopMatrix();
	
	ppogl::UIManager::getInstance().draw(resolutionX, resolutionY,
										 false); //no decoration
}

bool
GameOver::mouseButtonEvent(int button, int x, int y, bool pressed)

{
	if ( GameMgr::getInstance().gametype!=GameMgr::PRACTICING ) {
		setMode(EVENT_RACE_SELECT);
	}else{
		setMode(RACE_SELECT);
	}
	return true;
}

bool
GameOver::keyPressEvent(SDLKey key)
{
	if(GameMgr::getInstance().gametype!=GameMgr::PRACTICING){
		setMode(EVENT_RACE_SELECT);
	}else{
		setMode(RACE_SELECT);
	}
	return true;
}
