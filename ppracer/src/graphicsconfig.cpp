/* 
 * PlanetPenguin Racer 
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
 
#include "graphicsconfig.h"

#include "stuff.h"
#include "hier.h"

/// minimum course detail level
#define MIN_CDL 20.0f

/// maximum course detail level
#define MAX_CDL 100.0f

/// minimum sphere devisions for player models
#define MIN_TSD 6.0f

/// maximum sphere devisions for player models
#define MAX_TSD 20.0f


GraphicsConfig::GraphicsConfig()
 : ConfigMode(_("Graphics Configuration")),
   m_fogLbl(_("Draw Fog:")),
   m_reflectionsLbl(_("Reflections:")),
   m_shadowsLbl(_("Shadows:")),
   m_detailsLbl(_("Detail settings")),
   m_terrainDetailLbl(_("Terrain:")),
   m_textureDetailLbl(_("Textures:")),
   //m_trackmarksDetailLbl(_("Track marks:")),
   m_playerDetailLbl(_("Player Model:"))
{	
	ppogl::Vec2d position(40,350);
	ppogl::Vec2d position2(600,350);
		
	m_fogLbl.setPosition(position);
	m_fogBox.setPosition(position2);
	m_fogBox.alignment.right();
	m_fogBox.setSelected(!PPConfig.getBool("disable_fog"));

	position.y()-=40;
	position2.y()-=40;
	
	m_reflectionsLbl.setPosition(position);
	m_reflectionsBox.setPosition(position2);
	m_reflectionsBox.alignment.right();
	m_reflectionsBox.setSelected(PPConfig.getBool("terrain_envmap"));

	position.y()-=40;
	position2.y()-=40;
	
	m_shadowsLbl.setPosition(position);
	m_shadowsBox.setPosition(position2);
	m_shadowsBox.alignment.right();
	m_shadowsBox.setSelected(PPConfig.getBool("draw_tux_shadow"));
		
	position.y()-=50;
	position2.y()-=50;
	
	m_detailsLbl.setPosition(position);
	
	position.y()-=40;
	position2.y()-=40;
	
	m_terrainDetailLbl.setPosition(position);
	m_terrainDetailHScl.setPosition(position2);
	m_terrainDetailHScl.alignment.right();
	m_terrainDetailHScl.setValue(getTerrainDetailLevel());
	
	position.y()-=40;
	position2.y()-=40;
	
	m_textureDetailLbl.setPosition(position);
	m_textureDetailHScl.setPosition(position2);
	m_textureDetailHScl.alignment.right();
	m_textureDetailHScl.setStep(0.2f);
	m_textureDetailHScl.setValue(getTextureDetailLevel());
	
	/*
	position.y()-=40;
	position2.y()-=40;
	
	m_trackmarksDetailLbl.setPosition(position);
	m_trackmarksDetailHScl.setPosition(position2);
	m_trackmarksDetailHScl.alignment.right();
	m_trackmarksDetailHScl.setValue((PPConfig.getInt("max_track_marks")-1000.f)/10000.0f);
	*/
		
	position.y()-=40;
	position2.y()-=40;
	
	m_playerDetailLbl.setPosition(position);
	m_playerDetailHScl.setPosition(position2);
	m_playerDetailHScl.alignment.right();
	m_playerDetailHScl.setStep(0.2f);
	m_playerDetailHScl.setValue(getPlayerDetailLevel());	
}

float
GraphicsConfig::getTerrainDetailLevel()
{
	const float cdl = PPConfig.getDouble("course_detail_level");
	const float details = (cdl-MIN_CDL)/(MAX_CDL-MIN_CDL);
	return details;
}


void
GraphicsConfig::setTerrainDetailLevel(float value)
{
	const float cdl = MIN_CDL + value*(MAX_CDL-MIN_CDL);	
	PPConfig.setDouble("course_detail_level", cdl);
}

float
GraphicsConfig::getTextureDetailLevel()
{
	const int filter = PPConfig.getInt("texture_filter");
		
	if(filter<=0) return 0.0f; 
	else if(filter<=1) return 0.2f;
	else if(filter<=2) return 0.4f;	
	else if(filter<=3) return 0.6f;
	else if(filter<=4) return 0.8f;
	else return 1.0f;	
}
	
void 
GraphicsConfig::setTextureDetailLevel(float value)
{
	PPConfig.setInt("texture_filter", int(value*5.0f));
}

float
GraphicsConfig::getPlayerDetailLevel()
{
	const int tsd = PPConfig.getInt("tux_sphere_divisions");
	const float details = (tsd-MIN_TSD)/(MAX_TSD-MIN_TSD);	
	return details;	
}

void 
GraphicsConfig::setPlayerDetailLevel(float value)
{
	const int tsd = int(MIN_TSD + value*(MAX_TSD-MIN_TSD));
	PPConfig.setInt("tux_sphere_divisions", tsd);
}

void
GraphicsConfig::apply()
{
	PPConfig.setBool("disable_fog", !m_fogBox.isSelected());
	PPConfig.setBool("terrain_envmap", m_reflectionsBox.isSelected());
	PPConfig.setBool("draw_tux_shadow", m_shadowsBox.isSelected());
	
	setTerrainDetailLevel(m_terrainDetailHScl.getValue());
	setTextureDetailLevel(m_textureDetailHScl.getValue());
	setPlayerDetailLevel(m_playerDetailHScl.getValue());
 	
	update_scene_nodes();
	
	//PPConfig.setInt("max_track_marks", int(m_trackmarksDetailHScl.getValue()*10000+1000));
	
	write_config_file();

	setMode( GameMode::prevmode );
}
