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

#ifndef _RACING_H_
#define _RACING_H_

#include "racingmode.h"
#include "player.h"

class Racing : public RacingMode
{
	/// struct holding several state for the players
	struct States
	{
		States();
		double chargeStartTime;
		int lastTerrain;
		bool rightTurn;
		bool leftTurn;
		bool trickModifier;
		bool paddling;
		bool charging;
		bool braking;
	}m_state[2];
		
	void calcJumpAmt(int player);

public:
	Racing();
	~Racing();

	void preDisplay(float timestep);
	void postDisplay(float timeStep);
	
	void prePlayer(int player, float timestep);

	bool keyboardEvent(SDLKey key, bool release);
	bool keyPressEvent(SDLKey key);
};


#endif
