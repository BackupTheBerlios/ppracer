/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005-2006 Volker Stroebel <volker@planetpenguin.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
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

#include "music.h"

#ifdef HAVE_SDL_MIXER

namespace ppogl{
	
Music::Music(const std::string &filename)
 : m_music(NULL),
   m_playing(false)
{
	PP_REQUIRE(filename.empty()==false, "Cannot create music from empty filename");
	
	m_music=Mix_LoadMUS(filename.c_str());
	if(!m_music){
    	PP_WARNING("Unable to load music: " << filename << "\n" << Mix_GetError());
	}
}

Music::~Music()
{
	if(m_music){
		Mix_HaltMusic();
		Mix_FreeMusic(m_music);
	}
}

bool
Music::start()
///start playing music
{
	if(!m_playing){
		if(m_music){
			if(Mix_PlayMusic(m_music, -1)==-1) {
    			PP_WARNING("Unable to start music: " << Mix_GetError());
			}
		}else{
			PP_PEDANTIC("Unable to start not loaded music");
		}
		m_playing=true;
		return true;
	}else{
		return false;
	}
}
	
bool
Music::stop()
///stop playing music
{
	if(m_playing){	
		Mix_HaltMusic();
		m_playing=false;
		return true;
	}else{
		return false;
	}
}


} //namepsace ppogl

#endif // HAVE_SDL_MIXER
