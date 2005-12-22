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

#include "track_marks.h"
#include "gl_util.h"
#include "tux.h"
#include "hier.h"
#include "phys_sim.h"
#include "course_render.h"
#include "render_util.h"
#include "course_mgr.h"
#include "lights.h"
#include "gameconfig.h"

#include "ppogl/base/defs.h"
#include "ppogl/base/glwrappers.h"

#include "game_mgr.h"


TrackQuad::TrackQuad()
 : trackType(TYPE_NONE),
   terrain(0),
   alpha(1.0f)
{
}


#undef TRACK_TRIANGLES

#define TRACK_WIDTH  0.7
#define MAX_CONTINUE_TRACK_DIST TRACK_WIDTH*4
#define MAX_CONTINUE_TRACK_TIME .1
#define SPEED_TO_START_TRENCH 0.0
#define SPEED_OF_DEEPEST_TRENCH 10

#define TRACK_HEIGHT 0.08
#define MAX_TRACK_DEPTH 0.7

extern TerrainTex terrain_texture[NUM_TERRAIN_TYPES];
extern unsigned int num_terrains;

int TrackMarks::maxNumQuads=-1;

void
TrackMarks::draw()
{
	int current_quad, num_quads;
    int first_quad;
    TrackQuad *q, *qnext;
    ppogl::Color trackColor = ppogl::Color::white;

    set_gl_options( TRACK_MARKS ); 

    gl::Color(ppogl::Color::black);

    gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    set_material( ppogl::Color::white, ppogl::Color::black, 1.0 );
    setup_course_lighting();

    num_quads = MIN( current_mark, maxNumQuads -
		     next_mark + current_mark );
    first_quad = current_mark - num_quads;

    for ( current_quad = 0;
	  current_quad < num_quads;
	  current_quad++ ) 
    {
	q = &quads[(first_quad + current_quad)%maxNumQuads];

	trackColor.a() = q->alpha;
	set_material( trackColor, ppogl::Color::black, 1.0 );

	switch (q->trackType){
		case TrackQuad::TYPE_HEAD:
			gl::BindTexture( GL_TEXTURE_2D, terrain_texture[q->terrain].trackmark.head);
			break;
		case TrackQuad::TYPE_MARK:
			gl::BindTexture( GL_TEXTURE_2D, terrain_texture[q->terrain].trackmark.mark);
			break;
		case TrackQuad::TYPE_TAIL:
			gl::BindTexture( GL_TEXTURE_2D, terrain_texture[q->terrain].trackmark.tail);
			break;		
		default:
			gl::BindTexture( GL_TEXTURE_2D, terrain_texture[q->terrain].trackmark.mark);
			break;	
	}

	if ((q->trackType == TrackQuad::TYPE_HEAD) || (q->trackType == TrackQuad::TYPE_TAIL)) { 
	    gl::Begin(GL_QUADS);
	    
	    gl::Normal( q->n1.x(), q->n1.y(), q->n1.z() );
	    gl::TexCoord( q->t1.x(), q->t1.y() );
	    gl::Vertex( q->v1.x(), q->v1.y(), q->v1.z() );
	
	    gl::Normal( q->n2.x(), q->n2.y(), q->n2.z() );
	    gl::TexCoord( q->t2.x(), q->t2.y() );
	    gl::Vertex( q->v2.x(), q->v2.y(), q->v2.z() );

	    gl::Normal( q->n4.x(), q->n4.y(), q->n4.z() );
	    gl::TexCoord( q->t4.x(), q->t4.y() );
	    gl::Vertex( q->v4.x(), q->v4.y(), q->v4.z() );
	
	    gl::Normal( q->n3.x(), q->n3.y(), q->n3.z() );
	    gl::TexCoord( q->t3.x(), q->t3.y() );
	    gl::Vertex( q->v3.x(), q->v3.y(), q->v3.z() );
	
	    gl::End();

	} else {
	      
	    gl::Begin(GL_QUAD_STRIP);

	    gl::Normal( q->n2.x(), q->n2.y(), q->n2.z() );
	    gl::TexCoord( q->t2.x(), q->t2.y() );
	    gl::Vertex( q->v2.x(), q->v2.y(), q->v2.z() );

	    gl::Normal( q->n1.x(), q->n1.y(), q->n1.z() );
	    gl::TexCoord( q->t1.x(), q->t1.y() );
	    gl::Vertex( q->v1.x(), q->v1.y(), q->v1.z() );

	    gl::Normal( q->n4.x(), q->n4.y(), q->n4.z() );
	    gl::TexCoord( q->t4.x(), q->t4.y() );
	    gl::Vertex( q->v4.x(), q->v4.y(), q->v4.z() );

	    gl::Normal( q->n3.x(), q->n3.y(), q->n3.z() );
	    gl::TexCoord( q->t3.x(), q->t3.y() );
	    gl::Vertex( q->v3.x(), q->v3.y(), q->v3.z() );
		gl::End();
		gl::Begin(GL_QUADS);
		
	    qnext = &quads[(first_quad+current_quad+1)%maxNumQuads];
	    while (( qnext->trackType == TrackQuad::TYPE_MARK ) && (current_quad+1 < num_quads)) {
		current_quad++;
		
		if (q->terrain != qnext->terrain){
			gl::End();
			gl::BindTexture( GL_TEXTURE_2D, terrain_texture[qnext->terrain].trackmark.mark);
			gl::Begin(GL_QUADS);		
		}
				
		q = &quads[(first_quad+current_quad)%maxNumQuads];
		trackColor.a() = qnext->alpha;
		set_material( trackColor, ppogl::Color::black, 1.0 );
		

	    gl::Normal( q->n1.x(), q->n1.y(), q->n1.z() );
	    gl::TexCoord( q->t1.x(), q->t1.y() );
	    gl::Vertex( q->v1.x(), q->v1.y(), q->v1.z() );
		
		gl::Normal( q->n2.x(), q->n2.y(), q->n2.z() );
	    gl::TexCoord( q->t2.x(), q->t2.y() );
	    gl::Vertex( q->v2.x(), q->v2.y(), q->v2.z() );	
					
		gl::Normal( q->n4.x(), q->n4.y(), q->n4.z() );
		gl::TexCoord( q->t4.x(), q->t4.y() );
		gl::Vertex( q->v4.x(), q->v4.y(), q->v4.z() );

		gl::Normal( q->n3.x(), q->n3.y(), q->n3.z() );
		gl::TexCoord( q->t3.x(), q->t3.y() );
		gl::Vertex( q->v3.x(), q->v3.y(), q->v3.z() );
		
		qnext = &quads[(first_quad+current_quad+1)%maxNumQuads];
	    }
	    gl::End();
	}

    }
}

void
TrackMarks::discontinue()
{
    TrackQuad *qprev, *qprevprev;
    qprev = &quads[(current_mark-1)%maxNumQuads];
    qprevprev = &quads[(current_mark-2)%maxNumQuads];

    if(current_mark > 0){
		qprev->trackType = TrackQuad::TYPE_TAIL;
		qprev->t1 = ppogl::Vec2d(0.0, 0.0);
		qprev->t2 = ppogl::Vec2d(1.0, 0.0);
		qprev->t3 = ppogl::Vec2d(0.0, 1.0);
		qprev->t4 = ppogl::Vec2d(1.0, 1.0);
		qprevprev->t3.y() = MAX(int(qprevprev->t3.y()+0.5), int(qprevprev->t1.y()+1));
		qprevprev->t4.y() = MAX(int(qprevprev->t3.y()+0.5), int(qprevprev->t1.y()+1));
    }
    last_mark_time = -99999;
    last_mark_pos = ppogl::Vec3d(-9999, -9999, -9999);
    continuing_track = false;
}

void
TrackMarks::update()
{
	
    if(GameConfig::drawTrackMarks == false) {
		return;
    }
	
    float tex_end;
	float terrain_compression=0;
	float old_terrain_weight=0;
	unsigned int i;

    TrackQuad* q = &quads[current_mark%maxNumQuads];
    TrackQuad* qprev = &quads[(current_mark-1)%maxNumQuads];

    ppogl::Vec3d vector_from_last_mark = player->pos - last_mark_pos;
    vector_from_last_mark.normalize();
		    
	float terrain_weights[NUM_TERRAIN_TYPES];
    get_surface_type(player->pos.x(), player->pos.z(), terrain_weights);
    
	bool break_marks=true;
	for (i=0;i<num_terrains;i++){
		if (terrain_texture[i].trackmark.mark){	
			if (terrain_weights[i] >= 0.5) {
				if (old_terrain_weight < terrain_weights[i]) {
					break_marks=false;
					terrain_compression = get_compression_depth(i);
					q->terrain=i;
					old_terrain_weight = terrain_weights[i];
				}
    		}
		}
	}
	
	if (break_marks==true){
		discontinue();
		return;
	}

	ppogl::Vec3d vel = player->vel;
    float speed = vel.normalize();
    if (speed < SPEED_TO_START_TRENCH) {
		discontinue();
		return;
    }

	ppogl::Vec3d width_vector = player->direction^ppogl::Vec3d( 0, 1, 0 );
    float magnitude = width_vector.normalize();
    if ( magnitude == 0 ) {
		discontinue();
		return;
    }
	
    ppogl::Vec3d left_vector = (TRACK_WIDTH/2.0)*width_vector;
    ppogl::Vec3d right_vector = (-TRACK_WIDTH/2.0)*width_vector;
    ppogl::Vec3d left_wing =  player->pos - left_vector;
    ppogl::Vec3d right_wing = player->pos - right_vector;
    float left_y = find_y_coord( left_wing.x(), left_wing.z() );
    float right_y = find_y_coord( right_wing.x(), right_wing.z() );
    if (fabs(left_y-right_y) > MAX_TRACK_DEPTH) {
		discontinue();
		return;
    }

    pp::Plane surf_plane = get_local_course_plane( player->pos );
    float dist_from_surface = surf_plane.distance( player->pos );
	float comp_depth = terrain_compression;
    if ( dist_from_surface >= (2*comp_depth) ) {
		discontinue();
		return;
    }

    if (!continuing_track) {
		discontinue();
	q->trackType = TrackQuad::TYPE_HEAD;
	q->v1 = ppogl::Vec3d( left_wing.x(), left_y + TRACK_HEIGHT, left_wing.z() );
	q->v2 = ppogl::Vec3d( right_wing.x(), right_y + TRACK_HEIGHT, right_wing.z() );
	q->n1 = find_course_normal( q->v1.x(), q->v1.z());
	q->n2 = find_course_normal( q->v2.x(), q->v2.z());
	q->t1 = ppogl::Vec2d(0.0, 0.0);
	q->t2 = ppogl::Vec2d(1.0, 0.0);
	next_mark = current_mark + 1;
    } else {
	if ( next_mark == current_mark ) {
	    q->v1 = qprev->v3;
	    q->v2 = qprev->v4;
	    q->n1 = qprev->n3;
	    q->n2 = qprev->n4;
	    q->t1 = qprev->t3; 
	    q->t2 = qprev->t4;
	    if ( qprev->trackType != TrackQuad::TYPE_HEAD ) {
			qprev->trackType = TrackQuad::TYPE_MARK;
	    }
	    q->trackType = TrackQuad::TYPE_MARK;
	}
	q->v3 = ppogl::Vec3d( left_wing.x(), left_y + TRACK_HEIGHT, left_wing.z() );
	q->v4 = ppogl::Vec3d( right_wing.x(), right_y + TRACK_HEIGHT, right_wing.z() );
	q->n3 = find_course_normal( q->v3.x(), q->v3.z());
	q->n4 = find_course_normal( q->v4.x(), q->v4.z());
	tex_end = speed*GameMgr::getInstance().getTimeStep()/TRACK_WIDTH;
	if (q->trackType == TrackQuad::TYPE_HEAD) {
	    q->t3= ppogl::Vec2d(0.0, 1.0);
	    q->t4= ppogl::Vec2d(1.0, 1.0);
	} else {
	    q->t3 = ppogl::Vec2d(0.0, q->t1.y() + tex_end);
	    q->t4 = ppogl::Vec2d(1.0, q->t2.y() + tex_end);
	}

	current_mark++;
	next_mark = current_mark;
    }

    q->alpha = MIN( (2*comp_depth-dist_from_surface)/(4*comp_depth), 1.0 );
		
    last_mark_time = GameMgr::getInstance().time;
    continuing_track = true;

}

void
TrackMarks::init()
{
	for(int i=0; i<GameMgr::getInstance().numPlayers; i++){
	    trackMarks[i].current_mark = 0;
		trackMarks[i].next_mark = 0;
		trackMarks[i].last_mark_time = -99999;
		trackMarks[i].last_mark_pos = ppogl::Vec3d(-9999, -9999, -9999);
		trackMarks[i].continuing_track = false;
		
		//new stuff
		trackMarks[i].player = &(players[i]);		
	}
	
	int maxMarks = PPConfig.getInt("max_track_marks");
	if(maxNumQuads!=maxMarks){
		for(int i=0; i<GameMgr::getInstance().numPlayers; i++){
			if(trackMarks[i].quads) delete [] trackMarks[i].quads;
	    	trackMarks[i].quads = new TrackQuad[maxMarks];
		}	
		maxNumQuads=maxMarks;
	}	
}

// begin new stuff

TrackMarks::TrackMarks()
 : player(NULL),
   quads(NULL)
{
}

TrackMarks::~TrackMarks()
{
	if(quads!=NULL) delete [] quads;
}

void
TrackMarks::discontinueAllPlayers()
{
	for(int i=0; i<GameMgr::getInstance().numPlayers; i++){
		trackMarks[i].discontinue();
	}
}

void
TrackMarks::drawAllPlayers()
{
	if(GameConfig::drawTrackMarks == false) {
		return;
    }
	
	for(int i=0; i<GameMgr::getInstance().numPlayers; i++){
		trackMarks[i].draw();
	}	
}

void
TrackMarks::mirror(float width)
{
	for(int i=0; i<maxNumQuads; i++){
		quads[i].v1.x()= width-quads[i].v1.x();
		quads[i].v2.x()= width-quads[i].v2.x();
		quads[i].v3.x()= width-quads[i].v3.x();
		quads[i].v4.x()= width-quads[i].v4.x();
	}	
}

void
TrackMarks::mirrorAllPlayers(float width)
{
	for(int i=0; i<GameMgr::getInstance().numPlayers; i++){
		trackMarks[i].mirror(width);
	}	
}

TrackMarks trackMarks[2];
