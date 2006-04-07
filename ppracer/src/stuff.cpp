/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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
 
#include "stuff.h" 
#include "SDL.h"
#include "ppracer.h"

#include "ppogl/base/errors.h"
#include "ppogl/base/os.h"

#include <fstream>
 
ppogl::Vec3d
projectIntoPlane(const ppogl::Vec3d& nml, const ppogl::Vec3d& v)
{
	return v-nml*v*nml;
}

void
getTimeComponents(	float time, 
					int& minutes,
					int& seconds,
					int& hundredths )
{
    minutes = int(time / 60);
    seconds = int(time) % 60;
    hundredths = int(time * 100 + 0.5) % 100;
}

std::string
get_config_dir_name()
{
#if defined(WIN32) 
	return CONFIG_DIR;
#else
	const std::string& dir = ppogl::os::getUserDir();	
	if(!ppogl::os::isDirectory(dir)){
		if(!ppogl::os::mkdir(dir)){
			PP_WARNING("Unable to create config directory: " << dir);
		}
	}
	return dir;	
#endif // defined(WIN32)
}

std::string
get_config_file_name()
{
    std::string dir = get_config_dir_name();
	dir += CONFIG_FILE;
    return dir;
}

// defined in main.cpp <- Todo: move this somewhere else
extern std::string cfile;

bool saveConfigFile = true;

/*
 * my_write_to_cfile and cfilelib is only a temporal sollution.
 * Should be replaced with a more powerful and secure class for file io.
 */
static int
my_write_to_cfile(ppogl::Script *vm)
{
	std::ofstream *stream = reinterpret_cast<std::ofstream*>(vm->getUserPointer(1));
	std::string string = vm->getString(2);
	
	PP_CHECK_POINTER(stream);

	*stream << string;

	return 0;
}

static const struct ppogl::Script::Lib cfilelib[]={
	{"write", my_write_to_cfile},
	{NULL, NULL}
};

void register_cfile_callbacks()
{
	script.registerLib("cfile", cfilelib);
}

void
write_config_file()
{
	PP_REQUIRE(cfile.empty()!=true, "Filename for configuration file is empty");

	if(saveConfigFile){	
		PP_MESSAGE("Writing config file: " <<cfile); 
		
		std::ofstream stream(cfile.c_str());
		if(stream.is_open()){		
			//call script function "write_config_to_file" with argument "stream"
			script.pushRootTable();
			script.pushString("write_config_to_file");
			script.get(-2);
			script.pushRootTable();
		
			script.pushUserPointer(&stream);
		
			script.call(2,false);
			script.pop(2);
		}else{
			PP_WARNING("Unable to write config file: " << cfile);
		}
	}else{
		PP_MESSAGE("Writing config file is disabled"); 
	}
}
