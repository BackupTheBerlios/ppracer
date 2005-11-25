/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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
 
 
#include "configmode.h"

ConfigMode::ConfigMode(const std::string& title)
 : m_titleLbl(title,"heading"),
   m_cancelBtn(_("Cancel")),
   m_applyBtn(_("Ok"))
{
	ppogl::UIManager::getInstance().setBoxDimension(ppogl::Vec2d(640,480));	
	
	ppogl::Vec2d position(320,440);
	
	m_titleLbl.setPosition(position);
	m_titleLbl.alignment.set(0.5f, 1.0f);
	
	position.x()=80;
	position.y()=0;
	
	m_cancelBtn.setPosition(position);
    m_cancelBtn.signalClicked.Connect(ppogl::CreateSlot(this,&ConfigMode::cancel));
    
	position.x()=560;
	
	m_applyBtn.setPosition(position);
	m_applyBtn.alignment.right();
    m_applyBtn.signalClicked.Connect(ppogl::CreateSlot(this,&ConfigMode::apply));
}

void
ConfigMode::loop(float timeStep)
{
    set_gl_options( GUI );
		
	// calls the custom loop of the derived class (if it exists)
	customLoop( timeStep );
	
	// draw snow if enabled
	drawSnow( timeStep );
	
	// draw ui
	ppogl::UIManager::getInstance().draw(resolutionX,
										 resolutionY);	
	
    reshape(resolutionX, resolutionY);
}

bool
ConfigMode::keyPressEvent(SDLKey key)
{
	switch (key){
		case SDLK_ESCAPE:
			cancel();
	    	return true;
		case SDLK_RETURN:
			apply();
	    	return true;
		default:
			return false;
	}
}

void
ConfigMode::cancel()
{
	setMode( GameMode::prevmode );
}
