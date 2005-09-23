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

#include "hier.h"

static int
tux_rotate(ppogl::Script *vm) 
{
    std::string nodename;
    std::string axis ="0";
    double angle ;

    if(vm->getTop() !=3){
		PP_WARNING("tux.rotate: Wrong number of arguments");
        return 0;
    }

    // obtain the nodename
    nodename = vm->getString(1);

    // obtain the axis 
    axis = vm->getString(2);
    if ("x" != axis && "y" != axis && "z" != axis){
		PP_WARNING("tux.rotate: Invalid rotation axes: " << axis);
		return 0;
    }
    
    // obtain the angle
	angle = vm->getFloat(3);
    
    rotate_scene_node(nodename,axis.c_str()[0],angle);
 
    return 0;
}

static int
tux_translate(ppogl::Script *vm) 
{
    std::string nodename;
    ppogl::Vec3d vec;

    if(vm->getTop()!=2){
		PP_WARNING("tux.translate: Invalid number of arguments");
		return 0;
    }

    // obtain the nodename
    nodename = vm->getString(1);

    // obtain the translation vector
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(2);
		vec.values[i]=vm->getFloat();
		vm->pop(2);		
	}
	
	translate_scene_node(nodename,vec);
 
    return 0;
}

static int
tux_scale(ppogl::Script *vm) 
{
    std::string errmsg;

    std::string nodename;
    ppogl::Vec3d origin;
    ppogl::Vec3d factors; 

    if(vm->getTop()!=3){
        PP_WARNING("tux.scale: Invalid number of arguments");
		return 0;
    }

    // obtain the nodename
    nodename = vm->getString(1);

    // obtain the origin point
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(2);
		origin.values[i]=vm->getFloat();
		vm->pop(2);		
	}

    // obtain the scale factors
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(3);
		factors.values[i]=vm->getFloat();
		vm->pop(2);		
	}
	
    errmsg = scale_scene_node(nodename,origin,factors);

    // report error, if any 
    if(!errmsg.empty()) {
        PP_WARNING(errmsg);
    }
  
    return 0;
}

static int
tux_transform(ppogl::Script *vm) 
{
    std::string errmsg;

    std::string parent_name;
    std::string child_name;

    if(vm->getTop() != 2){
        PP_WARNING("tux.transform: Invalid number of arguments");
		return 0;
	}

    // obtain parent's name
    parent_name = vm->getString(1);

    // obtain child's name
    child_name = vm->getString(2);

    errmsg = create_tranform_node(parent_name, child_name);

    // report error, if any
    if(!errmsg.empty()) {
        PP_WARNING("tux.transform: " << errmsg << "\n" << parent_name << " -> " << child_name); 
    }
    return 0;
}

static int
tux_sphere(ppogl::Script *vm) 
{
    std::string errmsg;

    std::string parent_name;
    std::string child_name;
    double resolution;

    if(vm->getTop() != 3){
		PP_WARNING("tux.sphere: Invalid number of arguments");
		return 0;
    }

    // obtain parent's name
    parent_name = vm->getString(1);
    	
	// obtain child's name
    child_name = vm->getString(2);
	
	resolution = vm->getFloat(3);
	
    errmsg = create_sphere_node(parent_name, child_name, resolution);

    // report error, if any
    if(!errmsg.empty()){
        PP_WARNING(errmsg);
    }
    return 0;
}

static int
tux_material(ppogl::Script *vm) 
{
    std::string mat_name;
    ppogl::Color diffuse;
    ppogl::Color specular;
    double spec_exp;

	if(vm->getTop() != 4){
        PP_WARNING("tux.material: Invalid number of arguments");
		return 0; 
    }
    // obtain material name
    mat_name = vm->getString(1);

    // obtain diffuse color
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(2);
		diffuse.values[i]=vm->getFloat();
		vm->pop(2);		
	}

    // obtain specular color
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(3);
		specular.values[i]=vm->getFloat();
		vm->pop(2);		
	}

    // obtain specular exponent
	spec_exp = vm->getFloat(4);

	create_material(mat_name, diffuse, specular, spec_exp);

    return 0;
}

static int
tux_surfaceproperty(ppogl::Script *vm) 
{
    std::string errmsg;

    std::string node_name;
    std::string mat_name;

    if(vm->getTop() != 2){
        PP_WARNING("tux.surfaceproperty: Invalid number of arguments");
		return 0;
    }

    // obtain node name
    node_name = vm->getString(1);
	
    // obtain material name
    mat_name = vm->getString(2);

    errmsg = set_scene_node_material(node_name, mat_name);

    // report error, if any
    if(!errmsg.empty()){
        PP_WARNING("tux.surfaceproperty: " << errmsg << "\n" << node_name << " -> " << mat_name); 
    }
    return 0;
}

static int
tux_shadow(ppogl::Script *vm) 
{
    std::string errmsg;

    std::string node_name;
    std::string state;

    if(vm->getTop() != 2){
        PP_WARNING("tux.shadow: Invalid number of arguments");
		return 0;
    }

    node_name = vm->getString(1);
    state = vm->getString(2);

    errmsg = set_scene_node_shadow_state(node_name, state);

    // report error, if any
    if(!errmsg.empty()){
        PP_WARNING("tux.shadow: " << errmsg << "\n" << node_name << " -> " << state);
    }
    return 0;
}

static int
tux_eye(ppogl::Script *vm) 
{
    std::string errmsg;

    std::string node_name;
    std::string which_eye;

    if(vm->getTop() !=2 ){
        PP_WARNING("tux.eye: Invalid number of arguments");
		return 0; 
    }

    node_name = vm->getString(1);
    which_eye = vm->getString(2);

    errmsg = set_scene_node_eye( node_name, which_eye );

    // report error, if any
    if(!errmsg.empty()){
        PP_WARNING(errmsg);
    }
    return 0;
}

static const struct ppogl::Script::Lib tuxlib[]={
	{"rotate",tux_rotate},
	{"translate",tux_translate},
	{"scale",tux_scale},
	{"sphere",tux_sphere},
	{"transform",tux_transform},
  	{"material",tux_material},
	{"surfaceproperty",tux_surfaceproperty},
	{"shadow",tux_shadow},
	{"eye",tux_eye},
	{NULL, NULL}
};

void register_hier_callbacks()
{
	script.registerLib("tux", tuxlib);
}
