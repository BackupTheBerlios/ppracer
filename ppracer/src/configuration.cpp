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


#include "configuration.h"

#include "gameconfig.h"

static GameMode::Mode preConfigMode = GameMode::NO_MODE;

Configuration::Configuration()
 : m_titleLbl(_("Configuration"),"heading"),
   m_generalBtn(_("General")),
   m_graphicsBtn(_("Graphics")),
   m_videoBtn(_("Video")),
   m_audioBtn(_("Audio")),
   m_keyboardBtn(_("Keyboard")),
   m_joystickBtn(_("Joystick")),
   m_backBtn(_("Back"))
{
	if(GameMode::prevmode==GAME_TYPE_SELECT || GameMode::prevmode == PAUSED){
		preConfigMode = GameMode::prevmode;
	}
		
	ppogl::UIManager::getInstance().setBoxDimension(ppogl::Vec2d(640,480));	
	
	ppogl::Vec2d position(320,400);
	
	m_titleLbl.setPosition(position);
	m_titleLbl.alignment.set(0.5f, 1.0f);
	
	position.y()-=100;
	
	m_generalBtn.setPosition(position);
	m_generalBtn.alignment.center();
    m_generalBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Configuration::general));
	
	position.y()-=40;
	
	m_graphicsBtn.setPosition(position);
	m_graphicsBtn.alignment.center();
	m_graphicsBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Configuration::graphics));	

	position.y()-=40;
	
	m_videoBtn.setPosition(position);
	m_videoBtn.alignment.center();
	m_videoBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Configuration::video));	

	position.y()-=40;
	
	m_audioBtn.setPosition(position);
	m_audioBtn.alignment.center();
	m_audioBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Configuration::audio));	
	
	position.y()-=40;

	m_keyboardBtn.setPosition(position);
	m_keyboardBtn.alignment.center();
	m_keyboardBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Configuration::keyboard));	

	position.y()-=40;

	m_joystickBtn.setPosition(position);
	m_joystickBtn.alignment.center();
	m_joystickBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Configuration::joystick));	

	position.y()-=60;

    m_backBtn.setPosition(position);
    m_backBtn.alignment.center();
    m_backBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Configuration::back));		

	// ensure music is played if setting have changed in the audio configuration
    ppogl::AudioMgr::getInstance().playMusic("start_screen");
}

void
Configuration::loop(float timeStep)
{
    set_gl_options( GUI );
	
	drawSnow(timeStep);

	// draw ui
	ppogl::UIManager::getInstance().draw(resolution);
    reshape(resolution);
}

bool
Configuration::keyPressEvent(SDLKey key)
{
	switch (key){
		case 27:
		case 'q':
			back();
	    	return true;
		default:
			return false;
	}	
}

void
Configuration::back()
{
	setMode( preConfigMode );
	
	//update static configuration if we are in paused mode
	if(preConfigMode==PAUSED) GameConfig::update();
		
	preConfigMode = NO_MODE;
}

void
Configuration::general()
{
	setMode( CONFIG_GENERAL );
}

void
Configuration::graphics()
{
	setMode( CONFIG_GRAPHICS );
}

void
Configuration::video()
{
	setMode( CONFIG_VIDEO );
}

void
Configuration::audio()
{
	setMode( CONFIG_AUDIO );
}

void
Configuration::keyboard()
{
	setMode( CONFIG_KEYBOARD );
}

void
Configuration::joystick()
{
	setMode( CONFIG_JOYSTICK );
}
