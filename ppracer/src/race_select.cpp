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

#include "race_select.h"

#include "course_load.h"
#include "joystick.h"

#include "stuff.h"
#include "game_mgr.h"

#include <iostream>

static std::list<CourseData> *courseList;
static std::list<CourseData>::iterator curElem;
static std::list<CourseData>::iterator lastCompletedRace;

RaceSelect::RaceSelect()
 : m_titleLbl(_("Select a race"),"heading"), 
   m_descTa(300,104),
   m_raceListBox(450,36),
   m_backBtn(_("Back")),
   m_startBtn(_("Race!")),
   m_previewImage(ppogl::Vec2d(128,96)),
   m_authorTitle(_("Contributed by:")),
   m_timeTitle(_("Time:")),
   m_herringTitle(_("Herring:")),
   m_scoreTitle(_("Score:"))
{
    ppogl::UIManager::getInstance().setBoxDimension(ppogl::Vec2d(640,480));	
	
	courseList = &openCourseList;
		
    // Unless we're coming back from a race, initialize the race data to 
    // defaults.
	
    if ( GameMode::prevmode != GAME_OVER ) {
		// Initialize the race data 
		curElem = courseList->begin();
		GameMgr::getInstance().setCurrentRace(curElem);
		
	    GameMgr::getInstance().getCurrentRace().mirrored = false;
	    GameMgr::getInstance().getCurrentRace().condition = CourseData::CONDITION_SUNNY;
	    GameMgr::getInstance().getCurrentRace().windy = false;
	    GameMgr::getInstance().getCurrentRace().snowing = false;
    }
	
	{
		std::list<CourseData>::iterator it;
		for(it=courseList->begin(); it!=courseList->end();it++){
			m_raceListBox.addElement((*it).getName(), it);			
		}
	}
	
	m_titleLbl.setPosition(ppogl::Vec2d(320,400));
	m_titleLbl.alignment.center();
	
    m_backBtn.setPosition(ppogl::Vec2d(80,0));
    m_backBtn.signalClicked.Connect(ppogl::CreateSlot(this,&RaceSelect::back));

	m_startBtn.setPosition(ppogl::Vec2d(560,0));
	m_startBtn.alignment.right();
	m_startBtn.signalClicked.Connect(ppogl::CreateSlot(this,&RaceSelect::start));
 
	m_raceListBox.setPosition(ppogl::Vec2d(320,320));
	m_raceListBox.alignment.center();
    m_raceListBox.setSelectedData(curElem);
	m_raceListBox.signalChanged.Connect(ppogl::CreateSlot(this,&RaceSelect::updateData));
    
	// Create text area 
    m_descTa.setPosition(ppogl::Vec2d(100,150));
	m_descTa.setText((*curElem).getDescription());
    
	// conditions
	m_conditionsIBtn.setPosition(ppogl::Vec2d(416,260));
	m_conditionsIBtn.setTexture("conditions_button");
	m_conditionsIBtn.addState(0,ppogl::Vec4f(0.0, 0.5, 0.5, 1.0));
	m_conditionsIBtn.addState(1,ppogl::Vec4f(0.5, 0.0, 1.0, 0.5));
	m_conditionsIBtn.addState(2,ppogl::Vec4f(0.5, 0.5, 1.0, 1.0));
	m_conditionsIBtn.addState(3,ppogl::Vec4f(0.0, 0.0, 0.5, 0.5));
	m_conditionsIBtn.setMode(ppogl::ImageButton::MODE_CYCLE);
	m_conditionsIBtn.setState(int(GameMgr::getInstance().getCurrentRace().condition));
	
	// wind
	m_windIBtn.setPosition(ppogl::Vec2d(452,260));
	m_windIBtn.setTexture("wind_button");
	m_windIBtn.addState(0,ppogl::Vec4f(0.0, 0.5, 0.5, 1.0));
	m_windIBtn.addState(1,ppogl::Vec4f(0.5, 0.5, 1.0, 1.0));
	m_windIBtn.setMode(ppogl::ImageButton::MODE_CYCLE);
	m_windIBtn.setState(int(GameMgr::getInstance().getCurrentRace().windy));

	// mirror 
	m_mirrorIBtn.setPosition(ppogl::Vec2d(486,260));
	m_mirrorIBtn.setTexture("mirror_button");
	m_mirrorIBtn.addState(0, ppogl::Vec4f(0.0, 0.5, 0.5, 1.0));
	m_mirrorIBtn.addState(1, ppogl::Vec4f(0.5, 0.5, 1.0, 1.0));
	m_mirrorIBtn.setMode(ppogl::ImageButton::MODE_CYCLE);
	m_mirrorIBtn.setState(int(GameMgr::getInstance().getCurrentRace().mirrored));
	
	// snow 
	m_snowIBtn.setPosition(ppogl::Vec2d(520,260));
	m_snowIBtn.setTexture("snow_button");
	m_snowIBtn.addState(0,ppogl::Vec4f(0.0, 0.5, 0.5, 1.0));
	m_snowIBtn.addState(1,ppogl::Vec4f(0.5, 0.5, 1.0, 1.0));
	m_snowIBtn.setState(int(GameMgr::getInstance().getCurrentRace().snowing));

	// preview image
	m_previewImage.setPosition(ppogl::Vec2d(420,154));
	m_previewImage.addState(0,ppogl::Vec4f(0.0, 0.0, 1.0, 1.0));
	m_previewImage.setState(0);
	m_previewImage.setBorder(4);	
		
	
	ppogl::FontRef titlefont =
			ppogl::FontMgr::getInstance().get("race_requirements_label");
	ppogl::FontRef labelfont =
			ppogl::FontMgr::getInstance().get("race_requirements");

	m_authorTitle.setPosition(ppogl::Vec2d(100,135));
	m_authorTitle.setFont(titlefont);
	m_authorLbl.setPosition(ppogl::Vec2d(
		105 + titlefont->advance(m_authorTitle.getText()),
		135));
	m_authorLbl.setFont(labelfont);
	
	m_timeTitle.setPosition(ppogl::Vec2d(100,260));
	m_timeTitle.setFont(titlefont);
	m_timeLbl.setPosition(ppogl::Vec2d(
		105 + titlefont->advance(m_timeTitle.getText()),
		260));
	m_timeLbl.setFont(labelfont);
	
	m_herringTitle.setPosition(ppogl::Vec2d(200,260));
	m_herringTitle.setFont(titlefont);
	m_herringLbl.setPosition(ppogl::Vec2d(
		205 + titlefont->advance(m_herringTitle.getText()),
		260));
	m_herringLbl.setFont(labelfont);
	
	m_scoreTitle.setPosition(ppogl::Vec2d(300,260));
	m_scoreTitle.setFont(titlefont);
	m_scoreLbl.setPosition(ppogl::Vec2d(
		305 + titlefont->advance(m_scoreTitle.getText()),
		260));
	m_scoreLbl.setFont(labelfont);
		
	updateButtonEnabledStates();
	updateData();

	ppogl::AudioMgr::getInstance().playMusic("start_screen");
}

void
RaceSelect::loop(float timeStep)
{
    set_gl_options( GUI );

	// draw snow and set windy to the winSSBtn state
	drawSnow(timeStep, m_windIBtn.getState());

	ppogl::UIManager::getInstance().draw(resolutionX,
										 resolutionY);
	
    reshape(resolutionX, resolutionY);
}

void
RaceSelect::updateRaceData()
{
  	GameMgr::getInstance().getCurrentRace().course = (*curElem).course;
	GameMgr::getInstance().getCurrentRace().setName((*curElem).getName());
	GameMgr::getInstance().getCurrentRace().setDescription((*curElem).getDescription());
	GameMgr::getInstance().getCurrentRace().mirrored = bool(m_mirrorIBtn.getState());
	GameMgr::getInstance().getCurrentRace().condition = static_cast<CourseData::Condition>(m_conditionsIBtn.getState());
	GameMgr::getInstance().getCurrentRace().windy = bool(m_windIBtn.getState());
	GameMgr::getInstance().getCurrentRace().snowing = bool(m_snowIBtn.getState());
}

void
RaceSelect::updateStatusMsg()
{
	PlayerCourseData data;
	
	if(players[0].getOpenCourseData((*curElem).getName(),data)){
		char buff[12];
		int minutes;
		int seconds;
		int hundredths;
	
		getTimeComponents( data.time, minutes, seconds, hundredths );

		snprintf(buff, 12, "%02d:%02d.%02d",minutes, seconds, hundredths);
		m_timeLbl.setText(buff);
		
		snprintf(buff, 12, "%03d", data.herring); 
		m_herringLbl.setText(buff);
				
		snprintf(buff, 12, "%06d", data.score);
		m_scoreLbl.setText(buff);
	}else{
		m_timeLbl.setText("");
		m_herringLbl.setText("");
		m_scoreLbl.setText("");
	}
	if(!(*curElem).author.empty()){
		m_authorLbl.setText((*curElem).author);
	}else{
		m_authorLbl.setText(_("Unknown"));
	}	
}


void
RaceSelect::updateButtonEnabledStates()
{
	//m_startBtn.setSensitive();
}

void
RaceSelect::back()
{
	setMode( GAME_TYPE_SELECT );
}

void
RaceSelect::start()
{
	updateRaceData();	
	loop( 0 );
    setMode( LOADING );
}


void
RaceSelect::updateData()
{	GameMgr::getInstance().setCurrentRace(curElem);	

	curElem=m_raceListBox.getSelectedData();
	GameMgr::getInstance().setCurrentRace(curElem);	
	updateRaceData();
	m_descTa.setText((*curElem).getDescription());
	updateButtonEnabledStates();
	
	ppogl::TextureRef texture =
		ppogl::TextureMgr::getInstance().get((*curElem).course);

	if(!texture){
		texture =
			ppogl::TextureMgr::getInstance().get("no_preview");
		if(!texture){
			PP_WARNING("Unable to get no_preview Image");
		}
	}
	
	m_previewImage.setTexture(texture);	
	updateStatusMsg();	
}

bool
RaceSelect::keyPressEvent(SDLKey key)
{
	switch (key){
		case SDLK_UP:
		case SDLK_LEFT:
			--m_raceListBox;
			updateData();
	    	return true;
		case SDLK_RIGHT:
		case SDLK_DOWN:
			++m_raceListBox;
			updateData();
	    	return true;
		case SDLK_RETURN: 
	    	start();
			return true;
		case SDLK_ESCAPE: 
	    	back();
			return true;
		case 'c': 
	    	m_conditionsIBtn.setClicked();
	    	return true;
		case 'w': 
	    	m_windIBtn.setClicked();
	    	return true;
		case 'm':
	    	m_mirrorIBtn.setClicked();
	    	return true;
		default:
			return false;
	}	
}
