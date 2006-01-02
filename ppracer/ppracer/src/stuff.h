/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * Copyright (C) 1999-2001 Jasmin F. Patry
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

#ifndef _STUFF_H_
#define _STUFF_H_

#include "ppogl/base/defs.h"
#include "ppogl/base/vec3d.h"

#include <string>

#if defined(WIN32)
#  define CONFIG_DIR "config"
#else
#  define CONFIG_DIR ".ppracer"
#endif // defined(WIN32)

#define CONFIG_FILE "config.nut"


#ifndef DATA_DIR
#  if defined(WIN32)
#    define DATA_DIR "."
#  else
#    define DATA_DIR PP_DATADIR
#  endif // defined(WIN32)
#endif

ppogl::Vec3d projectIntoPlane(const ppogl::Vec3d& nml, const ppogl::Vec3d& v);

void getTimeComponents(	float time,
						int& minutes,
						int& seconds,
			  			int& hundredths );

std::string get_config_dir_name();
std::string get_config_file_name();
void write_config_file();

#endif // _STUFF_H_
