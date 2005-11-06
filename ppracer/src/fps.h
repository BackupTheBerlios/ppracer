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

#ifndef _FPS_H_
#define _FPS_H_

class FPS
{
	unsigned int m_frames;
	float m_time;
	float m_fps;
	
	// members used for the max fps stuff
	float m_maxStep;
	float m_lastTime;
	unsigned int m_maxFPSFrames;
	
public:
	FPS();

	void update(float timestep);
	float get(){return m_fps;};
	
	void setMaxFPS(int fps);
};

extern FPS fpsCounter;

#endif //_FPS_H
