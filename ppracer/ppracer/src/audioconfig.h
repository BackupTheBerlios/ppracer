/* 
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
 
#ifndef _AUDIO_CONFIG_H_
#define _AUDIO_CONFIG_H_

#include "configmode.h"

#include "ppogl/ui.h"

class AudioConfig
 : public ConfigMode
{
	ppogl::Label  m_bpsListLbl;
	ppogl::ListBox<int> m_bpsListBox;
	
	ppogl::Label m_freqListLbl;
	ppogl::ListBox<int> m_freqListBox;
	
	ppogl::Label m_audioLbl;
	ppogl::CheckBox m_audioBox;
	
	ppogl::Label m_stereoLbl;
	ppogl::CheckBox m_stereoBox;
			
	ppogl::Label m_warningLbl;
	
	ppogl::Label m_soundVolumeLbl;
	ppogl::HScale m_soundVolumeHScl;
	
	ppogl::Label m_musicVolumeLbl;
	ppogl::HScale m_musicVolumeHScl;
	
	int m_oldMusicVolume;
	
public:
	AudioConfig();

	void cancel();
	void apply();

	void musicVolumeChanged();
};

#endif // _AUDIO_CONFIG_H_
