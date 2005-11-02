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
 
#ifndef _GENERAL_CONFIG_H_
#define _GENERAL_CONFIG_H_

#include "configmode.h"

#include "ppogl/ui.h"
#include "ppogl/ui/listbox.h"

class GeneralConfig : public ConfigMode
{
	ppogl::CheckBox m_uiSnowBox;
	ppogl::Label    m_uiSnowLbl;
	
	ppogl::CheckBox m_fpsBox;
	ppogl::Label    m_fpsLbl;
	
	ppogl::CheckBox m_coursePercentageBox;
	ppogl::Label    m_coursePercentageLbl;
	
	ppogl::Label    m_langListLbl;
	ppogl::ListBox<std::string> m_langListBox;

public:
	GeneralConfig();

	void apply();
};

#endif // _GENERAL_CONFIG_H_
