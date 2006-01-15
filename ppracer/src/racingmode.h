/*  
 * PlanetPenguin Racer 
 * Copyright (C) 2005-2006 Volker Stroebel <volker@planetpenguin.de>
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
 
#ifndef _RACINGMODE_H_
#define _RACINGMODE_H_

#include "loop.h"

class RacingMode
 : public GameMode
{
protected:
	bool m_paused;

	RacingMode();
	void renderCourse(int player, float timestep);	

public:
	void loop(float timestep);
	
	virtual void preDisplay(float timestep){};
	virtual void postDisplay(float timestep){};
			
	virtual void prePlayer(int player, float timestep){};
	virtual void postPlayer(int player, float timestep){};
};

#endif // _RACINGMODE_H_
