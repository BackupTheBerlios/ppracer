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
 
#ifndef _PPOGL_ASSERT_H_
#define _PPOGL_ASSERT_H_

#include <sstream>

#include "errors.h"

namespace ppogl {
	
/// An error class that gets thrown from assertion exceptions
class Assertion : public ppogl::Error
{
public:
	/// The available types for assertions
	enum Type{
		Assert,
		Require,
		Ensure,
		CheckPointer,
		CheckAlloc
	};
	
	Assertion(Type type, const std::string& reason,
			  const std::string& file, int line, const std::string& description);
	
	void printMessage();
	
private:
	
	/// the type of the assertion 
	Type m_type;
	
	/// a string that holds the type and expression
	const std::string m_reason;
	
	/// the file from which the assertion gets thrown
	const std::string m_file;

	/// the line in the file the assertion gets thrown
	int m_line;

	/// the description of the assertion
	std::string m_description;
};

} //namespace pp


#define PP_ASSERTION(type, expression,description)	\
{	\
	std::stringstream tmp(std::ios::in| std::ios::out | std::ios::trunc);	\
	tmp << description;	\
	throw ppogl::Assertion(ppogl::Assertion::type, #type"("expression")", __FILE__, __LINE__, tmp.str());	\
}	

#ifdef PPOGL_ENABLE_ASSERT

	/// A generic assertion
	#define PP_ASSERT(expression, description)	\
		if(!(expression)) {PP_ASSERTION(Assert, #expression, description);}else{;}	

	/// A precondition
	#define PP_REQUIRE(expression, description)	\
		if(!(expression)) {PP_ASSERTION(Require, #expression, description);}else{;}
	
	/// A postcondition
	#define PP_ENSURE(expression, description)	\
		if(!(expression)) {PP_ASSERTION(Ensure, #expression, description);}else{;}	

	/// Ensures the pointer is not NULL			
	#define PP_CHECK_POINTER(ptr)	\
		if(ptr==NULL) {PP_ASSERTION(CheckPointer, "pointer!=NULL", "");}else{;}

	/// Ensures the pointer returned from an memory allocation is not NULL			
	#define PP_CHECK_ALLOC(ptr)	\
		if(ptr==NULL) {PP_ASSERTION(CheckAlloc, "Memeroy allocation returned NULL", "");}else{;}	

#else
	#define PP_ASSERT(expression, description)
	#define PP_REQUIRE(expression, description)
	#define PP_ENSURE(expression, description)
	#define PP_CHECK_POINTER(ptr)
	#define PP_CHECK_ALLOC(ptr)
#endif

#endif // _PPOGL_ASSERT_H_
