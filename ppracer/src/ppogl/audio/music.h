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

#ifndef _PPOGL_MUSIC_H_
#define _PPOGL_MUSIC_H_

#include "../base/refptr.h"
	
#ifdef USE_SDL_MIXER

#include <SDL_mixer.h>
	
namespace ppogl{
	
///A class for playing a music file
class Music : public RefObject
{
private:
	///The SDL_mixer data for this music
	Mix_Music *m_music;
	
	///Whether this music is playing
	bool m_playing;

public:	
	Music(const std::string &filename);
	~Music();

	bool start();
	bool stop();
};


typedef RefPtr<Music> MusicRef;

} // namespace ppogl

#else
#ifdef USE_OPENAL

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

namespace ppogl{
	
///A class for playing a music file
class Music : public RefObject
{
private:
	///The buffer for this sound
	ALuint m_buffer;

	///The source for this sound
	ALuint m_source;
		
	///Whether this music is playing
	bool m_playing;

public:	
	Music(const std::string &filename);
	~Music();

	bool start();
	bool stop();
};

typedef RefPtr<Music> MusicRef;

} // namespace ppogl

#else
// stubs used if there is no uadio support

namespace ppogl{

class Music : public RefObject
{
public:	
	Music(const std::string &filename){};
	~Music(){};
	bool start(){return true;};
	bool stop(){return true;};
};
typedef RefPtr<Music> MusicRef;

} //namepsace ppogl

#endif // USE_OPENAL
#endif // USE_SDL_MIXER


#endif // _PPOGL_MUSIC_H_
