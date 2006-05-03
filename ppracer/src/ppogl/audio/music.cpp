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

#ifdef USE_SDL_MIXER

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

#else
#ifdef USE_OPENAL

#include "vorbis.h"

/// Position of the source sound
static ALfloat s_sourcePos[] = { 0.0, 0.0, 0.0 };

/// Velocity of the source sound
static ALfloat s_sourceVel[] = { 0.0, 0.0, 0.0 };

namespace ppogl{
	
Music::Music(const std::string &filename)
 : m_buffer(0),
   m_source(0),
   m_playing(false)
{
	PP_REQUIRE(filename.empty()==false, "Cannot create music from empty filename");
	
	ALboolean loop = AL_TRUE;
	
	m_buffer = ppogl::Vorbis::CreateBufferFromFile(filename.c_str());
	
	if(m_buffer == AL_NONE){
		PP_WARNING("Unable to load sound1: " << filename);
		return;
	}
	
	alGenSources(1, &m_source);

	if(alGetError() != AL_NO_ERROR){
		PP_WARNING("Unable to load sound: " << filename);
		return;
	}

	alSourcei(m_source, AL_BUFFER, m_buffer);
	alSourcef(m_source, AL_PITCH, 1.0);
	alSourcef(m_source, AL_GAIN, 1.0);
	alSourcefv(m_source, AL_POSITION, s_sourcePos);
	alSourcefv(m_source, AL_VELOCITY, s_sourceVel);
	alSourcei(m_source, AL_LOOPING,  loop);

	if(alGetError() != AL_NO_ERROR){
		PP_WARNING("Unable to load sound: " << filename << ": "<< alGetString(alGetError()) );
		return;
	}
}

Music::~Music()
{
	stop();
	if(m_buffer){
		alDeleteBuffers(1, &m_buffer);
	}
	
	if(m_source){
		alDeleteSources(1, &m_source);
	}
}

bool
Music::start()
///start playing music
{
	if(!m_playing){
		alSourcePlay(m_source);
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
		alSourceStop(m_source);
		m_playing=false;
		return true;
	}else{
		return false;
	}
}

} //namepsace ppogl

#endif // USE_OPENAL
#endif // USE_SDL_MIXER
