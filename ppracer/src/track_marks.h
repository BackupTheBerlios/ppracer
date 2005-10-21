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

#ifndef _TRACK_MARKS_H_
#define _TRACK_MARKS_H_

#include "player.h"
#include "ppogl/base/vec2d.h"
#include "ppogl/base/vec3d.h"

enum TrackTypes
{
    TRACK_HEAD,
    TRACK_MARK,
    TRACK_TAIL,
    NUM_TRACK_TYPES
};

struct TrackQuad
{
    ppogl::Vec3d v1, v2, v3, v4;
    ppogl::Vec2d t1, t2, t3, t4;
    ppogl::Vec3d n1, n2, n3, n4;
    TrackTypes track_type;
	int terrain;
    double alpha;
};

class TrackMarks
{
public:

// begin old stuff
	void draw();
	void update();
	void discontinue();

	TrackQuad* quads;
    int current_mark;
    int next_mark;
    double last_mark_time;
    ppogl::Vec3d last_mark_pos;
	bool continuing_track;
// end old stuff

	TrackMarks();
	
	///reference for the player
	Player* player;
		
// static stuff

	static void init();
	static void discontinueAllPlayers();
	static void drawAllPlayers();
	
private:
	static int maxNumQuads;	
};

extern TrackMarks trackMarks[2];

#endif // _TRACK_MARKS_H_
