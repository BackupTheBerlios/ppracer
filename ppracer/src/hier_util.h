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

#ifndef _HIER_UTIL_H
#define _HIER_UTIL_H

#include "ppogl/sg/geoms.h"
#include "ppogl/sg/materials.h"

#include "algstuff.h"


enum Geometry
{ 
    Empty,
	Sphere
};

/// Data for Sphere node type.
struct SphereNode
{
    float radius;
	
	/// How many divisions do we use to draw a sphere?
    int divisions;		
};

struct SceneNode
{
    SceneNode* parent;
    SceneNode* next;
    SceneNode* child;

	/// type of node
    Geometry geom;

    union{
        SphereNode sphere;   
    }param;
      
	ppogl::Material* mat;

    /// The forward and inverse transforms
    pp::Matrix trans;
    pp::Matrix invtrans;
	
    /// Do we draw the shadow of this node?
    bool renderShadow;
};


void draw_sphere(int num_divisions);

void traverse_dag(SceneNode *node, ppogl::Material *mat);

void trans_polyhedron(const pp::Matrix& mat, const ppogl::Polyhedron& ph);

bool check_polyhedron_collision_with_dag(
    SceneNode *node,const pp::Matrix& modelMatrix, const pp::Matrix& invModelMatrix,
    const ppogl::Polyhedron& ph );

#endif // _HIER_UTIL_H
