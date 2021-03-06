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

#ifndef _EVENT_SELECT_H_
#define _EVENT_SELECT_H_

#include "loop.h"

#include "course_mgr.h"

#include "ppogl/ui.h"

class EventSelect
 : public GameMode
{
	ppogl::ListBox< std::list<EventData>::iterator > m_eventListBox;
	ppogl::ListBox< std::list<CupData>::iterator > m_cupListBox;

	std::list<EventData>::iterator m_curEvent;
	std::list<CupData>::iterator m_curCup;
		
	ppogl::Button m_backBtn;
	ppogl::Button m_continueBtn;
		
	ppogl::Label m_titleLbl;
	ppogl::Label m_eventLbl;
	ppogl::Label m_cupLbl;
	ppogl::Label m_statusLbl;
	
	bool m_curCupComplete;
	bool m_curCupPlayable;
	
	void updateCupStates();
	void updateButtonEnabledStates();
	
public:
	EventSelect();

	void loop(float timestep);

	void cupChanged();
	void eventChanged();

	void back();
	void apply();
	
	bool keyPressEvent(SDLKey key);
};

#endif // _EVENT_SELECT_H_
