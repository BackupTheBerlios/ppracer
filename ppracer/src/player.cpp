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

#include "player.h"
#include "course_load.h"
#include "stuff.h"

#define PLAYER_MAX_LIVES 5

Player players[2];


PlayerCourseData::PlayerCourseData()
 : won(false), time(0.0), herring(0), score(0)
{	
}

bool
PlayerCourseData::update(double _time, int _herring, int _score, bool _won)
{
	won=_won;
	if(score<_score){
		time=_time;
		herring=_herring;
		score=_score;
		return true;
	}
	return false;
}

PlayerEventData::PlayerEventData()
 : won(false)
{	
}

bool
PlayerEventData::isCupWon(std::string cup)
{
	std::map<std::string,PlayerCupData>::iterator it;
	
	if((it=cups.find(cup))!=cups.end()){
		return (*it).second.won;
	}else{
		return false;	
	}	
}

bool
PlayerEventData::getCupCourseData(std::string cup,
					std::string course,
					PlayerCourseData& data)
{
	std::map<std::string,PlayerCupData>::iterator it;
	if( (it=cups.find(cup))!=cups.end() ){
		return (*it).second.getCupCourseData(course,data);
	}
	return false;	
}

bool
PlayerEventData::updateCupCourseData(std::string cup,
					std::string course, 
					double time, 
					int herring,
					int score,
					bool _won)
{
	return cups[cup].updateCupCourseData(course,time,herring,score,_won);
}

void
PlayerEventData::setCupComplete(std::string cup)
{
	cups[cup].won=true;	
}

void
PlayerEventData::clearCupData(std::string cup)
{
	std::map<std::string,PlayerCupData>::iterator it;
	if( (it=cups.find( cup ))!=cups.end() ){
		cups.erase(it);
	}	
}

void
PlayerEventData::saveData(std::ofstream& sfile)
{
	sfile << won << std::endl;
	sfile << cups.size() << std::endl;
	if(cups.size()>0){	
		std::map<std::string,PlayerCupData>::iterator it;
		for(it=cups.begin(); it!=cups.end(); it++){
			sfile << (*it).first << std::endl;
			(*it).second.saveData(sfile);	
		}
	}
}

void
PlayerEventData::loadData(std::ifstream& sfile)
{
	char buff[256];
	int numcups;
	
	sfile >> won;
	sfile >> numcups;
	for(int i=0; i<numcups; i++){	
		sfile.get();
		sfile.getline(buff,256);
		cups[buff].loadData(sfile);
	}
}

PlayerCupData::PlayerCupData()
 : won(false)
{
}

bool
PlayerCupData::getCupCourseData(std::string course,
						PlayerCourseData& data)
{
	std::map<std::string,PlayerCourseData>::iterator it;
	if( (it=courses.find(course))!=courses.end() ){
		data = (*it).second;
		return true;
	}
	return false;	
}

bool
PlayerCupData::updateCupCourseData(std::string course, 
					double time, 
					int herring,
					int score,
					bool _won)
{
	return courses[course].update(time,herring,score,_won);
}

void
PlayerCupData::saveData(std::ofstream& sfile)
{
	sfile << won << std::endl;
	sfile << courses.size() << std::endl;
	if(courses.size()>0){
		std::map<std::string,PlayerCourseData>::iterator it;
		for(it=courses.begin(); it!=courses.end(); it++){
			sfile << (*it).first << std::endl;
			sfile << (*it).second.won << std::endl;
			sfile << (*it).second.time << std::endl;
			sfile << (*it).second.herring << std::endl;
			sfile << (*it).second.score << std::endl;
		}
	}
}

void
PlayerCupData::loadData(std::ifstream& sfile)
{
	char buff[256];
	int numcourses;
	
	sfile >> won;
	sfile >> numcourses;
	for(int i=0; i<numcourses; i++){	
		sfile.get();
		sfile.getline(buff,256);
		PlayerCourseData& course=courses[buff];
		sfile >> course.won;
		sfile >> course.time;
		sfile >> course.herring;
		sfile >> course.score;		
	}
}

Player::Player()
 : health(100)
{
}

bool
Player::isCupComplete( std::string event,
						std::string cup)
{
	std::map<std::string,PlayerEventData>::iterator it;	
	if( (it=events.find( event ))!=events.end() ){
		return (*it).second.isCupWon( cup );
	}else{
		return false;	
	}
}

bool
Player::isFirstIncompleteCup( std::list<EventData>::iterator event,
						std::list<CupData>::iterator cup)
{
	if (cup==(*event).cupList.begin()){
		//first cup in event always playable
		return true;
	} else {
		//check if previous cup was completed
		cup--;
		return isCupComplete((*event).getName(),(*cup).getName());
	}
}

bool
Player::getCupCourseData(std::string event,
					std::string cup,
					std::string course,
					PlayerCourseData& data)
{
	std::map<std::string,PlayerEventData>::iterator it;
	if( (it=events.find( event ))!=events.end() ){
		return (*it).second.getCupCourseData(cup,course,data);
	}
	return false;	
}

bool
Player::updateCupCourseData(std::string event,
					std::string cup,
					std::string course, 
					double time, 
					int _herring,
					int _score,
					bool _won)
{
	return events[event].updateCupCourseData(cup,course,time,_herring,_score,_won);
}

void
Player::setCupComplete(std::string event, std::string cup)
{
	events[event].setCupComplete(cup);	
}
	
void
Player::setEventComplete(std::string event)
{
	events[event].won=true;	
}

void
Player::clearCupData(std::string event, std::string cup)
{
	std::map<std::string,PlayerEventData>::iterator it;
	if( (it=events.find( event ))!=events.end() ){
		(*it).second.clearCupData(cup);
	}
}

void
Player::incLives()
{
	// todo:
	// in practicing mode incrementing lives should increase score
	if(m_lives < PLAYER_MAX_LIVES){
		m_lives++;
	}
}



bool
Player::getOpenCourseData(std::string course, PlayerCourseData& data)
{
	std::map<std::string,PlayerCourseData>::iterator it;
	
	if( (it=courses.find( course ))!=courses.end() ){
		data = (*it).second;
		return true;
	}
	return false;
}

bool
Player::updateOpenCourseData(std::string course, double time, 
								int _herring, int _score)
{
	return courses[course].update(time,_herring,_score);
}

bool
Player::saveData()
{
	std::string filename=get_config_dir_name();
	
	filename+="/";
	filename+=name;
	filename+=".sav";
	
	std::ofstream sfile(filename.c_str());
	
	sfile << "PPRacer SAVE " << 1 << std::endl;
	sfile << "tux" << std::endl;
	sfile << "easy" << std::endl;
	sfile << 1 << std::endl;
	sfile << courses.size() << std::endl;
	
	{
		std::map<std::string,PlayerCourseData>::iterator it;
		for(it=courses.begin(); it!=courses.end(); it++){
			sfile << (*it).first << std::endl;
			sfile << (*it).second.time << std::endl;
			sfile << (*it).second.herring << std::endl;
			sfile << (*it).second.score << std::endl;
		}
	}
	
	
	sfile << events.size() << std::endl;
	
	if(events.size()>0){
		std::map<std::string,PlayerEventData>::iterator it;
		for (it=events.begin(); it!=events.end(); it++){
			sfile << (*it).first << std::endl;	
			(*it).second.saveData(sfile);		
		}
	}
	
	return true;
}

bool
Player::loadData()
{
	char buff[256];
	
	std::string filename = get_config_dir_name();
	
	filename+="/";
	filename+=name;
	filename+=".sav";
	
	std::ifstream sfile(filename.c_str());
	
	if(!sfile) return false;
	
	int version;
	int numcourses;
	
	std::string notused;
			
	sfile >> buff >> buff >> version;
	sfile >> notused; //character
	sfile >> notused; //difficulty
	sfile >> notused; //num difficulties
	sfile >> numcourses;
	
	for (int i=0; i<numcourses ;i++){
		double time;
		int _herring;
		int _score;
		sfile.get();	
		sfile.getline(buff,256);
		sfile >> time >> _herring >> _score;
		PlayerCourseData& data = courses[buff];
		data.time=time;
		data.herring=_herring;
		data.score=_score;		
	}
	
	int numevents;
	
	sfile >> numevents;
	for(int i=0; i<numevents; i++){	
		sfile.get();
		sfile.getline(buff,256);
		events[buff].loadData(sfile);
	}
	
	return true;
}


float
Player::getCoursePercentage() const
///returns the the percentage of the course completion related to 
///the player position in the heightmap
{
	if(pos.y()<0){
		const ppogl::Vec2d& courseDim = Course::getDimensions();
		float correction = 100/tan((Course::getAngle())*M_PI/180.0);
		return (((-1)*pos.y())/courseDim.y())*correction;
	}else{
		return 0;
	}
}
