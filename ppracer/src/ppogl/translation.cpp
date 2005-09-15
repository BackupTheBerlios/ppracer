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
 
#include "translation.h"

#include <iostream>

namespace ppogl{
	
PPOGL_SINGLETON(Translation);

/// return translation for string
const std::string&
Translation::get(const std::string& string)
{
	std::map<std::string, std::string>::iterator it;
	
	if((it=m_translations.find(string))!=m_translations.end()){
		return (*it).second;
	}else{
		return string;
	}	
}

/// set translation string
void 
Translation::set(const std::string& string, const std::string& translation)
{
	m_translations[string]=translation;
}

/// set language name
void 
Translation::addLanguage(const std::string& language, const std::string& name)
{
	m_languages[language]=name;
}

int
Translation::sqSet(ppogl::Script *vm)
{
	std::string string = vm->getString(1);
	std::string translation = vm->getString(2);

	getInstance().set(string, translation);	
	return 0;
}

int
Translation::sqGet(ppogl::Script *vm)
{
	std::string string = vm->getString(1);
	std::string translation = getInstance().get(string);
	
	vm->pushString(translation);
	return 1;
}

int
Translation::sqAddLanguage(ppogl::Script *vm)
{
	std::string language = vm->getString(1);
	std::string name = vm->getString(2);
		
	getInstance().addLanguage(language, name);

	return 0;
}



} // namespace ppogl
