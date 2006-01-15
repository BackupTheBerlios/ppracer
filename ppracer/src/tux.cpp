/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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

#include "tux.h"

#include "lights.h"
#include "hier_cb.h"
#include "hier.h"
#include "gl_util.h"
#include "course_mgr.h"

#include "ppogl/base/defs.h"
#include "ppogl/base/glwrappers.h"

#define MAX_ARM_ANGLE 30.0
#define MAX_PADDLING_ANGLE 35.0
#define MAX_EXT_PADDLING_ANGLE 30.0
#define MAX_KICK_PADDLING_ANGLE 20.0

Tux tux[2];

void
Tux::adjustJoints( double turnFact, bool isBraking, 
			double paddling_factor, double speed,
			const ppogl::Vec3d& net_force, double flap_factor )
{
    double turning_angle[2] = {0., 0.};
    double paddling_angle = 0.;
    double ext_paddling_angle = 0.;  // arm extension during paddling 
    double kick_paddling_angle = 0.;  // leg kicking during paddling 
    double braking_angle = 0.;
    double force_angle = 0.; // amount that legs move because of force
    double turn_leg_angle = 0.; // amount legs move when turning
    double flap_angle = 0.;

    // move arms
    reset_scene_node( leftShoulderJoint );
    reset_scene_node( rightShoulderJoint );

    if(isBraking){
		braking_angle = MAX_ARM_ANGLE;
    }

    paddling_angle = MAX_PADDLING_ANGLE * sin(paddling_factor * M_PI);
    ext_paddling_angle = MAX_EXT_PADDLING_ANGLE * sin(paddling_factor * M_PI);
    kick_paddling_angle = MAX_KICK_PADDLING_ANGLE * 
	sin(paddling_factor * M_PI * 2.0);

    turning_angle[0] = MAX(-turnFact,0.0) * MAX_ARM_ANGLE;
    turning_angle[1] = MAX(turnFact,0.0) * MAX_ARM_ANGLE;

    flap_angle = MAX_ARM_ANGLE * (0.5 + 0.5*sin(M_PI*flap_factor*6-M_PI/2));

    // Adjust arms for turning 
    rotate_scene_node( leftShoulderJoint, 'z', 
		       MIN( braking_angle + paddling_angle + turning_angle[0],
			    MAX_ARM_ANGLE ) + flap_angle );
    rotate_scene_node( rightShoulderJoint, 'z',
		       MIN( braking_angle + paddling_angle + turning_angle[1], 
			    MAX_ARM_ANGLE ) + flap_angle );


    // Adjust arms for paddling 
    rotate_scene_node( leftShoulderJoint, 'y', -ext_paddling_angle );
    rotate_scene_node( rightShoulderJoint, 'y', ext_paddling_angle );

    force_angle = MAX( -20.0, MIN( 20.0, -net_force.z() / 300.0 ) );
    turn_leg_angle = turnFact * 10;
    
	// Adjust hip joints 
    reset_scene_node( leftHipJoint );
    rotate_scene_node( leftHipJoint, 'z', -20 + turn_leg_angle
		       + force_angle );
    reset_scene_node( rightHipJoint );
    rotate_scene_node( rightHipJoint, 'z', -20 - turn_leg_angle
		       + force_angle );
	
    // Adjust knees
    reset_scene_node( leftKneeJoint );
    rotate_scene_node( leftKneeJoint, 'z', -10 + turn_leg_angle
		       - MIN( 35, speed ) + kick_paddling_angle
		       + force_angle );
    reset_scene_node( rightKneeJoint );
    rotate_scene_node( rightKneeJoint, 'z', -10 - turn_leg_angle
		       - MIN( 35, speed ) - kick_paddling_angle 
		       + force_angle );

    // Adjust ankles 
    reset_scene_node( leftAnkleJoint );
    rotate_scene_node( leftAnkleJoint, 'z', -20 + MIN(50, speed ) );
    reset_scene_node( rightAnkleJoint );
    rotate_scene_node( rightAnkleJoint, 'z', -20 + MIN(50, speed ) );

	// Turn tail 
    reset_scene_node( tailJoint );
    rotate_scene_node( tailJoint, 'z', turnFact * 20 );

	// Adjust head and neck 
    reset_scene_node( neck );
    rotate_scene_node( neck, 'z', -50 );
    reset_scene_node( head );
    rotate_scene_node( head, 'z', -30 );

	// Turn head when turning 
    rotate_scene_node( head, 'y', -turnFact * 70 );

}

void
Tux::draw()
{
    // XXX: For some reason, inserting this call here makes Tux render
    // with correct lighting under Mesa 3.1. I'm guessing it's a Mesa bug.
	gl::Material( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ppogl::Color::black);

    set_gl_options( TUX );

    // Turn on lights
    Light::setup();
	
    draw_scene_graph( rootNode );	
} 

static int
head_cb(ppogl::Script *vm)
{
    int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].head = vm->getString(2);
	}else{
		PP_WARNING("tux.head: Player " << player << " not supported");
	}
    return 0; 
} 

static int
neck_cb(ppogl::Script *vm) 
{	
    int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].neck = vm->getString(2);
	}else{
		PP_WARNING("tux.neck: Player " << player << " not supported");
	}
    return 0; 
} 

static int
root_node_cb(ppogl::Script *vm) 
{
    int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].rootNode = vm->getString(2);
	}else{
		PP_WARNING("tux.root_node: Player " << player << " not supported");
	}
    return 0;
} 

static int
left_shoulder_cb(ppogl::Script *vm)
{	
    int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].leftShoulderJoint = vm->getString(2);
	}else{
		PP_WARNING("tux.left_shoulder: Player " << player << " not supported");
	}
    return 0; 
} 

static int
right_shoulder_cb(ppogl::Script *vm)
{	
    int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].rightShoulderJoint = vm->getString(2);
	}else{
		PP_WARNING("tux.right_shoulder: Player " << player << " not supported");
	}
    return 0;
} 

static int
left_hip_cb(ppogl::Script *vm)
{	
    int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].leftHipJoint = vm->getString(2);
	}else{
		PP_WARNING("tux.left_hip: Player " << player << " not supported");
	}
    return 0;
} 

static int
right_hip_cb(ppogl::Script *vm) 
{	
    int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].rightHipJoint = vm->getString(2);
	}else{
		PP_WARNING("tux.right_hip: Player " << player << " not supported");
	}
    return 0;
} 

static int
left_knee_cb(ppogl::Script *vm) 
{	
    int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].leftKneeJoint = vm->getString(2);
	}else{
		PP_WARNING("tux.left_knee: Player " << player << " not supported");
	}
    return 0;
} 

static int
right_knee_cb(ppogl::Script *vm) 
{	
    int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].rightKneeJoint  = vm->getString(2);
	}else{
		PP_WARNING("tux.right_knee: Player " << player << " not supported");
	}
    return 0; 
} 

static int
left_ankle_cb(ppogl::Script *vm) 
{	
	int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].leftAnkleJoint = vm->getString(2);
	}else{
		PP_WARNING("tux.left_ankle: Player " << player << " not supported");
	}
    return 0; 
} 

static int
right_ankle_cb(ppogl::Script *vm) 
{	
	int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].rightAnkleJoint= vm->getString(2);
	}else{
		PP_WARNING("tux.right_ankle: Player " << player << " not supported");
	}
    return 0; 
} 

static int
tail_cb(ppogl::Script *vm) 
{	
	int player = vm->getInt(1);
	if(player==1 || player==2){
		player--;
		tux[player].tailJoint = vm->getString(2);
	}else{
		PP_WARNING("tux.tail: Player " << player << " not supported");
	}
    return 0; 
} 

static const struct ppogl::Script::Lib tuxlib[]={
	{"root_node", root_node_cb},
    {"left_shoulder",  left_shoulder_cb},
    {"right_shoulder",  right_shoulder_cb},
    {"left_hip",  left_hip_cb},
    {"right_hip",  right_hip_cb},
    {"left_knee",  left_knee_cb},
    {"right_knee",  right_knee_cb},
    {"left_ankle",  left_ankle_cb},
    {"right_ankle",  right_ankle_cb},
    {"neck",  neck_cb},
    {"head", head_cb},
    {"tail", tail_cb},	
	{NULL, NULL}
};

void register_tux_callbacks()
{
	script.registerLib("tux", tuxlib);
}
