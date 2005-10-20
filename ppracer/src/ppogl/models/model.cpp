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

#include "model.h"

#include "model_ac.h"

#include "ppogl/base/glwrappers.h"

#include <iostream>

namespace ppogl{
	
Model::Model(const std::string& filename)
 : m_displayList(-1)
{	
	mp_model = new ModelAC(filename);
	m_displayList = mp_model->getDisplayList();
}

Model::~Model()
{
	if(m_displayList != -1){
		gl::DeleteLists(m_displayList,1);
	}
	delete mp_model;
}

void
Model::draw()
{
	if(m_displayList != -1){
		gl::CallList(m_displayList);
	}
}


void
Model::copyObject(ppogl::Polyhedron* ph, ppogl::ModelObject* object)
{
	if(object->num_vert>0 && object->num_surf>0){
		int vert0 = ph->num_vertices;
		int poly0 = ph->num_polygons;
		
		for(int i=0; i<object->num_vert; i++){
			ph->vertices[vert0+i]=object->vertices[i].vec;
			ph->num_vertices++;
		}

		for(int i=0; i<object->num_surf; i++){
			int num_vertices = object->surfaces[i].numVertices;
			
			if(num_vertices >10){
				std::cout << "Warning: Vertices-> " <<
						num_vertices << std::endl;
			}else{			
				ph->polygons[poly0+i].numVertices = num_vertices; 
				ph->polygons[poly0+i].vertices = new int[num_vertices];
				
				for(int u=0; u<num_vertices;u++){
					ph->polygons[poly0+i].vertices[u]=
						object->surfaces[i].vertices[u] + vert0;		
				}
				ph->num_polygons++;
			}
		}
	}	
	
	for (int i = 0; i < object->num_kids; i++){
		copyObject(ph,object->kids[i]);	
	}	
}

ppogl::Polyhedron*
Model::getPolyhedron()
{
	ppogl::Polyhedron* ph = new ppogl::Polyhedron;
	
	ph->num_vertices=0;
	ph->num_polygons=0;
	ph->vertices = new ppogl::Vec3d[1024];
	ph->polygons = new ppogl::Polygon[256];
		
	copyObject(ph,mp_model->getModel());
		
    return ph;	
}

} //namespace pp
