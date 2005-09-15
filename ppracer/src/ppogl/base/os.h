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
 
#ifndef _PPOGL_OS_H
#define _PPOGL_OS_H

#include <string>
#include <list>

namespace ppogl {
	
	
///OS dependant functionality
namespace os {

bool chdir(const std::string& dir);
	
std::string cwd();

bool mkdir(const std::string& dir);	
	
/// A class for handling directory content
class dir : public std::list<std::string>
{
public:
	dir(std::string path, const bool only_dirs=false);	
};

bool isFile(const std::string& name, const bool follow_link=false);
bool isDirectory(const std::string& name, const bool follow_link=false);


} // namespace os
} // namespace ppogl

#endif // _PPOGL_OS_H
