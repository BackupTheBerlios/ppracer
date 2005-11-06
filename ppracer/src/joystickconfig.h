/* 
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
 
#ifndef _JOYSTICK_CONFIG_H_
#define _JOYSTICK_CONFIG_H_

#include "configmode.h"

#include "ppogl/ui.h"

class JoystickConfig
 : public ConfigMode
{
	ppogl::CheckBox m_enableJoystickBox;
	
	ppogl::Label m_enableJoystickLbl;
	
	ppogl::Label m_paddleLbl;
	ppogl::ListBox<int> m_paddleListBox;
	
	ppogl::Label m_brakeLbl;
	ppogl::ListBox<int> m_brakeListBox;
	
	ppogl::Label m_jumpLbl;
	ppogl::ListBox<int> m_jumpListBox;
	
	ppogl::Label m_trickLbl;
	ppogl::ListBox<int> m_trickListBox;
	
	void createButtonList(ppogl::ListBox<int> &list);
	
public:
	JoystickConfig();

	void apply();

	void paddleClicked();
	void brakeClicked();
	void jumpClicked();
	void trickClicked();
	
	void customLoop(double TimeStep);
};

#endif // _JOYSTICK_CONFIG_H_
