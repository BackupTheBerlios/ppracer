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

#include "fps.h"

#include "game_mgr.h"
#include "bench.h"

FPS fpsCounter;

FPS::FPS() 
 : m_frames(0), 
   m_time(0.0f),
   m_fps(0.0f),
   m_maxStep(0.0f),
   m_lastTime(0.0f),
   m_maxFPSFrames(0)
{
}

void
FPS::update(float timestep)
{
	m_time += timestep;
	m_frames++;
	if(m_time>0.5f){
		m_fps = m_frames / m_time;
		m_time = 0.0f;
		m_frames = 0;
		if( Benchmark::getMode()!= Benchmark::NONE ){
			Benchmark::updateFPS(m_fps);
		}
	}
		
	// If a maximum fps is set, 
	// delay the programm to a certan amount of ms if necessary.
	// This implementation is not perfect but should be sufficient.
		
	if(m_maxStep>0){
		m_maxFPSFrames++;
		m_lastTime+=timestep;
		const float delay = m_maxStep*float(m_maxFPSFrames) - m_lastTime;
		if(delay>0){
			SDL_Delay(int(1000.0f*delay));
		}else{
			// reset counters
			m_maxFPSFrames=0;
			m_lastTime=0.0f;
		}	
	}
}

void
FPS::setMaxFPS(int fps)
{
	if(fps>0){
		m_maxFPSFrames=0;
		m_maxStep=1.0f/float(fps);
		m_lastTime=0.0f;
	}else{
		// no max fps
		m_maxStep=0.0f;
	}
}
