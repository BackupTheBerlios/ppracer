/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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

#include "game_mgr.h"

#include "ppogl/base/defs.h"

#include "SDL.h"

PPOGL_SINGLETON(GameMgr);

GameMgr::GameMgr()
{
	m_lastTicks = SDL_GetTicks()/1000.0;
}


void
GameMgr::setupEventAndCup( std::list<EventData>::iterator event,
						std::list<CupData>::iterator cup)
{
	currentEvent=event;
	currentCup=cup;							
}

void
GameMgr::setCurrentRace(std::list<CourseData>::iterator race)
{
	mi_currentRace = race;
}

void
GameMgr::reset(GameType type)
{
	gametype=type;
}

unsigned int
GameMgr::calculateScore(double _time, int herring, int health)
{
	const int timescore = MAX( 0, int(100*((*mi_currentRace).time_req[CourseData::DIFFICULTY_EASY]-_time)));
	const int herringscore = 200*herring;
	
	//for future use
	const int healthscore = 0*health;
	
	return timescore+herringscore+healthscore;
}


void
GameMgr::updatePlayersScores()
{
	for(int i=0; i<numPlayers; i++){
		players[i].score=calculateScore(time,
										players[i].herring,
										players[i].health);
	}
}

bool
GameMgr::updateCurrentRaceData()
{
	m_eventWon=false;
	m_cupWon=false;
	
	if(	time <= (*mi_currentRace).time_req[difficulty] &&
	 	players[0].herring >= (*mi_currentRace).herring_req[difficulty] &&
	 	players[0].score >= (*mi_currentRace).score_req[difficulty] ) 
    {
		m_raceWon=true;		
	} else {
		m_raceWon=false;
    }	
	
	const bool bestScore = players[0].updateCupCourseData(
					(*currentEvent).getName(),
					(*currentCup).getName(),
					(*mi_currentRace).getName(),
					time,
					players[0].herring,
					players[0].score,
					m_raceWon);
	
	if(m_raceWon){	
		std::list<CourseData>::iterator lastRace=(*currentCup).raceList.end();
		lastRace--;
		
		if(mi_currentRace == lastRace){
			m_cupWon = true;
			players[0].setCupComplete(	(*currentEvent).getName(),
										(*currentCup).getName());
			std::list<CupData>::iterator lastCup=(*currentEvent).cupList.end();
			lastCup--;
			if(currentCup == lastCup){
				m_eventWon = true;
				players[0].setEventComplete((*currentEvent).getName());
			}
			if(!PPConfig.getBool("always_save_event_race_data")){
				players[0].saveData();
			}
		}
	}
	if(PPConfig.getBool("always_save_event_race_data")){
		players[0].saveData();
	}
	return bestScore;
}

bool
GameMgr::wasRaceWon()
{
	return m_raceWon;	
}

bool
GameMgr::wasCupWon()
{
	return m_cupWon;	
}

bool
GameMgr::wasEventWon()
{
	return m_eventWon;	
}

void
GameMgr::resetTimeStep()
{
	m_lastTicks = SDL_GetTicks()/1000.0;
}

void
GameMgr::updateTimeStep()
{
	const double ticks = SDL_GetTicks()/1000.0;
	timeStep = ticks - m_lastTicks;
	timeStep = MAX(timeStep,EPS);
	m_lastTicks = ticks;
}
