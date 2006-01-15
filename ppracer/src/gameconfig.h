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
 
#ifndef _GAMECONFIG_H_
#define _GAMECONFIG_H_

namespace GameConfig{
	
extern bool enableStencilBuffer;
extern bool drawTuxShadow;
extern int tuxShadowSphereDivisions;
	
extern float courseDetails;
extern bool useCVA;
extern bool useTerrainEnvmap;
	
extern bool disableCollisionDetection;
extern bool drawParticles;

extern bool displayFPS;
extern bool displayCoursePercentage;
	
extern int forwardClipDistance;	
extern int backwardClipDistance;		
extern int fov;
	
extern bool drawTrackMarks;
	
extern int joystickBrakeButton;
extern int joystickPaddleButton;
extern int joystickJumpButton;
extern int joystickTrickButton;
	
extern bool disableFog;
	
void update();

} // namespace GameConfig

#endif // _GAMECONFIG_H_
