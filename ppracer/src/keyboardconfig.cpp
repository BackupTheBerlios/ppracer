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
 
#include "keyboardconfig.h"

#include "stuff.h"
#include "game_mgr.h"

#include <string.h>
#include <stdlib.h>	
	
struct KeyMap{
	SDLKey key;
	const char *string;
};

static KeyMap keymap[] = {
	{SDLK_BACKSPACE,"backspace"},
	{SDLK_TAB,"tab"},
	{SDLK_CLEAR,"clear"},
	{SDLK_PAUSE,"pause"},
	{SDLK_SPACE,"space"},
	{SDLK_DELETE,"delete"},
		
	{SDLK_KP0,"keypad 0"},
	{SDLK_KP1,"keypad 1"},
	{SDLK_KP2,"keypad 2"},
	{SDLK_KP3,"keypad 3"},
	{SDLK_KP4,"keypad 4"},
	{SDLK_KP5,"keypad 5"},
	{SDLK_KP6,"keypad 6"},
	{SDLK_KP7,"keypad 7"},
	{SDLK_KP8,"keypad 8"},
	{SDLK_KP9,"keypad 9"},
	{SDLK_KP_PERIOD,"keypad ."},
	{SDLK_KP_DIVIDE,"keypad /"},
	{SDLK_KP_MULTIPLY,"keypad *"},
	{SDLK_KP_MINUS,"keypad -"},
	{SDLK_KP_PLUS,"keypad +"},
	{SDLK_KP_EQUALS,"keypad ="},
	{SDLK_KP_ENTER,"keypad enter"},

	{SDLK_LEFT,"left"},
	{SDLK_RIGHT,"right"},
	{SDLK_UP,"up"},
	{SDLK_DOWN,"down"},
	
	{SDLK_INSERT,"insert"},
	{SDLK_HOME,"home"},
	{SDLK_END,"end"},
	{SDLK_PAGEUP,"page up"},
	{SDLK_PAGEDOWN,"page down"},
	
	{SDLK_F1,"F1"},
	{SDLK_F2,"F2"},
	{SDLK_F3,"F3"},
	{SDLK_F4,"F4"},
	{SDLK_F5,"F5"},
	{SDLK_F6,"F6"},
	{SDLK_F7,"F7"},
	{SDLK_F8,"F8"},
	{SDLK_F9,"F9"},
	{SDLK_F10,"F10"},
	{SDLK_F11,"F11"},
	{SDLK_F12,"F12"},
	{SDLK_F13,"F13"},
	{SDLK_F14,"F14"},
	{SDLK_F15,"F15"},
	
	{SDLK_NUMLOCK,"numlock"},
	{SDLK_CAPSLOCK,"capslock"},
	{SDLK_SCROLLOCK,"scrollock"},
	{SDLK_RSHIFT,"right shift"},
	{SDLK_LSHIFT,"left shift"},
	{SDLK_RCTRL,"right ctrl"},
	{SDLK_LCTRL,"left ctrl"},
	{SDLK_RALT,"right alt"},
	{SDLK_LALT,"left alt"},
	{SDLK_RMETA,"right meta"},
	{SDLK_LMETA,"left meta"},
	{SDLK_LSUPER,"left windows key"},
	{SDLK_RSUPER,"right windows key"},
	{SDLK_MODE,"mode shift"},
	{SDLK_HELP,"help"},
	{SDLK_PRINT,"print-screen"},
	{SDLK_SYSREQ,"SysRq"},
	{SDLK_BREAK,"break"},
	{SDLK_MENU,"menu"},
	{SDLK_POWER,"power"},
	{SDLK_EURO,"euro"}	
};

	
KeyboardConfig::KeyboardConfig()
 : m_leftLbl(_("Turn left:")),
   m_rightLbl(_("Turn right:")),
   m_paddleLbl(_("Paddle:")),
   m_brakeLbl(_("Brake:")),
   m_jumpLbl(_("Jump:")),
   m_trickLbl(_("Trick:")),
   m_resetLbl(_("Reset:")),
   m_pauseLbl(_("Pause:"))
{
	setTitle(_("Keyboard Configuration"));
	
	ppogl::Vec2d position(40,350);
	ppogl::Vec2d position2(600,350);
	
	m_leftLbl.setPosition(position);
	m_leftEntry.setPosition(position2);
	m_leftEntry.alignment.right();
	m_leftEntry.setEditable(false);
	m_leftEntry.signalKeyPressed.Connect(ppogl::CreateSlot(this,&KeyboardConfig::setKey));
   	m_leftEntry.setText(getKey(SDLKey(PPConfig.getInt("turn_left_key"))));
		
	position.y()-=40;
	position2.y()-=40;	
	
	m_rightLbl.setPosition(position);
	m_rightEntry.setPosition(position2);
	m_rightEntry.alignment.right();
	m_rightEntry.setEditable(false);
	m_rightEntry.signalKeyPressed.Connect(ppogl::CreateSlot(this,&KeyboardConfig::setKey));
   	m_rightEntry.setText(getKey(SDLKey(PPConfig.getInt("turn_right_key"))));
	
	position.y()-=40;
	position2.y()-=40;
	
	m_paddleLbl.setPosition(position);
	m_paddleEntry.setPosition(position2);
	m_paddleEntry.alignment.right();
	m_paddleEntry.setEditable(false);
	m_paddleEntry.signalKeyPressed.Connect(ppogl::CreateSlot(this,&KeyboardConfig::setKey));
   	m_paddleEntry.setText(getKey(SDLKey(PPConfig.getInt("paddle_key"))));
		
	position.y()-=40;
	position2.y()-=40;	
		
	m_brakeLbl.setPosition(position);
	m_brakeEntry.setPosition(position2);
	m_brakeEntry.alignment.right();
	m_brakeEntry.setEditable(false);
	m_brakeEntry.signalKeyPressed.Connect(ppogl::CreateSlot(this,&KeyboardConfig::setKey));
   	m_brakeEntry.setText(getKey(SDLKey(PPConfig.getInt("brake_key"))));
	
	position.y()-=40;
	position2.y()-=40;	
		
	m_jumpLbl.setPosition(position);
	m_jumpEntry.setPosition(position2);
	m_jumpEntry.alignment.right();
	m_jumpEntry.setEditable(false);
	m_jumpEntry.signalKeyPressed.Connect(ppogl::CreateSlot(this,&KeyboardConfig::setKey));
   	m_jumpEntry.setText(getKey(SDLKey(PPConfig.getInt("jump_key"))));
	
	position.y()-=40;
	position2.y()-=40;	
	
	m_trickLbl.setPosition(position);
	m_trickEntry.setPosition(position2);
	m_trickEntry.alignment.right();
	m_trickEntry.setEditable(false);
	m_trickEntry.signalKeyPressed.Connect(ppogl::CreateSlot(this,&KeyboardConfig::setKey));
   	m_trickEntry.setText(getKey(SDLKey(PPConfig.getInt("trick_modifier_key"))));
	
	position.y()-=40;
	position2.y()-=40;	
	
	m_resetLbl.setPosition(position);
	m_resetEntry.setPosition(position2);
	m_resetEntry.alignment.right();
	m_resetEntry.setEditable(false);
	m_resetEntry.signalKeyPressed.Connect(ppogl::CreateSlot(this,&KeyboardConfig::setKey));
   	m_resetEntry.setText(getKey(SDLKey(PPConfig.getInt("reset_key"))));	

	position.y()-=40;
	position2.y()-=40;	
	
	m_pauseLbl.setPosition(position);
	m_pauseEntry.setPosition(position2);
	m_pauseEntry.alignment.right();
	m_pauseEntry.setEditable(false);
	m_pauseEntry.signalKeyPressed.Connect(ppogl::CreateSlot(this,&KeyboardConfig::setKey));
   	m_pauseEntry.setText(getKey(SDLKey(PPConfig.getInt("pause_key"))));	
}

void
KeyboardConfig::apply()
{
	PPConfig.setInt("turn_left_key", getKey(m_leftEntry.getText()));
	PPConfig.setInt("turn_right_key", getKey(m_rightEntry.getText()));		
	PPConfig.setInt("paddle_key", getKey(m_paddleEntry.getText()));		
	PPConfig.setInt("brake_key", getKey(m_brakeEntry.getText()));		
	PPConfig.setInt("jump_key", getKey(m_jumpEntry.getText()));		
	PPConfig.setInt("trick_modifier_key", getKey(m_trickEntry.getText()));		
	PPConfig.setInt("reset_key", getKey(m_resetEntry.getText()));		
	PPConfig.setInt("pause_key", getKey(m_pauseEntry.getText()));		
	
	write_config_file();
	setMode( GameMode::prevmode );
}

void
KeyboardConfig::setKey(ppogl::Entry *widget, SDLKey key)
{
	widget->setText(getKey(key));
}

std::string KeyboardConfig::getKey(SDLKey key)
{
	std::string content;
	
	for(unsigned int i=0;i<sizeof(keymap)/sizeof(KeyMap);i++){
		if(key==keymap[i].key){
			content=keymap[i].string;
			break;
		}
	}
		
	if(content.empty()){
		if(isprint(key)){
			content=key;
		}else{
			char temp[10];
			snprintf(temp,12,"%d",key);
			content = "key ";
			content += temp;
		}
	}
	
	return content;
}

SDLKey
KeyboardConfig::getKey(std::string& string)
{
	if(string!=""){
		for(unsigned int i=0;i<sizeof(keymap)/sizeof(KeyMap);i++){
			if(string==keymap[i].string){
				return keymap[i].key;
			}
		}
		
		if(string.length()>5){
			if(!strncmp("key ",string.c_str(),4)){
				int key = atoi(string.c_str()+4);
				return SDLKey(key);				
			}
		}
		
		return SDLKey(string.c_str()[0]);		
	}else{
		return SDLKey(0);
	}
}
