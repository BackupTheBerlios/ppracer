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

#ifndef _COURSE_MGR_H
#define _COURSE_MGR_H

#include "ppracer.h"
#include "pp_types.h"

#include <list>
#include <map>
#include <string>

class CourseData
{
public:
	CourseData()
	 : par_time(0),
	   condition(RACE_CONDITIONS_SUNNY),
	   mirrored(false),
	   windy(false),
	   snowing(false)
	{};
	~CourseData(){};

	std::string course;
	std::map<std::string, std::string> names;
	std::map<std::string, std::string> descriptions;
	std::string author;	
		
	const std::string& getName();	
	const std::string& getDescription();
		
	void setName(const std::string& name, const std::string& lang="default");
	void setDescription(const std::string& description, const std::string& lang="default");
		
	double par_time;
    int herring_req[DIFFICULTY_NUM_LEVELS];
	int score_req[DIFFICULTY_NUM_LEVELS];
	double time_req[DIFFICULTY_NUM_LEVELS];

	RaceConditions condition;
	bool mirrored;
	bool windy;
	bool snowing;
};

class CupData
{
	std::map<std::string, std::string> m_names;
public:
	const std::string& getName();
	void setName(const std::string& name, const std::string& lang="default");

    std::list<CourseData> raceList;
};

class EventData
{	
	std::map<std::string, std::string> m_names;
	
public:
	const std::string& getName();
	void setName(const std::string& name, const std::string& lang="default");
	
	std::list<CupData> cupList;
};

extern std::list<CourseData> openCourseList;
extern std::list<EventData> eventList;


// register scripting callbacks
void register_course_manager_callbacks();

#endif // _COURSE_MGR_H
