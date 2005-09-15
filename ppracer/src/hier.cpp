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
#include "hier_util.h"

#include "ppogl/base/defs.h"

#include <map>
#include <string>

std::map<std::string, SceneNode *> nodes;
std::map<std::string, Material*> materials;


/*
 * State variables
 */


/* Default Material */
Material g_hier_default_material ={ 
				ppogl::Color(0., 0., 1.),  /* diffuse color  = blue */
				ppogl::Color(0., 0., 0.),  /* specular color = black */
				0.0              /* specular exp. = 0.0 */
			};
/*
 * Functions used to access and update the name-to-pointer hash tables
 */

/* Add a new node name to the node name hash table.
   node_name contains the child's name only. */
static bool
add_scene_node( std::string& parent_name, std::string& node_name, SceneNode *node ) 
{
    //int newEntry;
	
    // Add the current node to the hash table 
	std::string new_name;
	
    if(parent_name==":"){
        new_name = ":" + node_name;
    } else {
        new_name = parent_name + ":" + node_name;
    }
	
	nodes[new_name]=node;
	
    return true;
}

/* Get node pointer from node name */
bool
get_scene_node(std::string& node_name, SceneNode **node ) 
{
	std::map<std::string, SceneNode *>::iterator it;
	
	if((it=nodes.find(node_name))!=nodes.end()){
		*node = (*it).second;
	}else{
		if(node_name==":"){
            //the root has been specified
            *node = 0;
        }else{	
			return false;
		}
	}
	return true;
}

/* Add a new material name to the material name hash table. */
void
add_material(std::string& mat_name, Material *mat)
{
    materials[mat_name]=mat;
}

/* Get material pointer from material name */
bool 
get_material(std::string& mat_name, Material **mat ) 
{
    std::map<std::string, Material*>::iterator it;

	if((it=materials.find(mat_name))!=materials.end()){
		*mat = (*it).second;
		return true;
	}else{
		return false;
	}
}

/* Creates a new node, add the node to the hash table, and inserts the
   node into the DAG.  Default values are given to all fields except
   the type-specific ones (geom, param).  */
std::string create_scene_node(std::string& parent_name, std::string& child_name, 
			 SceneNode **node )
{
    SceneNode *parent, *child;
		
    if(get_scene_node(parent_name, &parent) != true){
        return "Parent node does not exist";
    }

    child = new SceneNode;
	PP_CHECK_ALLOC(child);
	
    /* Initialize node */
    child->parent = parent;
    child->next = NULL;
    child->child = NULL;
    child->mat = NULL;
    child->render_shadow = true;
    child->eye = false;
    child->trans.makeIdentity();
    child->invtrans.makeIdentity();

    if(add_scene_node(parent_name, child_name, child ) != true){
        delete child;
        return "Child already exists";
    }


    /* Add node to parent's children */
    if ( parent != NULL ) {
        if ( parent->child == NULL ) {
            parent->child = child;
        } else {
            for (parent = parent->child; parent->next != NULL; 
                 parent = parent->next) {/* do nothing */}
            parent->next = child;
        } 
    } 

    *node = child;
    return "";
} 

std::string
reset_scene_node( std::string& node ) 
{  
    SceneNode *nodePtr;

    if(get_scene_node( node, &nodePtr )!= true){
        return "No such node";
    } 

    nodePtr->trans.makeIdentity();
    nodePtr->invtrans.makeIdentity();

    return "";
}

std::string 
rotate_scene_node(std::string& node, char axis, double angle ) 
{
    SceneNode *nodePtr;
    pp::Matrix rotMatrix;

    if (get_scene_node( node, &nodePtr ) != true){
        return "No such node";
    } 

    rotMatrix.makeRotation( angle, axis );
    nodePtr->trans=nodePtr->trans*rotMatrix;
	
    rotMatrix.makeRotation( -angle, axis );
    nodePtr->invtrans=rotMatrix*nodePtr->invtrans;

    return "";
}

std::string 
translate_scene_node(std::string& node, ppogl::Vec3d vec ) 
{
    SceneNode *nodePtr;
    pp::Matrix xlateMatrix;

    if(get_scene_node( node, &nodePtr ) != true){
        return "No such node";
    } 

    xlateMatrix.makeTranslation( vec.x(), vec.y(), vec.z() );
	nodePtr->trans=nodePtr->trans*xlateMatrix;
	
    xlateMatrix.makeTranslation( -vec.x(), -vec.y(), -vec.z() );
    nodePtr->invtrans=xlateMatrix*nodePtr->invtrans;

    return "";
}

std::string 
scale_scene_node(std::string& node, ppogl::Vec3d center, ppogl::Vec3d factor) 
{
    SceneNode *nodePtr;
    pp::Matrix matrix;

    if(get_scene_node( node, &nodePtr ) != true){
        return "No such node";
    } 

    matrix.makeTranslation( -center.x(), -center.y(), -center.z() );
    nodePtr->trans=nodePtr->trans*matrix;
	
    matrix.makeTranslation( center.x(), center.y(), center.z() );
    nodePtr->invtrans=matrix*nodePtr->invtrans;

    matrix.makeScaling( factor.values[0], factor.values[1], factor.values[2] );
    nodePtr->trans=nodePtr->trans*matrix;
	
    matrix.makeScaling( 1./factor.values[0], 1./factor.values[1], 1./factor.values[2] );
    nodePtr->invtrans=matrix*nodePtr->invtrans;

    matrix.makeTranslation( center.x(), center.y(), center.z() );
    nodePtr->trans=nodePtr->trans*matrix;
	
    matrix.makeTranslation( -center.x(), -center.y(), -center.z() );
    nodePtr->invtrans=matrix*nodePtr->invtrans;

    return "";
}

std::string 
transform_scene_node(std::string& node, pp::Matrix mat, pp::Matrix invmat ) 
{
    SceneNode *nodePtr;

    if(get_scene_node( node, &nodePtr ) != true){
        return "No such node";
    } 

    nodePtr->trans=nodePtr->trans*mat;
    nodePtr->invtrans=invmat*nodePtr->invtrans;

    return "";
}

std::string 
set_scene_node_material(std::string& node, std::string& mat) 
{
    Material *matPtr;
    SceneNode *nodePtr;

    if(get_scene_node(node, &nodePtr) != true){
        return "No such node";
    }

    if(get_material( mat, &matPtr ) != true){
        return "No such material";
    } 

    nodePtr->mat = matPtr;

    return "";
}

std::string 
set_scene_node_shadow_state(std::string& node, std::string& state) 
{
    SceneNode *nodePtr;

    if(get_scene_node(node, &nodePtr) != true){
        return "No such node";
    } 

    if(state=="off"){
		nodePtr->render_shadow = false;
    }else if(state=="on"){
		nodePtr->render_shadow = true;
    } else {
		return "Shadow state must be 'on' or 'off'";
    }

    return "";
}

std::string 
set_scene_node_eye(std::string& node, std::string& which_eye )
{
    SceneNode *nodePtr;

    if(get_scene_node(node, &nodePtr) != true){
        return "No such node";
    } 

    if (which_eye=="right" ){
		nodePtr->eye = true;
		nodePtr->which_eye = TuxRightEye;
    }else if(which_eye == "left"){
		nodePtr->eye = true;
		nodePtr->which_eye = TuxLeftEye;
    }else{
		return "'eye' must be right or left";
    }

    return "";
}

std::string 
create_tranform_node(std::string& parent_name, std::string& child_name ) 
{
    SceneNode *node;
    std::string msg = create_scene_node(parent_name, child_name, &node);
    if (!msg.empty()){
        return msg;
    } 

    node->geom = Empty;

    return msg;
}

std::string
create_sphere_node(std::string& parent_name, std::string& child_name, double resolution ) 
{
    SceneNode *node;

    std::string msg = create_scene_node(parent_name, child_name, &node);
    if(!msg.empty()){
        return msg;
    } 

    node->geom = Sphere;
    node->param.sphere.radius = 1.0;
    node->param.sphere.divisions = MIN( 
	MAX_SPHERE_DIVISIONS, MAX( 
	    MIN_SPHERE_DIVISIONS, 
	    ROUND_TO_NEAREST(PPConfig.getInt("tux_sphere_divisions") * resolution ) 
	    ) );

    return msg;
}

std::string 
create_material(std::string& mat, ppogl::Color diffuse, 
		 ppogl::Color specular, double specular_exp ) 
{
    Material *matPtr;

    matPtr = new Material;
	PP_CHECK_ALLOC(matPtr);
		
    matPtr->diffuse.r() = diffuse.r();
    matPtr->diffuse.g() = diffuse.g();
    matPtr->diffuse.b() = diffuse.b();
    matPtr->diffuse.a() = 1.0;

    matPtr->specular.r() = specular.r();
    matPtr->specular.g() = specular.g();
    matPtr->specular.b() = specular.b();
    matPtr->specular.a() = 1.0;

    matPtr->specular_exp = specular_exp;

	add_material( mat, matPtr );

    return "";
}

void initialize_scene_graph() 
{
    /* Initialize state */

    g_hier_default_material.diffuse.r() = 0.0;
    g_hier_default_material.diffuse.g() = 0.0;
    g_hier_default_material.diffuse.b() = 1.0;
    g_hier_default_material.diffuse.a() = 1.0;

    g_hier_default_material.specular.r() = 0.0;
    g_hier_default_material.specular.g() = 0.0;
    g_hier_default_material.specular.b() = 0.0;
    g_hier_default_material.specular.a() = 1.0;

    g_hier_default_material.specular_exp = 0.0;
}

void draw_scene_graph(std::string& node)
{
    SceneNode *nodePtr;

    if(get_scene_node( node, &nodePtr ) != true){
        PP_ERROR( "draw_scene_graph: No such node: " << node);
    } 

    traverse_dag( nodePtr, &g_hier_default_material );
    
} 

bool collide(std::string& node, ppogl::Polyhedron ph )
{
    SceneNode *nodePtr;
    pp::Matrix mat, invmat;

    mat.makeIdentity();
    invmat.makeIdentity();

    if(get_scene_node(node, &nodePtr) != true){
        PP_ERROR( "draw_scene_graph: No such node: " << node );
    } 

    return check_polyhedron_collision_with_dag( nodePtr, mat, invmat, ph );
}
