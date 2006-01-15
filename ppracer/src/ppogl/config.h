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
	
	void init(ppogl::Script *vm, const std::string& table="ppconfig");
	
	double getDouble(const std::string& key);
	int getInt(const std::string& key);
	std::string getString(const std::string& key);
	bool getBool(const std::string& key);

	void setDouble(const std::string& key, double value);
	void setInt(const std::string& key, int value);
	void setString(const std::string& key, const std::string& value);
	void setBool(const std::string& key, bool value);

private:
	ppogl::Script *m_vm;
	std::string m_table;
};

} //namepsace ppogl

#endif // _PPOGL_CONFIG_H_
