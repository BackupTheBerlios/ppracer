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

#ifndef _PPOGL_RESOURCEMGR_H_
#define _PPOGL_RESOURCEMGR_H_

#include "errors.h"
#include "refptr.h"

#include <map>
#include <string>

namespace ppogl{
	
/// A template for building resource managers	
template <class T>
class ResourceMgr
{
public:
	/// returns a Data pointer
	RefPtr<T> get(const std::string& binding, bool warning=true);
	void unbind(const std::string& binding);

protected:
	/// stores the data bindings
	typename std::map<std::string, RefPtr<T> > m_bindings;

};

template <class T> RefPtr<T> 
ResourceMgr<T>::get(const std::string& binding, bool warning)
/// returns the datareferences for the binding or a NULL-RefPtr
{
	if(binding.empty()){
		PP_WARNING("Trying to get data for emtpy binding name");
		return NULL;
	}
	
	typename std::map<std::string, RefPtr<T> >::iterator it;
	
	it = m_bindings.find(binding);
	if (it != m_bindings.end()){
		return (*it).second;
	}else if(warning){
		PP_WARNING("Cannot find data for binding: " << binding);
		return NULL;
	}else{
		return NULL;
	}
}

template <class T> void 
ResourceMgr<T>::unbind(const std::string& binding)
/// removes the binding from the internal list
{
	if(binding.empty()){
		PP_WARNING("Trying to unbind data for empty binding name");
		return;
	}
	
	typename std::map<std::string, RefPtr<T> >::iterator it;
	
	it = m_bindings.find(binding);
	if (it != m_bindings.end()){
		m_bindings.erase(it);
	}else{
		PP_WARNING("Cannot find for binding: " << binding);
		return;
	}
}

} //namepsace ppogl

#endif // _PPOGL_RESOURCEMGR_H_
