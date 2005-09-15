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

#include "loading.h"

#include "gl_util.h"
#include "fps.h"
#include "render_util.h"
#include "phys_sim.h"
#include "view.h"
#include "course_render.h"
#include "loop.h"
#include "fog.h"
#include "viewfrustum.h"
#include "course_mgr.h"
#include "mirror_course.h"
#include "joystick.h"
#include "part_sys.h"

#include "game_mgr.h"

Loading::Loading()
 : m_loadingLbl(_("Loading, Please Wait..."),"loading")
{
	ppogl::UIManager::getInstance().setBoxDimension(ppogl::Vec2d(640,480));	

	m_loadingLbl.setPosition(ppogl::Vec2d(320,240));
	m_loadingLbl.alignment.set(0.5,0.5);
	
	m_loadedCondition = static_cast<RaceConditions>(-1);
	ppogl::AudioMgr::getInstance().playMusic("loading");
}


extern void updateDisplay();


void
Loading::loop(float timeStep)
{
    set_gl_options( GUI );

	drawSnow(timeStep, GameMgr::getInstance().getCurrentRace().windy);
	
	ppogl::UIManager::getInstance().draw(resolutionX,
										 resolutionY);

	updateDisplay();
	
    if ( m_loadedCourse.empty() ||
		m_loadedCourse != GameMgr::getInstance().getCurrentRace().course ||
		m_loadedCondition != GameMgr::getInstance().getCurrentRace().condition ) 
    {
	// Load the course
	Course::load( GameMgr::getInstance().getCurrentRace().course );

	m_loadedCourse = GameMgr::getInstance().getCurrentRace().course;
	m_loadedCondition = GameMgr::getInstance().getCurrentRace().condition;
    }

	set_course_mirroring( GameMgr::getInstance().getCurrentRace().mirrored );

	reshape(resolutionX, resolutionY);

    // We're done here, enter INTRO mode
    setMode( INTRO );	
}
