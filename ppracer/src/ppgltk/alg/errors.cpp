/*
 * PPGLTK
 *
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
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
 
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace pp {
	
Log* Log::sm_instance = NULL;

Log*
Log::Instance()
{
	if(sm_instance==NULL){
		sm_instance = new Log();
	}	
	return sm_instance;
}

Log::Log()

 : m_level(0),
   mp_logfile(NULL)
{
	
}


Log::~Log()
{
	
}
	
void
Log::setFilename(const char* filename)
{
	if(mp_logfile!=NULL){
		delete mp_logfile;
	}
	
	if(filename==NULL){
		//not using a logfile
		mp_logfile=NULL;	
	}else{
		//try to open logfile
		mp_logfile = new std::ofstream(filename);
		
		if(mp_logfile==NULL){
			PP_WARNING("unable to open logfile: %s",filename);
		}
	}	
}
	
void
Log::setLevel(int level)
{
	m_level=level;
}


void 
Log::message(int mode,
			const char* file, int line,
			const char* format,...)
{
	if(checkLevel(mode)==false)return;
	
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
	}else{
		*logstream << "Log("<< mode << "): ";
	}
		
	{
		va_list args;
		va_start( args, format );

		char buffer[512]; 
		vsnprintf(buffer,512, format, args);
		*logstream << buffer << std::endl;
		
		va_end( args );
	}
	
	*logstream << "File: " << file << std::endl;
	*logstream << "Line: " << line << "\n" << std::endl;
		
	if(mode==LogError){
		throw pp::Error();
	}	
}

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


} //namespace pp
