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
 
#include "bench.h"

#include "game_mgr.h"

#include <iostream>

Benchmark::BenchMode Benchmark::sm_mode = Benchmark::NONE;

std::string Benchmark::sm_course;
double Benchmark::sm_frames=0.0;
double Benchmark::sm_oldFrames=0.0;
int Benchmark::sm_framesCounter=0;
int Benchmark::sm_maxFrames=0;
int Benchmark::sm_fc=0;
ppogl::Vec2d Benchmark::sm_pos;
double Benchmark::sm_timeStep=-1;
CourseData::Condition Benchmark::sm_condition=CourseData::CONDITION_SUNNY;

void
Benchmark::loop(float timeStep)
{
	static std::list<CourseData>::iterator it =
			openCourseList.begin();

	for(it=openCourseList.begin(); 
		it != openCourseList.end();
		it++)
	{
		if((*it).course == sm_course) break;			
	}
	
	if(it != openCourseList.end()){
		GameMgr::getInstance().setCurrentRace(it);
		GameMgr::getInstance().getCurrentRace().condition = sm_condition;
	}else{
		std::cout << "Benchmark error: unable to set course: " 
					<< sm_course << std::endl;
		exit(0);
	}
	
	GameMode::setMode(GameMode::LOADING);
}

void
Benchmark::setCourse(const std::string& course)
{
	sm_course = course;	
	if(sm_mode == Benchmark::NONE){
		sm_mode = Benchmark::DEMO;
	}
}

void
Benchmark::setMaxFrames(int frames)
{
	sm_maxFrames = frames;
}

void
Benchmark::setPosition(const ppogl::Vec2d& position)
{
	sm_pos = position;
}

const ppogl::Vec2d&
Benchmark::getPosition()
{
	return sm_pos;
}

void
Benchmark::setMode(Benchmark::BenchMode _mode)
{
	sm_mode = _mode;
}

Benchmark::BenchMode
Benchmark::getMode()
{
	return sm_mode;
}

void
Benchmark::setTimeStep(double timeStep)
{
	sm_timeStep = timeStep;
}

double
Benchmark::getTimeStep()
{
	return sm_timeStep;
}

void
Benchmark::setRaceCondition(int condition)
{
	if (condition > 0 && 
		condition < CourseData::NUM_CONDITIONS)
	{
		sm_condition = static_cast<CourseData::Condition>(condition);
	}
}

void
Benchmark::updateFPS(double fps)
{
	sm_frames+=fps;
	sm_framesCounter++;
	
	if(sm_fc>=2){
		sm_fc=0;
		std::cout << "FPS: "<< (sm_frames-sm_oldFrames)/3 << std::endl;
		sm_oldFrames = sm_frames;		
	}else{
		sm_fc++;
	}
		
	if( sm_maxFrames > 0 && sm_frames >= sm_maxFrames ){
		GameMode::setMode(GAME_OVER);		
	}
}

void
Benchmark::displayState()
{
	std::cout << std::endl;
	std::cout << "Frames: "
			<< sm_frames << std::endl;
	
	std::cout << "Average FPS: "
			<< sm_frames/sm_framesCounter << std::endl;
}
