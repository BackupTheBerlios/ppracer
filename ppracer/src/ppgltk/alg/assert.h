/*
 * PPGLTK
 *
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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
 
#ifndef _PP_ASSERT_H
#define _PP_ASSERT_H

#include <stdlib.h>

#include "errors.h"


namespace pp {
	
class Assertion : public pp::Error
{
public:
	
	/// The available types of the assertions
	enum Type{
		Assert,
		Require,
		Ensure,
		CheckPointer
	};
	
	Assertion(Type type, const char* reason,
			  const char* file, int line, const char* description, ...);
	virtual ~Assertion();
	
	
	void printMessage();
		
private:
	
	/// the type of the assertion 
	Type m_type;
	
	/// a string that holds the type and expression
	const char* m_reason;
	
	/// the file from which the assertion gets thrown
	const char* m_file;

	/// the line in the file the assertion gets thrown
	int m_line;

	/// the description of the assertion
	char* m_description;

};	

} //namespace pp

#define PP_ASSERTION(type, expression, ...)	\
throw pp::Assertion(pp::Assertion::type, #type"("expression")", __FILE__, __LINE__, __VA_ARGS__)


#ifndef PPGLTK_NO_ASSERT

	/// A normal assertion
	#define PP_ASSERT(expression, ...)	\
		if(!(expression)) {PP_ASSERTION(Assert, #expression,  __VA_ARGS__);}else{;}	

	/// A precondition
	#define PP_REQUIRE(expression, ...)	\
		if(!(expression)) {PP_ASSERTION(Require, #expression, __VA_ARGS__);}else{;}
	
	/// A postcondition
	#define PP_ENSURE(expression, ...)	\
		if(!(expression)) {PP_ASSERTION(Ensure, #expression,  __VA_ARGS__);}else{;}	

	/// Ensures the pointer is not NULL			
	#define PP_CHECK_POINTER(ptr)	\
		if(ptr==NULL) {PP_ASSERTION(CheckPointer, "pointer!=NULL", NULL);}else{;}

#else
	#define PP_ASSERT(expression, ...)
	#define PP_REQUIRE(expression, ...)
	#define PP_ENSURE(expression, ...)
	#define PP_IMPLIES(expression1, expression2, ...)	
	#define PP_CHECK_POINTER(ptr)
#endif


#endif // _PP_ASSERT_H
