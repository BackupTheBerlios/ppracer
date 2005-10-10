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
 
/*
 * Wrapper for systemdependants functions/features
 */ 

#include "os.h"

#include "assert.h"

#include <unistd.h>
#include <stdarg.h> 
#include <stdio.h> 
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

#define MAX_BUFFER_SIZE 512

namespace ppogl {
namespace os {

bool
chdir(const std::string& dir)
/// change the current working directory
{	
	PP_REQUIRE(dir.empty()!=true,"Directory name is empty"); 

	PP_LOG(LogOS,"Changing directory: " << dir);
	
	if(::chdir(dir.c_str())){
		PP_PEDANTIC("Error changing working directory: " << dir);
		return false;
	}else{
		return true;
	}
}

std::string
cwd()
/// returns the current working directory
{
	char dir[MAX_BUFFER_SIZE];
	
	if(getcwd(dir, MAX_BUFFER_SIZE)!=NULL){
		return dir;
	}else{
		PP_WARNING("Unable to get working directory");
		return "";
	}
}

bool
mkdir(const std::string& dir)
/// create directory
{
	if(::mkdir(dir.c_str(),S_IRWXU)){
		PP_PEDANTIC("Error creating directory: " << dir);
		return false;
	}else{
		return true;	
	}	
}

dir::dir(std::string path, const bool only_dirs)
/// Constructs a std::list with the directory content
{
	DIR *dir;
	if((dir=opendir(path.c_str())) == NULL){
		PP_WARNING("Unable to open directory: " << path);
		return;
	}
	
	if(!path.empty()) path+="/"; 
	
	struct dirent *entry;
	while((entry=readdir(dir)) != NULL){
		if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..")){
			continue;
		}
		
		if(only_dirs){
			struct stat attrs;
			if(lstat((path+entry->d_name).c_str(),&attrs)==-1){
				continue;
			}else{
				if(!S_ISDIR(attrs.st_mode)){
					continue;
				}		
			}
		}
		push_back(entry->d_name);
	}
	
	closedir(dir);
}

bool
isFile(const std::string& name, const bool follow_link)
///returns true if name points to a regular file
{
	struct stat attrs;
	
	if(follow_link){
		if(stat(name.c_str(),&attrs)==-1){
			PP_PEDANTIC("Unable to get info for file: " << name); 
			return false;
		}		
	}else{
		if(lstat(name.c_str(),&attrs)==-1){
			PP_PEDANTIC("Unable to get info for file: " << name); 
			return false;
		}	
	}		
	return S_ISREG(attrs.st_mode);
}

bool
isDirectory(const std::string& name, const bool follow_link)
///returns true if name points to a directory
{
	struct stat attrs;
	
	if(follow_link){
		if(stat(name.c_str(),&attrs)==-1){
			PP_PEDANTIC("Unable to get info for directory: " << name); 
			return false;
		}		
	}else{
		if(lstat(name.c_str(),&attrs)==-1){
			PP_PEDANTIC("Unable to get info for directory: " << name); 
			return false;
		}	
	}		
	return S_ISDIR(attrs.st_mode);
}

/// base directory
static std::string baseDir;

void
setBaseDir()
/// set current working directory to be base directory
{
	baseDir=os::cwd();
}

const std::string&
getBaseDir()
/// return base directory
{
	return baseDir;
}


std::string
getHomeDir()
/// get user's home directory
{
	// get curent userid
	const int userid = getuid();
	
	struct passwd* pw;
		
	if( (pw=getpwuid(userid)) == NULL){
		PP_WARNING("Unable to get home directory -> userid: " << userid);
		return "";
	}
	
	if(pw->pw_dir==NULL){
		PP_WARNING("Unable to get home directory -> userid: " << userid);
		return "";
	}else{
		std::string dir = pw->pw_dir;
		return dir + "/";
	}
}

/// user directory
static std::string userDir;

void
setUserDir(const std::string& dir)
/// set directory for storing user data
{
	userDir=dir;
}

const std::string&
getUserDir()
/// returns the directory for storing user data
{
	return userDir;
}

} // namespace os
} // namespace ppogl
