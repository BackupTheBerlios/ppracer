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

#ifndef _PPOGL_FONT_H_
#define _PPOGL_FONT_H_

#include <map>
#include <string>

#ifdef _WIN32
	#include <WTypes.h>
#endif

#include "font/FTFont.h"
#include "base/color.h"
#include "base/vec2d.h"
#include "base/refptr.h"
#include "base/resourcemgr.h"
#include "base/singleton.h"

#include "ppogl_script.h"

namespace ppogl{
		
class Font
 : public RefObject
{
private:
	Font(const Font& font);
	ppogl::RefPtr<ppogl::FTFont> mp_font;
	ppogl::Color m_color;

public:	
	Font(const std::string& filename, unsigned int size, const ppogl::Color &color);
	Font(ppogl::RefPtr<ppogl::FTFont> font, const ppogl::Color &color);

	~Font();

	///draws the utf8 string at the specified position 
	void draw(const std::string& , const ppogl::Vec2d& position);

	///draws the utf8 string at position x,y
	void draw(const std::string& string, const float x, const float y)
	{
		draw(string, ppogl::Vec2d(x, y));
	}
	
	///draws the unicode string at the specified position
	void draw(const wchar_t *string, const ppogl::Vec2d& position);
	
	///draws the unicode string at position x,y 
	void draw(const wchar_t *string, const float x, const float y)
	{
		draw(string, ppogl::Vec2d(x, y));
	}

	float ascender() const 
	{
		return mp_font->Ascender();
	}
	
	float descender() const 
	{
		return mp_font->Descender();
	}
	
	float advance(const std::string& string)
	{
		const wchar_t* u_string
			= Font::utf8ToUnicode(string);	
	
		const float adv = mp_font->Advance(u_string);
		delete [] u_string;
		return adv;	
	}
	
	float advance(const wchar_t* string)
	{
		return mp_font->Advance(string);	
	}

	const ppogl::Color& getColor() const
	{
		return m_color;
	}
		
	ppogl::RefPtr<ppogl::FTFont> getFTFont()
	{
		return mp_font;
	}
		
//static stuff	
public:
	/// returns the unicode of the utf8 string.
	static const wchar_t* utf8ToUnicode(const std::string& string);

	/// fills the buff array with the unicode string.
	static void utf8ToUnicode(wchar_t* buff, const std::string& string);
};

typedef RefPtr<Font> FontRef;

class FontMgr
 : public Singleton<FontMgr>,
   public ResourceMgr<Font>
{
public:
	/// loads and registers the ttf file to the specified binding
	bool registerFont(const std::string& binding, const std::string& fileName, unsigned int size, const ppogl::Color &color);
	
	/// bind font to the specified font
	bool bindFont(const std::string& binding, const std::string& fontName);
	
	/// bind font to the specified font with a different color
	bool bindFont(const std::string& binding, const std::string& fontName, const ppogl::Color& color);
	
	static int sqLoad(ppogl::Script *vm);
	static int sqBind(ppogl::Script *vm);
};


} //namepsace ppogl

#endif // _PPOGL_FONT_H_
