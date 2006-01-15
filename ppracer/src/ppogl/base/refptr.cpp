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
 
#include "refptr.h"
#include "assert.h"

namespace ppogl{
	
RefObject::RefObject()
 : m_references(0)
{		
}

RefObject::~RefObject()
{
}

void
RefObject::reference()
///increse the reference counter
{
	PP_REQUIRE(m_references>=0,"Reference counter has illegal value: " << m_references << " : " << int(this));
	m_references++;
}

void
RefObject::unreference()
///decrease reference counter and delete object if no references are left
{	
	PP_REQUIRE(m_references>0,"Unable to unreference: " << m_references << " : " << int(this));
	m_references--;
	if(m_references<=0){
		delete this;
	}
}

} // namspace ppogl
