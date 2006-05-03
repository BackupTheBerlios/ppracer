/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2006 Volker Stroebel <volker@planetpenguin.de>
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
 
#include "vorbis.h"

#include <AL/alext.h>

#include "../base/os.h"

// define AL_FORMAT_VORBIS_EXT if it's not in alext.h and hope that it works
#ifndef AL_FORMAT_VORBIS_EXT
	#define AL_FORMAT_VORBIS_EXT 0x10003
#endif

namespace ppogl{

ALuint
Vorbis::CreateBufferFromFile(const std::string& filename)
{
	ALuint buffer = AL_NONE; 
	alGenBuffers(1, &buffer);

	ppogl::os::File file(filename);
	if(file){
		alBufferData(buffer, AL_FORMAT_VORBIS_EXT, file.getData(), file.getSize(), 1);
	}
	
	return buffer;
}

} //namepsace ppogl
