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

#include "sound.h"

#ifdef USE_SDL_MIXER

namespace ppogl{
		
Sound::Sound(const std::string &filename)
 : m_sound(NULL),
   m_channel(-1),
   m_counter(0)
{
	PP_REQUIRE(filename.empty()==false, "Cannot create sound from empty filename");
	
	m_sound=Mix_LoadWAV(filename.c_str());
	if(!m_sound){
    	PP_WARNING("Unable to load sound: " << filename << "\n" << Mix_GetError());
	}
}

Sound::~Sound()
{
	PP_REQUIRE(m_counter>=0,"Playing counter may not be smaller than 0");
	
	if(m_sound){
		if(m_counter>0){
			Mix_HaltChannel(m_channel);
		}	
		Mix_FreeChunk(m_sound);
	}
}

void
Sound::start(int loops)
///Start playing the sound for the specified loops.
///Use -1 for endless playback
{	
	PP_REQUIRE(m_counter>=0,"Playing counter may not be smaller than 0");

	if(m_sound){
		if(m_counter<1){
			if((m_channel=Mix_PlayChannel(-1, m_sound, loops))==-1) {
    			PP_WARNING("Unable to start sound: " << Mix_GetError());
			}
		}
		if(loops<0){
			m_counter++;
		}
	}else{
		PP_PEDANTIC("Unable to start not loaded sound");	
	}
}
	
void
Sound::stop(bool hard)
///stops the playback
{
	PP_REQUIRE(m_counter>=0,"Playing counter may not be smaller than 0");

	if(!m_sound){
		PP_PEDANTIC("Unable to stop not loaded sound");
		return;
	}
	
	if(hard && m_counter>0){
		m_counter=0;
		Mix_HaltChannel(m_channel);
	}else if(m_counter==1){
		m_counter=0;
		Mix_HaltChannel(m_channel);
	}else if(m_counter>1){
		m_counter--;
	}
}

} // namespace ppogl

#else
#ifdef USE_OPENAL

/// Position of the source sound
static ALfloat s_sourcePos[] = { 0.0, 0.0, 0.0 };

/// Velocity of the source sound
static ALfloat s_sourceVel[] = { 0.0, 0.0, 0.0 };

namespace ppogl{
		
Sound::Sound(const std::string &filename, bool loop)
 : m_buffer(0),
   m_source(0),
   m_channel(-1),
   m_counter(0)
{
	PP_REQUIRE(filename.empty()==false, "Cannot create sound from empty filename");
			
	m_buffer = alutCreateBufferFromFile(filename.c_str());
	
	if(alGetError() != AL_NO_ERROR){
		PP_WARNING("Unable to load sound: " << filename);
		return;
	}
	
	
	alGenSources(1, &m_source);

	if(alGetError() != AL_NO_ERROR){
		PP_WARNING("Unable to load sound5: " << filename);
		return;
	}

	alSourcei(m_source, AL_BUFFER, m_buffer);
	alSourcef(m_source, AL_PITCH, 1.0);
	alSourcef(m_source, AL_GAIN, 1.0);
	alSourcefv(m_source, AL_POSITION, s_sourcePos);
	alSourcefv(m_source, AL_VELOCITY, s_sourceVel);
	alSourcei(m_source, AL_LOOPING,  loop);

	if(alGetError() != AL_NO_ERROR){
		PP_WARNING("Unable to load sound6: " << filename << ": "<< alGetString(alGetError()) );
		return;
	}
}

Sound::~Sound()
{
	PP_REQUIRE(m_counter>=0, "Playing counter may not be smaller than 0");
		
	if(m_buffer){
		alDeleteBuffers(1, &m_buffer);
	}
	
	if(m_source){
		alDeleteSources(1, &m_source);
	}
}

void
Sound::start(int loops)
///Start playing the sound for the specified loops.
///Use -1 for endless playback
{	
	PP_REQUIRE(m_counter>=0,"Playing counter may not be smaller than 0");

	alSourcePlay(m_source);
}
	
void
Sound::stop(bool hard)
///stops the playback
{
	PP_REQUIRE(m_counter>=0,"Playing counter may not be smaller than 0");

	alSourceStop(m_source);
}

} //namepsace ppogl

#endif //HAVE_OPENAL
#endif //HAVE_SDL_MIXER
