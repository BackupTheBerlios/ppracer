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

#include "tux_shadow.h"
#include "gl_util.h"
#include "tux.h"
#include "hier.h"
#include "phys_sim.h"
#include "gameconfig.h"

#include "ppogl/base/glwrappers.h"

#define SHADOW_HEIGHT 0.1

static void traverse_dag_for_shadow(SceneNode *node, const pp::Matrix& model_matrix);
static void draw_shadow_sphere(const pp::Matrix& model_matrix);
static void draw_shadow_vertex(const ppogl::Vec3d& point, 
			 const pp::Matrix& model_matrix );


void
draw_tux_shadow(const int player)
{
    if(!GameConfig::drawTuxShadow){
		return;
	}
			
    set_gl_options( TUX_SHADOW ); 
	
	// Make the shadow darker if the stencil buffer is active
	if(GameConfig::enableStencilBuffer){
    	gl::Color(0.0f,0.0f,0.0f,0.3f);
	}else{
    	gl::Color(0.0f,0.0f,0.0f,0.1f);
	}
	
	pp::Matrix model_matrix;
    model_matrix.makeIdentity();

	const std::string& tux_root_node_name = tux[player].getRootNode();
	SceneNode *tux_root_node;
	
    if(get_scene_node(tux_root_node_name, &tux_root_node ) != true){
		PP_ERROR( "couldn't find tux's root node" );
    } 

    traverse_dag_for_shadow( tux_root_node, model_matrix );
}

static void
traverse_dag_for_shadow(SceneNode *node, const pp::Matrix& model_matrix)
{
    PP_CHECK_POINTER( node );
	
	const pp::Matrix new_model_matrix = model_matrix*node->trans;

    if(node->isSphere == true && node->renderShadow){
		draw_shadow_sphere( new_model_matrix );
    } 

    SceneNode* child = node->child;
    while (child != NULL) {
        traverse_dag_for_shadow(child, new_model_matrix);
        child = child->next;
    } 
}

static void
draw_shadow_sphere(const pp::Matrix& model_matrix)
{
	int div = GameConfig::tuxShadowSphereDivisions;
 
	double eps = 1e-15;
	double twopi = M_PI * 2.0;
	
	double d_phi;
    double d_theta = d_phi = M_PI / div;
	
	ppogl::Vec3d point;
	
    for(double phi = 0.0; phi + eps < M_PI; phi += d_phi ){
		double cos_theta, sin_theta;
		double sin_phi = sin( phi );
		double cos_phi = cos( phi );
		double sin_phi_d_phi = sin( phi + d_phi );
		double cos_phi_d_phi = cos( phi + d_phi );
        
        if(phi<=eps){
			glBegin( GL_TRIANGLE_FAN );
			draw_shadow_vertex(ppogl::Vec3d(0.0, 0.0, 1.0), model_matrix );

			for(double theta = 0.0; theta + eps < twopi; theta += d_theta){
		    	sin_theta = sin( theta );
		    	cos_theta = cos( theta );

                point.x() = cos_theta * sin_phi_d_phi;
		    	point.y() = sin_theta * sin_phi_d_phi;
                point.z() = cos_phi_d_phi;
		    	draw_shadow_vertex(point, model_matrix);
			} 

			point.x() = sin_phi_d_phi;
			point.y() = 0.0;
			point.z() = cos_phi_d_phi;
			draw_shadow_vertex(point, model_matrix);
            glEnd();

        }else if(phi+d_phi+eps >= M_PI){
			gl::Begin( GL_TRIANGLE_FAN );
			draw_shadow_vertex(ppogl::Vec3d(0.0, 0.0, -1.0), model_matrix );
			for(double theta = twopi; theta - eps > 0; theta -= d_theta ){
				sin_theta = sin( theta );
				cos_theta = cos( theta );

				point.x() = cos_theta * sin_phi;
				point.y() = sin_theta * sin_phi;
				point.z() = cos_phi;
		    	draw_shadow_vertex(point, model_matrix);
			} 
			point.x() = sin_phi;
			point.y() = 0.0;
			point.z() = cos_phi;
			draw_shadow_vertex(point, model_matrix);
			gl::End();

        }else{
			gl::Begin( GL_TRIANGLE_STRIP );
			for(double theta = 0.0; theta + eps < twopi; theta += d_theta ) {
				sin_theta = sin( theta );
				cos_theta = cos( theta );

				point.x() = cos_theta * sin_phi;
				point.y() = sin_theta * sin_phi;
				point.z() = cos_phi;
				draw_shadow_vertex(point, model_matrix);

				point.x() = cos_theta * sin_phi_d_phi;
				point.y() = sin_theta * sin_phi_d_phi;
				point.z() = cos_phi_d_phi;
				draw_shadow_vertex(point, model_matrix);
			}
			point.x() = sin_phi;
			point.y() = 0.0;
			point.z() = cos_phi;
			draw_shadow_vertex(point, model_matrix);

			point.x() = sin_phi_d_phi;
			point.y() = 0.0;
			point.z() = cos_phi_d_phi;
			draw_shadow_vertex(point, model_matrix);
			gl::End();
		}
	}
}

static void
draw_shadow_vertex(const ppogl::Vec3d& point, 
			 const pp::Matrix& model_matrix )
{
    ppogl::Vec3d pt =
		model_matrix.transformPoint(point);
    double old_y = pt.y();
    ppogl::Vec3d nml = find_course_normal( pt );
	
    pt.y() = find_y_coord( pt ) + SHADOW_HEIGHT;

    if(pt.y() > old_y){
		pt.y() = old_y;
	}
    gl::Normal(nml);
    gl::Vertex(pt);
}
