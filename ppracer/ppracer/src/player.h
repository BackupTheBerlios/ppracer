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

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "course_mgr.h"
#include "algstuff.h"

#include "ppogl/base/vec3d.h"

#include <list>
#include <map>
#include <string>
#include <fstream>

/// View mode
enum ViewMode
{
    BEHIND,
    FOLLOW,
    ABOVE,
    NUM_VIEW_MODES
};

/// View point
struct View
{
    /// View mode
	ViewMode mode;       		
    
	/// position of camera
	ppogl::Vec3d pos;           		
    
	/// position of player
	ppogl::Vec3d plyr_pos;           	
    
	/// viewing direction
	ppogl::Vec3d dir;            		
    
	/// up direction
	ppogl::Vec3d up;               	
    
	/// inverse view matrix
	pp::Matrix inv_view_mat;    	
    
	/// has view been initialized?
	bool initialized;          		
};

/// Control mode
enum ControlMode
{
    KEYBOARD = 0,
    MOUSE = 1,
    JOYSTICK = 2
};

/// Control data
struct Control
{
    /// control mode
	ControlMode mode; 
	
    /// turning [-1,1]
	float turn_fact;
	
    /// animation step [-1,1]
	float turn_animation;
	
    /// is player braking?
	bool is_braking;
	
    /// is player paddling?
	bool is_paddling;
	
    float paddle_time;
    bool begin_jump;
    bool jumping;
    bool jump_charging;
    float jump_amt;
    float jump_start_time;
    bool barrel_roll_left;
    bool barrel_roll_right;
    float barrel_roll_factor;
    bool front_flip;
    bool back_flip;
    float flip_factor;
};

class PlayerCourseData
{
public:
	PlayerCourseData();	

	bool update(double time,int herring,int score, bool won=true);

	bool won;
    double time;
    int herring;
    int score;
};


class PlayerCupData
{
	std::map<std::string,PlayerCourseData> courses;
public:
	PlayerCupData();

	bool won;

	bool getCupCourseData(std::string course,
					PlayerCourseData& data);
	bool updateCupCourseData(std::string course, 
					double time, 
					int herring,
					int score,
					bool won);

	void saveData(std::ofstream& sfile);
	void loadData(std::ifstream& sfile);
};

class PlayerEventData
{
	std::map<std::string,PlayerCupData> cups;	
public:
	PlayerEventData();

	bool won;

	bool isCupWon(std::string cup);
	bool getCupCourseData(std::string cup,
					std::string course,
					PlayerCourseData& data);
	bool updateCupCourseData(std::string cup,
					std::string course, 
					double time, 
					int herring,
					int score,
					bool won);
	void setCupComplete(std::string cup);

	void clearCupData(std::string cup);

	void saveData(std::ofstream& sfile);
	void loadData(std::ifstream& sfile);
};


class Player
{
	std::map<std::string,PlayerEventData> events;
	
	std::map<std::string,PlayerCourseData> courses;
	
	int m_lives;
	
public:
	Player();

	bool isCupComplete(std::string event,
						std::string cup);
	bool isFirstIncompleteCup( std::list<EventData>::iterator event,
						std::list<CupData>::iterator cup);
	
	bool getCupCourseData(std::string event,
					std::string cup,
					std::string course,
					PlayerCourseData& data);

	bool updateCupCourseData(std::string event,
					std::string cup,
					std::string course, 
					double time, 
					int herring,
					int score,
					bool won);

	void setCupComplete(std::string event, std::string cup);
	void setEventComplete(std::string event);
	
	void clearCupData(std::string event, std::string cup);
	

	bool getOpenCourseData(std::string course,
								PlayerCourseData& data);

	bool updateOpenCourseData(std::string course, 
								double time, 
								int herring,
								int score);

	void resetLives();
	int getLives();
	void decLives();
	void incLives();
	
	
	bool saveData();
	bool loadData();
	
	
	/// name of player 
	std::string name;
	
	// for future use
	int health;
	
	int herring;
	
	int score;
	
	int max_speed;

		

    /// current position
	ppogl::Vec3d pos;   
	
    /// current velocity
	ppogl::Vec3d vel;
	
	/// current orientation
	pp::Quat orientation; 
	
    /// is orientation initialized?
	bool orientation_initialized;
	
    /// vector sticking out of bellybutton (assuming on back)
	ppogl::Vec3d plane_nml;
	
	/// vector sticking out of feet
	ppogl::Vec3d direction; 
	
    /// net force on player 
	ppogl::Vec3d net_force;
	
    /// terrain force on player
	ppogl::Vec3d normal_force;  
	
    /// is plyr in the air?
	bool airborne;
	
    /// has plyr collided with obstacle?
	bool collision;
	
	/// player control data
	Control control;

    /// player's view point
	View view;
	
	/// amount of time the player has been flying
	double airbornetime;

	
	float getCoursePercentage() const;

	/// position of the player in the player array
	/// this is a hack that will be removed later
	int num;
};

///global array of players
extern Player players[2];

#endif // _PLAYER_H_
