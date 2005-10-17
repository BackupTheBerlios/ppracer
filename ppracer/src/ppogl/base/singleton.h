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
 
#ifndef _PPOGL_SINGLETON_H_
#define _PPOGL_SINGLETON_H_

#include "assert.h"

#include <memory>

namespace ppogl{

/// Template class for creating singletons
template <class T>
class Singleton
{
public:
	static T&
	getInstance() throw(std::bad_exception)
	/// create the instance if necessary and returns a reference of the instance
	{
		
		T* temp(sm_instance.get());
    	if(temp==0){
			temp=new T;
			PP_ENSURE(temp!=NULL,"Unable to create singleton instance")
        	sm_instance.reset(temp);
		}
		return *temp;
	}
	
protected:
	Singleton(){};
	virtual ~Singleton(){};
		
private:
//	static T* sm_instance;
	static std::auto_ptr<T> sm_instance;
};

} //namespace ppogl

/// creates the static auto pointer for the instance that is necessary for each singleton
#define PPOGL_SINGLETON(T) template <> std::auto_ptr<T> ppogl::Singleton<T>::sm_instance(0)

#endif // _PPOGL_SINGLETON_H_
