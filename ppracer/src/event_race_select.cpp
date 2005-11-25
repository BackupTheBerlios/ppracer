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

#include "event_race_select.h"

#include "ppogl/font.h"

#include "course_load.h"
#include "joystick.h"

#include "stuff.h"
#include "game_mgr.h"
#include "player.h"

#include <iostream>

static std::list<CourseData>::iterator curElem;

EventRaceSelect::EventRaceSelect()
 : m_titleLbl(_("Select a race"),"heading"), 
   m_descTa(300,104),
   m_raceListBox(450,36),
   m_backBtn(_("Back")),
   m_startBtn(_("Race!")),
   m_previewImage(ppogl::Vec2d(128,96)),
   m_authorTitle(_("Contributed by:")),
   m_timeTitle(_("Time:")),
   m_herringTitle(_("Herring:")),
   m_scoreTitle(_("Score:")),
   m_statusLbl("","race_requirements_label")
{
	ppogl::UIManager::getInstance().setBoxDimension(ppogl::Vec2d(640,480));	

	if ( GameMode::prevmode != GAME_OVER ) {
		curElem = GameMgr::getInstance().getCurrentCup().raceList.begin();
    } else {
		if( GameMgr::getInstance().wasRaceWon() ){
			if(curElem != --GameMgr::getInstance().getCurrentCup().raceList.end()){
				curElem++;
			}
		}
    }
	
	{
		std::list<CourseData> &courses = 
			GameMgr::getInstance().getCurrentCup().raceList;
				
		std::list<CourseData>::iterator it;
		for(it=courses.begin(); it!=courses.end();it++){
			m_raceListBox.addElement((*it).getName(), it);			
		}
	}
	
	
	m_titleLbl.setPosition(ppogl::Vec2d(320,400));
	m_titleLbl.alignment.center();
	
    m_backBtn.setPosition(ppogl::Vec2d(80,0));
    m_backBtn.signalClicked.Connect(ppogl::CreateSlot(this,&EventRaceSelect::back));

	m_startBtn.setPosition(ppogl::Vec2d(560,0));
	m_startBtn.alignment.right();
	m_startBtn.signalClicked.Connect(ppogl::CreateSlot(this,&EventRaceSelect::start));
 
	m_raceListBox.setPosition(ppogl::Vec2d(320,320));
	m_raceListBox.alignment.center();
    m_raceListBox.setSelectedData(curElem);
	m_raceListBox.signalChanged.Connect(ppogl::CreateSlot(this,&EventRaceSelect::updateData));
    
	// Create text area 
    m_descTa.setPosition(ppogl::Vec2d(100,150));
	m_descTa.setText((*curElem).getDescription());
    
	for(int i=0; i<4; i++){
		m_livesImg[i].setPosition(ppogl::Vec2d(416+i*36,260));
		m_livesImg[i].setTexture("life_icon");
		m_livesImg[i].addState(0,ppogl::Vec4f(0.0, 0.0, 1.0, 0.5));
		m_livesImg[i].addState(1,ppogl::Vec4f(0.0, 0.5, 1.0, 1.0));
		m_livesImg[i].setState(0);	
	}

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
		
	// status message
	m_statusLbl.setPosition(ppogl::Vec2d(100,275));

	updateStates();
	updateData();

	ppogl::AudioMgr::getInstance().playMusic("start_screen");
}

void
EventRaceSelect::loop(float timeStep)
{
    set_gl_options( GUI );

	drawSnow(timeStep, true);
	
    ppogl::UIManager::getInstance().draw(resolutionX,
										 resolutionY);

    reshape(resolutionX, resolutionY);
}

void
EventRaceSelect::updateStatusMsg()
{
    std::string msg;
	
	switch(state){
		case DEAD:
			msg = _("You don't have any lives left.");
			break;
		
		case RACEWON:
			msg = _("Race won! Your result:");	
			break;
		
		case PLAYABLE:
			msg = _("Needed to advance:");
			break;
		
		case UNPLAYABLE:
			msg = _("You can't enter this race yet.");
			break;
		default:
			msg = "";
			break;
	}
	
	m_statusLbl.setText(msg);
		
	int minutes=0;
	int seconds=0;
	int hundredths=0;

	getTimeComponents( m_data.time, minutes, seconds, hundredths );
		
	char buff[12];

	snprintf(buff, 12, "%02d:%02d.%02d",minutes, seconds, hundredths);
	m_timeLbl.setText(buff);
		
	snprintf(buff, 12, "%03d", m_data.herring ); 
	m_herringLbl.setText(buff);;
		
	snprintf(buff, 12, "%06d", m_data.score );
	m_scoreLbl.setText(buff);
		
	if(!(*curElem).author.empty()){
		m_authorLbl.setText((*curElem).author);
	}else{
		m_authorLbl.setText(_("Unknown"));
	}	
}

void
EventRaceSelect::updateStates()
{
	if ( players[0].getLives() <= 0 ) {
		state=DEAD;
		m_startBtn.setInsensitive(true);
	} else {
		m_data.won=false;
		players[0].getCupCourseData(
				std::string(GameMgr::getInstance().getCurrentEvent().getName()),
				std::string(GameMgr::getInstance().getCurrentCup().getName()),
				std::string((*curElem).getName()),
				m_data);
	
		if(m_data.won){
			state=RACEWON;
			m_startBtn.setInsensitive(true);	
		}else{
			
			CourseData::Difficulty d = GameMgr::getInstance().difficulty;
			m_data.time = (*curElem).time_req[d];
			m_data.herring = (*curElem).herring_req[d];
			m_data.score = int((*curElem).score_req[d]);

			if(curElem==GameMgr::getInstance().getCurrentCup().raceList.begin()){
				state=PLAYABLE;
				m_startBtn.setInsensitive(false);
			}else{
				PlayerCourseData prev;
				curElem--;
				players[0].getCupCourseData(
					std::string(GameMgr::getInstance().getCurrentEvent().getName()),
					std::string(GameMgr::getInstance().getCurrentCup().getName()),
					std::string((*curElem).getName()),
					prev);
				curElem++;
				
				if(prev.won){
					state=PLAYABLE;
					m_startBtn.setInsensitive(false);
				} else {
					state=UNPLAYABLE;
					m_startBtn.setInsensitive(true);
				}
			}
		}
	}
}

void
EventRaceSelect::back()
{
	setMode( EVENT_SELECT );
}

void
EventRaceSelect::start()
{
    loop( 0 );
	GameMgr::getInstance().setCurrentRace(curElem);
    setMode( LOADING );
}


void
EventRaceSelect::updateData()
{
	curElem = m_raceListBox.getSelectedData();
	m_descTa.setText((*curElem).getDescription());
	updateStates();
	updateStatusMsg();
	
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
	
	// set life images
	PP_ASSERT(INIT_NUM_LIVES==4, 
				"Assumption about number of lives invalid -- "
				"need to recode this part" );

	for(int i=0; i<4; i++ ){
		if(players[0].getLives() > i){
			m_livesImg[i].setState(1);	
		}else{
			m_livesImg[i].setState(0);
		}
	}

}

bool
EventRaceSelect::keyPressEvent(SDLKey key)
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
			if(!m_startBtn.isInsensitive()){
				start();
			}
			return true;
		case SDLK_ESCAPE:
	    	back();
			return true;
		default:
			return false;
	}	
}
