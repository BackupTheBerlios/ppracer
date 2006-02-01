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
 
#ifndef _CONFIGMODE_H_
#define _CONFIGMODE_H_

#include "loop.h"

#include "ppogl/ui.h"

#include <string>

class ConfigMode
 : public GameMode
{
protected:
	ppogl::Label m_titleLbl;
	ppogl::Button m_cancelBtn;
	ppogl::Button m_applyBtn;
	std::string m_title;

public:
	ConfigMode(const std::string& title);

	void loop(float timeStep);

	bool keyReleaseEvent(SDLKey key);

	virtual void cancel();

	virtual void customLoop(float timeStep){};
	virtual void apply() = 0;
};

#endif // _CONFIGMODE_H_
