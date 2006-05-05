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

#include "lights.h"
#include "gl_util.h"

#include "ppogl/base/glwrappers.h"

Light Light::lights[NUM_COURSE_LIGHTS];

void
Light::reset()
{
    for(int i=0; i<NUM_COURSE_LIGHTS; i++){
		// Note: we initialize the lights to default OpenGL values
        // EXCEPT that light 0 isn't treated differently than the
        // others 
		lights[i].enabled = false;
		lights[i].spot_direction.z()=-1.0;
		lights[i].spot_exponent = 0.0;
		lights[i].spot_cutoff = 180.0;
		lights[i].constant_attenuation = 1.0;
		lights[i].linear_attenuation = 0.0;
		lights[i].quadratic_attenuation = 0.0;
		lights[i].ambient = ppogl::Color::black;
    	lights[i].diffuse = ppogl::Color::black;
    	lights[i].specular = ppogl::Color::black;
    }

    // Turn off global ambient light
    gl::LightModel(GL_LIGHT_MODEL_AMBIENT, ppogl::Color::black);
}

void
Light::setup()
{
    for(int i=0; i<NUM_COURSE_LIGHTS; i++){
		if(!lights[i].enabled){
			gl::Disable(GL_LIGHT0 + i);
			continue;
		}
		gl::Enable(GL_LIGHT0 + i);

		gl::Light( GL_LIGHT0 + i, GL_AMBIENT, lights[i].ambient );
		gl::Light( GL_LIGHT0 + i, GL_DIFFUSE, lights[i].diffuse );
		gl::Light( GL_LIGHT0 + i, GL_SPECULAR, lights[i].specular );
		gl::Light( GL_LIGHT0 + i, GL_POSITION, lights[i].position );
		gl::Light( GL_LIGHT0 + i, GL_SPOT_DIRECTION, lights[i].spot_direction );
		gl::Light( GL_LIGHT0 + i, GL_SPOT_EXPONENT, lights[i].spot_exponent );
		gl::Light( GL_LIGHT0 + i, GL_SPOT_CUTOFF, lights[i].spot_cutoff );
		gl::Light( GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, lights[i].constant_attenuation );
		gl::Light( GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, lights[i].linear_attenuation );
		gl::Light( GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, lights[i].quadratic_attenuation );
	}
}

static int
course_light_cb(ppogl::Script *vm)
{
	const int light_num = vm->getIntFromTable("id");
	
	if(light_num < 0 || light_num >= NUM_COURSE_LIGHTS ) {
		PP_WARNING("pptheme.course_light: Invalid light number: " << light_num);
		return 0;
    }
	
	Light::lights[light_num].enabled = true;
	
	if(vm->isKeyInTable("position")){
		Light::lights[light_num].position = vm->getVec4fFromTable("position");
	}

	if(vm->isKeyInTable("diffuse")){
		Light::lights[light_num].diffuse = vm->getColorFromTable("diffuse");
	}

	if(vm->isKeyInTable("specular")){
		Light::lights[light_num].specular = vm->getColorFromTable("specular");
	}
	
	if(vm->isKeyInTable("ambient")){
		Light::lights[light_num].ambient = vm->getColorFromTable("ambient");
	}
		
	return 0;
}

static const struct ppogl::Script::Lib ppthemelib[]={
	{"course_light", course_light_cb},
	{NULL, NULL}
};

void
register_course_light_callbacks()
{
	script.registerLib("pptheme", ppthemelib);
}
