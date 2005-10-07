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

/// minimum course detail level
#define MIN_CDL 20.0f

/// maximum course detail level
#define MAX_CDL 100.0f

/// minimum sphere devisions for player models
#define MIN_TSD 6.0f

/// maximum sphere devisions for player models
#define MAX_TSD 20.0f


GraphicsConfig::GraphicsConfig()
 : m_terrainDetailLbl(_("Terrain Details:")),
   m_textureDetailLbl(_("Texture Details:")),
   m_warningLbl(_("(needs restart)")),
   m_playerDetailLbl(_("Player Model Details:"))
{
	setTitle(_("Graphics Configuration"));	
			
	ppogl::Vec2d position(40,350);
	ppogl::Vec2d position2(600,350);
	
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
	
	position.y()-=60;
	position2.y()-=60;
	
	m_warningLbl.setPosition(position);
	
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
	float cdl = PPConfig.getDouble("course_detail_level");
	float details = (cdl-MIN_CDL)/(MAX_CDL-MIN_CDL);	
	return details;
}


void
GraphicsConfig::setTerrainDetailLevel(float value)
{
	float cdl = MIN_CDL + value*(MAX_CDL-MIN_CDL);	
	PPConfig.setDouble("course_detail_level", cdl);
}

float
GraphicsConfig::getTextureDetailLevel()
{
	int filter = PPConfig.getInt("texture_filter");
		
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
	int tsd = PPConfig.getInt("tux_sphere_divisions");
	float details = (tsd-MIN_TSD)/(MAX_TSD-MIN_TSD);	
	return details;	
}

void 
GraphicsConfig::setPlayerDetailLevel(float value)
{
	int tsd = int(MIN_TSD + value*(MAX_TSD-MIN_TSD));
	PPConfig.setInt("tux_sphere_divisions", tsd);
}

void
GraphicsConfig::apply()
{
	setTerrainDetailLevel(m_terrainDetailHScl.getValue());
	setTextureDetailLevel(m_textureDetailHScl.getValue());
	setPlayerDetailLevel(m_playerDetailHScl.getValue());
 	
	write_config_file();

	setMode( GameMode::prevmode );
}
