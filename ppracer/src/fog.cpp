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
	if(!m_isOn || GameConfig::disableFog){
		gl::Disable(GL_FOG);
    }else{
		gl::Enable(GL_FOG);
	}

	// not sure why this is still necessary if fog is disabled :?
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
	if(vm->isKeyInTable("enabled")){
		fogPlane.setEnabled(vm->getBoolFromTable("enabled"));
	}
	
	//mode 
	if(vm->isKeyInTable("mode")){
		std::string mode = vm->getStringFromTable("mode");
			
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
	
	if(vm->isKeyInTable("density")){
		fogPlane.setDensity(vm->getFloatFromTable("density"));
	}

	if(vm->isKeyInTable("color")){
		fogPlane.setColor(vm->getColorFromTable("color"));
	}
	
	if(vm->isKeyInTable("start")){
		fogPlane.setStart(vm->getFloatFromTable("start"));	
	}
	
	if(vm->isKeyInTable("end")){
		fogPlane.setEnd(vm->getFloatFromTable("end"));	
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
