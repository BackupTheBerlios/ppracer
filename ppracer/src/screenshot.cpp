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

#include "ppracer.h"
#include "screenshot.h"
#include "ppogl/base/os.h"
#include "ppogl/images/image.h"
#include "ppogl/base/glwrappers.h"

#include <sstream>

static int screenshot_num = 0;

bool
take_screenshot(const std::string& filename)
{
	int viewport[4];
	gl::GetValue(GL_VIEWPORT, viewport);
	gl::ReadBuffer(GL_FRONT);
	
	ppogl::Image image(viewport[2],viewport[3],3);
	
	image.width=viewport[2];
	image.height=viewport[3];
	image.depth=3;
	
	for (int i=0; i<viewport[3]; i++){
		gl::ReadPixels(viewport[0], viewport[1]+viewport[3]-1-i,
			viewport[2], 1, GL_RGB, 
			GL_UNSIGNED_BYTE, image.data+viewport[2]*i*3
			);
	}
	
	return image.writeToFile(filename);
}

void
screenshot()
{
    screenshot_num++;
		
	std::stringstream temp(std::ios::in| std::ios::out | std::ios::trunc);

	temp << ppogl::os::getBaseDir() 
		 << PP_DIR_SEPARATOR
	     << screenshot_num
		 << ".ppm";

	if(!take_screenshot(temp.str())){
        PP_WARNING("Couldn't save screenshot");
    };
}
