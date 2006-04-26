/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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

#ifndef _GAME_TYPE_SELECT_H_
#define _GAME_TYPE_SELECT_H_

#include "loop.h"
#include "ppogl/ui.h"

class GameTypeSelect
 : public GameMode
{
	ppogl::Button m_enterEventBtn;
	ppogl::Button m_practiceBtn;
	ppogl::Button m_configureBtn;
	ppogl::Button m_creditsBtn;
	ppogl::Button m_quitBtn;
	
public:
	GameTypeSelect();

	void loop(float timeStep);
	bool keyPressEvent(SDLKey key);

	void eventSelect();
	void credits();
	void practice();
	void configuration();
	void quit();
};

#endif // _GAME_TYPE_SELECT_H_
