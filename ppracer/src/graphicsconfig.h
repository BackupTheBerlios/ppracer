/* 
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
 
#ifndef _GRAPHICS_CONFIG_H_
#define _GRAPHICS_CONFIG_H_

#include "configmode.h"

#include "ppogl/ui.h"

class GraphicsConfig : public ConfigMode
{
	ppogl::CheckBox m_fogBox;
	ppogl::Label    m_fogLbl;
	
	ppogl::CheckBox m_reflectionsBox;
	ppogl::Label    m_reflectionsLbl;
	
	ppogl::CheckBox m_shadowsBox;
	ppogl::Label    m_shadowsLbl;
		
	ppogl::Label    m_detailsLbl;
	
	ppogl::HScale   m_terrainDetailHScl;
	ppogl::Label    m_terrainDetailLbl;
	
	ppogl::HScale   m_textureDetailHScl;
	ppogl::Label    m_textureDetailLbl;
		
	//ppogl::HScale   m_trackmarksDetailHScl;
	//ppogl::Label    m_trackmarksDetailLbl;
	
	ppogl::Label    m_warningLbl;
	
	ppogl::HScale   m_playerDetailHScl;
	ppogl::Label    m_playerDetailLbl;
	
	float getTerrainDetailLevel();
	void setTerrainDetailLevel(float value);
	
	float getTextureDetailLevel();
	void setTextureDetailLevel(float value);
	
	float getPlayerDetailLevel();
	void setPlayerDetailLevel(float value);	

public:
	GraphicsConfig();

	void apply();
};

#endif // _GRAPHICS_CONFIG_H_
