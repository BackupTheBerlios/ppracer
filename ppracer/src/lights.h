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

#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "ppogl/base/color.h"
#include "ppogl/base/vec3f.h"
#include "ppogl/base/vec4f.h"
#include "ppogl/base/glwrappers.h"

#define NUM_COURSE_LIGHTS 8

/// Light
struct Light
{
	ppogl::Color ambient;
    ppogl::Color diffuse;
    ppogl::Color specular;
    ppogl::Vec4f position;
	ppogl::Vec3f spot_direction;
	
    float spot_exponent;
    float spot_cutoff;
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
	
	bool enabled;
	
// static stuff
	static Light lights[NUM_COURSE_LIGHTS];
	
	static void setup();
	static void reset();	
};

void register_course_light_callbacks();

#endif // _LIGHTS_H_
