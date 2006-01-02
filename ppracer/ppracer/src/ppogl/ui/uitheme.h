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

#ifndef _PPOGL_UITHEME_H_
#define _PPOGL_UITHEME_H_

#include "../base/vec2i.h"
#include "../base/singleton.h"
#include "../base/signal.h"
#include "../font.h"
#include "../textures.h"

namespace ppogl{

class UITheme
 : public Singleton<UITheme>
{
public:
	UITheme();
	~UITheme();

	///default font for text labels
	ppogl::FontRef labelFont;

	///default highlightfont for text labels
	ppogl::FontRef hilitLabelFont;

	///default font for insensitive text labels
	ppogl::FontRef insensitiveLabelFont;

	///default font for listboxes
	ppogl::FontRef listBoxFont;

	///default font for text entries
	ppogl::FontRef entryFont;

	///defalut font for textareas
	ppogl::FontRef textareaFont;

	///default texture for checkboxes
	ppogl::TextureRef checkBoxTex;
	
	///default texture for up/down buttons in listboxes	
	ppogl::TextureRef listBoxTex;
	
	///default texture for buttons in hscale widgets
	ppogl::TextureRef hScaleTex;

	///draw the background for the ui
	void drawDecoration(const ppogl::Vec2i& resolution);
};	

} //namepsace ppogl

#endif // _PPOGL_UITHEME_H_
