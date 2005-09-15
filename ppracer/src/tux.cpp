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

std::string     tuxRootNode;
std::string     tuxLeftShoulderJoint;
std::string     tuxRightShoulderJoint;
std::string     tuxLeftHipJoint;
std::string     tuxRightHipJoint;
std::string     tuxLeftKneeJoint;
std::string     tuxRightKneeJoint;
std::string     tuxLeftAnkleJoint;
std::string     tuxRightAnkleJoint;
std::string     tuxTailJoint;
std::string     tuxNeck;
std::string     tuxHead;

void adjust_tux_joints( double turnFact, bool isBraking, 
			double paddling_factor, double speed,
			ppogl::Vec3d net_force, double flap_factor )
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
    reset_scene_node( tuxLeftShoulderJoint );
    reset_scene_node( tuxRightShoulderJoint );

    if ( isBraking ) {
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
    rotate_scene_node( tuxLeftShoulderJoint, 'z', 
		       MIN( braking_angle + paddling_angle + turning_angle[0],
			    MAX_ARM_ANGLE ) + flap_angle );
    rotate_scene_node( tuxRightShoulderJoint, 'z',
		       MIN( braking_angle + paddling_angle + turning_angle[1], 
			    MAX_ARM_ANGLE ) + flap_angle );


    // Adjust arms for paddling 
    rotate_scene_node( tuxLeftShoulderJoint, 'y', -ext_paddling_angle );
    rotate_scene_node( tuxRightShoulderJoint, 'y', ext_paddling_angle );

    force_angle = MAX( -20.0, MIN( 20.0, -net_force.z() / 300.0 ) );
    turn_leg_angle = turnFact * 10;
    
	// Adjust hip joints 
    reset_scene_node( tuxLeftHipJoint );
    rotate_scene_node( tuxLeftHipJoint, 'z', -20 + turn_leg_angle
		       + force_angle );
    reset_scene_node( tuxRightHipJoint );
    rotate_scene_node( tuxRightHipJoint, 'z', -20 - turn_leg_angle
		       + force_angle );
	
    // Adjust knees
    reset_scene_node( tuxLeftKneeJoint );
    rotate_scene_node( tuxLeftKneeJoint, 'z', -10 + turn_leg_angle
		       - MIN( 35, speed ) + kick_paddling_angle
		       + force_angle );
    reset_scene_node( tuxRightKneeJoint );
    rotate_scene_node( tuxRightKneeJoint, 'z', -10 - turn_leg_angle
		       - MIN( 35, speed ) - kick_paddling_angle 
		       + force_angle );

    // Adjust ankles 
    reset_scene_node( tuxLeftAnkleJoint );
    rotate_scene_node( tuxLeftAnkleJoint, 'z', -20 + MIN(50, speed ) );
    reset_scene_node( tuxRightAnkleJoint );
    rotate_scene_node( tuxRightAnkleJoint, 'z', -20 + MIN(50, speed ) );

	// Turn tail 
    reset_scene_node( tuxTailJoint );
    rotate_scene_node( tuxTailJoint, 'z', turnFact * 20 );

	// Adjust head and neck 
    reset_scene_node( tuxNeck );
    rotate_scene_node( tuxNeck, 'z', -50 );
    reset_scene_node( tuxHead );
    rotate_scene_node( tuxHead, 'z', -30 );

	// Turn head when turning 
    rotate_scene_node( tuxHead, 'y', -turnFact * 70 );

}

void draw_tux()
{
    // XXX: For some reason, inserting this call here makes Tux render
    // with correct lighting under Mesa 3.1. I'm guessing it's a Mesa bug.
    //
    gl::Material( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ppogl::Color::black);

    set_gl_options( TUX );

    // Turn on lights
    setup_course_lighting();

    draw_scene_graph( tuxRootNode );	
} 

std::string& get_tux_root_node() { return tuxRootNode; } 
std::string& get_tux_left_shoulder_joint() { return tuxLeftShoulderJoint; } 
std::string& get_tux_right_shoulder_joint() { return tuxRightShoulderJoint; } 
std::string& get_tux_left_hip_joint() { return tuxLeftHipJoint; } 
std::string& get_tux_right_hip_joint() { return tuxRightHipJoint; } 
std::string& get_tux_left_knee_joint() { return tuxLeftKneeJoint; } 
std::string& get_tux_right_knee_joint() { return tuxRightKneeJoint; } 
std::string& get_tux_left_ankle_joint() { return tuxLeftAnkleJoint; } 
std::string& get_tux_right_ankle_joint() { return tuxRightAnkleJoint; } 
std::string& get_tux_tail_joint() { return tuxTailJoint; } 
std::string& get_tux_neck() { return tuxNeck; } 
std::string& get_tux_head() { return tuxHead; } 

static int
head_cb(ppogl::Script *vm)
{
    tuxHead = vm->getString(1);
    return 0;
} 

static int
neck_cb(ppogl::Script *vm) 
{	
    tuxNeck = vm->getString(1);
    return 0;
} 

static int
root_node_cb(ppogl::Script *vm) 
{
    tuxRootNode = vm->getString(1);
    return 0;
} 

static int
left_shoulder_cb(ppogl::Script *vm)
{	
    tuxLeftShoulderJoint = vm->getString(1);
    return 0;
} 

static int
right_shoulder_cb(ppogl::Script *vm)
{	
    tuxRightShoulderJoint = vm->getString(1);
    return 0;
} 

static int
left_hip_cb(ppogl::Script *vm)
{	
    tuxLeftHipJoint = vm->getString(1);
    return 0;
} 

static int
right_hip_cb(ppogl::Script *vm) 
{	
    tuxRightHipJoint = vm->getString(1);
    return 0;
} 

static int
left_knee_cb(ppogl::Script *vm) 
{	
    tuxLeftKneeJoint = vm->getString(1);
    return 0;
} 

static int
right_knee_cb(ppogl::Script *vm) 
{	
	tuxRightKneeJoint = vm->getString(1);
    return 0;
} 

static int
left_ankle_cb(ppogl::Script *vm) 
{	
	tuxLeftAnkleJoint = vm->getString(1);
    return 0;
} 

static int
right_ankle_cb(ppogl::Script *vm) 
{	
	tuxRightAnkleJoint = vm->getString(1);
    return 0;
} 

static int
tail_cb(ppogl::Script *vm) 
{	
	tuxTailJoint = vm->getString(1);
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
