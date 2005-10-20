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

#ifndef _PPOGL_REFPTR_H_
#define _PPOGL_REFPTR_H_

#include "assert.h"

namespace ppogl{
	
/// An object that provides a simple reference counter
class RefObject
{
protected:
	int m_references;	
	virtual ~RefObject();
public:
	RefObject();
	void reference();
	void unreference();

	/// return the number of references
	inline int getReferences(){return m_references;};
};
	
/// A template for managing references counted objects
template <class T>
class RefPtr
{
protected:
	/// the pointer to the sored object
	T* m_pointer;

public:	
	inline RefPtr() : m_pointer(0){};
	inline RefPtr(T* ptr) : m_pointer(ptr)
	{
		if(m_pointer!=NULL){
			m_pointer->reference();
		}else{
			PP_PEDANTIC("Assinging NULL to RefPtr");
		}			
	};
	
	inline RefPtr(const RefPtr<T>& ref) : m_pointer(ref.m_pointer)
	{
		if(m_pointer!=NULL){
			m_pointer->reference();
		}else{
			PP_PEDANTIC("Asinging NULL to RefPtr");
		}		
	}	

	inline ~RefPtr()
	{
		if(m_pointer!=NULL){
			m_pointer->unreference();			
		}
	}

	inline void swap(RefPtr<T>& ref)
	/// swaps referenced objects
	{
		T* temp = m_pointer;
  		m_pointer = ref.m_pointer;
  		ref.m_pointer = temp;	
	}
		
	inline T* operator ->() const
	///return pointer to the object
	{
		PP_CHECK_POINTER(m_pointer);
		return m_pointer;
	}	
	
	inline T& operator *() const
	///return reference of the object
	{
		PP_CHECK_POINTER(m_pointer);
		return *m_pointer;
	}	

	inline RefPtr<T> operator=(const RefPtr<T>& ref)
	///attaches an object to the refptr
	{
		//create temporary copy that increases the 
		//reference counter of the object	
		RefPtr<T> temp(ref);
		
		//swap the objects to replace the old object
  		this->swap(temp);
		
		// this has now our newly asinged object and
		// the old one (if !NULL) will be unreferenced
		// with the destruction of temp
  		return *this;		
	}
	
	inline bool isNull() const 
	{
		return (m_pointer==0);	
	}
	
	inline operator bool() const
	{
		return (m_pointer != NULL);
	}

	inline bool operator==(const RefPtr<T>& ref) const
	{
		return (m_pointer == ref.m_pointer);
	}

	inline bool operator!=(const RefPtr<T>& ref) const
	{
		return (m_pointer != ref.m_pointer);
	}
	
	inline int getRefCount()
	///return references for the object
	{
		if(m_pointer!=NULL){
			return m_pointer->getReferences();
		}else{
			//not realy necessary but may be usefull for debug broken RefObjects	
			return -65536;
		}
	}
};

} //namepsace ppogl

#endif // _PPOGL_REFPTR_H_
