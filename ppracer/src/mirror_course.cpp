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

#include "course_load.h"
#include "course_render.h"
#include "keyframe.h"
#include "phys_sim.h"
#include "course_quad.h"
#include "track_marks.h"
#include "course_mgr.h"
#include "elements.h"

#include <iostream>

static bool mirrored = false;

void mirror_course() 
{
    int x, y;
    int idx1, idx2;
    float tmp;
    int tmp_terrain;
    ppogl::Vec3d tmp_vec;
    float *elevation;
    ppogl::Vec3d *nmls;
    int *terrain;
    ppogl::Vec2d start_pt;
    int nx, ny;
    float course_width, course_length;

    Course::getDimensions( &course_width, &course_length );
    Course::getDivisions( &nx, &ny );
    elevation = Course::getElevData();
    terrain = Course::getTerrainData();
    nmls = get_course_normals();

    for ( y=0; y<ny; y++ ) {
	for ( x=0; x<nx/2; x++ ) {
	    tmp = ELEV(x,y);
	    ELEV(x,y) = ELEV(nx-1-x, y);
	    ELEV(nx-1-x,y) = tmp;

	    // first column of texture values not used
            idx1 = (x+1) + nx*(y);
            idx2 = (nx-1-x) + nx*(y);
	    tmp_terrain = terrain[idx1];
	    terrain[idx1] = terrain[idx2];
	    terrain[idx2] = tmp_terrain;

            idx1 = (x) + nx*(y);
            idx2 = (nx-1-x) + nx*(y);
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
		(*it).getPosition().x() = course_width - (*it).getPosition().x(); 
		(*it).getPosition().y() = 
	    find_y_coord((*it).getPosition().x(),
			  (*it).getPosition().z() ) + (*it).getType()->height;
    }
	}
	
	{
	std::list<Item>::iterator it;
	for(it=itemLocs.begin();it!=itemLocs.end();it++){
		(*it).getPosition().x() = course_width - (*it).getPosition().x(); 
		(*it).getPosition().y() = 
	    find_y_coord((*it).getPosition().x(),
			  (*it).getPosition().z() ) + (*it).getItemType()->above_ground;
    }
	}
	
	{
	std::list<ppogl::Vec2d>::iterator it;
	for(it=resetLocs.begin();it!=resetLocs.end();it++){
	(*it).x() = course_width - (*it).x();
    }
	}
	
    Course::fillGLArrays();

    reset_course_quadtree();
    if ( nx > 0 && ny > 0 ) {
	PP_LOG( DEBUG_QUADTREE, "mirroring quadtree" );
	init_course_quadtree( elevation, nx, ny, course_width/(nx-1), 
			      -course_length/(ny-1),
			      players[0].view.pos);
    }

    start_pt = Course::getStartPt();
    start_pt.x() = course_width - start_pt.x();
    Course::setStartPt( start_pt );
}

void mirror_key_frame()
{
    float course_width, course_length;
    int num_frames;
    KeyFrame *frames;

    get_key_frame_data( &frames, &num_frames );

    Course::getDimensions( &course_width, &course_length );

    for(int i=0; i<num_frames; i++){
		frames[i].yaw = - frames[i].yaw;
		frames[i].pos.x() = course_width - frames[i].pos.x();
    }
}

void set_course_mirroring( bool state )
{
    if ( mirrored != state ) {
		mirror_key_frame();
		mirror_course();
		TrackMarks::init();
    }
    mirrored = state;
    
}

bool get_course_mirroring( )
{
    return mirrored;
}
