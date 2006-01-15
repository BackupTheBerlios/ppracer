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

#ifndef _GAME_OVER_H_
#define _GAME_OVER_H_

#include "loop.h"
#include "racingmode.h"

#include "ppogl/ui.h"

class GameOver
 : public RacingMode
{
	bool m_aborted;
	bool m_bestScore;
	
	ppogl::Label m_raceOverLbl;
	
	ppogl::Label m_timeLbl;
	ppogl::Label m_herringLbl;
	ppogl::Label m_scoreLbl;
	ppogl::Label m_maxspeedLbl;
	ppogl::Label m_flyingLbl;
	
	ppogl::Label m_resultsLbl;
	
public:
	GameOver();

	void preDisplay(float timestep);
	void postDisplay(float timestep);

	bool keyPressEvent(SDLKey key);
	bool mouseButtonEvent(int button, const ppogl::Vec2i& position, bool pressed);

	bool useDefaultDisplay(){return false;};
};

#endif // _GAME_OVER_H_
