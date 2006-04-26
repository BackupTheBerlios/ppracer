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

#ifndef _EVENT_RACE_SELECT_H_
#define _EVENT_RACE_SELECT_H_

#include "loop.h"

#include "course_mgr.h"
#include "player.h"

#include "ppogl/ui.h"

class EventRaceSelect
 : public GameMode
{
	ppogl::Label m_titleLbl;
	ppogl::Textarea m_descTa;
	
	ppogl::ListBox< std::list<CourseData>::iterator > m_raceListBox;
	
	ppogl::Button m_backBtn;
	ppogl::Button m_startBtn;
	
	ppogl::ImageButton m_livesImg[4];
	
	ppogl::ImageButton m_previewImage;
	
	ppogl::Label m_authorTitle;
	ppogl::Label m_authorLbl;
	
	ppogl::Label m_timeTitle;
	ppogl::Label m_timeLbl;
	
	ppogl::Label m_herringTitle;
	ppogl::Label m_herringLbl;
	
	ppogl::Label m_scoreTitle;
	ppogl::Label m_scoreLbl;
	
	ppogl::Label m_statusLbl;
	
	PlayerCourseData m_data;

	enum{
		DEAD,
		RACEWON,
		PLAYABLE,
		UNPLAYABLE		
	}state;
	
	void updateStates();
	void updateStatusMsg();

public:
	EventRaceSelect();

	void loop(float timeStep);

	void start();
	void back();
	void updateData();

	bool keyPressEvent(SDLKey key);
};

#endif // _EVENT_RACE_SELECT_H_
