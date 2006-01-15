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

#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_

#include "player.h"

#define MAX_NUM_KEY_FRAMES 128

/// Key frame for animation sequences

struct KeyFrame
{
    float time;
    ppogl::Vec3d pos;
	
	/// angle of rotation about y axis
    float yaw;
	
	/// angle of rotation about x axis
    float pitch;
    float l_shldr;
    float r_shldr;
    float l_hip;
    float r_hip;
}; 


class PlayerKeyFrames
{		
public:
	KeyFrame frames[MAX_NUM_KEY_FRAMES];
	int numFrames;
	double keyTime;
	int player;	

	PlayerKeyFrames():numFrames(0){}

	void init(int plyr);
	void getData(KeyFrame **fp, int *n);
	void reset();
	void update(Player& plyr, double dt);
	void mirror();
};

extern PlayerKeyFrames keyFrames[2];

void register_key_frame_callbacks();

#endif // _KEYFRAME_H_
