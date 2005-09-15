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

#ifndef _PPOGL_CONFIG_H_
#define _PPOGL_CONFIG_H_

#include "base/singleton.h"
#include <string>
#include <map>

namespace ppogl{
	
class Script;

/// The singleton for handling the configuration
class Config : public Singleton<Config>
{
public:
	Config();
	
	void init(ppogl::Script *vm, std::string table="ppconfig");
	
	double getDouble(std::string key);
	int getInt(std::string key);
	std::string getString(std::string key);
	bool getBool(std::string key);

	void setDouble(std::string key, double value);
	void setInt(std::string key, int value);
	void setString(std::string key, std::string value);
	void setBool(std::string key, bool value);

private:
	ppogl::Script *m_vm;
	std::string m_table;
};

} //namepsace ppogl

#endif // _PPOGL_CONFIG_H_
