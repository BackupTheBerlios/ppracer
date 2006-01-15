/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2005-2006 Volker Stroebel <volker@planetpenguin.de>
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

#include "course_mgr.h"

#include "game_mgr.h"

#include "ppogl/ppogl_script.h"

std::list<CourseData> openCourseList;
std::list<EventData> eventList;

const std::string&
CupData::getName()
{
	std::map<std::string, std::string>::iterator it;
	std::string lang = ppogl::Config::getInstance().getString("ui_language");
	
	if((it=m_names.find(lang))!=m_names.end()){
		return (*it).second;
	}else{
		return m_names["default"];
	}
}

void
CupData::setName(const std::string& name, const std::string& lang)
{
	m_names[lang]=name;
}

const std::string&
EventData::getName()
{
	std::map<std::string, std::string>::iterator it;
	std::string lang = ppogl::Config::getInstance().getString("ui_language");
	
	if((it=m_names.find(lang))!=m_names.end()){
		return (*it).second;
	}else{
		return m_names["default"];
	}
}

void
EventData::setName(const std::string& name, const std::string& lang)
{
	m_names[lang]=name;
}


static int
register_course_cb(ppogl::Script *vm) 
{
	if(!vm->isTable(1)){
		PP_WARNING("ppracer.register_course: First argument is no table");
		return 0;
	}
	
	CourseData data;
	
	data.course = vm->getStringFromTable("path");
	data.setName(vm->getStringFromTable("name"));
	data.setDescription(vm->getStringFromTable("description"));
	data.author=vm->getStringFromTable("author");
		
	vm->pushString("names");
	if(vm->get(-2) && vm->isTable()){
		vm->pushNull();
		while(vm->next(-2)){
			data.setName(vm->getString(),vm->getString(-2));				
			vm->pop(2);
		}
		vm->pop(2);
	}

	vm->pushString("descriptions");
	if(vm->get(-2) && vm->isTable()){
		vm->pushNull();
		while(vm->next(-2)){
			data.setDescription(vm->getString(),vm->getString(-2));				
			vm->pop(2);
		}
		vm->pop(2);
	}
	
	if(vm->isKeyInTable("par_time")){
		data.par_time=vm->getFloatFromTable("par_time");	
	}
	
	openCourseList.push_back(data);
	
    return 0;
}

CourseData::CourseData()
 : par_time(0),
   condition(CONDITION_SUNNY),
   mirrored(false),
   windy(false),
   snowing(false)
{
}

const std::string&
CourseData::getName()
{
	std::map<std::string, std::string>::iterator it;
	std::string lang = ppogl::Config::getInstance().getString("ui_language");
	
	if((it=names.find(lang))!=names.end()){
		return (*it).second;
	}else{
		return names["default"];
	}
}

const std::string&
CourseData::getDescription()
{
	std::map<std::string, std::string>::iterator it;
	std::string lang = ppogl::Config::getInstance().getString("ui_language");
	
	if((it=descriptions.find(lang))!=descriptions.end()){
		return (*it).second;
	}else{
		return descriptions["default"];
	}
}
		
void
CourseData::setName(const std::string& name, const std::string& lang)
{
	names[lang]=name;
}

void
CourseData::setDescription(const std::string& description, const std::string& lang)
{
	descriptions[lang]=description;		
}

static std::list<CourseData>
register_courses(ppogl::Script *vm)
{
	std::list<CourseData> list;
	vm->pushString("courses");
	if(vm->get() && vm->isArray()){
		vm->pushNull();
		while(vm->next(-2)){
			std::string name = vm->getStringFromTable("name");
			CourseData course;
			std::list<CourseData>::iterator it;
			for(it=openCourseList.begin();it!=openCourseList.end();it++){
				if((*it).course==name){
					course = (*it);
					break;
				}		
			}
			
			vm->pushString("herrings");
			if(vm->get() && vm->isArray()){
				vm->pushNull();
				for(int i=0; i<4; i++){
					if(!vm->next(-2)) break;
					course.herring_req[i]=vm->getInt();
					vm->pop(2);
				}			
				vm->pop(2);
			}

			vm->pushString("scores");
			if(vm->get() && vm->isArray()){
				vm->pushNull();
				for(int i=0; i<4; i++){
					if(!vm->next(-2)) break;
					course.score_req[i]=vm->getInt();
					vm->pop(2);
				}			
				vm->pop(2);
			}

			vm->pushString("times");
			if(vm->get() && vm->isArray()){
				vm->pushNull();
				for(int i=0; i<4; i++){
					if(!vm->next(-2)) break;
					course.time_req[i]=vm->getFloat();
					vm->pop(2);
				}			
				vm->pop(2);
			}
			
			if(vm->isKeyInTable("condition")){
				std::string condition = vm->getStringFromTable("condition");
				if(condition=="cloudy"){
					course.condition=CourseData::CONDITION_CLOUDY;
				}else if(condition=="night"){
					course.condition=CourseData::CONDITION_NIGHT;
				}else if(condition=="evening"){
					course.condition=CourseData::CONDITION_EVENING;
				}else{
					course.condition=CourseData::CONDITION_SUNNY;
				}			
			}
			
			if(vm->isKeyInTable("mirrored")){
				course.mirrored = vm->getBoolFromTable("mirrored");				
			}

			if(vm->isKeyInTable("windy")){
				course.windy = vm->getBoolFromTable("windy");				
			}		
			
			if(vm->isKeyInTable("snowing")){
				course.snowing = vm->getBoolFromTable("snowing");				
			}
						
			list.push_back(course);
			vm->pop(2);
		}
		vm->pop(2);
	}else{
		PP_WARNING("ppracer.register_event: Unable to get course list for cup");
	}
	
	return list;	
}

static std::list<CupData>
register_cups(ppogl::Script *vm)
{
	std::list<CupData> list;
	
	vm->pushString("cups");
	if(vm->get() && vm->isArray()){
		vm->pushNull();
		while(vm->next(-2)){
			CupData cup;
			cup.setName(vm->getStringFromTable("name"));
			cup.raceList = register_courses(vm);
			vm->pushString("names");
			if(vm->get(-2) && vm->isTable()){
				vm->pushNull();
				while(vm->next(-2)){
					cup.setName(vm->getString(),vm->getString(-2));				
					vm->pop(2);
				}
				vm->pop(2);
			}
			vm->pop(2);
			list.push_back(cup);
		}
		vm->pop(2);
	}else{
		PP_WARNING("ppracer.register_event: Unable to get cups list for event");
	}
	return list;
}

static int
register_event_cb(ppogl::Script *vm)
{
	if(vm->isTable()){
		EventData event;
		event.setName(vm->getStringFromTable("name"));
		event.cupList = register_cups(vm);
		vm->pushString("names");
		if(vm->get(-2) && vm->isTable()){
			vm->pushNull();
			while(vm->next(-2)){
				event.setName(vm->getString(),vm->getString(-2));				
				vm->pop(2);
			}
			vm->pop(2);
		}
		eventList.push_back(event);
	}else{
		PP_WARNING("ppracer.register_event: Argument is no array");
	}
	return 0;
}

static const struct ppogl::Script::Lib ppracerlib[]={
	{"register_course", register_course_cb},
	{"register_event", register_event_cb},
	{NULL, NULL}
};

void
register_course_manager_callbacks()
{
	script.registerLib("ppracer", ppracerlib);	
}
