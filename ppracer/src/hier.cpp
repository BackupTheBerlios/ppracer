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
#include "hier_util.h"

#include "ppogl/base/defs.h"

#include <map>
#include <string>

static std::map<std::string, SceneNode *> nodes;
static std::map<std::string, ppogl::Material> materials;

// Default Material
static ppogl::Material g_hier_default_material;

/*
 * Functions used to access and update the name-to-pointer hash tables
 */

/** Add a new node name to the node name hash table.
   node_name contains the child's name only. */
static bool
add_scene_node( const std::string& parent_name, const std::string& node_name, SceneNode *node ) 
{
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

/** Get node pointer from node name */
bool
get_scene_node(const std::string& node_name, SceneNode **node ) 
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

/** Get material pointer from material name */
bool 
get_material(const std::string& mat_name, ppogl::Material **mat ) 
{
    std::map<std::string, ppogl::Material>::iterator it;

	if((it=materials.find(mat_name))!=materials.end()){
		*mat = &(*it).second;
		return true;
	}else{
		return false;
	}
}

/** Creates a new node, add the node to the hash table, and inserts the
   node into the DAG.  Default values are given to all fields except
   the type-specific ones (geom, param). */
std::string
create_scene_node(const std::string& parent_name, const std::string& child_name, 
			 SceneNode **node )
{
    SceneNode *parent;
		
    if(get_scene_node(parent_name, &parent) != true){
        return "Parent node does not exist";
    }

    SceneNode* child = new SceneNode;
	PP_CHECK_ALLOC(child);
	
    // Initialize node
    child->parent = parent;
    child->next = NULL;
    child->child = NULL;
    child->mat = NULL;
    child->renderShadow = true;
    child->trans.makeIdentity();
    child->invtrans.makeIdentity();

    if(add_scene_node(parent_name, child_name, child ) != true){
        delete child;
        return "Child already exists";
    }


    // Add node to parent's children 
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

void
reset_scene_node(const std::string& node) 
{  
    SceneNode *nodePtr;

    if(get_scene_node( node, &nodePtr )!= true){
        // node doesn't exist
		return;
    } 

    nodePtr->trans.makeIdentity();
    nodePtr->invtrans.makeIdentity();
}

void 
rotate_scene_node(const std::string& node, char axis, double angle ) 
{
    SceneNode *nodePtr;

    if(get_scene_node( node, &nodePtr )!= true){
        PP_WARNING("No such node");
		return;
    }
	
    pp::Matrix rotMatrix;
    rotMatrix.makeRotation( angle, axis );
    nodePtr->trans=nodePtr->trans*rotMatrix;
	
    rotMatrix.makeRotation( -angle, axis );
    nodePtr->invtrans=rotMatrix*nodePtr->invtrans;
}

void 
translate_scene_node(const std::string& node, const ppogl::Vec3d& vec ) 
{
    SceneNode *nodePtr;

    if(get_scene_node( node, &nodePtr ) != true){
        PP_WARNING("No such node");
		return;
    }
	pp::Matrix xlateMatrix;
	
    xlateMatrix.makeTranslation( vec.x(), vec.y(), vec.z() );
	nodePtr->trans=nodePtr->trans*xlateMatrix;
	
    xlateMatrix.makeTranslation( -vec.x(), -vec.y(), -vec.z() );
    nodePtr->invtrans=xlateMatrix*nodePtr->invtrans;
}

void 
scale_scene_node(const std::string& node, const ppogl::Vec3d& center, const ppogl::Vec3d& factor) 
{
    SceneNode *nodePtr;

    if(get_scene_node(node, &nodePtr)!=true){
		PP_WARNING("No such node");
		return;
    } 
	
	pp::Matrix matrix;
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
}

void 
transform_scene_node(const std::string& node, const pp::Matrix& mat, const pp::Matrix& invmat ) 
{
    SceneNode *nodePtr;

    if(get_scene_node(node, &nodePtr)!=true){
		PP_WARNING("No such node");
		return;
    } 

    nodePtr->trans=nodePtr->trans*mat;
    nodePtr->invtrans=invmat*nodePtr->invtrans;
}

void 
set_scene_node_material(const std::string& node, const std::string& mat) 
{
    SceneNode *nodePtr;

    if(get_scene_node(node, &nodePtr) != true){
        PP_WARNING("No such node");
		return;
    }

	ppogl::Material *matPtr;

    if(get_material( mat, &matPtr ) != true){
        PP_WARNING("No such material");
		return;
    } 

    nodePtr->mat = matPtr;
}

void 
set_scene_node_shadow_state(const std::string& node, const std::string& state) 
{
    SceneNode *nodePtr;

    if(get_scene_node(node, &nodePtr) != true){
        PP_WARNING("No such node");
		return;
	}

    if(state=="off"){
		nodePtr->renderShadow = false;
    }else if(state=="on"){
		nodePtr->renderShadow = true;
    } else {
		PP_WARNING("Shadow state must be 'on' or 'off'");
		return;
    }
}

void 
create_tranform_node(const std::string& parent_name, const std::string& child_name ) 
{
    SceneNode *node;
    create_scene_node(parent_name, child_name, &node);

    node->isSphere = false;
}

void
create_sphere_node(const std::string& parent_name, const std::string& child_name, float resolution ) 
{
    SceneNode *node;

    create_scene_node(parent_name, child_name, &node);

    node->isSphere = true;
    node->sphere.radius = 1.0;
    node->sphere.divisions = MIN( 
	MAX_SPHERE_DIVISIONS, MAX( 
	    MIN_SPHERE_DIVISIONS, 
	    ROUND_TO_NEAREST(PPConfig.getInt("tux_sphere_divisions") * resolution ) 
	    ) );
	node->sphere.resolution = resolution;
}

void
add_material(const std::string& name, const ppogl::Material& material)
{
	materials[name]=material;
}

void
draw_scene_graph(const std::string& node)
{
    SceneNode *nodePtr;

    if(get_scene_node( node, &nodePtr ) != true){
        PP_ERROR( "draw_scene_graph: No such node: " << node);
    } 

    traverse_dag( nodePtr, &g_hier_default_material );
} 

bool
collide(const std::string& node, const ppogl::Polyhedron& ph)
{
    SceneNode *nodePtr;

    if(get_scene_node(node, &nodePtr) != true){
        PP_ERROR( "draw_scene_graph: No such node: " << node );
    } 
	
	pp::Matrix mat, invmat;
    mat.makeIdentity();
    invmat.makeIdentity();

    return check_polyhedron_collision_with_dag( nodePtr, mat, invmat, ph );
}

void
update_scene_nodes()
{
	//update sphere divisions for all nodes	
	std::map<std::string, SceneNode *>::iterator it;
	for(it=nodes.begin(); it!=nodes.end();it++){	
		(*it).second->sphere.divisions = MIN( 
			MAX_SPHERE_DIVISIONS, MAX( 
	    	MIN_SPHERE_DIVISIONS, 
	    	ROUND_TO_NEAREST(PPConfig.getInt("tux_sphere_divisions") * (*it).second->sphere.resolution) 
	    	));	
	}
}

void
cleanup_scene_node()
{
	std::map<std::string, SceneNode *>::iterator it;
	for(it=nodes.begin(); it!=nodes.end();it++){	
		delete (*it).second;
	}
	nodes.clear();
}
