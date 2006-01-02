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
 
#ifndef _PPOGL_TRANSLATION_H_
#define _PPOGL_TRANSLATION_H_

#include <string>
#include <map>

#include "ppogl_script.h"

#include "base/singleton.h"

namespace ppogl{

/// Class for handling translations
class Translation : public Singleton<Translation>
{
public:
	void set(const std::string& language, const std::string& name);
	const std::string& get(const std::string& string);

	void addLanguage(const std::string& language, const std::string& name);
    std::map<std::string, std::string> getLanguages(){return m_languages;}

	void clear(){m_translations.clear();};
	
	static int sqSet(ppogl::Script *vm);
	static int sqGet(ppogl::Script *vm);
	
	static int sqAddLanguage(ppogl::Script *vm);
	
private:	
	
	/// the map for storing the translation strings
	std::map<std::string, std::string> m_translations;	
		
	/// the map for storing the languages
	std::map<std::string, std::string> m_languages;
};

} //namespace ppogl

///a gettext like macro that returns the translated string
#define _(string) ppogl::Translation::getInstance().get(string)

#endif // _PPOGL_TRANSLATION_H_
