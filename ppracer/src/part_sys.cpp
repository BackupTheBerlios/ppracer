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

#include "part_sys.h"
#include "phys_sim.h"
#include "gl_util.h"
#include "course_render.h"
#include "render_util.h"

#include "ppogl/base/defs.h"
#include "ppogl/base/glwrappers.h"

/** This constant is here as part of a debugging check to prevent an infinite 
   number of particles from being created */
#define MAX_PARTICLES 500000

#define START_RADIUS 0.04
#define OLD_PART_SIZE 0.07
#define NEW_PART_SIZE 0.02
#define MIN_AGE     -0.2
#define MAX_AGE      1.0

#define VARIANCE_FACTOR 0.8

#define PARTICLE_SHADOW_HEIGHT 0.05
#define PARTICLE_SHADOW_ALPHA 0.1

inline double
frand()
{
    return double(rand())/RAND_MAX;
}

class Particle
{
public:
	static Particle* head;
	static int numParticles;
	
	enum Types{
		PARTICLE_TYPE_1=0,
		PARTICLE_TYPE_2,
		PARTICLE_TYPE_3,
		PARTICLE_TYPE_4,
		NUM_PARTICLE_TYPES
	};

	Particle(const ppogl::Vec3d& _pt,
				GLuint _binding,
				const ppogl::Vec3d& _vel)
     : pt(_pt),
	   binding(_binding),
	   vel(_vel),
	   next(head)
	{
		// attach ourself to the head
		head = this;	
		numParticles++;
		
		type = Types(frand() * (double(NUM_PARTICLE_TYPES) - EPS));
		
		base_size = ( frand() + 0.5 ) * OLD_PART_SIZE;
		cur_size = NEW_PART_SIZE;

        age = frand() * MIN_AGE;
        death = frand() * MAX_AGE;
	}
	
	ppogl::Vec3d pt;

	Types type;

    double base_size;
    double cur_size;
    double terrain_height;
    double age;
    double death;
    double alpha;
    ppogl::Vec3d vel;
	GLuint binding;
    Particle *next;
	
};

static ppogl::Color particleColor;

Particle* Particle::head = NULL;
int Particle::numParticles = 0;

void
create_new_particles(const ppogl::Vec3d& loc, ppogl::Vec3d vel, int num, GLuint particle_binding) 
{
    // debug check to track down infinite particle bug 
    //if(Particle::numParticles + num > MAX_PARTICLES){
	//	PP_ERROR("maximum number of particles exceeded");
    //}	
	PP_REQUIRE(Particle::numParticles + num < MAX_PARTICLES, "Maximum number of particles exceeded");
	
	const double speed = vel.normalize();
    for(int i=0; i<num; i++){      
		Particle* newp = new Particle(
				ppogl::Vec3d(
					loc.x() + 2.*(frand() - 0.5) * START_RADIUS,
        			loc.y(),
        			loc.z() + 2.*(frand() - 0.5) * START_RADIUS
				),
				particle_binding,
				(speed*vel)+ppogl::Vec3d(
						VARIANCE_FACTOR * (frand() - 0.5) * speed, 
						VARIANCE_FACTOR * (frand() - 0.5) * speed,
						VARIANCE_FACTOR * (frand() - 0.5) * speed
				)
		);
    }
} 

void
update_particles(double time_step)
{
    Particle **p, *q;
    double ycoord;

    for (p = &Particle::head; *p != NULL; ) {
        (**p).age += time_step;

        if ( (**p).age < 0 ) continue;

	(**p).pt = ( (**p).pt)+(time_step*(**p).vel );

	ycoord = find_y_coord( (**p).pt.x(), (**p).pt.z() );

	if ( (**p).pt.y() < ycoord - 3 ) {
	    (**p).age = (**p).death + 1;
	} 

        if ( (**p).age >= (**p).death ) {
            q = *p;
            *p = q->next;
            delete q;
            Particle::numParticles--;
            continue;
        } 

        (**p).alpha = ( (**p).death - (**p).age ) / (**p).death;

	(**p).cur_size = NEW_PART_SIZE + 
	    ( OLD_PART_SIZE - NEW_PART_SIZE ) * ( (**p).age / (**p).death );

        (**p).vel.y() += -EARTH_GRAV * time_step;

        p = &( (**p).next );
    } 
} 

void
draw_particles(const Player& plyr)
{
    ppogl::Vec2d min_tex_coord, max_tex_coord;
    set_gl_options( PARTICLES );
    gl::TexEnv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
    for(Particle *p=Particle::head; p!=NULL; p = p->next){
        if(p->age < 0) continue;
		gl::BindTexture(GL_TEXTURE_2D, p->binding);
		if(p->type == 0 || p->type == 1){
	    	min_tex_coord.y() = 0;
	    	max_tex_coord.y() = 0.5;
		}else{
		    min_tex_coord.y() = 0.5;
		    max_tex_coord.y() = 1.0;
		}

		if(p->type == 0 || p->type == 3){
	    	min_tex_coord.x() = 0;
	    	max_tex_coord.x() = 0.5;
		}else{
	    	min_tex_coord.x() = 0.5;
	    	max_tex_coord.x() = 1.0;
		}

		gl::Color(particleColor, particleColor.a() * p->alpha);

		draw_billboard(plyr, p->pt, p->cur_size, p->cur_size,
			min_tex_coord, max_tex_coord);
    }
} 

void
clear_particles()
{
    Particle* q;
    Particle* p=Particle::head;
    for(;;){
        if(p==NULL) break;
        q=p;
        p=p->next;
        delete q;
    } 
	Particle::head = NULL;
    Particle::numParticles = 0;
}

void
reset_particles()
{
	particleColor = ppogl::Color::white;
} 

static int
particle_color_cb(ppogl::Script *vm)
{
    if(vm->getTop()!=1){
		PP_WARNING("pptheme.particle_color: Invalid number of arguments");
		return 0;
	}
				
	vm->pushNull();
	for(int i=0; i<4; i++){
		vm->next(1);
		particleColor.values[i]=vm->getFloat(-1);
		vm->pop();
	}
	
	return 0;
}

static const struct ppogl::Script::Lib ppthemelib[]={
	{"particle_color", particle_color_cb},
	{NULL, NULL}
};

void
register_particle_callbacks()
{
	script.registerLib("pptheme", ppthemelib);
}
