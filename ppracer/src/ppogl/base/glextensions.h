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
 /*
 * Inline functions that wraps a set of OpenGL
 * functions to the gl/glu namespace.
 * In addition it wrapps several ppogl data types
 */ 
 
#ifndef _PPOGL_GL_EXTENSIONS_H_
#define _PPOGL_GL_EXTENSIONS_H_
 
#include "assert.h"

#ifdef _WIN32
	#include <WTypes.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
 
namespace gl{
	
//EXTcompiledVertexArray
extern PFNGLLOCKARRAYSEXTPROC p_LockArraysEXT;
extern PFNGLUNLOCKARRAYSEXTPROC p_UnlockArraysEXT;

/// initial compiled vertex array extension if possible
bool EXTcompiledVertexArrayInit();
	
/// return true if the compiled vertex array extension is available 
inline bool
EXTcompiledVertexArray()
{
	return(p_LockArraysEXT && p_UnlockArraysEXT);
}
	
/// lock arrays
inline void
LockArraysEXT(GLint first, GLsizei count)
{
	PP_CHECK_POINTER(p_LockArraysEXT);
	p_LockArraysEXT(first, count);
}

/// unlock locked arrays
inline void
UnlockArraysEXT()
{
	PP_CHECK_POINTER(p_UnlockArraysEXT);
	p_UnlockArraysEXT();
} 


} // namespace gl
 
#endif // _PPOGL_GL_EXTENSIONS_H_
