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
 
#include "joystickconfig.h"

#include "stuff.h"
#include "joystick.h"

#define MAX_JOY_BUTTON 8

JoystickConfig::JoystickConfig()
 : m_enableJoystickLbl(_("Enable Joystick")),
   m_paddleLbl(_("Paddle:")),
   m_brakeLbl(_("Brake:")),
   m_jumpLbl(_("Jump:")),
   m_trickLbl(_("Trick:"))
{	
	setTitle(_("Joystick Configuration"));
		
	ppogl::Vec2d position(40,350);
	ppogl::Vec2d position2(600,350);
	
	m_enableJoystickLbl.setPosition(position);
	m_enableJoystickBox.setPosition(position2);
	m_enableJoystickBox.alignment.right();
	m_enableJoystickBox.setSelected(!PPConfig.getBool("disable_joystick"));
	
	position.y()-=60;
	position2.y()-=60;
	
	//paddle button
	createButtonList(m_paddleListBox);
	m_paddleLbl.setPosition(position);
	m_paddleListBox.setPosition(position2);
	m_paddleListBox.alignment.right();
	m_paddleListBox.signalClicked.Connect(ppogl::CreateSlot(this,&JoystickConfig::paddleClicked));
	m_paddleListBox.setSelectedData(PPConfig.getInt("joystick_paddle_button"));
		
	position.y()-=40;
	position2.y()-=40;
	
	//brake button
	createButtonList(m_brakeListBox);
	m_brakeLbl.setPosition(position);
	m_brakeListBox.setPosition(position2);
	m_brakeListBox.alignment.right();
	m_brakeListBox.signalClicked.Connect(ppogl::CreateSlot(this,&JoystickConfig::brakeClicked));
	m_brakeListBox.setSelectedData(PPConfig.getInt("joystick_brake_button"));
	
	position.y()-=40;
	position2.y()-=40;
	
	//jump button
	createButtonList(m_jumpListBox);
	m_jumpLbl.setPosition(position);
	m_jumpListBox.setPosition(position2);
	m_jumpListBox.alignment.right();
	m_jumpListBox.signalClicked.Connect(ppogl::CreateSlot(this,&JoystickConfig::jumpClicked));
	m_jumpListBox.setSelectedData(PPConfig.getInt("joystick_jump_button"));
	
	position.y()-=40;
	position2.y()-=40;
	
	//trick button
	createButtonList(m_trickListBox);
	m_trickLbl.setPosition(position);
	m_trickListBox.setPosition(position2);
	m_trickListBox.alignment.right();
	m_trickListBox.signalClicked.Connect(ppogl::CreateSlot(this,&JoystickConfig::trickClicked));
	m_trickListBox.setSelectedData(PPConfig.getInt("joystick_trick_button"));
}

void
JoystickConfig::createButtonList(ppogl::ListBox<int> &list)
{
	char buff[4];
	for(int i=0; i<MAX_JOY_BUTTON; i++){
		snprintf(buff,4,"%d",i);
		list.addElement(buff, i);		
	}	
}

void
JoystickConfig::customLoop(double TimeStep)
{
	// Check joystick
    if(joystick.isActive() ){
		joystick.update();

		int button = joystick.getDownButton();
		if(button>-1){
			if(m_paddleListBox.hasFocus()){
				m_paddleListBox.setSelectedData(button);
			}else if(m_brakeListBox.hasFocus()){
				m_brakeListBox.setSelectedData(button);
			}else if(m_jumpListBox.hasFocus()){
				m_jumpListBox.setSelectedData(button);
			}else if(m_trickListBox.hasFocus()){
				m_trickListBox.setSelectedData(button);
			}
		}
    }
}

void
JoystickConfig::apply()
{
	PPConfig.setBool("disable_joystick",!m_enableJoystickBox.isSelected());
	PPConfig.setInt("joystick_paddle_button", m_paddleListBox.getSelectedData());
	PPConfig.setInt("joystick_brake_button", m_brakeListBox.getSelectedData());
	PPConfig.setInt("joystick_jump_button", m_jumpListBox.getSelectedData());
	PPConfig.setInt("joystick_trick_button", m_trickListBox.getSelectedData());	
	
	write_config_file();
	setMode( GameMode::prevmode );
}

void
JoystickConfig::paddleClicked()
{
	m_paddleListBox.setFocus();
}

void
JoystickConfig::brakeClicked()
{
	m_brakeListBox.setFocus();
}

void
JoystickConfig::jumpClicked()
{
	m_jumpListBox.setFocus();
}

void
JoystickConfig::trickClicked()
{
	m_trickListBox.setFocus();
}
