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

#ifndef _PPOGL_SCRIPT_H_
#define _PPOGL_SCRIPT_H_

#include <string>

extern "C"{
// this is copied from squirrel.h so we don't need to include it here
typedef struct SQVM* HSQUIRRELVM;
typedef int SQInteger;
typedef SQInteger SQRESULT;
}

namespace ppogl{
	
class Script;
	
typedef int ScriptFunc(Script *vm);

class Script
{
public:
	struct Lib
	{
		const char* name;
		ScriptFunc* function;
	};

protected:
	HSQUIRRELVM m_vm;

	void bindStuff();
	void prepare();
	
public:
	Script();
	~Script();

	bool doFile(const std::string& filename);

	void registerLib(const std::string& name, const Script::Lib *callbacks);

	static int defaultCb(HSQUIRRELVM vm);


	bool isKeyInTable(const std::string& key, int index=-1);
	
	const std::string getStringFromTable(const std::string& key, int index=-1);
	double getFloatFromTable(const std::string& key, int index=-1);
	int getIntFromTable(const std::string& key, int index=-1);
	bool getBoolFromTable(const std::string& key, int index=-1);

	const std::string getString(int index=-1);
	double getFloat(int index=-1);
	int getInt(int index=-1);
	bool getBool(int index=-1);
	
	bool isNull(int index=-1);
	bool isString(int index=-1);
	bool isFloat(int index=-1);
	bool isInt(int index=-1);
	bool isBool(int index=-1);
	bool isTable(int index=-1);
	bool isArray(int index=-1);
	
	void pushNull();
	void pushString(const std::string& string);
	void pushFloat(double number);
	void pushInt(int number);
	void pushBool(bool boolean);
	
	void newArray(int size = 0);

	bool get(int index=-2);
	void set(int index=-3);
	
	bool rawGet(int index=-2);
	void rawSet(int index=-3);
	
	void arrayAppend(int index=-2);
	
	bool next(int index=-1);
	
	void pop(int n=1);
	
	int getTop();	
	
	void pushRootTable();
	
	bool call(int arguments, bool retValue=false);
	
	SQRESULT throwError(const std::string& message);
	SQRESULT defaultError();
};

} //namespace ppogl

#endif // _PPOGL_SCRIPT_H_
