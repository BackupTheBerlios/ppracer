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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace ppogl {

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
   mp_logfile(NULL)
{
}


Log::~Log()
{
}
	

/// Set the file for the log.
/// You can reset it with NULL.
void
Log::setFilename(const std::string& filename)
{
	if(mp_logfile!=NULL){
		delete mp_logfile;
	}
	
	if(filename.empty()){
		//not using a logfile
		mp_logfile=NULL;
	}else{
		//try to open logfile
		mp_logfile = new std::ofstream(filename.c_str());
		
		if(mp_logfile==NULL){
			PP_WARNING("unable to open logfile: " << filename);
		}
	}	
}
	
/// Set the logging level
void
Log::setLevel(int level)
{
	m_level=level;
}


/// Sends the message to std::cerr or the logfile.
void 
Log::mesg(int mode,
			const std::string& file, int line,
			const std::string& message)
{
	//check if we should handle this message
	if(checkLevel(mode)==false) return;
	
	std::ostream *logstream;
	
	if(mp_logfile==NULL){
		logstream = &std::cerr;
	}else{
		logstream = mp_logfile;
	}
		
	if(mode==LogMessage){
		*logstream << "Message: ";
	}else if(mode==LogWarning){
		*logstream << "Warning: ";
	}else if(mode==LogError){
		*logstream << "Error: ";
	}else if(mode==LogPedantic){
		*logstream << "Pedantic Warning: ";
	}else{
		*logstream << "Log("<< mode << "): ";
	}
		
	*logstream << message << std::endl;
	*logstream << "File: " << file << std::endl;
	*logstream << "Line: " << line << "\n" << std::endl;
		
	if(mode==LogError){
		throw ppogl::Error();
	}	
}

/// Returns ostream for logging
std::ostream*
Log::getStream()
{
	if(mp_logfile==NULL){
		return &std::cerr;
	}else{
		return mp_logfile;
	}
}

bool
Log::checkLevel(int mode)
{
	if(mode>0){
		// custom mode
		// todo: implment me!!!
		if(mp_logfile){
			return true;
		}else{		
			return false;
		}
	}else{
		// default modes
		return(mode<=m_level);
	}
}


} //namespace ppogl