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

#include "splash_screen.h"

#include "gl_util.h"

#include "ppogl/textures.h"
#include "ppogl/ui.h"

#include "ppogl/base/glstuff.h"
 
SplashScreen::SplashScreen()
 : m_anykeyLbl(_("PRESS ANY KEY TO START"),"event_and_cup_label"),
   m_versionLbl(PP_RELEASE_NAME, "race_requirements_label")
{
	ppogl::Vec2d pos = (resolution/2);
	
	m_anykeyLbl.setPosition(pos);
	m_anykeyLbl.alignment.set(0.5,0.0);
	
	m_versionLbl.setPosition(ppogl::Vec2d(resolution.x()-5,5));
	m_versionLbl.alignment.right();
	
    ppogl::AudioMgr::getInstance().playMusic("splash_screen");
}

SplashScreen::~SplashScreen()
{
	// we don't need this binding anymore
	ppogl::TextureMgr::getInstance().unbind("splash_screen");
	ppogl::AudioMgr::getInstance().unbindMusic("splash_screen");
}

void
SplashScreen::loop(float timeStep)
{
    set_gl_options( GUI );
	
	drawSnow(timeStep);

	{
		ppogl::Vec2d pos(resolution.x()/2 -256,resolution.y()/2);
		ppogl::Vec2d size(512, 256);
		ppogl::TextureRef texture =
			ppogl::TextureMgr::getInstance().get("splash_screen");
			
		ppogl::drawRectangle(pos, size, texture);
	}	

	ppogl::UIManager::getInstance().draw(resolution,
										 false); // don't draw decorations	
		
    reshape(resolution);
}


bool
SplashScreen::mouseButtonReleaseEvent(int button, const ppogl::Vec2i& position)
{
	setMode( GAME_TYPE_SELECT );
	return true;
}

bool
SplashScreen::keyPressEvent(SDLKey key)
{
	setMode( GAME_TYPE_SELECT );
	return true;
}
