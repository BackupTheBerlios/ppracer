/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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
 
#include "generalconfig.h"

#include "stuff.h"
#include "game_mgr.h"

GeneralConfig::GeneralConfig()
 : ConfigMode(_("General Configuration")),
   m_uiSnowLbl(_("Show UI Snow:")),
   m_fpsLbl(_("Display FPS:")),
   m_coursePercentageLbl(_("Display Progress Bar:")),
   m_langListLbl(_("Language:")),
   m_langListBox(240)
{
	std::map<std::string, std::string> languages=
		ppogl::Translation::getInstance().getLanguages();
	
	std::map<std::string, std::string>::iterator iter;
	ppogl::ListBox<std::string>::Iterator langit;

	if(languages.empty()){	
		PP_WARNING("Language list is empty");
	}else{
		for (iter=languages.begin();iter != languages.end(); ++iter){
			langit = m_langListBox.addElement((*iter).second,(*iter).first);
			if((*iter).first==PPConfig.getString("ui_language")){
				m_langListBox.setSelected(langit);
			}
		}
	}
		
	ppogl::Vec2d position(40,350);
	ppogl::Vec2d position2(600,350);

	m_langListLbl.setPosition(position);
	m_langListBox.setPosition(position2);
	m_langListBox.alignment.right();
	
	position.y()-=40;
	position2.y()-=40;
	
	m_uiSnowLbl.setPosition(position);
	m_uiSnowBox.setPosition(position2);
	m_uiSnowBox.alignment.right();
	m_uiSnowBox.setSelected(PPConfig.getBool("ui_snow"));
	
	position.y()-=40;
	position2.y()-=40;
	
	m_fpsLbl.setPosition(position);	
	m_fpsBox.setPosition(position2);
	m_fpsBox.alignment.right();
	m_fpsBox.setSelected(PPConfig.getBool("display_fps"));
	
	position.y()-=40;
	position2.y()-=40;
	
	m_coursePercentageLbl.setPosition(position);
	m_coursePercentageBox.setPosition(position2);
	m_coursePercentageBox.alignment.right();
	m_coursePercentageBox.setSelected(PPConfig.getBool("display_course_percentage"));
}

void
GeneralConfig::apply()
{
	ppogl::ListBox<std::string>::Iterator langit =
		m_langListBox.getSelected();
	
	PPConfig.setString("ui_language", (*langit).data);
	PPConfig.setBool("ui_snow", m_uiSnowBox.isSelected());
	PPConfig.setBool("display_fps", m_fpsBox.isSelected());
	PPConfig.setBool("display_course_percentage", m_coursePercentageBox.isSelected());
	
	write_config_file();

	ppogl::Translation::getInstance().clear();
	
	std::string data_dir = PPConfig.getString("data_dir");
	script.doFile(data_dir+"/translations/"+PPConfig.getString("ui_language")+".nut");		
	
	setMode( GameMode::prevmode );
}
