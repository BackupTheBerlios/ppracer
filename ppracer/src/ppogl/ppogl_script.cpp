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

#include "ppogl_script.h"

#include "base/errors.h"

// classes/functions to bind
#include "translation.h"
#include "font.h"
#include "textures.h"
#include "audio/audiomgr.h"

#include <squirrel.h> 
#include <sqstdio.h>
#include <sqstdstring.h>
#include <sqstdaux.h>

#include <stdarg.h> 
#include <stdio.h> 
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>

#include "base/os.h"

#define DEFAULT_STACKSIZE 2048


// some utility functions

static int
my_chdir(ppogl::Script *vm)
{
	std::string dir=vm->getString(1);
	bool result = ppogl::os::chdir(dir);
	vm->pushBool(result);
	return 1;
}

static int
my_cwd(ppogl::Script *vm)
{
	std::string dir = ppogl::os::cwd();
	if(dir.empty()){
		return 0;
	}else{
		vm->pushString(dir);
		return 1;
	}
}

static int
my_dir(ppogl::Script *vm)
{
	std::string path = vm->getString(1);
	bool only_dirs=false;
	if(vm->getTop()>=2){
		only_dirs = vm->getBool(2);
	}
	vm->pushRootTable();
	vm->newArray();		
	
	ppogl::os::dir dir(path,only_dirs);
	ppogl::os::dir::iterator it;
	for(it=dir.begin(); it!=dir.end();it++){
		vm->pushString(*it);
		vm->arrayAppend();
	}
	return 1;
}

static int
my_isFile(ppogl::Script *vm)
{
	std::string path=vm->getString(1);
	bool result = ppogl::os::isFile(path);
	vm->pushBool(result);
	return 1;
}

static int
my_isDirectory(ppogl::Script *vm)
{
	std::string path=vm->getString(1);
	bool result = ppogl::os::isDirectory(path);
	vm->pushBool(result);
	return 1;
}

static int
my_getHomeDir(ppogl::Script *vm)
{
	std::string dir = ppogl::os::getHomeDir();
	vm->pushString(dir);
	return 1;
}

static int
my_getBaseDir(ppogl::Script *vm)
{
	std::string dir = ppogl::os::getBaseDir();
	vm->pushString(dir);
	return 1;
}

static int
my_getUserDir(ppogl::Script *vm)
{
	std::string dir = ppogl::os::getUserDir();
	vm->pushString(dir);
	return 1;
}

static void
my_printfunc(HSQUIRRELVM vm, const SQChar *s, ...) 
{ 
	va_list arglist; 
	va_start(arglist, s); 
	vprintf(s, arglist); 
	va_end(arglist); 
	fflush(stdout);
} 

namespace ppogl{

static const struct ppogl::Script::Lib ppogllib[]={
	{"cd",my_chdir},
	{"cwd",my_cwd},
	{"dir",my_dir},
	{"isFile",my_isFile},
	{"isDirectory",my_isDirectory},
	{"getHomeDir",my_getHomeDir},
	{"getBaseDir",my_getBaseDir},
	{"getUserDir",my_getUserDir},
	{NULL, NULL}
};
	
static const struct ppogl::Script::Lib translationlib[]={
	{"set", ppogl::Translation::sqSet},
	{"get", ppogl::Translation::sqGet},
	{"addLanguage", ppogl::Translation::sqAddLanguage},
	{NULL, NULL}
};

static const struct ppogl::Script::Lib fontlib[]={
	{"load", ppogl::FontMgr::sqLoad},
	{"bind", ppogl::FontMgr::sqBind},
	{NULL, NULL}
};

static const struct ppogl::Script::Lib texturelib[]={
	{"load", ppogl::TextureMgr::sqLoad},
	{"bind", ppogl::TextureMgr::sqBind},
	{NULL, NULL}
};

static const struct ppogl::Script::Lib audiolib[]={
	{"loadSound", ppogl::AudioMgr::sqLoadSound},
	{"bindSound", ppogl::AudioMgr::sqBindSound},
	{"loadMusic", ppogl::AudioMgr::sqLoadMusic},
	{"bindMusic", ppogl::AudioMgr::sqBindMusic},	
	{NULL, NULL}
};
	
Script::Script()
{
	m_vm = sq_open(DEFAULT_STACKSIZE);
	sqstd_seterrorhandlers(m_vm);
	sq_setprintfunc(m_vm, my_printfunc); //sets the print function
		
	if(m_vm==NULL){
		PP_ERROR("Unable to create squirrel vm");		
	}
	
	sq_pushroottable(m_vm);
	sqstd_register_iolib(m_vm);
	sqstd_register_stringlib(m_vm);
	
	registerLib("ppogl", ppogllib);
	
	registerLib("Translation", translationlib);
	registerLib("Fonts", fontlib);
	registerLib("Textures", texturelib);
	registerLib("Audio", audiolib);	
}

Script::~Script()
{
	sq_close(m_vm);
}

bool
Script::doFile(const std::string& filename)
{
	PP_MESSAGE("Running script: "<< filename);
	
	std::string::size_type pos;
	
	pos = filename.find_last_of('/');
	if(pos!=std::string::npos){
		ppogl::os::chdir(filename.substr(0,pos));
	}
	
	sq_pushroottable(m_vm);
	
	if(SQ_SUCCEEDED(sqstd_dofile(m_vm, _SC(filename.c_str()), SQFalse, SQTrue)))
	{
	}else{
		PP_WARNING("Error parsing script: " << filename.c_str());
		return false;
	}
	sq_pop(m_vm,1); 
	
	return true;
}

void 
Script::registerLib(const std::string& name, const Script::Lib *callbacks)
{
	sq_pushstring(m_vm,name.c_str(),-1);
	if(SQ_FAILED(sq_get(m_vm,-2))){
		sq_pushroottable(m_vm);
		sq_pushstring(m_vm,name.c_str(),-1);
		sq_newtable(m_vm);		
	}
	
	for(int i=0; callbacks[i].name!=NULL; i++){
		sq_pushstring(m_vm,callbacks[i].name,-1);
					
		sq_pushuserpointer(m_vm,reinterpret_cast<SQUserPointer *>(this));
		sq_pushuserpointer(m_vm,*(reinterpret_cast<const SQUserPointer *>(&(callbacks[i].function))));
				
    	sq_newclosure(m_vm,Script::defaultCb,2);
		sq_newslot(m_vm,-3,SQFalse);
	}
	sq_rawset(m_vm,-3);	
}

int
Script::defaultCb(HSQUIRRELVM vm)
{
	Script *script;
	ScriptFunc *func;
	
	sq_getuserpointer(vm,-1,reinterpret_cast<SQUserPointer *>(&script));
	sq_getuserpointer(vm,-2,reinterpret_cast<SQUserPointer *>(&func));

	sq_pop(vm,2);
	sq_remove(vm,1);
		
	return (*func)(script);
}

bool
Script::isKeyInTable(const std::string& key, int index)
{
	bool exists=false;
	
	pushString(key);
	if(get(index-1)){
		exists=true;
		pop();
	}
	
	return exists;
}

const std::string
Script::getStringFromTable(const std::string& key, int index)
{
	pushString(key);
	std::string string;
	
	if(get(index-1)){
		string = getString();
		pop();
	}
	
	return string;
}

double
Script::getFloatFromTable(const std::string& key, int index)
{
	pushString(key);
	double value = 0;
	if(get(index-1)){
		value = getFloat();
		pop();
	}
	return value;
}

int
Script::getIntFromTable(const std::string& key, int index)
{
	pushString(key);
	int value = 0;
	if(get(index-1)){
		value = getInt();
		pop();
	}
	return value;
}

bool
Script::getBoolFromTable(const std::string& key, int index)
{
	pushString(key);
	bool boolean = false;
	
	if(get(index-1)){
		boolean = getBool();
		pop();
	}
		
	return boolean;	
}

const std::string
Script::getString(int index)
{
	const SQChar* value;
	sq_getstring(m_vm, index, &value);
	if(value != NULL){
		return value;
	}else{
		return "";
	}
}
	
double
Script::getFloat(int index)
{
	SQFloat value=0;
	sq_getfloat(m_vm, index, &value);
	return value;
}

int
Script::getInt(int index)
{
	SQInteger value=0;
	sq_getinteger(m_vm, index, &value);
	return value;
}

	
bool
Script::getBool(int index)
{
	SQBool value;
	sq_getbool(m_vm, index, &value);
	return value;
}
	
bool
Script::isNull(int index)
{
	return sq_gettype(m_vm, index)==OT_NULL;
}
	
bool
Script::isString(int index)
{	
	return sq_gettype(m_vm, index)==OT_STRING;
}
	
bool
Script::isFloat(int index)
{
	return sq_gettype(m_vm, index)==OT_FLOAT;
}

bool
Script::isInt(int index)
{
	return sq_gettype(m_vm, index)==OT_INTEGER;
}
	
bool
Script::isBool(int index)
{
	return sq_gettype(m_vm, index)==OT_BOOL;
}
	
bool
Script::isTable(int index)
{
	return sq_gettype(m_vm, index)==OT_TABLE;
}

bool
Script::isArray(int index)
{
	return sq_gettype(m_vm, index)==OT_ARRAY;
}



void
Script::pushNull()
{
	sq_pushnull(m_vm);
}

void
Script::pushString(const std::string& string)
{
	sq_pushstring(m_vm, string.c_str(), -1);
}

void
Script::pushFloat(double number)
{
	sq_pushfloat(m_vm, number);
}

void
Script::pushInt(int number)
{
	sq_pushinteger(m_vm, number);
}


void
Script::pushBool(bool boolean)
{
	sq_pushbool(m_vm, boolean);
}
	
void
Script::newArray(int size)
{
	sq_newarray(m_vm,size);	
}

bool
Script::get(int index)
{
	return SQ_SUCCEEDED(sq_get(m_vm, index));
}

void
Script::set(int index)
{
	sq_set(m_vm, index);
}
	
bool
Script::rawGet(int index)
{
	return SQ_SUCCEEDED(sq_rawget(m_vm, index));
}

void
Script::rawSet(int index)
{
	sq_rawset(m_vm, index);
}
	
void
Script::arrayAppend(int index)
{
	sq_arrayappend(m_vm, index);	
}

bool
Script::next(int index)
{
	return SQ_SUCCEEDED(sq_next(m_vm, index));
}
	
void 
Script::pop(int n)
{
	sq_pop(m_vm,n);
}
	
int
Script::getTop()
{
	return sq_gettop(m_vm);
}
	
void
Script::pushRootTable()
{
	sq_pushroottable(m_vm);
}

bool
Script::call(int arguments, bool retValue)
{
	return SQ_SUCCEEDED(sq_call(m_vm,arguments,retValue));
}

SQRESULT
Script::throwError(const std::string& message)
{
	return sq_throwerror(m_vm,message.c_str());	
}
	
SQRESULT
Script::defaultError()
{
	return sq_throwerror(m_vm,"Function call contains errors. Please check your script");	
}

} // namespace ppogl
