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

#ifndef _PP_TYPES_H_
#define _PP_TYPES_H_

#include "ppracer.h"

#include "ppogl/base/color.h"
#include "ppogl/base/vec2d.h"
#include "ppogl/base/vec3d.h"
#include "ppogl/base/vec3f.h"
#include "ppogl/base/vec4f.h"

#include "ppogl/base/glwrappers.h"

#include "algstuff.h"

struct Triangle
{
    ppogl::Vec3d p[3];
    ppogl::Vec2d t[3];
};

/// Ray (half-line)
struct Ray
{ 
    ppogl::Vec3d pt;
    ppogl::Vec3d vec;
};

/// Material
struct Material
{
    ppogl::Color diffuse;
    ppogl::Color specular;
    float specular_exp;
};

/// Light
struct Light
{
    bool is_on;

	ppogl::Color ambient;
    ppogl::Color diffuse;
    ppogl::Color specular;
    ppogl::Vec4f position;
	ppogl::Vec3f spot_direction;
	
    GLfloat spot_exponent;
    GLfloat spot_cutoff;
    GLfloat constant_attenuation;
    GLfloat linear_attenuation;
    GLfloat quadratic_attenuation;
};

/// Scene graph node types.
enum Geometry
{ 
    Empty, Sphere
};

/// Data for Sphere node type.
struct SphereNode
{
    float radius;
	
	/// How many divisions do we use to draw a sphere?
    int divisions;		
};

/// Tux's eyes
enum TuxEye
{
    TuxLeftEye = 0, 
    TuxRightEye = 1
};
  
/// Scene graph node.
struct SceneNode
{
    SceneNode* parent;
    SceneNode* next;
    SceneNode* child;

	/// type of node
    Geometry geom;

    union{
        SphereNode sphere;   
    }param;
      
    Material* mat;


    /// Do we draw the shadow of this node?
    bool render_shadow;

    /// Is this node one of tux's eyes?
    bool eye;

    /// If so, which one?
    TuxEye which_eye;

    /// The forward and inverse transforms
    pp::Matrix trans;
    pp::Matrix invtrans;   

};

/// Difficulty levels
enum DifficultyLevel
{
    DIFFICULTY_LEVEL_EASY,
    DIFFICULTY_LEVEL_NORMAL,
    DIFFICULTY_LEVEL_HARD,
    DIFFICULTY_LEVEL_INSANE,
    DIFFICULTY_NUM_LEVELS
};

/// Race conditions
enum RaceConditions
{
    RACE_CONDITIONS_SUNNY,
    RACE_CONDITIONS_CLOUDY,
    RACE_CONDITIONS_NIGHT,
	RACE_CONDITIONS_EVENING,
    RACE_CONDITIONS_NUM_CONDITIONS
};

/// View mode
enum ViewMode
{
    BEHIND,
    FOLLOW,
    ABOVE,
    NUM_VIEW_MODES
};

//// View point
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


/// Max number of terrains types
#define NUM_TERRAIN_TYPES 64

#endif // _PP_TYPES_H_
