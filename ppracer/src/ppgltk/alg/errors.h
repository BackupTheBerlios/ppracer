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
	
/// The predefined types for the log message.
/// All values are <= 0.
/// Therefore developers can freely use values >0 for custom modes
enum{
	LogUnknown  =  0,
	LogMessage  = -1,
	LogWarning  = -2,
	LogError    = -3,
};


/// The base class for exceptions
/// All custom exception should be derived from this class
class Error
{	
public:
	Error(){};
	virtual void printMessage(){};		
};
	

/// The global log system (singleton)
/// that sends log messages to std::cerr or a file
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

	/// the pointer to the one and only instance (singleton)
	static Log* sm_instance;
		
	/// the current logging level
	int m_level;

	/// a pointer to the logfile stream (if spezified)
	std::ofstream* mp_logfile;
};


#ifndef PPGLTK_NO_LOGGIN

/// Logs custom messages.
#define PP_LOG(mode, ...)	\
	pp::Log::Instance()->message(mode, __FILE__, __LINE__, __VA_ARGS__)


/// A normal message.
/// Use this if you want to give the user a hint.
#define PP_MESSAGE(...)	\
	pp::Log::Instance()->message(pp::LogMessage, __FILE__, __LINE__, __VA_ARGS__)

/// A warning message.
/// Use this for problems the program can handle.
#define PP_WARNING(...)	\
	pp::Log::Instance()->message(pp::LogWarning, __FILE__, __LINE__, __VA_ARGS__)

/// An error message.
/// Use this for errors the program can't handle.
/// This will throw an empty pp::Error object after logging the message.
#define PP_ERROR(...)	\
	pp::Log::Instance()->message(pp::LogError, __FILE__, __LINE__, __VA_ARGS__)

#else

#define PP_LOG(mode, ...)
#define PP_MESSAGE(...)
#define PP_WARNING(...)
#define PP_ERROR(...)

#endif


/// Code that cannot be reach should be marked with this.
#define PP_NOT_REACHED()	\
	pp::Log::Instance()->message(pp::LogError, __FILE__, __LINE__, "unreachable code reached")


} //namespace pp

#endif // _PP_ERRORS_H
