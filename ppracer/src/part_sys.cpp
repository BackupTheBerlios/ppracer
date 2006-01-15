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

struct Particle
{
    ppogl::Vec3d pt;
    short type;
    double base_size;
    double cur_size;
    double terrain_height;
    double age;
    double death;
    double alpha;
    ppogl::Vec3d vel;
	GLuint particle_binding;
    Particle *next;
};

static ppogl::Color particleColor;

static Particle* head = NULL;
static int num_particles = 0;


inline double
frand()
{
    return double(rand())/RAND_MAX;
}

void
create_new_particles(const ppogl::Vec3d& loc, ppogl::Vec3d vel, int num, GLuint particle_binding) 
{
    double speed = vel.normalize();

    // debug check to track down infinite particle bug 
    if(num_particles + num > MAX_PARTICLES){
		PP_ERROR("maximum number of particles exceeded");
    }

    for(int i=0; i<num; i++){
        Particle* newp = new Particle;

        num_particles += 1;

        newp->next = head;
        head = newp;

        newp->pt.x() = loc.x() + 2.*(frand() - 0.5) * START_RADIUS;
        newp->pt.y() = loc.y();
        newp->pt.z() = loc.z() + 2.*(frand() - 0.5) * START_RADIUS;

		newp->type = int(frand() * (4.0 - EPS));
		
		newp->particle_binding = particle_binding;	
	
		newp->base_size = ( frand() + 0.5 ) * OLD_PART_SIZE;
		newp->cur_size = NEW_PART_SIZE;

        newp->age = frand() * MIN_AGE;
        newp->death = frand() * MAX_AGE;

        newp->vel=(speed*vel)+
			ppogl::Vec3d(
				VARIANCE_FACTOR * (frand() - 0.5) * speed, 
				VARIANCE_FACTOR * (frand() - 0.5) * speed,
				VARIANCE_FACTOR * (frand() - 0.5) * speed
			);
    }
} 

void
update_particles(double time_step)
{
    Particle **p, *q;
    double ycoord;

    for (p = &head; *p != NULL; ) {
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
            num_particles -= 1;
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
	
    for(Particle *p=head; p!=NULL; p = p->next){
        if(p->age < 0) continue;
		gl::BindTexture(GL_TEXTURE_2D, p->particle_binding);
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
    Particle* p=head;
    for(;;){
        if(p==NULL) break;
        q=p;
        p=p->next;
        delete q;
    } 
    head = NULL;
    num_particles = 0;
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
