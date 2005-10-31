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
 
#ifndef _PPOGL_ERRORS_H
#define _PPOGL_ERRORS_H

#include <iostream>
#include <fstream>
#include <sstream>

///PlanetPenguin Open Game Library
namespace ppogl {
	
/// The predefined types for the log message.
/// All values are <= 0.
/// Therefore developers can freely use values >0 for custom modes
enum{
	LogUnknown  =  0,
	LogMessage  = -1000,
	
	LogPPOGL	= -1001,
	LogBase		= -1002,
	LogAudio	= -1003,	
	LogUI		= -1004,
	LogFonts	= -1005,
	LogImages	= -1006,
	LogTextures	= -1007,
	LogOS		= -1008,
	LogModels	= -1009,
	
	LogWarning  = -2000,
	LogError    = -3000,
	
	LogPedantic = -10000
};

///The base class for exceptions
/**All custom exceptions should be derived from this class*/
class Error
{	
public:
	virtual void printMessage();
};

/// The global log system (singleton) 
class Log
{
public:
	
	static Log* Instance();

	void setFilename(const std::string& filename);
	void setLevel(int level);
	void setVerbose(bool verbose);

	void mesg(int mode,
				const std::string& file, int line,
				const std::string& description);

	std::ostream& getStream();

	~Log();

private:
	Log();

	bool checkLevel(int mode);

	/// the pointer to the one and only instance (singleton)
	static Log* sm_instance;
		
	/// the current logging level
	int m_level;

	/// a pointer to the logfile stream (if spezified)
	std::ofstream m_logfile;

	/// show long log format
	bool m_verbose;
};

#ifndef PPOGL_NO_LOGGIN

/// Logs custom messages.
#define PP_LOG(mode, message) \
{	\
	std::stringstream tmp(std::ios::in| std::ios::out | std::ios::trunc);	\
	tmp << message;	\
	ppogl::Log::Instance()->mesg(mode, __FILE__, __LINE__, tmp.str()); \
}

/// A normal message.
/// Use this if you want to give the user a hint.
#define PP_MESSAGE(message)	PP_LOG(ppogl::LogMessage, message)

/// A warning message.
/// Use this for problems the program can handle.
#define PP_WARNING(message)	PP_LOG(ppogl::LogWarning, message)

/// An error message.
/// Use this for errors the program can't handle.
/// This will throw an pp::Error object after logging the message.
#define PP_ERROR(message) PP_LOG(ppogl::LogError, message)

#else

#define PP_LOG(mode,message)
#define PP_MESSAGE(message)
#define PP_WARNING(message)
#define PP_ERROR(message)

#endif

/// Code that cannot be reach should be marked with this.
#define PP_NOT_REACHED()	\
	ppogl::Log::Instance()->mesg(ppogl::LogError, __FILE__, __LINE__, "unreachable code reached")

#ifdef PPOGL_PEDANTIC
/// A padantic message.
/// Only use this for pedantic checks that aren't realy neccesarry for standard builds.
/// This is only activated if PPOGL_PEDANTIC is defined
#define PP_PEDANTIC(message)	\
{	\
	std::stringstream tmp(std::ios::in| std::ios::out | std::ios::trunc);	\
	tmp << message;	\
	ppogl::Log::Instance()->mesg(ppogl::LogPedantic, __FILE__, __LINE__, tmp.str()); \
}
#else
	#define PP_PEDANTIC(message)
#endif

} //namespace ppogl

#endif // _PPOGL_ERRORS_H
