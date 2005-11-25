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
 : ConfigMode(_("Audio Configuration")),
   m_bpsListLbl(_("Bits Per Sample:")),
   m_freqListLbl(_("Samples Per Second:")),
   m_audioLbl(_("Disable Audio:")),
   m_stereoLbl(_("Stereo:")),
   m_warningLbl(_("(needs restart)")),
   m_soundVolumeLbl(_("Sound Volume:")),
   m_musicVolumeLbl(_("Music Volume:"))  
{

	ppogl::Vec2d position(40,350);
	ppogl::Vec2d position2(600,350);
	
	m_audioLbl.setPosition(position);
	m_audioBox.setPosition(position2);
	m_audioBox.alignment.right();
	m_audioBox.setSelected(PPConfig.getBool("disable_audio"));
	
	position.y()-=40;
	position2.y()-=40;
	
	m_stereoLbl.setPosition(position);
	m_stereoBox.setPosition(position2);
	m_stereoBox.alignment.right();
	m_stereoBox.setSelected(PPConfig.getBool("audio_stereo"));
	
	position.y()-=40;
	position2.y()-=40;
	
	m_soundVolumeLbl.setPosition(position);
	m_soundVolumeHScl.setPosition(position2);
	m_soundVolumeHScl.alignment.right();
	m_soundVolumeHScl.setValue(
			PPConfig.getInt("sound_volume")/100.0f);
	
	position.y()-=40;
	position2.y()-=40;
	
	m_oldMusicVolume = PPConfig.getInt("music_volume");
	
	m_musicVolumeLbl.setPosition(position);
	m_musicVolumeHScl.setPosition(position2);
	m_musicVolumeHScl.alignment.right();
	m_musicVolumeHScl.setValue(m_oldMusicVolume/100.0f);
	m_musicVolumeHScl.signalClicked.Connect(ppogl::CreateSlot(this,&AudioConfig::musicVolumeChanged));
		
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
AudioConfig::musicVolumeChanged()
{
	const int musicVolume = int(m_musicVolumeHScl.getValue()*100.0f);
	
	if(musicVolume>0){
		ppogl::AudioMgr::getInstance().enableMusic(true);
		ppogl::AudioMgr::getInstance().playMusic("start_screen");	
	}else{	
		ppogl::AudioMgr::getInstance().enableMusic(false);
	}
	ppogl::AudioMgr::getInstance().setMusicVolume(musicVolume);	
}

void
AudioConfig::apply()
{
	PPConfig.setBool("disable_audio",m_audioBox.isSelected());
	PPConfig.setBool("audio_stereo",m_stereoBox.isSelected());
	
	const int soundVolume = int(m_soundVolumeHScl.getValue()*100.0f);
	if(soundVolume<=0) PPConfig.setBool("sound_enabled",false);	
	else PPConfig.setBool("sound_enabled",true);
	PPConfig.setInt("sound_volume",soundVolume);

	const int musicVolume = int(m_musicVolumeHScl.getValue()*100.0f);
	if(musicVolume<=0) PPConfig.setBool("music_enabled",false);	
	else PPConfig.setBool("music_enabled",true);
	PPConfig.setInt("music_volume",musicVolume);
	
	ppogl::ListBox<int>::Iterator bpsit = m_bpsListBox.getSelected();
	PPConfig.setInt("audio_format_mode",(*bpsit).data);
		
	ppogl::ListBox<int>::Iterator freqit = m_freqListBox.getSelected();
	PPConfig.setInt("audio_freq_mode",(*freqit).data);
		
	write_config_file();
	
	if(ppogl::AudioMgr::getInstance().isInitialized()==false){
		PP_WARNING("Fixme: Enabling audio needs restart :(");		
	}
		
	if(m_audioBox.isSelected()){
		ppogl::AudioMgr::getInstance().enableSound(false);
		ppogl::AudioMgr::getInstance().enableMusic(false);
	}else{
		ppogl::AudioMgr::getInstance().enableSound(soundVolume>0);
		ppogl::AudioMgr::getInstance().enableMusic(musicVolume>0);
		
		ppogl::AudioMgr::getInstance().setSoundVolume(soundVolume);
		ppogl::AudioMgr::getInstance().setMusicVolume(musicVolume);
	}	
		
	setMode(GameMode::prevmode);
}

void
AudioConfig::cancel()
{
	// reset old music volume
	ppogl::AudioMgr::getInstance().enableMusic(m_oldMusicVolume>0);
	ppogl::AudioMgr::getInstance().setMusicVolume(m_oldMusicVolume);	
	
	ConfigMode::cancel();
}
