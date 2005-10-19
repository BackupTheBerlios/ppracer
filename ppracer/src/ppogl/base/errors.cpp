/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
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
 
#include "errors.h"
#include "os.h"
#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace ppogl {
	
static const char* LevelMessages[] = {
	"NULL",
	"PPOGL",
	"Base",
	"Audio",
	"UI",
	"Font",
	"Images",
	"Textures",
	"OS",
	"Models"
};	

// the pointer to the one and only instance (singleton)
Log* Log::sm_instance = NULL;

	
/// Returns the one and only instance
Log*
Log::Instance()
{
	/// check whether there is an instance
	/// and create a new one if it fails.
	if(sm_instance==NULL){
		sm_instance = new Log();
	}	
	return sm_instance;
}

Log::Log()
 : m_level(LogWarning),
   m_verbose(false)
{
	m_logfile.std::ios::rdbuf(std::cerr.rdbuf());
}


Log::~Log()
{
}
	

/// Set the file for the log.
void
Log::setFilename(const std::string& filename)
{
	if(filename.empty()){
		//reset logfile to stderr
		PP_WARNING("unable to open logfile: filename is empty");
	}else{
		//try to open logfile
		std::ofstream* filestr = new std::ofstream;
		filestr->open(filename.c_str());
		if(filestr->is_open()){
			m_logfile.std::ios::rdbuf(filestr->rdbuf());
		}else{
			m_logfile.std::ios::rdbuf(std::cerr.rdbuf());
			PP_WARNING("unable to open logfile: " << filename << " -> in: " << ppogl::os::cwd());
		}
	}
}
	
/// Set the logging level
void
Log::setLevel(int level)
{
	m_level=level;
}

/// Set log system to use long messages
void
Log::setVerbose(bool verbose)
{
	m_verbose=verbose;	
}

/// Sends the message to std::cerr or the logfile.
void 
Log::mesg(int mode,
			const std::string& file, int line,
			const std::string& message)
{
	//check if we should handle this message
	if(checkLevel(mode)==false) return;
		
	if(mode==LogMessage){
		m_logfile << "Message: ";
	}else if(mode==LogWarning){
		m_logfile << "Warning: ";
	}else if(mode==LogError){
		m_logfile << "Error: ";
	}else if(mode==LogPedantic){
		m_logfile << "Pedantic Warning: ";
	}else if(mode<-1000 && mode > int(PP_NUM_ELEMENTS(LevelMessages))*(-1)-1000){
		m_logfile << "Log("<< LevelMessages[mode*(-1)-1000] << "): ";		
	}else{
		m_logfile << "Log("<< mode << "): ";
	}
		
	m_logfile << message << std::endl;

	if(m_verbose){	
		m_logfile << "File: " << file << std::endl;
		m_logfile << "Line: " << line << "\n" << std::endl;
	}
	
	if(mode==LogError){
		throw ppogl::Error();
	}	
}

/// Returns ostream for logging
std::ostream&
Log::getStream()
{
	return m_logfile;
}

bool
Log::checkLevel(int mode)
{
	if(mode>0){
		// custom mode
		// todo: implement me!!!
		//if(m_logfile){
		//	return true;
		//}else{		
			return false;
		//}
	}else{
		// default modes
		return(mode<=m_level);
	}
}


} //namespace ppogl
