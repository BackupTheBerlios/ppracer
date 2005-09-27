/* 
 * PlanetPenguin Racer 
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
 
#include "audioconfig.h"

#include "stuff.h"
#include "ppogl/base/defs.h"

AudioConfig::AudioConfig()
 : m_bpsListLbl(_("Bits Per Sample:")),
   m_freqListLbl(_("Samples Per Second:")),
   m_audioLbl(_("Disable Audio:")),
   m_soundLbl(_("Sound Effects:")),
   m_musicLbl(_("Music:")),
   m_stereoLbl(_("Stereo:")),
   m_warningLbl(_("(needs restart)"))
{
	setTitle(_("Audio Configuration"));

	ppogl::Vec2d position(40,350);
	ppogl::Vec2d position2(600,350);
	
	m_audioLbl.setPosition(position);
	m_audioBox.setPosition(position2);
	m_audioBox.alignment.right();
	m_audioBox.setSelected(PPConfig.getBool("disable_audio"));
		
	position.y()-=40;
	position2.y()-=40;
	
	m_soundLbl.setPosition(position);
	m_soundBox.setPosition(position2);
	m_soundBox.alignment.right();
	m_soundBox.setSelected(PPConfig.getBool("sound_enabled"));
	
	position.y()-=40;
	position2.y()-=40;
	
	m_musicLbl.setPosition(position);
	m_musicBox.setPosition(position2);
	m_musicBox.alignment.right();
	m_musicBox.setSelected(PPConfig.getBool("music_enabled"));

	
	position.y()-=40;
	position2.y()-=40;
	
	m_stereoLbl.setPosition(position);
	m_stereoBox.setPosition(position2);
	m_stereoBox.alignment.right();
	m_stereoBox.setSelected(PPConfig.getBool("audio_stereo"));
	
	position.y()-=60;
	position2.y()-=60;
	
	m_warningLbl.setPosition(position);
	
	position.y()-=40;
	position2.y()-=40;
	
	//frequency
	m_freqListBox.addElement("11025", 0);
	m_freqListBox.addElement("22050", 1);
	m_freqListBox.addElement("44100", 2);
	m_freqListBox.setSelectedData(PPConfig.getInt("audio_freq_mode"));
	
	m_freqListLbl.setPosition(position);
	m_freqListBox.setPosition(position2);
	m_freqListBox.alignment.right();

	position.y()-=40;
	position2.y()-=40;

	//bits per sample
	m_bpsListBox.addElement("8", 0);
	m_bpsListBox.addElement("16",1);
	m_bpsListBox.setSelectedData(PPConfig.getInt("audio_format_mode"));
		
	m_bpsListLbl.setPosition(position);
	m_bpsListBox.setPosition(position2);
	m_bpsListBox.alignment.right();
}

void
AudioConfig::apply()
{
	PPConfig.setBool("disable_audio",m_audioBox.isSelected());
	PPConfig.setBool("sound_enabled",m_soundBox.isSelected());
	PPConfig.setBool("music_enabled",m_musicBox.isSelected());
	PPConfig.setBool("audio_stereo",m_stereoBox.isSelected());
	
	ppogl::ListBox<int>::iterator bpsit = m_bpsListBox.getSelected();
	PPConfig.setInt("audio_format_mode",(*bpsit).data);
		
	ppogl::ListBox<int>::iterator freqit = m_freqListBox.getSelected();
	PPConfig.setInt("audio_freq_mode",(*freqit).data);
		
	write_config_file();
	
	if(ppogl::AudioMgr::getInstance().isInitialized()==false){
		PP_WARNING("Fixme: Enabling audio needs restart :(");		
	}
		
	if(m_audioBox.isSelected()){
		ppogl::AudioMgr::getInstance().enableSound(false);
		ppogl::AudioMgr::getInstance().enableMusic(false);
	}else{
		ppogl::AudioMgr::getInstance().enableSound(m_soundBox.isSelected());
		ppogl::AudioMgr::getInstance().enableMusic(m_musicBox.isSelected());
	}	
	
	setMode(GameMode::prevmode);
}
