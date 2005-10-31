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
 
#include "geoms.h"

namespace ppogl{
	
Polygon::Polygon(const Polygon& poly)
{
	numVertices=poly.numVertices;
	if(numVertices>0 && poly.vertices){
		vertices = new int[numVertices];
		for(int i=0; i<numVertices; i++) {
        	vertices[i] = poly.vertices[i];
    	}
	}		
}
	
Polygon::~Polygon()
{
	if(vertices) delete [] vertices;		
}

Polygon&
Polygon::operator=(const Polygon& poly)
{
	if(this != &poly){
		if(vertices) delete [] vertices;
		
		numVertices=poly.numVertices;
		
		if(numVertices>0 && poly.vertices){
			vertices = new int[numVertices];
			for(int i=0; i<numVertices; i++) {
        		vertices[i] = poly.vertices[i];
    		}
		}	
	}
	return *this;
}
	
Polyhedron::Polyhedron(const Polyhedron& ph)
{
	num_vertices=ph.num_vertices;
	num_polygons=ph.num_polygons;
	
	if(num_vertices>0 && ph.vertices!=NULL){
		vertices = new ppogl::Vec3d[num_vertices];
		for(int i=0; i<num_vertices; i++) {
        	vertices[i] = ph.vertices[i];
    	}
	}
	
	if(num_polygons>0 && ph.polygons!=NULL){
		polygons = new ppogl::Polygon[num_polygons];
		for(int i=0; i<num_polygons; i++){
        	polygons[i] = ph.polygons[i];
    	}
	}
}	
	
Polyhedron::~Polyhedron()
{
	if(vertices) delete [] vertices;
	if(polygons) delete [] polygons;	
}

} //namepsace ppogl
