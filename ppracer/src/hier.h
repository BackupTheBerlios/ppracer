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

#ifndef _HIER_H_
#define _HIER_H_

#include "pp_types.h"

#include "ppogl/sg/geoms.h"

#define MIN_SPHERE_DIVISIONS 3
#define MAX_SPHERE_DIVISIONS 16

bool get_scene_node(const std::string& node_name, SceneNode **node );

void reset_scene_node(const std::string& node);

std::string rotate_scene_node(const std::string& node, char axis, double angle);
std::string translate_scene_node(const std::string& node, const ppogl::Vec3d& trans);
std::string scale_scene_node(const std::string& node, const ppogl::Vec3d& origin, const ppogl::Vec3d& factor);
std::string transform_scene_node(const std::string& node, const pp::Matrix& mat, const pp::Matrix& invMat);

std::string set_scene_node_material(const std::string& node, const std::string& mat);

void create_material(const std::string& mat, const ppogl::Color& d, const ppogl::Color& s, double s_exp);

std::string set_scene_resolution(const std::string& resolution);

std::string set_scene_node_shadow_state(const std::string& node, const std::string& state );
std::string set_scene_node_eye(const std::string& node, const std::string& which_eye );

std::string create_tranform_node(const std::string& parent, const std::string& name);
std::string create_sphere_node(const std::string& parent_name, const std::string& child_name, double resolution );

void initialize_scene_graph();

void draw_scene_graph(const std::string& node );
bool collide(const std::string& node, const ppogl::Polyhedron& ph );

#endif // _HIER_H_
