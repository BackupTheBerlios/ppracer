/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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

#include "font.h"

#include <iostream>

#include "ftgl/FTGLTextureFont.h"
#include "base/glwrappers.h"
#include "config.h"

namespace ppogl{
	
Font::Font(const std::string& filename, unsigned int size, const ppogl::Color &color)
 : RefObject(),
   mp_font(NULL),
   m_color(color)
{
	mp_font = new FTGLTextureFont(filename.c_str());
	
	if (mp_font->Error()){
		PP_ERROR("Unable to load font: " << filename);		
	}else{
		mp_font->FaceSize(size);
		mp_font->CharMap(ft_encoding_unicode);
	}
}

Font::Font(FTFont *font, const ppogl::Color &color)
 : RefObject(),
   mp_font(font),
   m_color(color)
{
}

Font::~Font()
{
	if(mp_font!=NULL){
		delete mp_font;
	}
}
	
void
Font::draw(const std::string& string, float x, float y)
{
	draw(string, ppogl::Vec2d(x, y));
}

void
Font::draw(const std::string& string, ppogl::Vec2d position)
{
	const wchar_t* u_string;
	u_string = Font::utf8ToUnicode(string);	

	gl::PushMatrix();
	{
		gl::Enable(GL_TEXTURE_2D);
		gl::Color(m_color);
		gl::Translate(position);
		mp_font->Render(u_string);
		gl::Disable(GL_TEXTURE_2D);
	}
	gl::PopMatrix();

	delete u_string;	
}

void
Font::draw(const wchar_t *string, float x, float y)
{
	draw(string, ppogl::Vec2d(x, y));
}

void
Font::draw(const wchar_t *string, ppogl::Vec2d position)
{
	gl::PushMatrix();
	{
		gl::Enable(GL_TEXTURE_2D);
		gl::Color(m_color);
		gl::Translate(position);
		mp_font->Render(string);
		gl::Disable(GL_TEXTURE_2D);
	}
	gl::PopMatrix();	
}

float
Font::ascender()
{
	return mp_font->Ascender();
}

float
Font::descender()
{
	return mp_font->Descender();
}

float
Font::advance(const std::string& string)
{
	const wchar_t* u_string;
	u_string = Font::utf8ToUnicode(string);	
	
	float adv = mp_font->Advance(u_string);
	
	delete u_string;
	return adv;	
}

float
Font::advance(const wchar_t* string)
{
	return mp_font->Advance(string);	
}

ppogl::Color&
Font::getColor()
{
	return m_color;
}

FTFont*
Font::getFTFont()
{
	return mp_font;
}
	
wchar_t*
Font::utf8ToUnicode(const std::string& string)
{
	wchar_t ch;
	int len = string.length();
	wchar_t *u_string = new wchar_t[len+1];
	int j=0;
		
	for (int i=0; i < len; ++i, ++j ) {
		ch = (reinterpret_cast<const unsigned char *>(string.c_str()))[i];
		if ( ch >= 0xF0 ) {
			ch  =  wchar_t(string[i]&0x07) << 18;
			ch |=  wchar_t(string[++i]&0x3F) << 12;
			ch |=  wchar_t(string[++i]&0x3F) << 6;
			ch |=  wchar_t(string[++i]&0x3F);
		} else
		if ( ch >= 0xE0 ) {
			ch  =  wchar_t(string[i]&0x0F) << 12;
			ch |=  wchar_t(string[++i]&0x3F) << 6;
			ch |=  wchar_t(string[++i]&0x3F);
		} else
		if ( ch >= 0xC0 ) {
			ch  =  wchar_t(string[i]&0x1F) << 6;
			ch |=  wchar_t(string[++i]&0x3F);
		}
		u_string[j] = ch;
	}
	u_string[j] = 0;
	
	return u_string;
}

void
Font::utf8ToUnicode(wchar_t* buff, const std::string& string)
{
	wchar_t ch;
	int len = string.length();
	int j=0;
		
	for (int i=0; i < len; ++i, ++j ) {
		ch = (reinterpret_cast<const unsigned char *>(string.c_str()))[i];
		if ( ch >= 0xF0 ) {
			ch  =  wchar_t(string[i]&0x07) << 18;
			ch |=  wchar_t(string[++i]&0x3F) << 12;
			ch |=  wchar_t(string[++i]&0x3F) << 6;
			ch |=  wchar_t(string[++i]&0x3F);
		} else
		if ( ch >= 0xE0 ) {
			ch  =  wchar_t(string[i]&0x0F) << 12;
			ch |=  wchar_t(string[++i]&0x3F) << 6;
			ch |=  wchar_t(string[++i]&0x3F);
		} else
		if ( ch >= 0xC0 ) {
			ch  =  wchar_t(string[i]&0x1F) << 6;
			ch |=  wchar_t(string[++i]&0x3F);
		}
		buff[j] = ch;
	}
	buff[j] = 0;
}


// FontMgr

PPOGL_SINGLETON(FontMgr);

FontMgr::FontMgr()
{
}

FontMgr::~FontMgr()
{	
}

bool
FontMgr::registerFont(const std::string& binding, const std::string& fileName, unsigned int size, const ppogl::Color &color)
{
	//todo: check wether the font alrady exists	
	m_bindings[binding] = FontRef(new ppogl::Font(fileName, size, color));
	return true;
}

bool
FontMgr::bindFont(const std::string& binding, const std::string& fontName)
{
	FontRef font = get(fontName);
	if(!font) return false;
		
	m_bindings[binding] = FontRef(new ppogl::Font(font->getFTFont(), font->getColor()));
	return true;
}	

bool
FontMgr::bindFont(const std::string& binding, const std::string& fontName, const ppogl::Color &color)
{
	FontRef font = get(fontName);
	if(!font) return false;
		
	m_bindings[binding] = FontRef(new ppogl::Font(font->getFTFont(), color));
	return true;
}

void
FontMgr::draw(const std::string& binding, const std::string& string, float x, float y)
{
	FontRef font = get(binding);
	
	if(font){	
		font->draw(string, x, y);
	}
}

int
FontMgr::sqLoad(ppogl::Script *vm)
{
	ppogl::Color color=ppogl::Color::white;
	
	std::string binding = vm->getString(1);
	std::string filename = vm->getString(2);
	int size = vm->getInt(3);
	
	if(vm->getTop()>=4){
		vm->pushNull();
		int i=0;		
		while(vm->next(4))
		{
			if(i>4) break;			
			double temp = vm->getFloat();
			
			PP_ASSERT(temp>=0.0 && temp<=1.0,"Invalid color value: " << temp);
		
			color.values[i]=temp;
			i++;			
			vm->pop(2); 
		}	
		vm->pop();
	}
	
	getInstance().registerFont(binding, filename, size, color);
	return 0;
}

int
FontMgr::sqBind(ppogl::Script *vm)
{
	std::string binding = vm->getString(1);
	std::string name = vm->getString(2);
	
	if(vm->getTop()>=3){
		ppogl::Color color=ppogl::Color::white;
		vm->pushNull();
		int i=0;		
        while(vm->next(3)!= 0){
			if(i>4) break;
			double value = vm->getFloat();			
  			PP_ASSERT(value>=0.0 && value<=1.0,"Invalid color value: " << value);
			vm->pop(2);		
			color.values[i]=value;
			i++;
		}
		vm->pop();
		if(!getInstance().bindFont(binding, name, color)){
			PP_WARNING("Unable to bind font: " << binding << " -> " << name);	
		}
	}else{
		if(!getInstance().bindFont(binding, name)){
			PP_WARNING("Unable to bind font: " << binding << " -> " << name);
		}
	}

	return 0;
}

} //namepsace ppogl
