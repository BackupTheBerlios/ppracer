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
 
#ifndef _KEYBOARD_CONFIG_H_
#define _KEYBOARD_CONFIG_H_

#include "configmode.h"

#include "ppogl/ui.h"

class KeyboardConfig
 : public ConfigMode
{
	ppogl::Label m_leftLbl;	
	ppogl::Entry m_leftEntry;
	
	ppogl::Label m_rightLbl;
	ppogl::Entry m_rightEntry;
	
	ppogl::Label m_paddleLbl;
	ppogl::Entry m_paddleEntry;
	
	ppogl::Label m_brakeLbl;
	ppogl::Entry m_brakeEntry;
	
	ppogl::Label m_jumpLbl;
	ppogl::Entry m_jumpEntry;
	
	ppogl::Label m_trickLbl;
	ppogl::Entry m_trickEntry;
	
	ppogl::Label m_resetLbl;
	ppogl::Entry m_resetEntry;
	
	ppogl::Label m_pauseLbl;
	ppogl::Entry m_pauseEntry;
	
public:
	KeyboardConfig();

	void apply();

	std::string getKey(SDLKey key);
	SDLKey getKey(std::string& string);

	void setKey(ppogl::Entry *widget, SDLKey key);
};

#endif // KEYBOARD_CONFIG_H_
