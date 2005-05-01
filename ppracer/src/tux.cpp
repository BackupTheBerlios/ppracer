/* 
 * PPRacer 
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
#include "game_config.h"
#include "string_util.h"

#include "course_mgr.h"

#include "ppgltk/alg/defs.h"
#include "ppgltk/alg/glwrappers.h"


#define MAX_ARM_ANGLE 30.0
#define MAX_PADDLING_ANGLE 35.0
#define MAX_EXT_PADDLING_ANGLE 30.0
#define MAX_KICK_PADDLING_ANGLE 20.0

static void register_tux_callbacks( Tcl_Interp *ip );

static bool    tuxLoaded = false;
static char*     tuxRootNode;
static char*     tuxLeftShoulderJoint;
static char*     tuxRightShoulderJoint;
static char*     tuxLeftHipJoint;
static char*     tuxRightHipJoint;
static char*     tuxLeftKneeJoint;
static char*     tuxRightKneeJoint;
static char*     tuxLeftAnkleJoint;
static char*     tuxRightAnkleJoint;
static char*     tuxTailJoint;
static char*     tuxNeck;
static char*     tuxHead;

void adjust_tux_joints( double turnFact, bool isBraking, 
			double paddling_factor, double speed,
			pp::Vec3d net_force, double flap_factor )
{
    double turning_angle[2] = {0., 0.};
    double paddling_angle = 0.;
    double ext_paddling_angle = 0.;  /* arm extension during paddling */
    double kick_paddling_angle = 0.;  /* leg kicking during paddling */
    double braking_angle = 0.;
    double force_angle = 0.; /* amount that legs move because of force */
    double turn_leg_angle = 0.; /* amount legs move when turning */
    double flap_angle = 0.;

    /* move arms */
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

    /* Adjust arms for turning */
    rotate_scene_node( tuxLeftShoulderJoint, 'z', 
		       MIN( braking_angle + paddling_angle + turning_angle[0],
			    MAX_ARM_ANGLE ) + flap_angle );
    rotate_scene_node( tuxRightShoulderJoint, 'z',
		       MIN( braking_angle + paddling_angle + turning_angle[1], 
			    MAX_ARM_ANGLE ) + flap_angle );


    /* Adjust arms for paddling */
    rotate_scene_node( tuxLeftShoulderJoint, 'y', -ext_paddling_angle );
    rotate_scene_node( tuxRightShoulderJoint, 'y', ext_paddling_angle );

    force_angle = MAX( -20.0, MIN( 20.0, -net_force.z / 300.0 ) );
    turn_leg_angle = turnFact * 10;
    
	/* Adjust hip joints */
    reset_scene_node( tuxLeftHipJoint );
    rotate_scene_node( tuxLeftHipJoint, 'z', -20 + turn_leg_angle
		       + force_angle );
    reset_scene_node( tuxRightHipJoint );
    rotate_scene_node( tuxRightHipJoint, 'z', -20 - turn_leg_angle
		       + force_angle );
	
    /* Adjust knees */
    reset_scene_node( tuxLeftKneeJoint );
    rotate_scene_node( tuxLeftKneeJoint, 'z', -10 + turn_leg_angle
		       - MIN( 35, speed ) + kick_paddling_angle
		       + force_angle );
    reset_scene_node( tuxRightKneeJoint );
    rotate_scene_node( tuxRightKneeJoint, 'z', -10 - turn_leg_angle
		       - MIN( 35, speed ) - kick_paddling_angle 
		       + force_angle );

    /* Adjust ankles */
    reset_scene_node( tuxLeftAnkleJoint );
    rotate_scene_node( tuxLeftAnkleJoint, 'z', -20 + MIN(50, speed ) );
    reset_scene_node( tuxRightAnkleJoint );
    rotate_scene_node( tuxRightAnkleJoint, 'z', -20 + MIN(50, speed ) );

	/* Turn tail */
    reset_scene_node( tuxTailJoint );
    rotate_scene_node( tuxTailJoint, 'z', turnFact * 20 );

	/* Adjust head and neck */
    reset_scene_node( tuxNeck );
    rotate_scene_node( tuxNeck, 'z', -50 );
    reset_scene_node( tuxHead );
    rotate_scene_node( tuxHead, 'z', -30 );

	/* Turn head when turning */
    rotate_scene_node( tuxHead, 'y', -turnFact * 70 );

}

void draw_tux()
{
    /* XXX: For some reason, inserting this call here makes Tux render
     * with correct lighting under Mesa 3.1. I'm guessing it's a Mesa bug.
     */
    gl::Material( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pp::Color::black);

    set_gl_options( TUX );

    /* Turn on lights
     */
    setup_course_lighting();

    draw_scene_graph( tuxRootNode );
} 

void load_tux()
{
    char cwd[BUFF_LEN];

    if ( tuxLoaded == true ) 
        return;

    tuxLoaded = true;

    registerHierCallbacks( tclInterp );
    register_tux_callbacks( tclInterp );

    initialize_scene_graph();

    if ( getcwd( cwd, BUFF_LEN ) == NULL ) {
		PP_ERROR( "getcwd failed" );
    }

    if ( chdir( getparam_data_dir() ) != 0 ) {
	/* Print a more informative warning since this is a common error */
	PP_ERROR( "Can't find the ppracer data "
	    "directory.  Please check the\nvalue of `data_dir' in "
	    "~/.ppracer/options and set it to the location where you\n"
	    "installed the ppracer-data files.\n\n"
	    "Couldn't chdir to %s", getparam_data_dir() );
	/*
        handle_system_error( 1, "couldn't chdir to %s", getparam_data_dir() );
	*/
    } 

    if ( Tcl_EvalFile( tclInterp, "tux.tcl") == TCL_ERROR ) {
        PP_ERROR( "error evalating %s/tux.tcl: %s\n"
		      "Please check the value of `data_dir' in ~/.ppracer/options "
		      "and make sure it\npoints to the location of the "
		      "latest version of the ppracer-data files.", 
		      getparam_data_dir(), 
		      Tcl_GetStringResult( tclInterp ) );
    } 

    PP_ASSERT( !Tcl_InterpDeleted( tclInterp ),
		     "Tcl interpreter deleted" );

    if ( chdir( cwd ) != 0 ) {
		PP_ERROR( "couldn't chdir to %s", cwd );
    } 
} 

char* get_tux_root_node() { return tuxRootNode; } 
char* get_tux_left_shoulder_joint() { return tuxLeftShoulderJoint; } 
char* get_tux_right_shoulder_joint() { return tuxRightShoulderJoint; } 
char* get_tux_left_hip_joint() { return tuxLeftHipJoint; } 
char* get_tux_right_hip_joint() { return tuxRightHipJoint; } 
char* get_tux_left_knee_joint() { return tuxLeftKneeJoint; } 
char* get_tux_right_knee_joint() { return tuxRightKneeJoint; } 
char* get_tux_left_ankle_joint() { return tuxLeftAnkleJoint; } 
char* get_tux_right_ankle_joint() { return tuxRightAnkleJoint; } 
char* get_tux_tail_joint() { return tuxTailJoint; } 
char* get_tux_neck() { return tuxNeck; } 
char* get_tux_head() { return tuxHead; } 

static int head_cb ( ClientData cd, Tcl_Interp *ip, int argc, CONST84 char *argv[]) 
{
    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
                         "Usage: ", argv[0], " <head joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxHead = string_copy( argv[1] );

    return TCL_OK;
} 

static int neck_cb ( ClientData cd, Tcl_Interp *ip, 
		     int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <neck joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxNeck = string_copy( argv[1] );

    return TCL_OK;
} 

static int root_node_cb ( ClientData cd, Tcl_Interp *ip, 
			  int argc, CONST84 char *argv[]) 
{
    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <root node>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxRootNode = string_copy( argv[1] );

    return TCL_OK;
} 

static int left_shoulder_cb ( ClientData cd, Tcl_Interp *ip, 
			      int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <left shoulder joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxLeftShoulderJoint = string_copy( argv[1] );

    return TCL_OK;
} 

static int right_shoulder_cb ( ClientData cd, Tcl_Interp *ip, 
			       int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <right shoulder joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxRightShoulderJoint = string_copy( argv[1] );

    return TCL_OK;
} 

static int left_hip_cb ( ClientData cd, Tcl_Interp *ip, 
			 int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <left hip joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxLeftHipJoint = string_copy( argv[1] );

    return TCL_OK;
} 

static int right_hip_cb ( ClientData cd, Tcl_Interp *ip, 
			  int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <right hip joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxRightHipJoint = string_copy( argv[1] );

    return TCL_OK;
} 

static int left_knee_cb ( ClientData cd, Tcl_Interp *ip, 
			  int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <left knee joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxLeftKneeJoint = string_copy( argv[1] );

    return TCL_OK;
} 

static int right_knee_cb ( ClientData cd, Tcl_Interp *ip, 
			   int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <right knee joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxRightKneeJoint = string_copy( argv[1] );

    return TCL_OK;
} 

static int left_ankle_cb ( ClientData cd, Tcl_Interp *ip, 
			   int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <left ankle joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxLeftAnkleJoint = string_copy( argv[1] );

    return TCL_OK;
} 

static int right_ankle_cb ( ClientData cd, Tcl_Interp *ip, 
			    int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <right ankle joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxRightAnkleJoint = string_copy( argv[1] );

    return TCL_OK;
} 

static int tail_cb ( ClientData cd, Tcl_Interp *ip, 
		     int argc, CONST84 char *argv[]) 
{

    if ( argc != 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <tail joint>",
			 NULL );
        return TCL_ERROR;
    } 

    tuxTailJoint = string_copy( argv[1] );

    return TCL_OK;
} 


static void register_tux_callbacks( Tcl_Interp *ip )
{
    Tcl_CreateCommand (ip, "tux_root_node", root_node_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_left_shoulder",  left_shoulder_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_right_shoulder",  right_shoulder_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_left_hip",  left_hip_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_right_hip",  right_hip_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_left_knee",  left_knee_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_right_knee",  right_knee_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_left_ankle",  left_ankle_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_right_ankle",  right_ankle_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_neck",  neck_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_head", head_cb,  0,0);
    Tcl_CreateCommand (ip, "tux_tail", tail_cb,  0,0);
}

/*
static void register_tuxplayer_callbacks( Tcl_Interp *ip )
{
    Tcl_CreateCommand (ip, "tux_root_node", root_node_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_left_shoulder",  left_shoulder_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_right_shoulder",  right_shoulder_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_left_hip",  left_hip_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_right_hip",  right_hip_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_left_knee",  left_knee_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_right_knee",  right_knee_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_left_ankle",  left_ankle_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_right_ankle",  right_ankle_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_neck",  neck_cb,   0,0);
    Tcl_CreateCommand (ip, "tux_head", head_cb,  0,0);
    Tcl_CreateCommand (ip, "tux_tail", tail_cb,  0,0);
}
*/
