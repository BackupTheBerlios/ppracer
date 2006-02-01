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

#ifndef _PART_SYS_H_
#define _PART_SYS_H_

#include "player.h"
#include "ppogl/base/color.h"
#include <list>

class Particle
{
public:
	Particle(){};
	Particle(const ppogl::Vec3d& _pt, GLuint _binding, const ppogl::Vec3d& _vel);
	
	ppogl::Vec3d pt;

	enum Types{
		PARTICLE_TYPE_1=0,
		PARTICLE_TYPE_2,
		PARTICLE_TYPE_3,
		PARTICLE_TYPE_4,
		NUM_PARTICLE_TYPES
	}type;

    double base_size;
    double cur_size;
    double terrain_height;
    double age;
    double death;
    double alpha;
	GLuint binding;
    ppogl::Vec3d vel;
};

class PartSys
{
	std::list<Particle> m_particles;	
	
public:
	void draw(const Player& plyr);
	void update(double timestep);

	void createParticles(const ppogl::Vec3d& loc, const ppogl::Vec3d& vel, int num, GLuint particle_binding);

protected:
	static ppogl::Color sm_color;
		
public:
	static void reset();
	static int sqParticleColor(ppogl::Script *vm);
	static void registerCallbacks();
};

extern PartSys partsys[2];

#endif // _PART_SYS_H_
