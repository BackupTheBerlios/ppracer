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

#include "pp_types.h"

#include "ppogl/models/model.h"

#include "ppogl/textures.h"


#include <list>
#include <string>

#ifndef _COURSE_LOAD_H_
#define _COURSE_LOAD_H_

#define STRIDE_GL_ARRAY ( 8 * sizeof(GLfloat) + 4 * sizeof(GLubyte) )

/* Convenience macro for accessing terrain elevations */
#define ELEV(x,y) ( elevation[(x) + nx*(y)] )

/* Convenience acro to create a course vertex */
#define COURSE_VERTEX(x,y) ppogl::Vec3d( float(x)/(nx-1.)*course_width, \
                       ELEV((x),(y)), -float(y)/(ny-1.)*course_length ) 

class Course
{
public:
	static void load(const std::string& course);
	static float* getElevData();
	static int* getTerrainData();
	static float getAngle();

	static void getDimensions(float *width, float *length);
	static void getPlayDimensions(float *width, float *length);

	static float getTerrainBaseHeight(float distance);
	static float getTerrainMaxHeight(float distance);
	static void getDivisions(int *nx, int *ny);
	static void setStartPt(const ppogl::Vec2d& p);
	static const ppogl::Vec2d& getStartPt();

	static const std::string& getAuthor();
	static const std::string& getName(); 

	static void getGLArrays(GLubyte **vertex_normal_arr);

	static void registerCallbacks();
	static void fillGLArrays();

};

class TerrainTex
{
public:
	TerrainTex()
	 : type(1),
	   value(0),
	   friction(0.5),
	   compression(0.1),
	   soundactive(false),
	   wheight(150),
	   count(0)
	{};
	   
    int type;
	int value;
	double friction;
	double compression;
	ppogl::TextureRef texture;
	ppogl::TextureRef particles;
	ppogl::TextureRef envmap;
	std::string sound;
	bool soundactive;
	struct { ppogl::TextureRef head,mark,tail; }trackmark;
	int wheight;
	int count;
};

extern std::list<int> usedTerrains;

#endif // _COURSE_LOAD_H_
