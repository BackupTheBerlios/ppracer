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

#include "keyframe.h"

#include "tux.h"
#include "phys_sim.h"
#include "hier.h"
#include "loop.h"

#include "ppogl/base/defs.h"

PlayerKeyFrames keyFrames[2];

static double
interp( double frac, double v1, double v2 ) 
{
    return frac*v1 + (1.-frac)*v2;
}

void
PlayerKeyFrames::getData(KeyFrame **fp, int *n)
{
    *fp = frames;
    *n = numFrames;
}

void
PlayerKeyFrames::reset()
{
    keyTime = 0;
    numFrames = 0;
} 

void
PlayerKeyFrames::init(int plyr)
{
    keyTime = frames[0].time;
	player=plyr;
	
    reset_scene_node( tux[player].getHead() );
    reset_scene_node( tux[player].getNeck() );
} 

void
PlayerKeyFrames::update(Player& plyr, double dt)
{
    int idx;
    double frac;
    ppogl::Vec3d pos;
    double v;
    pp::Matrix cob_mat, rot_mat;

    const std::string& root = tux[player].getRootNode();
    const std::string& lsh  = tux[player].getLeftShoulderJoint();
    const std::string& rsh  = tux[player].getRightShoulderJoint();
    const std::string& lhp  = tux[player].getLeftHipJoint();
    const std::string& rhp  = tux[player].getRightHipJoint();
    const std::string& lkn  = tux[player].getLeftKneeJoint();
    const std::string& rkn  = tux[player].getRightKneeJoint();
    const std::string& lank = tux[player].getLeftAnkleJoint();
    const std::string& rank = tux[player].getRightAnkleJoint();
    const std::string& head = tux[player].getHead();
    const std::string& neck = tux[player].getNeck();
    const std::string& tail = tux[player].getTailJoint();

    keyTime += dt;

    for(idx = 1; idx < numFrames; idx ++) {
        if ( keyTime < frames[idx].time )
            break;
    } 

    if ( idx == numFrames || numFrames == 0 ) {
        GameMode::setMode( GameMode::RACING );
        return;
    } 

    reset_scene_node( root );
    reset_scene_node( lsh );
    reset_scene_node( rsh );
    reset_scene_node( lhp );
    reset_scene_node( rhp );
    reset_scene_node( lkn );
    reset_scene_node( rkn );
    reset_scene_node( lank );
    reset_scene_node( rank );
    reset_scene_node( head );
    reset_scene_node( neck );
    reset_scene_node( tail );

    PP_ASSERT( idx > 0, "invalid keyframe index" );

    if ( fabs( frames[idx-1].time - frames[idx].time ) < EPS ) {
	frac = 1.;
    } else {
	frac = (keyTime - frames[idx].time) 
	    / ( frames[idx-1].time - frames[idx].time );
    }

    pos.x() = interp( frac, frames[idx-1].pos.x(), frames[idx].pos.x() );
    pos.z() = interp( frac, frames[idx-1].pos.z(), frames[idx].pos.z() );
    pos.y() = interp( frac, frames[idx-1].pos.y(), frames[idx].pos.y() );
    pos.y() += find_y_coord( pos.x(), pos.z() );

    set_tux_pos( plyr, pos );

	cob_mat.makeIdentity();

    v = interp( frac, frames[idx-1].yaw, frames[idx].yaw );
    rotate_scene_node( root, 'y', v );
    rot_mat.makeRotation( v, 'y' );
    cob_mat=cob_mat*rot_mat;

    v = interp( frac, frames[idx-1].pitch, frames[idx].pitch );
    rotate_scene_node( root, 'x', v );
    rot_mat.makeRotation( v, 'x' );
    cob_mat=cob_mat*rot_mat;

    v = interp( frac, frames[idx-1].l_shldr, frames[idx].l_shldr );
    rotate_scene_node( lsh, 'z', v );

    v = interp( frac, frames[idx-1].r_shldr, frames[idx].r_shldr );
    rotate_scene_node( rsh, 'z', v );

    v = interp( frac, frames[idx-1].l_hip, frames[idx].l_hip );
    rotate_scene_node( lhp, 'z', v );

    v = interp( frac, frames[idx-1].r_hip, frames[idx].r_hip );
    rotate_scene_node( rhp, 'z', v );

    /* Set orientation */
    plyr.orientation = pp::Matrix( cob_mat); //make_quaternion_from_matrix( cob_mat );
    plyr.orientation_initialized = true;
} 

static int
key_frame_cb(ppogl::Script *vm) 
{
   	ppogl::Vec2d start_pt = Course::getStartPt();
	
    if(vm->getTop()!=11){
		PP_WARNING("tux.key_frame: Invalid number of arguments");
		return 0;
    } 
	
	int player = vm->getInt(1);
	if(player!=1 && player !=2){
		PP_WARNING("tux.key_frame: Player " << player << " not supported");
		return 0;
	}
	player--;
	
	if(keyFrames[player].numFrames == MAX_NUM_KEY_FRAMES ){
        PP_WARNING("tux.key_frame: max. num. of frames reached");
		return 0;
    }	
	
	KeyFrame frame;
	frame.time = vm->getFloat(2);
  
	frame.pos.x() = vm->getFloat(3) + start_pt.x();
    
	frame.pos.z() = (-1)*vm->getFloat(4) + start_pt.y();
    
	frame.pos.y() = vm->getFloat(5);
  
	frame.yaw = vm->getFloat(6);
   
	frame.pitch = vm->getFloat(7);
    
	frame.l_shldr = vm->getFloat(8);

	frame.r_shldr = vm->getFloat(9);
    
	frame.l_hip = vm->getFloat(10);
 
	frame.r_hip = vm->getFloat(11);
    
    keyFrames[player].frames[keyFrames[player].numFrames] = frame;
    keyFrames[player].numFrames++;

    return 0;
} 

static const struct ppogl::Script::Lib tuxlib[]={
	{"key_frame", key_frame_cb}, 
	{NULL, NULL}
};

void register_key_frame_callbacks()
{
	script.registerLib("tux", tuxlib);
}
