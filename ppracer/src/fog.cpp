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

#include "fog.h"
#include "gl_util.h"
#include "gameconfig.h"

#include "ppogl/base/glwrappers.h"

#include <iostream>


FogPlane fogPlane;

void 
FogPlane::reset()
{
    m_isOn = true;
    m_mode = GL_LINEAR;
    m_density = 0.005;
    m_start = 0.0;
    m_end = GameConfig::forwardClipDistance;
	m_color = ppogl::Color::white;
}

void 
FogPlane::setup()
{
    if(!m_isOn || PPConfig.getBool("disable_fog") ) {
		gl::Disable(GL_FOG);
		return;	
    }

    gl::Enable(GL_FOG);

    gl::Fog(GL_FOG_MODE, m_mode);
    gl::Fog(GL_FOG_DENSITY, m_density);
    gl::Fog(GL_FOG_START, m_start);
    gl::Fog(GL_FOG_END, m_end);
    gl::Fog(GL_FOG_COLOR, m_color);

	gl::Hint(GL_FOG_HINT, GL_NICEST);

}

static int
fog_cb(ppogl::Script *vm)
{
	int num_args = vm->getTop();
	
    if(num_args !=6){
		PP_WARNING("pptheme.fog: Inavalid number of arguments");
		return 0;
    }

	// activated
	fogPlane.setEnabled(vm->getBool(1));
	
	//mode 
	if(num_args>=2){
		std::string mode = vm->getString(2);
			
	    if(mode=="exp"){
			fogPlane.setMode(GL_EXP);
	    }else if(mode=="exp2"){
			fogPlane.setMode(GL_EXP2);
	    }else if(mode=="linear"){
			fogPlane.setMode(GL_LINEAR);
	    }else {
			PP_WARNING("pptheme.fog: mode must be one of "
			       "`exp', `exp2', or `linear'");
			return 0;
	    }
	}
	
	//density
	if(num_args>=3){
		fogPlane.setDensity(vm->getFloat(3));
	}

	//color
	if(num_args>=4){
		ppogl::Color color;
		vm->pushNull();
		for(int i=0; i<4; i++){
			vm->next(4);
			color.values[i]=vm->getFloat();	
			vm->pop();
		}
		fogPlane.setColor(color);
	}
	
	//start
	if(num_args>=5){
		fogPlane.setStart(vm->getFloat(5));	
	}
	
	//end
	if(num_args>=6){
		fogPlane.setEnd(vm->getFloat(6));	
	}

    return 0;
}

static const struct ppogl::Script::Lib ppthemelib[]={
	{"fog", fog_cb},
	{NULL, NULL}
};


void
FogPlane::registerCallbacks()
{
	script.registerLib("pptheme", ppthemelib);
}
