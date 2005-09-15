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


#include "game_type_select.h"

#include "ppogl/base/defs.h"

#include "game_mgr.h"

#include "winsys.h"

GameTypeSelect::GameTypeSelect()
 : m_enterEventBtn(_("Enter an event")),
   m_practiceBtn(_("Practice")),
   m_configureBtn(_("Configuration")),
   m_creditsBtn(_("Credits")),
   m_quitBtn(_("Quit"))   
{
	ppogl::UIManager::getInstance().setBoxDimension(ppogl::Vec2d(640,480));	
	
	ppogl::Vec2d position(320,280);
	
	m_enterEventBtn.setPosition(position);
	m_enterEventBtn.alignment.center();
    m_enterEventBtn.signalClicked.Connect(ppogl::CreateSlot(this,&GameTypeSelect::eventSelect));
	
	position.y()-=40;

    m_practiceBtn.setPosition(position);
    m_practiceBtn.alignment.center();
	m_practiceBtn.signalClicked.Connect(ppogl::CreateSlot(this,&GameTypeSelect::practice));

	position.y()-=40;
		
	m_configureBtn.setPosition(position);
	m_configureBtn.alignment.center();
    m_configureBtn.signalClicked.Connect(ppogl::CreateSlot(this,&GameTypeSelect::configuration));

	position.y()-=40;
	
	m_creditsBtn.setPosition(position);
	m_creditsBtn.alignment.center();
	m_creditsBtn.signalClicked.Connect(ppogl::CreateSlot(this,&GameTypeSelect::credits));
	
	position.y()-=40;

	m_quitBtn.setPosition(position);
	m_quitBtn.alignment.center();
    m_quitBtn.signalClicked.Connect(ppogl::CreateSlot(this,&GameTypeSelect::quit));

    ppogl::AudioMgr::getInstance().playMusic("start_screen");	
}

void
GameTypeSelect::loop(float timeStep)
{
    set_gl_options( GUI );

	drawSnow(timeStep);

	ppogl::UIManager::getInstance().draw(resolutionX,
										 resolutionY);	
	reshape(resolutionX, resolutionY);
}

bool
GameTypeSelect::keyPressEvent(SDLKey key)
{
	switch (key){
		case 'q':
		case 27: // Esc 
			winsys_exit(0);
	    	return true;;
		case 'e':
		case 13: // Enter
			eventSelect();
	    	return true;
		case 'p':
			practice();
			return true;
		case 'c':
			credits();
			return true;
		default:
			return false;
	}	
}

void
GameTypeSelect::eventSelect()
{
	GameMgr::getInstance().reset(GameMgr::EVENT);
	setMode( EVENT_SELECT );
}

void
GameTypeSelect::credits()
{
	setMode( CREDITS );
}

void
GameTypeSelect::practice()
{
	GameMgr::getInstance().reset(GameMgr::PRACTICING);
	setMode( RACE_SELECT );
}

void
GameTypeSelect::configuration()
{
	setMode( CONFIGURATION );
}

void
GameTypeSelect::quit()
{
	winsys_exit( 0 );
}
