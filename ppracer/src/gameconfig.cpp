/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
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
#include "gameconfig.h"
#include "ppracer.h"

namespace GameConfig
{

bool enableStencilBuffer=false;
bool drawTuxShadow=true;
int tuxShadowSphereDivisions=3;
	
float courseDetails=100;
bool useCVA=true;
bool useTerrainEnvmap=true;
	
bool disableCollisionDetection = false;
bool drawParticles = true;
	
bool displayFPS = false;
bool displayCoursePercentage = false;
	
int forwardClipDistance = 75;
int backwardClipDistance = 10;	

int fov = 60;

bool drawTrackMarks = true;

int joystickBrakeButton;
int joystickPaddleButton;
int joystickJumpButton;
int joystickTrickButton;

bool disableFog=false;

/// update configuration data
void
update()
{
	enableStencilBuffer = PPConfig.getBool("stencil_buffer");
	drawTuxShadow = PPConfig.getBool("draw_tux_shadow");
	tuxShadowSphereDivisions = PPConfig.getInt("tux_shadow_sphere_divisions");
	
	courseDetails = PPConfig.getInt("course_detail_level");
	useCVA = PPConfig.getBool("use_cva");
	useTerrainEnvmap = PPConfig.getBool("terrain_envmap");
	
	disableCollisionDetection = PPConfig.getBool("disable_collision_detection");
	drawParticles = PPConfig.getBool("draw_particles");
	
	displayFPS =PPConfig.getBool("display_fps");
	displayCoursePercentage = PPConfig.getBool("display_course_percentage");

	forwardClipDistance = PPConfig.getInt("forward_clip_distance");
	backwardClipDistance = PPConfig.getInt("backward_clip_distance");
	
	fov = PPConfig.getInt("fov");
	
	drawTrackMarks = PPConfig.getBool("track_marks");

	joystickBrakeButton = PPConfig.getInt("joystick_brake_button");
	joystickPaddleButton = PPConfig.getInt("joystick_paddle_button");
	joystickJumpButton = PPConfig.getInt("joystick_jump_button");
	joystickTrickButton = PPConfig.getInt("joystick_trick_button");

	disableFog = PPConfig.getBool("disable_fog");
}

} // namespace GameConfig
