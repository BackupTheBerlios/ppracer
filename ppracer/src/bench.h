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


#ifndef _BENCH_H_
#define _BENCH_H_

#include "loop.h"

#include "course_mgr.h"

#include <string>


class Benchmark
 : public GameMode
{	
	static std::string sm_course;
	static double sm_frames;
	static double sm_oldFrames;
	static int sm_framesCounter;
	static int sm_maxFrames;
	static int sm_fc;
	static ppogl::Vec2d sm_pos;
	static double sm_timeStep;
	static CourseData::Condition sm_condition;
	
public:
	void loop(float timeStep);

	enum BenchMode{
		NONE=0,
		AUTO=1,
		DEMO,
		PAUSED		
	};

	static void setCourse(const std::string& course);
	static void setMaxTime(double time);
	static void setMaxFrames(int frames);
	static void setPosition(const ppogl::Vec2d& position);
	static const ppogl::Vec2d& getPosition(); 
	static Benchmark::BenchMode getMode();
	static void setMode(Benchmark::BenchMode);
	static void setTimeStep(double timeStep);
	static double getTimeStep(); 
	static void setRaceCondition(int condition);

	static void updateFPS(double fps);
	static void displayState();
	
private:
	static BenchMode sm_mode;
};

#endif // _BENCH_H_
