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
 
/*
 * A simple assertion system.
 * 
 * An instance of the Assertion class is thrown from PP_ASSERTION 
 * This should be catched in main().
 */ 
 
#include "assert.h"

#include <iostream>
#include <stdio.h>

namespace ppogl {

Assertion::Assertion(Type type, const std::string& reason, 
				const std::string& file, int line,
				const std::string& description)
 : m_type(type),
   m_reason(reason),
   m_file(file),
   m_line(line),
   m_description(description)
{
	printMessage();
}

/// Send an error message to std::cerr
void
Assertion::printMessage()
{
	std::cerr << "Assertion failed: " << m_reason << std::endl;
	std::cerr << "File: " << m_file << std::endl;
	std::cerr << "Line: " << m_line << std::endl;
	std::cerr << "Description: " << m_description << "\n" << std::endl;
}

} //namespace ppogl
