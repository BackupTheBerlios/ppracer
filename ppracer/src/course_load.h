/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * Copyright (C) 1999-2001 Jasmin F. Patry
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
#ifndef _COURSE_LOAD_H_
#define _COURSE_LOAD_H_

#include "course_mgr.h"

#include "ppogl/models/model.h"
#include "ppogl/textures.h"
#include "ppogl/sg/vnc.h"

#include <list>
#include <string>

class Course
{
private:
	static std::string sm_loadedCourse;
	static CourseData::Condition sm_loadedCondition;
	
public:
	
	static ppogl::Vec2d dimension;
	static ppogl::Vec2d playDimension;
	static ppogl::VNCArray *sm_vncArray;
	
	static float angle;
	static float* elevation;
	static int* terrain;
	static int nx, ny;

	static bool load(const std::string& course);

	static float* getElevData(){return elevation;}
	static int* getTerrainData(){return terrain;}
	
	static const float& getAngle(){return angle;}
	
	static const ppogl::Vec2d& getDimensions(){return dimension;}
	static const ppogl::Vec2d& getPlayDimensions(){return playDimension;}

	static float getTerrainBaseHeight(float distance);
	static float getTerrainMaxHeight(float distance);
		
	static void setStartPt(const ppogl::Vec2d& p);
	static const ppogl::Vec2d& getStartPt();

	static ppogl::VNCArray* getGLArrays(){return sm_vncArray;}

	static void registerCallbacks();
	static void fillGLArrays();
		
	/// get elevation at point x,y	
	static float& getElevation(int x, int y)
	{
		PP_REQUIRE(x>=0 && x<nx ,"Invalid value: " << x);
		PP_REQUIRE(y>=0 && y<ny ,"Invalid value: " << y);
		return Course::elevation[x + nx*y];
	}
	
	/// get vertex for point x,y	
	static const ppogl::Vec3d getVertex(int x, int y)
	{
		PP_REQUIRE(x>=0 && x<nx ,"Invalid value: " << x);
		PP_REQUIRE(y>=0 && y<ny ,"Invalid value: " << y);
		return ppogl::Vec3d(float(x)/(nx-1.)*(dimension.x()),
			Course::getElevation(x,y),-float(y)/(ny-1.)*(dimension.y()));
	}
	
	static void cleanup();
};

class TerrainTex
{
public:
	TerrainTex()
	 : type(1),
	   value(0),
	   friction(0.5),
	   compression(0.1),
	   wheight(150),
	   count(0),
	   soundactive(false)
	{};
	   
    int type;
	int value;
	double friction;
	double compression;
	ppogl::TextureRef texture;
	ppogl::TextureRef particles;
	ppogl::TextureRef envmap;
	std::string sound;
	struct { ppogl::TextureRef head,mark,tail; }trackmark;
	int wheight;
	int count;
	bool soundactive;
};

extern std::list<int> usedTerrains;

#endif // _COURSE_LOAD_H_
