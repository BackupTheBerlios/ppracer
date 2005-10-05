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
 
#include "uitheme.h"

#include "../base/glstuff.h"

namespace ppogl{
	
PPOGL_SINGLETON(UITheme);	

UITheme::UITheme()
{
	labelFont = ppogl::FontMgr::getInstance().get("button_label");		
	hilitLabelFont = ppogl::FontMgr::getInstance().get("button_label_hilit");	
	insensitiveLabelFont = ppogl::FontMgr::getInstance().get("button_label_insensitive");	

	listBoxFont = ppogl::FontMgr::getInstance().get("listbox_item");	
	entryFont = ppogl::FontMgr::getInstance().get("listbox_item");	
	textareaFont = ppogl::FontMgr::getInstance().get("textarea");	
	
	checkBoxTex = ppogl::TextureMgr::getInstance().get("checkmark");
	listBoxTex = ppogl::TextureMgr::getInstance().get("listbox_arrows");
	hScaleTex = ppogl::TextureMgr::getInstance().get("hscale_arrows");	
}

void
UITheme::drawDecoration(int width, int height)
{
	ppogl::Vec2d size(255,255);
	ppogl::TextureRef texture;
	
	texture = ppogl::TextureMgr::getInstance().get("menu_bottom_left");
	ppogl::drawRectangle(ppogl::Vec2d(0,0),
						 size, texture);
	texture = ppogl::TextureMgr::getInstance().get("menu_bottom_right");
	ppogl::drawRectangle(ppogl::Vec2d(width-256,0),
						 size, texture);
	texture = ppogl::TextureMgr::getInstance().get("menu_top_left");
	ppogl::drawRectangle(ppogl::Vec2d(0,height-256),
						 size, texture);
	texture = ppogl::TextureMgr::getInstance().get("menu_top_right");
	ppogl::drawRectangle(ppogl::Vec2d(width-256,height-256),
						 size, texture);
	texture = ppogl::TextureMgr::getInstance().get("menu_title");
	ppogl::drawRectangle(ppogl::Vec2d(width/2-256, height-128),
						 ppogl::Vec2d(512,128), texture);
}

} // namespace ppogl
