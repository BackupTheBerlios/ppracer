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
 
#ifndef _PP_ERRORS_H
#define _PP_ERRORS_H

#include <stdarg.h>

#include <fstream>



namespace pp {
	
enum{
	LogMessage  = -1,
	LogWarning  = -2,
	LogError = -3,
};


class Error
{	
public:
	Error(){};
	virtual void printMessage(){};		
};
	


class Log
{
public:
	
	static Log* Instance();

	void setFilename(const char* filename);
	void setLevel(int level);

	void message(int mode,
				const char* file, int line,
				const char* format,...);

	std::ostream* getStream();

	~Log();

private:
	Log();

	bool checkLevel(int mode);

	static Log* sm_instance;
		

	int m_level;
	std::ofstream* mp_logfile;
};


#ifndef PPGLTK_NO_LOGGIN

#define PP_LOG(mode, ...)	\
	pp::Log::Instance()->message(mode, __FILE__, __LINE__, __VA_ARGS__)

#define PP_MESSAGE(...)	\
	pp::Log::Instance()->message(pp::LogMessage, __FILE__, __LINE__, __VA_ARGS__)

#define PP_WARNING(...)	\
	pp::Log::Instance()->message(pp::LogWarning, __FILE__, __LINE__, __VA_ARGS__)

#define PP_ERROR(...)	\
	pp::Log::Instance()->message(pp::LogError, __FILE__, __LINE__, __VA_ARGS__)

#else

#define PP_LOG(mode, ...)
#define PP_MESSAGE(...)
#define PP_WARNING(...)
#define PP_ERROR(...)

#endif


#define PP_NOT_REACHED()	\
	pp::Log::Instance()->message(pp::LogError, __FILE__, __LINE__, "unreachable code reached")


} //namespace pp

#endif // _PP_ERRORS_H
