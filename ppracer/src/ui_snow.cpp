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

#include "ui_snow.h"
#include "gl_util.h"
#include "course_render.h"
#include "render_util.h"
#include "loop.h"

#include "stuff.h"

#include "ppogl/base/defs.h"
#include "ppogl/base/glwrappers.h"
#include "ppogl/textures.h"

#define MAX_NUM_PARTICLES 10000
#define BASE_NUM_PARTICLES 400
#define GRAVITY_FACTOR 0.015
#define BASE_VELOCITY 0.05
#define VELOCITY_RANGE 0.02
#define PUSH_DECAY_TIME_CONSTANT 0.2
#define PUSH_DIST_DECAY 100 
#define PUSH_FACTOR 0.5
#define MAX_PUSH_FORCE 5
#define WIND_FORCE 0.03
#define AIR_DRAG 0.4

#define PARTICLE_MIN_SIZE 1
#define PARTICLE_SIZE_RANGE 10

struct Particle
{
    ppogl::Vec2d pt;
    double size;
    ppogl::Vec2d vel;
    ppogl::Vec2d tex_min;
    ppogl::Vec2d tex_max;
};

static Particle particles[MAX_NUM_PARTICLES];
static int num_particles = BASE_NUM_PARTICLES;
static ppogl::Color particleColor(1, 1, 1, 0.4);
static ppogl::Vec2d push_position(0,0);
static ppogl::Vec2d last_push_position;
static double last_update_time = -1;
static bool push_position_initialized = false;

static double
frand()
{
    return double(rand())/RAND_MAX;
} 

static void
make_particle(int i, double x, double y)
{
    double p_dist;
    int type;

    particles[i].pt.x() = x;
    particles[i].pt.y() = y;
    p_dist = frand();
    particles[i].size = PARTICLE_MIN_SIZE + (1.0 - p_dist)*PARTICLE_SIZE_RANGE;
    particles[i].vel.x() = 0;
    particles[i].vel.y() = -BASE_VELOCITY-p_dist*VELOCITY_RANGE;
    type = int(frand() * (4.0 - EPS));
    if (type == 0) {
		particles[i].tex_min = ppogl::Vec2d( 0.0, 0.0 );
		particles[i].tex_max = ppogl::Vec2d( 0.5, 0.5 );
    } else if (type == 1) {
		particles[i].tex_min = ppogl::Vec2d( 0.5, 0.0 );
		particles[i].tex_max = ppogl::Vec2d( 1.0, 0.5 );
    } else if (type == 2) {
		particles[i].tex_min = ppogl::Vec2d( 0.5, 0.5 );
		particles[i].tex_max = ppogl::Vec2d( 1.0, 1.0 );
    } else {
		particles[i].tex_min = ppogl::Vec2d( 0.0, 0.5 );
		particles[i].tex_max = ppogl::Vec2d( 0.5, 1.0 );
    }
}

void
init_ui_snow()
{
    for(int i=0; i<num_particles; i++){
		make_particle(i, frand(), frand());
    }
    push_position = ppogl::Vec2d(0.0, 0.0);
}

void
update_ui_snow(double time_step, bool windy)
{
    ppogl::Vec2d *v, f;
    ppogl::Vec2d *pt;
    double size;
    double dist_from_push, p_dist;
    ppogl::Vec2d push_vector;
    int i;
    double push_timestep, time;

    time = getClockTime();

    push_vector.x() = 0;
    push_vector.y() = 0;
    push_timestep = 0;
	
    if (push_position_initialized){
		push_vector.x() = push_position.x() - last_push_position.x();
		push_vector.y() = push_position.y() - last_push_position.y();
		push_timestep = time - last_update_time;
    }
    last_push_position = push_position;
    last_update_time = time;

    for ( i=0; i<num_particles; i++) {
	pt = &particles[i].pt;
	v = &particles[i].vel;
	size = particles[i].size;

	f.x() = 0;
	f.y() = 0;

	/* Mouse push and gravity */
	dist_from_push = (pow((pt->x() - push_position.x()), 2) +
			  pow((pt->y() - push_position.y()), 2));
	if ( push_timestep > 0 ) {
	    f.x() = PUSH_FACTOR * push_vector.x() / push_timestep; 
	    
	    f.y() = PUSH_FACTOR * push_vector.y() / push_timestep; 

	    f.x() = MIN(MAX_PUSH_FORCE,f.x());
	    f.x() = MAX(-MAX_PUSH_FORCE,f.x());
	    f.y() = MIN(MAX_PUSH_FORCE,f.y());
	    f.y() = MAX(-MAX_PUSH_FORCE,f.y());

	    f.x() *= 1.0/(PUSH_DIST_DECAY*dist_from_push + 1) * 
		size/PARTICLE_SIZE_RANGE;
	    f.y() *= 1.0/(PUSH_DIST_DECAY*dist_from_push + 1) *
		size/PARTICLE_SIZE_RANGE;
	}

	/* Update velocity */
	v->x() += ( f.x() + ( windy ? WIND_FORCE : 0.0 ) - v->x() * AIR_DRAG ) * 
	    time_step;
	v->y() += ( f.y() - GRAVITY_FACTOR - v->y() * AIR_DRAG ) * 
	    time_step;

	/* Update position */
        pt->x() += v->x() * time_step * ( size / PARTICLE_SIZE_RANGE ); 
        pt->y()+= v->y() * time_step * ( size / PARTICLE_SIZE_RANGE );

	if ( pt->x() < 0 ) {
	    pt->x() = 1;
	} else if ( pt->x() > 1 ) {
	    pt->x() = 0.0;
	}
    }

    /* Kill off & regenerate particles */
    for (i=0; i<num_particles; i++) {
	Particle *p = &particles[i];

	if (p->pt.y() < -0.05) {
	    /* If we have an excess of particles, kill off with
	       50% probability */
	    if ( num_particles > BASE_NUM_PARTICLES && frand() > 0.5 ) {
		/* Delete the particle */
		*p = particles[num_particles-1];
		num_particles -= 1;
	    } else {
		p->pt.x() = frand();
		p->pt.y() = 1+frand()*BASE_VELOCITY;
		p_dist = frand();
		p->size = PARTICLE_MIN_SIZE + 
		    ( 1.0 - p_dist ) * PARTICLE_SIZE_RANGE;
		p->vel.x() = 0;
		p->vel.y() = -BASE_VELOCITY-p_dist*VELOCITY_RANGE;
	    }
	}
    }

    if ( time_step < PUSH_DECAY_TIME_CONSTANT ) {
	push_vector.x() *= 1.0 - time_step/PUSH_DECAY_TIME_CONSTANT;
	push_vector.y() *= 1.0 - time_step/PUSH_DECAY_TIME_CONSTANT;
    } else {
	push_vector.x() = 0.0;
	push_vector.y() = 0.0;
    }
} 

void
draw_ui_snow()
{
    ppogl::Vec2d *pt, *tex_min, *tex_max;
    double size;
    int i;
	
	gl::PushMatrix();
	{
	gl::MatrixMode(GL_PROJECTION);
    gl::LoadIdentity();
    gl::Ortho(0.0, GameMode::resolutionX, 0.0, GameMode::resolutionY, -1.0, 1.0);
    gl::MatrixMode(GL_MODELVIEW);
    gl::LoadIdentity();
    gl::Translate(0.0, 0.0, -1.0);
    gl::Color(ppogl::Color::white);
		    
	ppogl::TextureRef texture =
		ppogl::TextureMgr::getInstance().get("ui_snow_particle");


    gl::TexEnv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    gl::BindTexture( GL_TEXTURE_2D, texture );

    gl::Color(particleColor);

    gl::PushMatrix();
    {
	for ( i=0; i<num_particles; i++) {
	    pt = &particles[i].pt;
	    size = particles[i].size;
	    tex_min = &particles[i].tex_min;
	    tex_max = &particles[i].tex_max;
	    gl::PushMatrix();
	    {
		gl::Translate(pt->x()*GameMode::resolutionX, pt->y()*GameMode::resolutionY);
		gl::Begin(GL_QUADS);
		{
		    gl::TexCoord(tex_min->x(), tex_min->y());
		    gl::Vertex(0, 0);
		    gl::TexCoord(tex_max->x(), tex_min->y());
		    gl::Vertex(size, 0);
		    gl::TexCoord(tex_max->x(), tex_max->y());
		    gl::Vertex(size, size);
		    gl::TexCoord(tex_min->x(), tex_max->y());
		    gl::Vertex(0, size);
		}
		gl::End();
	    }
	    gl::PopMatrix();
	} 
    }
    gl::PopMatrix();
	
	}
	gl::PopMatrix();
} 

void
reset_ui_snow_cursor_pos(const ppogl::Vec2d& pos ) 
{
    push_position = ppogl::Vec2d( pos.x()/double(GameMode::resolutionX),
				  pos.y()/double(GameMode::resolutionY) );
    last_push_position = push_position;
    push_position_initialized = true;
}

void 
push_ui_snow(const ppogl::Vec2d& pos)
{
    push_position = ppogl::Vec2d( pos.x()/double(GameMode::resolutionX),
				  pos.y()/double(GameMode::resolutionY) );
    if ( !push_position_initialized ) {
		last_push_position = push_position;
    }
    push_position_initialized = true;
}

void
make_ui_snow(const ppogl::Vec2d& pos)
{
    if(num_particles < MAX_NUM_PARTICLES){
		make_particle( num_particles, pos.x()/GameMode::resolutionX, pos.y()/GameMode::resolutionY);
		num_particles++;
    }
}
