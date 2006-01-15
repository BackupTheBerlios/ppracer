/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005-2006 Volker Stroebel <volker@planetpenguin.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
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
 
#include "config.h"

#include "base/glwrappers.h"
#include "ppogl_script.h"

#include <iostream>


namespace ppogl{
	
PPOGL_SINGLETON(Config);
	
Config::Config()
 : m_vm(NULL)
{
}
	
void
Config::init(ppogl::Script *vm, const std::string& table)
/// initialize the Config singleton with the Script vm and the table name
{
	m_vm = vm;
	m_table = table;
}

double
Config::getDouble(const std::string& key)
/// return double value from config table
{
	if(m_vm==NULL){
		PP_WARNING("Configuration is not initialized");
		return 0;
	}
	m_vm->pushRootTable();
	m_vm->pushString(m_table);
	if(!m_vm->get()){
		PP_WARNING("Table doesn't exists: " << m_table);
		m_vm->pop();
		return 0;
	}else{
		PP_ASSERT(m_vm->isKeyInTable(key)==true,"Unable to find key in configuration: " << key);	
		double value = m_vm->getFloatFromTable(key);
		m_vm->pop(2);
		return value;
	}
}

int
Config::getInt(const std::string& key)
/// return integer value from config table
{
	if(m_vm==NULL){
		PP_WARNING("Configuration is not initialized");
		return 0;
	}
	m_vm->pushRootTable();
	m_vm->pushString(m_table);
	if(!m_vm->get()){
		PP_WARNING("Table doesn't exists: " << m_table);
		m_vm->pop();
		return 0;
	}else{
		PP_ASSERT(m_vm->isKeyInTable(key)==true,"Unable to find key in configuration: " << key);	
		int value = m_vm->getIntFromTable(key);
		m_vm->pop(2);
		return value;
	}
}

std::string
Config::getString(const std::string& key)
/// return string from config table
{
	if(m_vm==NULL){
		PP_WARNING("Configuration is not initialized");
		return "";
	}
	
	m_vm->pushRootTable();
	m_vm->pushString(m_table);
	if(!m_vm->get()){
		PP_WARNING("Table doesn't exists: " << m_table);
		m_vm->pop();
		return "";
	}else{
		PP_ASSERT(m_vm->isKeyInTable(key)==true,"Unable to find key in configuration: " << key);	
		std::string value = m_vm->getStringFromTable(key);
		m_vm->pop(2);
		return value;
	}
}

bool
Config::getBool(const std::string& key)
/// return boolean value from config table
{
	if(m_vm==NULL){
		PP_WARNING("Configuration is not initialized");
		return false;
	}
	
	m_vm->pushRootTable();
	m_vm->pushString(m_table);
	if(!m_vm->get()){
		PP_WARNING("Table doesn't exists: " << m_table);
		m_vm->pop();
		return false;
	}else{
		PP_ASSERT(m_vm->isKeyInTable(key)==true,"Unable to find key in configuration: " << key);	
		bool value = m_vm->getBoolFromTable(key);
		m_vm->pop(2);
		return value;
	}
}

void
Config::setDouble(const std::string& key, double value)
/// set double value for key
{
	if(m_vm==NULL){
		PP_WARNING("Configuration is not initialized");
		return;
	}
	m_vm->pushRootTable();
	m_vm->pushString(m_table);
	if(!m_vm->get()){
		PP_WARNING("Table doesn't exists: " << m_table);
		m_vm->pop();
	}else{
		m_vm->pushString(key);
      	m_vm->pushFloat(value);
		m_vm->set(-3);
		m_vm->pop(2);
	}
}

void
Config::setInt(const std::string& key, int value)
/// set integer value for key
{
	if(m_vm==NULL){
		PP_WARNING("Configuration is not initialized");
		return;
	}
	m_vm->pushRootTable();
	m_vm->pushString(m_table);
	if(!m_vm->get()){
		PP_WARNING("Table doesn't exists: " << m_table);
		m_vm->pop();
	}else{
		m_vm->pushString(key);
      	m_vm->pushInt(value);
		m_vm->set(-3);
		m_vm->pop(2);
	}
}
	
void
Config::setString(const std::string& key, const std::string& value)
/// set string for key
{
	if(m_vm==NULL){
		PP_WARNING("Configuration is not initialized");
		return;
	}	
	m_vm->pushRootTable();
	m_vm->pushString(m_table);
	if(!m_vm->get()){
		PP_WARNING("Table doesn't exists: " << m_table);
		m_vm->pop();
	}else{
		m_vm->pushString(key);
      	m_vm->pushString(value);
		m_vm->set(-3);
		m_vm->pop(2);
	}
}

void
Config::setBool(const std::string& key, bool value)
/// set boolean value for key
{
	if(m_vm==NULL){
		PP_WARNING("Configuration is not initialized");
		return;
	}
	m_vm->pushRootTable();
	m_vm->pushString(m_table);
	if(!m_vm->get()){
		PP_WARNING("Table doesn't exists: " << m_table);
		m_vm->pop();
	}else{
		m_vm->pushString(key);
      	m_vm->pushBool(value);
		m_vm->set(-3);
		m_vm->pop(2);
	}
}

} //namepsace ppogl
