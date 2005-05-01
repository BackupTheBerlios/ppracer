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
	enum Type{
		Assert,
		Require,
		Ensure,
		Implies,
		CheckPointer
	};
	
	Assertion(Type type, const char* reason,
			  const char* file, int line, const char* description);

	void printMessage();
		
private:
	Type m_type;
	const char* m_reason;
	const char* m_file;
	int m_line;
	const char* m_description;

};	

} //namespace pp

#define PP_ASSERTION(type, expression, description)	\
throw pp::Assertion(pp::Assertion::type, #type"("expression")", __FILE__, __LINE__, description)


#ifndef PPGLTK_NO_ASSERT

	#define PP_ASSERT(expression, description)	\
		if(!(expression)) {PP_ASSERTION(Assert, #expression, description);}else{;}	

	#define PP_REQUIRE(expression, description)	\
		if(!(expression)) {PP_ASSERTION(Require, #expression, description);}else{;}
	
	#define PP_ENSURE(expression, description)	\
		if(!(expression)) {PP_ASSERTION(Ensure, #expression, description);}else{;}	

	#define PP_IMPLIES(expression1, expression2, description)	\
		if(!(expression1)){	\
			if(!(expression1)){PP_ASSERTION(Implies, #expression1","#expression2, description);}else{;}	\
		}else{;}
			
	#define PP_CHECK_POINTER(ptr)	\
		if(ptr==NULL) {PP_ASSERTION(CheckPointer, "pointer!=NULL", NULL);}else{;}

#else
	#define PP_ASSERT(expression, description)
	#define PP_REQUIRE(expression, description)
	#define PP_ENSURE(expression, description)
	#define PP_IMPLIES(expression1, expression2, description)	
	#define PP_CHECK_PTR(ptr)
#endif


#endif // _PP_ASSERT_H
