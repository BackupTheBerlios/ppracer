/* 
 * PPRacer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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
 
#ifndef _PP_GL_WRAPPERS_H
#define _PP_GL_WRAPPERS_H

#include "color.h"
#include "vec2d.h"
#include "vec3d.h"
#include "vec3f.h"
#include "vec4f.h"
#include "matrix.h"

#ifdef _WIN32
	#include <WTypes.h>
#endif

#include <GL/gl.h>

namespace gl{
	inline void Begin(GLenum mode){glBegin(mode);}; 
	inline void End(){glEnd();};
	
	inline void Enable(GLenum cap){glEnable(cap);};
	inline void Disable(GLenum cap){glDisable(cap);};
	
	inline void PushMatrix(){glPushMatrix();};
	inline void PopMatrix(){glPopMatrix();};
	
	inline void MatrixMode(GLenum mode){glMatrixMode(mode);};
	
	inline void LoadIdentity(){glLoadIdentity();}
	inline void Ortho( GLdouble left, GLdouble right,GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val ){glOrtho(left, right, bottom, top, near_val, far_val);};
	inline void Viewport(GLint x, GLint y, GLsizei width, GLsizei height){glViewport(x, y, width, height);};
	
	inline void Translate(const pp::Vec2d pos){glTranslated(pos.x, pos.y, 0.0);};
	inline void Translate(const pp::Vec3d pos){glTranslated(pos.x, pos.y, pos.z);};
	inline void Translate(GLdouble x, GLdouble y, GLdouble z=0.0){glTranslated(x, y, z);};
	
	inline void TexEnv(GLenum target, GLenum pname, GLfloat param){glTexEnvf(target, pname, param);};
	
	inline void TexGen(GLenum coord, GLenum pname, const GLfloat *params){glTexGenfv(coord, pname, params);};
	inline void TexGen(GLenum coord, GLenum pname, const GLint param){glTexGeni(coord, pname, param);};
	
	inline void TexCoord(GLdouble s, GLdouble t){glTexCoord2d(s, t);};
	inline void TexCoord(GLint s, GLint t){glTexCoord2i(s, t);};
	inline void TexCoord(pp::Vec2d st){glTexCoord2dv(st.values);};
	inline void TexCoord(pp::Vec3d st){glTexCoord3dv(st.values);};

	inline void TexParameter(GLenum target, GLenum pname, GLfloat param){glTexParameterf(target, pname, param );};
	
	inline void GetTexLevelParameter(GLenum target, GLint level, GLenum pname, GLint *params){glGetTexLevelParameteriv(target, level, pname, params);};
	
	inline void Vertex(GLdouble x, GLdouble y){glVertex2d(x, y);};
	inline void Vertex(pp::Vec2d pos){glVertex2dv(pos.values);};
	inline void Vertex(pp::Vec3d pos){glVertex3dv(pos.values);};
	inline void Vertex(GLfloat x, GLfloat y, GLfloat z){glVertex3f(x, y, z);};
	
	inline void Normal(GLfloat nx, GLfloat ny, GLfloat nz){glNormal3f(nx, ny, nz);};
	inline void Normal(pp::Vec3d normal){glNormal3dv(normal.values);};
		
	inline void Color(const pp::Color color){glColor4fv(color.colors);};
	inline void Color(const pp::Color color, GLfloat alpha){glColor4f(color.r, color.g, color.b, alpha);};
	inline void Color(const GLfloat *color){glColor4fv(color);};
	inline void Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a=1.0f){glColor4f(r,g,b,a);};

	inline void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha){glClearColor(red, green, blue, alpha);};
	inline void ClearColor(pp::Color color){glClearColor(color.r, color.g, color.b, color.a);};
	inline void Clear(GLbitfield mask){glClear(mask);};
	
	inline void Rect(const pp::Vec2d pos1, const pp::Vec2d pos2){glRectd(pos1.x, pos1.y, pos2.x, pos2.y);};
	inline void Rect(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2){glRectd(x1, y1, x2, y2);};
	inline void Rect(GLint x1, GLint y1, GLint x2, GLint y2){glRecti(x1, y1, x2, y2);};

	inline void BindTexture(GLenum target, GLuint texture){glBindTexture(target, texture);};
	inline void GenTextures(GLsizei n, GLuint *textures){glGenTextures(n, textures);};
	inline void DeleteTextures(GLsizei n, const GLuint *textures){glDeleteTextures(n, textures);};
	
	inline GLuint GenLists(GLsizei range){return glGenLists(range);};
	inline void NewList(GLuint list, GLenum mode){glNewList(list, mode);};
	inline void EndList(){glEndList();};	
	inline void CallList(GLuint list){glCallList(list);};
	
	inline void Fog(GLenum pname, GLint param){glFogi(pname, param);};
	inline void Fog(GLenum pname, const GLint *params){glFogiv(pname,params);};
	inline void Fog(GLenum pname, GLfloat param){glFogf(pname, param);};
	inline void Fog(GLenum pname, const GLfloat *params ){glFogfv(pname, params);};

	inline void Hint(GLenum target, GLenum mode){glHint(target, mode);};

	inline void DepthMask(GLboolean flag){glDepthMask(flag);};
	inline void ShadeModel(GLenum mode){glShadeModel(mode);};
	inline void DepthFunc(GLenum func){glDepthFunc(func);};
	inline void AlphaFunc(GLenum func, GLclampf ref){glAlphaFunc(func, ref);};
	inline void BlendFunc(GLenum sfactor, GLenum dfactor){glBlendFunc(sfactor, dfactor);};
	inline void StencilFunc(GLenum func, GLint ref, GLuint mask){glStencilFunc(func, ref, mask);};
	inline void StencilOp(GLenum fail, GLenum zfail, GLenum zpass){glStencilOp(fail, zfail, zpass);};
	inline void ClearStencil(GLint s){glClearStencil(s);};	
		
	inline const char* GetString(GLenum name){return reinterpret_cast<const char*>(glGetString(name));};

	inline void GetValue(GLenum pname, GLboolean *params){glGetBooleanv(pname, params );};
	inline void GetValue(GLenum pname, GLdouble *params){glGetDoublev(pname, params);};
	inline void GetValue(GLenum pname, GLfloat *params){glGetFloatv(pname, params);};
	inline void GetValue(GLenum pname, GLint *params){glGetIntegerv(pname, params);};
	
	inline void MultMatrix(const GLdouble *m){glMultMatrixd(m);};
	inline void MultMatrix(pp::Matrix &matrix){glMultMatrixd(matrix.rawdata);};
	
	inline void LightModel(GLenum pname, const pp::Color& color){glLightModelfv(pname, color.colors);};
	
	inline void Light(GLenum light, GLenum pname, const pp::Color color){glLightfv(light, pname, color.colors);};
	inline void Light(GLenum light, GLenum pname, const pp::Vec3f vec){glLightfv(light, pname, vec.values);};
	inline void Light(GLenum light, GLenum pname, const pp::Vec4f vec){glLightfv(light, pname, vec.values);};
	inline void Light(GLenum light, GLenum pname, GLfloat param){glLightf(light, pname, param);};
	inline void Light(GLenum light, GLenum pname, GLint param){glLighti(light, pname, param);};

	inline void DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices){glDrawElements(mode, count, type, indices);};
	
	inline void EnableClientState(GLenum cap){glEnableClientState(cap);};
	inline void DisableClientState(GLenum cap){glDisableClientState(cap);};
	
	inline void Material(GLenum face, GLenum pname, GLfloat param){glMaterialf(face, pname, param);};
	inline void Material(GLenum face, GLenum pname, const GLfloat *params){glMaterialfv(face, pname, params);};
	inline void Material(GLenum face, GLenum pname, const pp::Color color){glMaterialfv(face, pname, color.colors);};
	
	inline void ReadBuffer(GLenum mode){glReadBuffer(mode);};
	inline void ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels){glReadPixels(x, y, width, height, format, type, pixels);};

	inline void PixelStore(GLenum pname, GLint param){glPixelStorei(pname, param);};	
	
} //namespace gl

#endif // _PP_GL_WRAPPERS_H
