/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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

#include "base/glwrappers.h"
#include "config.h"

namespace ppogl{
	
Font::Font(const std::string& filename, unsigned int size, const ppogl::Color &color)
 : RefObject(),
   m_color(color)
{
	mp_font = new ppogl::FTFont(filename.c_str());
	
	if(mp_font->Error()){
		PP_ERROR("Unable to load font: " << filename);		
	}else{
		mp_font->FaceSize(size);
	}
}

Font::Font(ppogl::RefPtr<ppogl::FTFont> font, const ppogl::Color &color)
 : RefObject(),
   mp_font(font),
   m_color(color)
{
}

Font::~Font()
{
}

void
Font::draw(const std::string& string, const ppogl::Vec2d& position)
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

	delete [] u_string;	
}

void
Font::draw(const wchar_t *string, const ppogl::Vec2d& position)
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

const wchar_t*
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

bool
FontMgr::registerFont(const std::string& binding, const std::string& filename, unsigned int size, const ppogl::Color &color)
{
	PP_LOG(LogFonts,"Loading font: " << binding << " -> " << filename << " Size: " << size << " Color: " << color); 
	
	//todo: check wether the font alrady exists	
	m_bindings[binding] = FontRef(new ppogl::Font(filename, size, color));
	return true;
}

bool
FontMgr::bindFont(const std::string& binding, const std::string& fontName)
{
	PP_LOG(LogFonts,"Binding font: " << binding << " -> " << fontName);
	
	FontRef font = get(fontName);
	if(!font) return false;
		
	m_bindings[binding] = FontRef(new ppogl::Font(font->getFTFont(), font->getColor()));
	return true;
}	

bool
FontMgr::bindFont(const std::string& binding, const std::string& fontName, const ppogl::Color &color)
{
	PP_LOG(LogFonts,"Binding font: " << binding << " -> " << fontName << " Color: " << color);
	
	FontRef font = get(fontName);
	if(!font) return false;
		
	m_bindings[binding] = FontRef(new ppogl::Font(font->getFTFont(), color));
	return true;
}

int
FontMgr::sqLoad(ppogl::Script *vm)
{
	ppogl::Color color=ppogl::Color::white;
	
	std::string binding = vm->getStringFromTable("name");
	std::string filename = vm->getStringFromTable("file");
	int size = vm->getIntFromTable("size");
		
	if(vm->isKeyInTable("color")){
		color = vm->getColorFromTable("color");
	}
		
	getInstance().registerFont(binding, filename, size, color);
	return 0;
}

int
FontMgr::sqBind(ppogl::Script *vm)
{
	std::string binding = vm->getStringFromTable("name");
	std::string name = vm->getStringFromTable("font");
	
	if(vm->isKeyInTable("color")){
		ppogl::Color color = vm->getColorFromTable("color");;
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
