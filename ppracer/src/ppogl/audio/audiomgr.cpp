/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
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

#include "audiomgr.h"

#include "SDL_mixer.h"

namespace ppogl{
		
PPOGL_SINGLETON(AudioMgr);

AudioMgr::AudioMgr()
 : m_initialized(false)
{
}

AudioMgr::~AudioMgr()
{
	if(m_initialized){
		Mix_CloseAudio();	
	}
}

bool
AudioMgr::init(	int freq, Format format,
				bool stereo, int buffers)
{
	if(m_initialized){
		PP_WARNING("Trying to initialize AudioMgr twice");
		return false;
	}
	
	Uint16 sdl_format;
	
	switch(format){
		case FORMAT_16:
			sdl_format=AUDIO_S16SYS;
			break;
		case FORMAT_8:
			sdl_format=AUDIO_U8;
			break;
		default:
			sdl_format=AUDIO_U8;
			break;
	}
	
	// set number of channels for stereo or mono
	int channels = stereo ? 2 : 1;
	
	if(Mix_OpenAudio(freq, sdl_format, channels, buffers)<0){
		PP_WARNING("Unable to open audio device:\n"
			"Audio deviced opened:\n"
			"FQ: " << freq << "Hz\n" << format <<
			"bit " << (stereo?"Stereo":"Mono") << 
			"\n" << SDL_GetError());
		return false;
	}else{
	    PP_LOG(ppogl::LogSound,
			"Audio deviced opened:\n"
			"FQ: " << freq << "Hz\n" << format <<
			"bit " << (stereo?"Stereo":"Mono"));
	}
	
	return true;
}

bool
AudioMgr::loadMusic(const std::string &binding, const std::string &filename)
{
	MusicRef music =
		new Music(filename);
	
	m_musicBindings[binding]=music;
	
	return true;
}

bool
AudioMgr::bindMusic(const std::string &binding, const std::string &name)
{
	std::map<std::string, MusicRef>::iterator it;

	if((it=m_musicBindings.find(name))!=m_musicBindings.end()){
		m_musicBindings[binding]=(*it).second;
		return true;
	}else{
		PP_WARNING("Couldn't find music for binding: " << name);
		return false;
	}	
}
	
bool
AudioMgr::unbindMusic(const std::string &binding)
{
	std::map<std::string, MusicRef>::iterator it;
	
	it = m_musicBindings.find(binding);
	if (it != m_musicBindings.end()){
		m_musicBindings.erase(it);
		return true;
	}else{
		PP_WARNING("Cannot find for binding: " << binding);
		return false;
	}	
}

bool
AudioMgr::playMusic(const std::string &binding)
{
	std::map<std::string, MusicRef>::iterator it;

	if((it=m_musicBindings.find(binding))!=m_musicBindings.end()){
		std::map<std::string, MusicRef>::iterator it2;
		for(it2=m_musicBindings.begin();it2!=m_musicBindings.end();it2++){
			if((*it).second!=(*it2).second) (*it2).second->stop();
		}
		(*it).second->start();
		return true;
	}else{
		PP_WARNING("Couldn't find music for binding: " << binding);
		return false;
	}
}

bool 
AudioMgr::stopMusic(const std::string &binding)
{
	std::map<std::string, MusicRef>::iterator it;

	if((it=m_musicBindings.find(binding))!=m_musicBindings.end()){
		(*it).second->stop();
		return true;
	}else{
		PP_WARNING("Couldn't find music for binding: " << binding);
		return false;
	}	
}


bool
AudioMgr::loadSound(const std::string &binding, const std::string &filename)
{
	SoundRef sound =
		new Sound(filename);
	
	m_soundBindings[binding]=sound;
	
	return true;
}

bool
AudioMgr::bindSound(const std::string &binding, const std::string &name)
{
	std::map<std::string, SoundRef>::iterator it;

	if((it=m_soundBindings.find(name))!=m_soundBindings.end()){
		m_soundBindings[binding]=(*it).second;
		return true;
	}else{
		PP_WARNING("Couldn't find sound for binding: " << name);
		return false;
	}	
}
	
bool
AudioMgr::unbindSound(const std::string &binding)
{
	std::map<std::string, SoundRef>::iterator it;
	
	it = m_soundBindings.find(binding);
	if (it != m_soundBindings.end()){
		m_soundBindings.erase(it);
		return true;
	}else{
		PP_WARNING("Cannot find sound for binding: " << binding);
		return false;
	}	
}

bool
AudioMgr::playSound(const std::string &binding, int loops)
{
	std::map<std::string, SoundRef>::iterator it;

	if((it=m_soundBindings.find(binding))!=m_soundBindings.end()){
		(*it).second->start(loops);
		return true;
	}else{
		PP_WARNING("Couldn't find sound for binding: " << binding);
		return false;
	}
}

bool 
AudioMgr::stopSound(const std::string &binding)
{
	std::map<std::string, SoundRef>::iterator it;

	if((it=m_soundBindings.find(binding))!=m_soundBindings.end()){
		(*it).second->stop();
		return true;
	}else{
		PP_WARNING("Couldn't find sound for binding: " << binding);
		return false;
	}	
}

void
AudioMgr::stopAllSounds()
{
	std::map<std::string, SoundRef>::iterator it;
	
	for(it=m_soundBindings.begin(); it!=m_soundBindings.end(); it++){
		(*it).second->stop(true);		
	}
}


int
AudioMgr::sqLoadMusic(ppogl::Script *vm)
{
	std::string binding = vm->getString(1);
	std::string filename = vm->getString(2);
	
	getInstance().loadMusic(binding, filename);
	
	return 0;
}

int
AudioMgr::sqBindMusic(ppogl::Script *vm)
{
	std::string binding = vm->getString(1);
	std::string name = vm->getString(2);
		
	getInstance().bindMusic(binding, name);

	return 0;
}

int
AudioMgr::sqLoadSound(ppogl::Script *vm)
{
	std::string binding = vm->getString(1);
	std::string filename = vm->getString(2);
	
	getInstance().loadSound(binding, filename);
	
	return 0;
}

int
AudioMgr::sqBindSound(ppogl::Script *vm)
{
	std::string binding = vm->getString(1);
	std::string name = vm->getString(2);
	
	getInstance().bindSound(binding, name);

	return 0;
}

} //namepsace ppogl
