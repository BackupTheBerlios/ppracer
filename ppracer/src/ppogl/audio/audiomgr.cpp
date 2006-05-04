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

#include "audiomgr.h"

PPOGL_SINGLETON(ppogl::AudioMgr);

#ifdef USE_SDL_MIXER

#include "SDL_mixer.h"

namespace ppogl{
	
AudioMgr::AudioMgr()
 : m_initialized(false),
   m_soundEnabled(true),
   m_musicEnabled(true)
{
}

AudioMgr::~AudioMgr()
{
	//free music and sounds before Mix_CloseAudio() is called
	m_musicBindings.clear();
	m_soundBindings.clear();
		
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
		PP_WARNING("Unable to open audio device: " 
			"FQ " << freq << "Hz " << format <<
			"bit " << (stereo?"Stereo":"Mono") << 
			"\nError: " << Mix_GetError() );
				
		PP_WARNING("Lets give it another try");
		
		// try to open the device with save default settings
		freq=11025;
		sdl_format=AUDIO_U8;
		format = FORMAT_8;
		channels=1;
		stereo=false;
			
		if(Mix_OpenAudio(freq, sdl_format, channels, buffers)<0){
			PP_WARNING("Unable to open audio device: " 
			"FQ " << freq << "Hz " << format <<
			"bit " << (stereo?"Stereo":"Mono") << 
			"\nDisabling Audio: " << Mix_GetError());
			return false;
		}else{
			PP_WARNING("Audio deviced opened:" 
			"FQ " << freq << "Hz " << format <<
			"bit " << (stereo?"Stereo":"Mono") << "\n" <<
			"Please change the audio configuration");
		
			m_initialized=true;
			return true;
		}
	}else{
	    PP_LOG(ppogl::LogAudio,
			"Audio deviced opened:"  
			"FQ " << freq << "Hz " << format <<
			"bit " << (stereo?"Stereo":"Mono"));
		
		m_initialized=true;
		return true;
	}
}

void
AudioMgr::enableSound(bool enable)
{
	if(!enable){
		stopAllSounds();
	}	
	m_soundEnabled=enable;
}

void
AudioMgr::enableMusic(bool enable)
{
	if(!enable){
		stopAllMusic();
	}	
	m_musicEnabled=enable;
}

void
AudioMgr::setMusicVolume(int volume)
{
	if(!m_initialized) return;
	
	if(volume<0) volume=0;
	else if(volume>100) volume=100;
	volume = MIX_MAX_VOLUME * volume/100;
	
	Mix_VolumeMusic(volume);
}

void
AudioMgr::setSoundVolume(int volume)
{
	if(!m_initialized) return;
	
	if(volume<0) volume=0;
	else if(volume>100) volume=100;
	volume = MIX_MAX_VOLUME * volume/100;
	
	Mix_Volume(-1,volume);
}

bool
AudioMgr::loadMusic(const std::string &binding, const std::string &filename)
{
	if(!m_initialized) return true;
	
	PP_LOG(LogAudio,"Loading music: " << binding << " -> " << filename);
	
	MusicRef music = new Music(filename);
	
	m_musicBindings[binding]=music;
	
	return true;
}

bool
AudioMgr::bindMusic(const std::string &binding, const std::string &name)
{
	if(!m_initialized) return true;
	
	PP_LOG(LogAudio,"Binding music: " << binding << " -> " << name);

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
	if(!m_initialized) return true;

	PP_LOG(LogAudio,"Unbinding music: " << binding );
	
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
	if(!m_initialized || !m_musicEnabled) return true;

	
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
	if(!m_initialized || !m_musicEnabled) return true;
	
	std::map<std::string, MusicRef>::iterator it;

	if((it=m_musicBindings.find(binding))!=m_musicBindings.end()){
		(*it).second->stop();
		return true;
	}else{
		PP_WARNING("Couldn't find music for binding: " << binding);
		return false;
	}	
}

void
AudioMgr::stopAllMusic()
{
	if(!m_initialized) return;
	
	std::map<std::string, MusicRef>::iterator it;
	
	for(it=m_musicBindings.begin(); it!=m_musicBindings.end(); it++){
		(*it).second->stop();		
	}
}

void
AudioMgr::pauseMusic(bool status)
{
	if(!m_initialized && !m_musicEnabled) return;
	
	if(status){
		Mix_PauseMusic();
	}else{
		Mix_ResumeMusic();
	}
}

bool
AudioMgr::loadSound(const std::string &binding, const std::string &filename)
{
	if(!m_initialized) return true;

	PP_LOG(LogAudio,"Loading sound: " << binding << " -> " << filename);
	
	SoundRef sound =
		new Sound(filename);
	
	m_soundBindings[binding]=sound;
	
	return true;
}

bool
AudioMgr::bindSound(const std::string &binding, const std::string &name)
{
	if(!m_initialized) return true;
	
	PP_LOG(LogAudio,"Binding sound: " << binding << " -> " << name);
	
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
	if(!m_initialized) return true;
	
	PP_LOG(LogAudio,"Unbinding sound: " << binding);
	
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
	if(!m_initialized || !m_soundEnabled) return true;
	
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
	if(!m_initialized || !m_soundEnabled) return true;
	
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
	if(!m_initialized || !m_soundEnabled) return;
	
	std::map<std::string, SoundRef>::iterator it;
	
	for(it=m_soundBindings.begin(); it!=m_soundBindings.end(); it++){
		(*it).second->stop(true);		
	}
}

int
AudioMgr::sqLoadMusic(ppogl::Script *vm)
{
	std::string binding = vm->getStringFromTable("name");
	std::string filename = vm->getStringFromTable("file");
	
	getInstance().loadMusic(binding, filename);
	
	return 0;
}

int
AudioMgr::sqBindMusic(ppogl::Script *vm)
{
	std::string binding = vm->getStringFromTable("name");
	std::string name = vm->getStringFromTable("music");
		
	getInstance().bindMusic(binding, name);

	return 0;
}

int
AudioMgr::sqLoadSound(ppogl::Script *vm)
{
	std::string binding = vm->getStringFromTable("name");
	std::string filename = vm->getStringFromTable("file");
	
	getInstance().loadSound(binding, filename);
	
	return 0;
}

int
AudioMgr::sqBindSound(ppogl::Script *vm)
{
	std::string binding = vm->getStringFromTable("name");
	std::string name = vm->getStringFromTable("sound");
	
	getInstance().bindSound(binding, name);

	return 0;
}

} //namepsace ppogl

#else
#ifdef USE_OPENAL


#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

// Position of the Listener.
ALfloat s_listenerPos[] = { 0.0, 0.0, 0.0 };

// Velocity of the Listener.
ALfloat s_listenerVel[] = { 0.0, 0.0, 0.0 };

// Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
// Also note that these should be units of '1'.
ALfloat s_listenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

namespace ppogl{

PPOGL_SINGLETON(AudioMgr);
		
AudioMgr::AudioMgr()
 : m_initialized(false),
   m_soundEnabled(true),
   m_musicEnabled(true)
{
}

AudioMgr::~AudioMgr()
{
	//free music and sounds before alutExit() is called
	m_musicBindings.clear();
	m_soundBindings.clear();
		
	if(m_initialized){
		alutExit();
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
	
	alutInit(NULL, 0);
	alGetError();
		
	alListenerfv(AL_POSITION, s_listenerPos);
	alListenerfv(AL_VELOCITY, s_listenerVel);
	alListenerfv(AL_ORIENTATION, s_listenerOri);
	
	m_initialized=true;
	return true;
}

void
AudioMgr::enableSound(bool enable)
{
	if(!enable){
		stopAllSounds();
	}	
	m_soundEnabled=enable;
}

void
AudioMgr::enableMusic(bool enable)
{
	if(!enable){
		stopAllMusic();
	}	
	m_musicEnabled=enable;
}

void
AudioMgr::setMusicVolume(int volume)
{
	if(!m_initialized) return;
	
	/*
	if(volume<0) volume=0;
	else if(volume>100) volume=100;
	volume = MIX_MAX_VOLUME * volume/100;
	
	Mix_VolumeMusic(volume);
	*/
}

void
AudioMgr::setSoundVolume(int volume)
{
	if(!m_initialized) return;
	
	/*
	if(volume<0) volume=0;
	else if(volume>100) volume=100;
	volume = MIX_MAX_VOLUME * volume/100;
	
	Mix_Volume(-1,volume);
	*/
}

bool
AudioMgr::loadMusic(const std::string &binding, const std::string &filename)
{
	if(!m_initialized) return true;
	
	PP_LOG(LogAudio,"Loading music: " << binding << " -> " << filename);
	
	MusicRef music = new Music(filename);
	
	m_musicBindings[binding]=music;
	
	return true;
}

bool
AudioMgr::bindMusic(const std::string &binding, const std::string &name)
{
	if(!m_initialized) return true;
	
	PP_LOG(LogAudio,"Binding music: " << binding << " -> " << name);

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
	if(!m_initialized) return true;

	PP_LOG(LogAudio,"Unbinding music: " << binding );
	
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
	if(!m_initialized || !m_musicEnabled) return true;

	
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
	if(!m_initialized || !m_musicEnabled) return true;
	
	std::map<std::string, MusicRef>::iterator it;

	if((it=m_musicBindings.find(binding))!=m_musicBindings.end()){
		(*it).second->stop();
		return true;
	}else{
		PP_WARNING("Couldn't find music for binding: " << binding);
		return false;
	}	
}

void
AudioMgr::stopAllMusic()
{
	if(!m_initialized) return;
	
	std::map<std::string, MusicRef>::iterator it;
	
	for(it=m_musicBindings.begin(); it!=m_musicBindings.end(); it++){
		(*it).second->stop();		
	}
}

void
AudioMgr::pauseMusic(bool status)
{
	if(!m_initialized && !m_musicEnabled) return;
	
	/*
	if(status){
		Mix_PauseMusic();
	}else{
		Mix_ResumeMusic();
	}
	*/
	PP_WARNING("AudioMgr::pauseMusic: not implemented");
}

bool
AudioMgr::loadSound(const std::string &binding, const std::string &filename, bool loop)
{
	if(!m_initialized) return true;

	PP_LOG(LogAudio,"Loading sound: " << binding << " -> " << filename);
	
	SoundRef sound =
		new Sound(filename, loop);
	
	m_soundBindings[binding]=sound;
	
	return true;
}

bool
AudioMgr::bindSound(const std::string &binding, const std::string &name)
{
	if(!m_initialized) return true;
	
	PP_LOG(LogAudio,"Binding sound: " << binding << " -> " << name);
	
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
	if(!m_initialized) return true;
	
	PP_LOG(LogAudio,"Unbinding sound: " << binding);
	
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
	if(!m_initialized || !m_soundEnabled) return true;
	
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
	if(!m_initialized || !m_soundEnabled) return true;
	
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
	if(!m_initialized || !m_soundEnabled) return;
	
	std::map<std::string, SoundRef>::iterator it;
	
	for(it=m_soundBindings.begin(); it!=m_soundBindings.end(); it++){
		(*it).second->stop(true);		
	}
}

int
AudioMgr::sqLoadMusic(ppogl::Script *vm)
{
	std::string binding = vm->getStringFromTable("name");
	std::string filename = vm->getStringFromTable("file2");
	
	getInstance().loadMusic(binding, filename);
		
	return 0;
}

int
AudioMgr::sqBindMusic(ppogl::Script *vm)
{
	std::string binding = vm->getStringFromTable("name");
	std::string name = vm->getStringFromTable("music");
			
	getInstance().bindMusic(binding, name);

	return 0;
}

int
AudioMgr::sqLoadSound(ppogl::Script *vm)
{
	std::string binding = vm->getStringFromTable("name");
	std::string filename = vm->getStringFromTable("file");
		
	bool loop = false;
	
	if(vm->isKeyInTable("loop")){
		loop = vm->getBoolFromTable("loop");
	}	
	
	getInstance().loadSound(binding, filename, loop);
		
	return 0;
}

int
AudioMgr::sqBindSound(ppogl::Script *vm)
{
	std::string binding = vm->getStringFromTable("name");
	std::string name = vm->getStringFromTable("sound");
	
	getInstance().bindSound(binding, name);

	return 0;
}

} //namepsace ppogl

#endif //USE_OPENAL
#endif //USE_SDL_MIXER
