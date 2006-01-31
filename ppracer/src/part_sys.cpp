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

Particle::Particle(const ppogl::Vec3d& _pt, GLuint _binding, const ppogl::Vec3d& _vel)
 : pt(_pt),
   binding(_binding),
   vel(_vel)
{
	type = Types(frand() * (double(NUM_PARTICLE_TYPES) - EPS));
		
	base_size = ( frand() + 0.5 ) * OLD_PART_SIZE;
	cur_size = NEW_PART_SIZE;

	age = frand() * MIN_AGE;
	death = frand() * MAX_AGE;
}

//new stuff
PartSys partsys[2];

ppogl::Color PartSys::sm_color;

void
PartSys::draw(const Player& plyr)
{
	ppogl::Vec2d min_tex_coord, max_tex_coord;
    set_gl_options( PARTICLES );
    gl::TexEnv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	std::list<Particle>::iterator it;	
	
    for(it=m_particles.begin(); it!=m_particles.end(); it++){
        if((*it).age < 0) continue;
		
		gl::BindTexture(GL_TEXTURE_2D, (*it).binding);
				
		if((*it).type == 0 || (*it).type == 1){
	    	min_tex_coord.y() = 0;
	    	max_tex_coord.y() = 0.5;
		}else{
		    min_tex_coord.y() = 0.5;
		    max_tex_coord.y() = 1.0;
		}

		if((*it).type == 0 || (*it).type == 3){
	    	min_tex_coord.x() = 0;
	    	max_tex_coord.x() = 0.5;
		}else{
	    	min_tex_coord.x() = 0.5;
	    	max_tex_coord.x() = 1.0;
		}

		gl::Color(sm_color, sm_color.a() * (*it).alpha);

		draw_billboard(plyr, (*it).pt, (*it).cur_size, (*it).cur_size,
			min_tex_coord, max_tex_coord);
    }
}

void
PartSys::update(double timestep)
{
	std::list<Particle>::iterator it;
    for(it=m_particles.begin(); it!=m_particles.end(); it++){
        (*it).age += timestep;

        if((*it).age < 0 ) continue;

		(*it).pt = ( (*it).pt)+(timestep*(*it).vel );

		const double ycoord = find_y_coord( (*it).pt );

		if( (*it).pt.y() < ycoord - 3 ){
	   		(*it).age = (*it).death + 1;
		} 

        if( (*it).age >= (*it).death ){
			it = m_particles.erase(it);
            continue;
        } 

        (*it).alpha = ( (*it).death - (*it).age ) / (*it).death;

		(*it).cur_size = NEW_PART_SIZE + 
	    		(OLD_PART_SIZE - NEW_PART_SIZE) * ( (*it).age / (*it).death );

        (*it).vel.y() += -EARTH_GRAV * timestep;
    } 
}

void
PartSys::createParticles(const ppogl::Vec3d& loc, const ppogl::Vec3d& vel, int num, GLuint particle_binding) 
{
	PP_REQUIRE(m_particles.size + num < MAX_PARTICLES, "Maximum number of particles exceeded");
	
	const double speed = vel.length();
    for(int i=0; i<num; i++){      
		m_particles.push_front(Particle(
				ppogl::Vec3d(
					loc.x() + 2.0*(frand() - 0.5) * START_RADIUS,
        			loc.y(),
        			loc.z() + 2.0*(frand() - 0.5) * START_RADIUS
				),
				particle_binding,
				vel+ppogl::Vec3d(
						VARIANCE_FACTOR * (frand() - 0.5) * speed, 
						VARIANCE_FACTOR * (frand() - 0.5) * speed,
						VARIANCE_FACTOR * (frand() - 0.5) * speed
				)
		));		
    }
} 

void
PartSys::reset()
{
	sm_color = ppogl::Color::white;
	for(int i=0; i<2; i++){
		partsys[i].m_particles.clear();
	}		
}

int
PartSys::sqParticleColor(ppogl::Script *vm)
{
    if(vm->getTop()!=1){
		PP_WARNING("pptheme.particle_color: Invalid number of arguments");
		return 0;
	}
				
	vm->pushNull();
	for(int i=0; i<4; i++){
		vm->next(1);
		PartSys::sm_color.values[i]=vm->getFloat(-1);
		vm->pop();
	}
	
	return 0;
}

static const struct ppogl::Script::Lib ppthemelib[]={
	{"particle_color", PartSys::sqParticleColor},
	{NULL, NULL}
};

void
PartSys::registerCallbacks()
{
	script.registerLib("pptheme", ppthemelib);
}
