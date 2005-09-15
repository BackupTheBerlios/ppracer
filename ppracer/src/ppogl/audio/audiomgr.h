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

#ifndef _PPOGL_AUDIOMGR_H_
#define _PPOGL_AUDIOMGR_H_

#include "../base/singleton.h"
#include "../ppogl_script.h"
#include "music.h"
#include "sound.h"

#include <map>
#include <string>
#include <SDL.h>

namespace ppogl{

/// The audio manager singleton
class AudioMgr
 : public Singleton<AudioMgr>
{
private:
	bool m_initialized;	
	
	std::map<std::string, MusicRef> m_musicBindings;
	std::map<std::string, SoundRef> m_soundBindings;

public:
	enum{
		FQ_11 = 11025,
	    FQ_22 = 22050,
	    FQ_44 = 44100	
	};
	
	enum Format{
		FORMAT_8 = 8,
	    FORMAT_16 = 16	
	};
	
	AudioMgr();
	~AudioMgr();

	bool init(int freq, Format format, bool stereo=true, int buffers=2048);

	bool loadMusic(const std::string &binding, const std::string &filename);
	bool bindMusic(const std::string &binding, const std::string &name);
	bool unbindMusic(const std::string &binding);
	
	bool playMusic(const std::string &binding);
	bool stopMusic(const std::string &binding);
	
	bool loadSound(const std::string &binding, const std::string &filename);
	bool bindSound(const std::string &binding, const std::string &name);
	bool unbindSound(const std::string &binding);
	
	bool playSound(const std::string &binding, int loops=-1);
	bool stopSound(const std::string &binding);
	void stopAllSounds();
		
	// callbacks for squirrel
	static int sqLoadMusic(ppogl::Script *vm);
	static int sqBindMusic(ppogl::Script *vm);

	static int sqLoadSound(ppogl::Script *vm);
	static int sqBindSound(ppogl::Script *vm);
};

} //namepsace ppogl

#endif // _PPOGL_AUDIOMGR_H_
