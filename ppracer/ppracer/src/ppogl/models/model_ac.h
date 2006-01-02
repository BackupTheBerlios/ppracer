/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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

#ifndef _PPOGL_MODEL_AC_H_
#define _PPOGL_MODEL_AC_H_


#include "../sg/geoms.h"
#include "../sg/materials.h"

#include <string>
#include <stdio.h>

namespace ppogl{
	
class Vertex 
{
public:
    ppogl::Vec3d vec;
    ppogl::Vec3d normal;
};
	
class Surface
 : public ppogl::Polygon
{
public:
	Surface();
	~Surface();

 	ppogl::UV *uvs;
    ppogl::Vec3d normal;
    int flags;
    int mat;
};

class ModelObject
{
public:
	ModelObject();
	~ModelObject();
	
	ppogl::Vec3d loc;
    std::string name;
    std::string data;
    std::string url;
    ppogl::Vertex *vertices;
    int num_vert;

    ppogl::Surface *surfaces;
    int num_surf;
    ppogl::Vec2d texture_repeat;
    ppogl::Vec2d texture_offset;

    int num_kids;
    ppogl::ModelObject **kids;
    double matrix[9];
    int type;
    int texture;
};

class ModelAC
{
	ppogl::ModelObject* mp_model;

	int m_tokc;
	char* ma_tokv[30];
	
	int m_line;
	char ma_buff[255];

	ppogl::Material ma_palette[255];
	int m_numPalette;
	int m_startMatIndex;
		
	int loadTexture(const char *filename);
	void prepareRender();	
	void render(ModelObject *ob);
	void setColor(long matno);
	void setSimpleColor(long matno);
	int stringToObjectType(const std::string& string);
	ModelObject* loadObject(FILE *f, const std::string& filename);
	void objectCalculateVertexNormals(ModelObject *ob);
	void calculateVertexNormals(ModelObject *ob);
	int getTokens(char *s, int *argc, char *argv[]);
	ppogl::Material* getMaterialFromPalette(int id);
	bool readLine(FILE *f);
	Surface* readSurface(FILE *f, Surface *s, ModelObject *ob, const std::string& filename);	
	void CalculateTriNormal(ppogl::Vec3d *v1, ppogl::Vec3d *v2, ppogl::Vec3d *v3, ppogl::Vec3d *n);
		
public:
	ModelAC(const std::string& filename);
	~ModelAC();

	ppogl::ModelObject* getModel(){return mp_model;}
	int getDisplayList();
};

} //namepsace pp

#endif // _PP_MODEL_AC_H_
