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

#ifndef _PPOGL_SG_GEOMS_H_
#define _PPOGL_SG_GEOMS_H_

#include "../base/vec3d.h"

namespace ppogl{
	
class Polygon
{
public:
	Polygon():numVertices(0),vertices(NULL){};
	Polygon(const Polygon& poly);
	~Polygon();
		
    int numVertices;
    int *vertices;
		
	Polygon& operator=(const Polygon& poly);
};
	
class Polyhedron
{
public:
	Polyhedron();
    Polyhedron(const Polyhedron& ph);
	~Polyhedron();
		
	int numVertices;
    int numPolygons;
    ppogl::Vec3d *vertices;
    ppogl::Polygon *polygons;
};

} //namepsace ppogl

#endif // _PPOGL_SG_GEOMS_H_
