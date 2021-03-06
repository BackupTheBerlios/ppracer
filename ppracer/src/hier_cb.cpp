/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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

#include "ppracer.h"

static int
tux_rotate(ppogl::Script *vm) 
{
    if(vm->getTop() !=3){
		PP_WARNING("tux.rotate: Wrong number of arguments");
        return 0;
    }

    // obtain the nodename
    const std::string nodename = vm->getString(1);

    // obtain the axis 
    const std::string axis = vm->getString(2);
	
	const char tempaxis = axis.c_str()[0];

	if(tempaxis < 'x' || tempaxis > 'z'){	
		PP_WARNING("tux.rotate: Invalid rotation axes: " << axis);
		return 0;
    }
    
    // obtain the angle
	const double angle = vm->getFloat(3);
    
    rotate_scene_node(nodename, static_cast<ppogl::Axis>(tempaxis-'x'), angle);
	
    return 0;
}

static int
tux_translate(ppogl::Script *vm) 
{
    if(vm->getTop()!=2){
		PP_WARNING("tux.translate: Invalid number of arguments");
		return 0;
    }

    // obtain the nodename
    const std::string nodename = vm->getString(1);

	ppogl::Vec3d vec;
    // obtain the translation vector	
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(2);
		vec.values[i]=vm->getFloat();
		vm->pop(2);		
	}
	
	translate_scene_node(nodename, vec);
 
    return 0;
}

static int
tux_scale(ppogl::Script *vm) 
{
    if(vm->getTop()!=3){
        PP_WARNING("tux.scale: Invalid number of arguments");
		return 0;
    }

    // obtain the nodename
    const std::string nodename = vm->getString(1);
    
    // obtain the origin point
	ppogl::Vec3d origin;
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(2);
		origin.values[i]=vm->getFloat();
		vm->pop(2);		
	}

    // obtain the scale factors
	ppogl::Vec3d factors; 
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(3);
		factors.values[i]=vm->getFloat();
		vm->pop(2);		
	}
	
    scale_scene_node(nodename,origin,factors);
  
    return 0;
}

static int
tux_transform(ppogl::Script *vm) 
{
    if(vm->getTop() != 2){
        PP_WARNING("tux.transform: Invalid number of arguments");
		return 0;
	}

    // obtain parent's name
    const std::string parent_name = vm->getString(1);

    // obtain child's name
    const std::string child_name = vm->getString(2);

    create_tranform_node(parent_name, child_name);

	return 0;
}

static int
tux_sphere(ppogl::Script *vm) 
{
    if(vm->getTop() != 3){
		PP_WARNING("tux.sphere: Invalid number of arguments");
		return 0;
    }

    // obtain parent's name
    const std::string parent_name = vm->getString(1);
    	
	// obtain child's name
    const std::string child_name = vm->getString(2);
	
	const double resolution = vm->getFloat(3);
	
    create_sphere_node(parent_name, child_name, resolution);

    return 0;
}

static int
tux_material(ppogl::Script *vm) 
{
	if(vm->getTop() != 4){
        PP_WARNING("tux.material: Invalid number of arguments");
		return 0; 
    }
    // obtain material name
    const std::string name = vm->getString(1);

    // obtain diffuse color
    ppogl::Material material;
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(2);
		material.diffuse.values[i]=vm->getFloat();
		vm->pop(2);		
	}

    // obtain specular color
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(3);
		material.specular.values[i]=vm->getFloat();
		vm->pop(2);		
	}

    // obtain specular exponent
	material.setSpecularExponent(vm->getFloat(4));

	add_material(name, material);

    return 0;
}

static int
tux_surfaceproperty(ppogl::Script *vm) 
{   
	if(vm->getTop() != 2){
        PP_WARNING("tux.surfaceproperty: Invalid number of arguments");
		return 0;
    }

    // obtain node name
    const std::string node_name = vm->getString(1);
	
    // obtain material name
    const std::string mat_name = vm->getString(2);

    set_scene_node_material(node_name, mat_name);

    return 0;
}

static int
tux_shadow(ppogl::Script *vm) 
{
    if(vm->getTop() != 2){
        PP_WARNING("tux.shadow: Invalid number of arguments");
		return 0;
    }

    const std::string node_name = vm->getString(1);
    const std::string state = vm->getString(2);

    set_scene_node_shadow_state(node_name, state);

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
	{NULL, NULL}
};

void register_hier_callbacks()
{
	script.registerLib("tux", tuxlib);
}
