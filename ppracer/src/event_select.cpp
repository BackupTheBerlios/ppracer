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

#include "event_select.h"

#include "course_load.h"
#include "joystick.h"

#include "player.h"
#include "game_mgr.h"
	
EventSelect::EventSelect()
 : m_eventListBox(400,36),
   m_cupListBox(400,36),
   m_backBtn(_("Back")),
   m_continueBtn(_("Continue")),	
   m_titleLbl(_("Select event and cup"),"menu_label"),
   m_eventLbl(_("Event:"),"event_and_cup_label"),
   m_cupLbl(_("Cup:"),"event_and_cup_label"), 
   m_statusLbl("","cup_status")
{
	if(GameMode::prevmode==EVENT_RACE_SELECT){
		m_curEvent = GameMgr::getInstance().currentEvent;
		m_curCup = GameMgr::getInstance().currentCup;
		if(players[0].isCupComplete((*m_curEvent).getName(),(*m_curCup).getName())){
			if(m_curCup != --(*m_curEvent).cupList.end()){
				m_curCup++;
			}				
		}
	}else{
		m_curEvent = eventList.begin();
		m_curCup = (*m_curEvent).cupList.begin();
	}
	
	
	{
		std::list<EventData>::iterator it;

		for(it=eventList.begin(); it!=eventList.end(); it++){
			m_eventListBox.addElement((*it).getName(), it);		
		}
	}
	
	{
		std::list<CupData>::iterator it;

		for(it=(*m_curEvent).cupList.begin(); it!=(*m_curEvent).cupList.end(); it++){
			m_cupListBox.addElement((*it).getName(), it);		
		}	
	}
		
	ppogl::UIManager::getInstance().setBoxDimension(ppogl::Vec2d(640,480));	

	ppogl::Vec2d position(320,390);
	
	m_titleLbl.setPosition(position);
	m_titleLbl.alignment.center();
		
	position.x()-=200;
	position.y()-=65;
	
	m_eventLbl.setPosition(position);
	
	position.y()-=45;
	m_eventListBox.setPosition(position);
	m_eventListBox.setSelectedData(m_curEvent);
	m_eventListBox.signalChanged.Connect(ppogl::CreateSlot(this,&EventSelect::eventChanged));
	
	position.y()-=45;
	m_cupLbl.setPosition(position);
		
	position.y()-=45;
	m_cupListBox.setPosition(position);
	m_cupListBox.setSelectedData(m_curCup);	
    m_cupListBox.signalChanged.Connect(ppogl::CreateSlot(this,&EventSelect::cupChanged));

	position.y()-=30;
	m_statusLbl.setPosition(ppogl::Vec2d(position.x()+200,position.y()));
	m_statusLbl.alignment.center();

    position.y()-=120;
	position.x()-=50;	
	m_backBtn.setPosition(position);
    m_backBtn.signalClicked.Connect(ppogl::CreateSlot(this,&EventSelect::back));

	position.x()+=350;	
	m_continueBtn.setPosition(position);
    m_continueBtn.signalClicked.Connect(ppogl::CreateSlot(this,&EventSelect::apply));
	
	updateCupStates();
	updateButtonEnabledStates();
}

void
EventSelect::loop(float timeStep)
{
    set_gl_options( GUI );
	drawSnow(timeStep);
    
	ppogl::UIManager::getInstance().draw(resolution);

    reshape(resolution);
}

void
EventSelect::updateCupStates()
{
	m_curCupComplete=false;
	m_curCupPlayable=false;
	
	if ( players[0].isCupComplete( (*m_curEvent).getName(), (*m_curCup).getName()) ) {
		m_curCupComplete=true;
		m_curCupPlayable=true;
    } else if ( players[0].isFirstIncompleteCup( m_curEvent, m_curCup ) ) {
		m_curCupPlayable=true;
    }
	
	std::string string;
	if(m_curCupComplete){
		string = _("You've won this cup!");
	}else if (m_curCupPlayable){
		string = _("You must complete this cup next");
	}else{
		string = _("You cannot enter this cup yet"); 
	}
	m_statusLbl.setText(string);
}

void
EventSelect::updateButtonEnabledStates()
{
	if(m_curCupPlayable){
		m_continueBtn.setInsensitive(false);
	}else{
		m_continueBtn.setInsensitive(true);
	}
}

void
EventSelect::cupChanged()
{
	m_curCup = m_cupListBox.getSelectedData();
	updateCupStates();
	updateButtonEnabledStates();
}

void
EventSelect::eventChanged()
{
	m_curEvent = m_eventListBox.getSelectedData();
	m_curCup = (*m_curEvent).cupList.begin();
	updateCupStates();
	updateButtonEnabledStates();
}

void
EventSelect::back()
{
	setMode(GAME_TYPE_SELECT);
}

void
EventSelect::apply()
{
    m_curEvent = m_eventListBox.getSelectedData();
	m_curCup = m_cupListBox.getSelectedData();

	GameMgr::getInstance().setupEventAndCup(m_curEvent,m_curCup);

	if(!PPConfig.getBool("always_save_event_race_data")){	
		if(!players[0].isCupComplete((*m_curEvent).getName(), (*m_curCup).getName() )){
			players[0].clearCupData((*m_curEvent).getName(), (*m_curCup).getName());
		}
	}
	players[0].resetLives();
    setMode( EVENT_RACE_SELECT );
}

bool
EventSelect::keyPressEvent(SDLKey key)
{
	switch (key){
		case SDLK_UP:
			--m_cupListBox;
			eventChanged();
	    	return true;
		case SDLK_DOWN:
			++m_cupListBox;
			eventChanged();
	    	return true;
		case SDLK_LEFT:
			--m_eventListBox;
	    	eventChanged();
			return true;
		case SDLK_RIGHT:
			++m_eventListBox;
			eventChanged();
	    	return true;
		case SDLK_RETURN:
			if(m_curCupPlayable){
	    		apply();
			}
			return true;
		case SDLK_ESCAPE: 
	    	back();
			return true;
		default:
			return false;
	}	
}
