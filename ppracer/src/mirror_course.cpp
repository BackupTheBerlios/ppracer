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

#include "course_load.h"
#include "course_render.h"
#include "keyframe.h"
#include "phys_sim.h"
#include "track_marks.h"
#include "course_mgr.h"
#include "game_mgr.h"
#include "elements.h"

#include <iostream>

static bool mirrored = false;

void
mirror_course() 
{
    int idx1, idx2;
    float tmp;
    int tmp_terrain;
    ppogl::Vec3d tmp_vec;
    ppogl::Vec2d start_pt;

	const ppogl::Vec2d& courseDim = Course::getDimensions();
    float* elevation = Course::getElevData();
    int* terrain = Course::getTerrainData();
    ppogl::Vec3d* nmls = courseRenderer.getNormals();

    for(int y=0; y<Course::ny; y++){
	for(int x=0; x<Course::nx/2; x++){
	    tmp = Course::getElevation(x,y);
	    Course::getElevation(x,y) = Course::getElevation(Course::nx-1-x, y);
	    Course::getElevation(Course::nx-1-x,y) = tmp;

	    // first column of texture values not used
            idx1 = (x+1) + Course::nx*(y);
            idx2 = (Course::nx-1-x) +Course:: nx*(y);
	    tmp_terrain = terrain[idx1];
	    terrain[idx1] = terrain[idx2];
	    terrain[idx2] = tmp_terrain;

            idx1 = (x) + Course::nx*(y);
            idx2 = (Course::nx-1-x) + Course::nx*(y);
	    tmp_vec = nmls[idx1];
	    nmls[idx1] = nmls[idx2];
	    nmls[idx2] = tmp_vec;
	    nmls[idx1].x() *= -1;
	    nmls[idx2].x() *= -1;
	}
    }

	{
	std::list<Model>::iterator it;
	for(it=modelLocs.begin();it!=modelLocs.end();it++) {
		(*it).getPosition().x() = courseDim.x() - (*it).getPosition().x(); 
		(*it).getPosition().y() = 
	    find_y_coord((*it).getPosition().x(),
			  (*it).getPosition().z() ) + (*it).getType()->height;
		(*it).mirror();		
    }
	}
	
	{
	std::list<Item>::iterator it;
	for(it=itemLocs.begin();it!=itemLocs.end();it++){
		(*it).getPosition().x() = courseDim.x() - (*it).getPosition().x(); 
		(*it).getPosition().y() = 
	    find_y_coord((*it).getPosition().x(),
			  (*it).getPosition().z() ) + (*it).getItemType()->above_ground;
    }
	}
	
	{
	std::list<ppogl::Vec2d>::iterator it;
	for(it=resetLocs.begin();it!=resetLocs.end();it++){
	(*it).x() = courseDim.x() - (*it).x();
    }
	}
	
    Course::fillGLArrays();

	TrackMarks::mirrorAllPlayers(courseDim.x());
	
	courseRenderer.resetQuadtree();
	
    if ( Course::nx > 0 && Course::ny > 0 ) {
		PP_LOG( DEBUG_QUADTREE, "mirroring quadtree" );
		courseRenderer.initQuadtree( elevation, Course::nx, Course::ny, courseDim.x()/(Course::nx-1), 
			      -courseDim.y()/(Course::ny-1),
			      players[0].view.pos);
    }

    start_pt = Course::getStartPt();
    start_pt.x() = courseDim.x() - start_pt.x();
    Course::setStartPt( start_pt );
}

void
mirror_key_frame()
{
    int num_frames;
    KeyFrame *frames;
	const ppogl::Vec2d& courseDim = Course::getDimensions();

	for(int player=0; player<GameMgr::getInstance().numPlayers; player++){
		keyFrames[player].getData(&frames, &num_frames);

   		for(int i=0; i<num_frames; i++){	
			frames[i].yaw = - frames[i].yaw;
			frames[i].pos.x() = courseDim.x() - frames[i].pos.x();
    	}
	}
}

void
set_course_mirroring(bool state)
{
	if(mirrored!=state){
		mirror_key_frame();
		mirror_course();
		//TrackMarks::init();		
		mirrored = state;
    }
}

void
reset_course_mirroring()
{
	mirrored=false;
}
