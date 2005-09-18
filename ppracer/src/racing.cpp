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

#include "racing.h"

#include "ppogl/base/defs.h"

#include "keyframe.h"
#include "course_render.h"
#include "gl_util.h"
#include "fps.h"
#include "loop.h"
#include "render_util.h"
#include "view.h"
#include "tux.h"
#include "tux_shadow.h"
#include "phys_sim.h"
#include "part_sys.h"
#include "screenshot.h"
#include "fog.h"
#include "viewfrustum.h"
#include "hud.h"
#include "joystick.h"
#include "track_marks.h"
#include "snow.h"
#include "winsys.h"
#include "lights.h"

#include "bench.h"
#include "game_mgr.h"

/* Time constant for automatic steering centering (s) */
#define TURN_DECAY_TIME_CONSTANT 0.5

/* Maximum time over which jump can be charged */
#define MAX_JUMP_AMT 1.0

/* Time constant for automatic rolling centering (s) */
#define ROLL_DECAY_TIME_CONSTANT 0.2

#define JUMP_CHARGE_DECAY_TIME_CONSTANT 0.1

/* If too high off the ground, tux flaps instead of jumping */
#define JUMP_MAX_START_HEIGHT 0.30

extern TerrainTex terrain_texture[NUM_TERRAIN_TYPES];
extern unsigned int num_terrains;

Racing::States::States()
 : chargeStartTime(0),
   lastTerrain(0),
   rightTurn(false),
   leftTurn(false),
   trickModifier(false),
   paddling(false),
   charging(false),
   braking(false)
{
}

Racing::Racing()
{
    // Initialize view
    int viewmode = PPConfig.getInt("view_mode");
		
	if( viewmode<0 || viewmode>=NUM_VIEW_MODES ) 
    {
		PPConfig.setInt("view_mode", ABOVE);
    }	
	
	for(int i=0; i<GameMgr::getInstance().numPlayers; i++){		
		set_view_mode(players[i], static_cast<ViewMode>(viewmode));
		
		// We need to reset controls here since callbacks won't have been
		// called in paused mode. This results in duplication between this
		// code and init_physical_simulation.  Oh well. 
		players[i].control.turn_fact = 0.0;
		players[i].control.turn_animation = 0.0;
		players[i].control.is_braking = false;
		players[i].control.is_paddling = false;
		players[i].control.jumping = false;
		players[i].control.jump_charging = false;
		players[i].max_speed = 0;
		
		init_snow(players[i].view.pos);
	}
   
    if(GameMode::prevmode != PAUSED){
		init_physical_simulation();
    }

    GameMgr::getInstance().abortRace(false);
			
    ppogl::AudioMgr::getInstance().playMusic("racing");	
}

Racing::~Racing()
{
	ppogl::AudioMgr::getInstance().stopAllSounds();
 	TrackMarks::discontinueAllPlayers();
}

void
Racing::prePlayer(int plyr, float timestep)
{
	Player& player=players[plyr];
	
    bool joy_left_turn = false;
    bool joy_right_turn = false;
    double joy_turn_fact = 0.0;
    bool joy_paddling = false;
    bool joy_braking = false;
    bool joy_tricks = false;
    bool joy_charging = false;
    bool airborne;
    ppogl::Vec3d dir;
    float speed;
    float terrain_weights[NUM_TERRAIN_TYPES];
    int new_terrain = 0;
    int slide_volume;
	unsigned int i;

	if(Benchmark::getMode() == Benchmark::AUTO){
		m_state[plyr].paddling = true;
	}
	
    dir = player.vel;
    speed = dir.normalize();
	
	//set max_speed
	if (speed > player.max_speed) player.max_speed=int(speed);

	
    airborne = ( player.pos.y() > ( find_y_coord(players[0].pos.x(), 
						       player.pos.z()) + 
					  JUMP_MAX_START_HEIGHT ) );

    // Joystick

    if ( is_joystick_active() ) {
	float joy_x;
	float joy_y;

	update_joystick();

	joy_x = get_joystick_x_axis();
	joy_y = get_joystick_y_axis();

	if ( joy_x > 0.1 ) {
	    joy_right_turn = true;
	    joy_turn_fact = joy_x;
	} else if ( joy_x < -0.1 ) {
	    joy_left_turn = true;
	    joy_turn_fact = joy_x;
	}

	if(PPConfig.getInt("joystick_brake_button") >= 0 ) {
	    joy_braking = 
		is_joystick_button_down(PPConfig.getInt("joystick_brake_button"));
	} 
	if( !joy_braking ) {
	    joy_braking = ( joy_y > 0.5 );
	}

	if(PPConfig.getInt("joystick_paddle_button") >= 0 ) {
	    joy_paddling = 
		is_joystick_button_down(PPConfig.getInt("joystick_paddle_button"));
	}
	if ( !joy_paddling ) {
	    joy_paddling = ( joy_y < -0.5 );
	}

	if (PPConfig.getInt("joystick_jump_button") >= 0 ) {
	    joy_charging = 
		is_joystick_button_down(PPConfig.getInt("joystick_jump_button"));
	}

	if (PPConfig.getInt("joystick_trick_button") >= 0 ) {
	    joy_tricks = 
		is_joystick_button_down(PPConfig.getInt("joystick_trick_button"));
	}
    }

    // Update braking 
    players[0].control.is_braking = ( m_state[plyr].braking || joy_braking );

    if ( airborne ) {
	new_terrain = (1<<4);

	// Tricks
	if ( m_state[plyr].trickModifier || joy_tricks ) {
	    if ( m_state[plyr].leftTurn || joy_left_turn ) {
		player.control.barrel_roll_left = true;
	    }
	    if ( m_state[plyr].rightTurn || joy_right_turn ) {
		player.control.barrel_roll_right = true;
	    }
	    if ( m_state[plyr].paddling || joy_paddling ) {
		player.control.front_flip = true;
	    }
	    if ( player.control.is_braking ) {
		player.control.back_flip = true;
	    }
	}

		for(i=0;i<num_terrains;i++){
			if ( !terrain_texture[i].sound.empty() && terrain_texture[i].soundactive==true) {
				ppogl::AudioMgr::getInstance().stopSound(terrain_texture[i].sound);
				terrain_texture[i].soundactive=false;
			}
		}
		
    } else {

	get_surface_type(player.pos.x(), player.pos.z(), terrain_weights);
	

    //Play sliding sound
		
		slide_volume = int(MIN( (((pow(player.control.turn_fact, 2)*128)) +
			 (player.control.is_braking?128:0) +
			 (player.control.jumping?128:0) +
			 20) *
			(speed/10), 128 ));
		
		for(i=0;i<num_terrains;i++){
			if ( !terrain_texture[i].sound.empty() ) {
				if (terrain_weights[i] > 0 ){
					//set_sound_volume(terrain_texture[i].sound, int(slide_volume * terrain_weights[i]));
					if (terrain_texture[i].soundactive==false){
						ppogl::AudioMgr::getInstance().playSound(terrain_texture[i].sound);
						terrain_texture[i].soundactive=true;
					}
				} else if (terrain_texture[i].soundactive==true){
					ppogl::AudioMgr::getInstance().stopSound(terrain_texture[i].sound);
					terrain_texture[i].soundactive=false;
				}
			}
		}
		
			
    }

    // Jumping

    calcJumpAmt(plyr);

    if ( ( m_state[plyr].charging || joy_charging ) && 
	 !player.control.jump_charging && !player.control.jumping ) 
    {
		player.control.jump_charging = true;
		m_state[plyr].chargeStartTime = GameMgr::getInstance().time;
    }

    if ( ( !m_state[plyr].charging && !joy_charging ) && player.control.jump_charging ) {
		player.control.jump_charging = false;
		player.control.begin_jump = true;
    }
 
    // Turning 
    if ( ( m_state[plyr].leftTurn || joy_left_turn )  ^ (m_state[plyr].rightTurn || joy_right_turn ) ) {
	bool turning_left = (m_state[plyr].leftTurn || joy_left_turn );

	if ( joy_left_turn || joy_right_turn ) {
	    player.control.turn_fact = joy_turn_fact;
	} else {
	    player.control.turn_fact = (turning_left?-1:1);
	}

	player.control.turn_animation += (turning_left?-1:1) *
	    0.15 * timestep / 0.05;
	player.control.turn_animation = 
	    MIN(1.0, MAX(-1.0, player.control.turn_animation));
    } else {
	player.control.turn_fact = 0;

	// Decay turn animation
	if ( timestep < ROLL_DECAY_TIME_CONSTANT ) {
	    player.control.turn_animation *= 
		1.0 - timestep/ROLL_DECAY_TIME_CONSTANT;
	} else {
	    player.control.turn_animation = 0.0;
	}
    }

    
    
    //Paddling
    if ( ( m_state[plyr].paddling || joy_paddling ) && player.control.is_paddling == false ) {
		player.control.is_paddling = true;
		player.control.paddle_time = GameMgr::getInstance().time;
    }

    
   	//Play flying sound

    if (new_terrain & (1<<4)) {
		//set_sound_volume("flying_sound", int(MIN(128, speed*2)));
		if (!(m_state[plyr].lastTerrain & (1<<4))) {
	 	   ppogl::AudioMgr::getInstance().playSound("flying_sound", -1);
		}
	    } else {
		if (m_state[plyr].lastTerrain & (1<<4)) {
		    ppogl::AudioMgr::getInstance().stopSound("flying_sound");
		}
	}

  	m_state[plyr].lastTerrain = new_terrain; 

	//Tricks
    if ( player.control.barrel_roll_left || player.control.barrel_roll_right ) {
	player.control.barrel_roll_factor += 
		( player.control.barrel_roll_left ? -1 : 1 ) * 0.15 * timestep / 0.05;
	if ( (player.control.barrel_roll_factor  > 1) ||
	     (player.control.barrel_roll_factor  < -1) ) {
	    player.control.barrel_roll_factor = 0;
	    player.control.barrel_roll_left = player.control.barrel_roll_right = false;
	}
    }
    if ( player.control.front_flip || player.control.back_flip ) {
	player.control.flip_factor += 
		( player.control.back_flip ? -1 : 1 ) * 0.15 * timestep / 0.05;
	if ( (player.control.flip_factor  > 1) ||
	     (player.control.flip_factor  < -1) ) {
	    player.control.flip_factor = 0;
	    player.control.front_flip = player.control.back_flip = false;
	}
    }

    update_player_pos( player, timestep );
	 
	GameMgr::getInstance().time += timestep;
	if(airborne) player.airbornetime += timestep;
}

void
Racing::postDisplay(float timeStep)
{
	if(Benchmark::getMode() == Benchmark::PAUSED){
		setMode(PAUSED);
	}
}

void
Racing::calcJumpAmt(int player)
{
    if(players[player].control.jump_charging){
		players[player].control.jump_amt = MIN( 
	    MAX_JUMP_AMT, GameMgr::getInstance().time - m_state[player].chargeStartTime );
    }else if(players[player].control.jumping){
		players[player].control.jump_amt *= 
	    ( 1.0 - ( GameMgr::getInstance().time - players[player].control.jump_start_time ) / 
	      JUMP_FORCE_DURATION );
    }else{
		players[player].control.jump_amt = 0;
    }
}

bool
Racing::keyboardEvent(SDLKey key, bool release)
{
	if(key==PPConfig.getInt("turn_left_key")){
		m_state[0].leftTurn = !release;
		return true;
	}else if(key==PPConfig.getInt("turn_right_key")){
		m_state[0].rightTurn = !release;
		return true;
	}else if(key==PPConfig.getInt("paddle_key")){
		m_state[0].paddling = !release;
		return true;
	}else if(key==PPConfig.getInt("brake_key")){
		m_state[0].braking = !release;
		return true;
	}else if(key==PPConfig.getInt("trick_modifier_key")){
		m_state[0].trickModifier = !release;
		return true;
	}else if(key==PPConfig.getInt("jump_key")){
		m_state[0].charging = !release;
		return true;
	}
	
	if(GameMgr::getInstance().numPlayers==2){
		if(key==PPConfig.getInt("turn_left_key2")){
			m_state[1].leftTurn = !release;
			return true;
		}else if(key==PPConfig.getInt("turn_right_key2")){
			m_state[1].rightTurn = !release;
			return true;
		}else if(key==PPConfig.getInt("paddle_key2")){
			m_state[1].paddling = !release;
			return true;
		}else if(key==PPConfig.getInt("brake_key2")){
			m_state[1].braking = !release;
			return true;
		}else if(key==PPConfig.getInt("trick_modifier_key2")){
			m_state[1].trickModifier = !release;
			return true;
		}else if(key==PPConfig.getInt("jump_key2")){
			m_state[1].charging = !release;
			return true;
		}	
	}	

	return false;
}

bool
Racing::keyPressEvent(SDLKey key)
{
	switch(key){
		case 'q':
		case SDLK_ESCAPE: 
			GameMgr::getInstance().abortRace();
    		setMode(GAME_OVER);
			return true;
		case '1':
    		set_view_mode( players[0], ABOVE );
    		PPConfig.setInt("view_mode", ABOVE);
			return true;
		case '2':
			set_view_mode( players[0], FOLLOW );
			PPConfig.setInt("view_mode", FOLLOW);
			return true;
		case '3':
			set_view_mode( players[0], BEHIND );
			PPConfig.setInt("view_mode", BEHIND);
			return true;
		case '4':
    		set_view_mode( players[1], ABOVE );
    		PPConfig.setInt("view_mode2", ABOVE);
			return true;
		case '5':
			set_view_mode( players[1], FOLLOW );
			PPConfig.setInt("view_mode2", FOLLOW);
			return true;
		case '6':
			set_view_mode( players[1], BEHIND );
			PPConfig.setInt("view_mode2", BEHIND);
			return true;
		default:
			if(key==PPConfig.getInt("reset_key")){
				setMode(RESET);
				return true;
			}else if(key==PPConfig.getInt("reset_key2")){
				//deactivating since reset isn't realy useable in multiplayer mode
				//setMode(RESET);
				return true;
			}else if(key==PPConfig.getInt("pause_key")){
				setMode(PAUSED);
				return true;
			}else if(key==PPConfig.getInt("screenshot_key")){
				screenshot();
				return true;
			}
	}
	return false;
}
