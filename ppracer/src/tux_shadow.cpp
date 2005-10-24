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

#include "tux_shadow.h"
#include "gl_util.h"
#include "tux.h"
#include "hier.h"
#include "phys_sim.h"
#include "gameconfig.h"

#include "ppogl/base/glwrappers.h"

#define SHADOW_HEIGHT 0.1

void
draw_tux_shadow(int player)
{
    if(!GameConfig::drawTuxShadow){
		return;
	}
	
	pp::Matrix model_matrix;
    SceneNode *tux_root_node;
		
    set_gl_options( TUX_SHADOW ); 
	
	/* 
 	* Make the shadow darker if the stencil buffer is active 
 	*/
	
	if(GameConfig::enableStencilBuffer){
    	gl::Color(0.0f,0.0f,0.0f,0.3f);
	}else{
    	gl::Color(0.0f,0.0f,0.0f,0.1f);
	}
	
    model_matrix.makeIdentity();

	const std::string& tux_root_node_name = tux[player].getRootNode();

    if(get_scene_node(tux_root_node_name, &tux_root_node ) != true){
		PP_ERROR( "couldn't find tux's root node" );
    } 

    traverse_dag_for_shadow( tux_root_node, model_matrix );
}

void
traverse_dag_for_shadow(SceneNode *node, const pp::Matrix& model_matrix)
{
    pp::Matrix new_model_matrix;
    SceneNode *child;

    PP_CHECK_POINTER( node );

    new_model_matrix=model_matrix*node->trans;

    if(node->geom == Sphere && node->renderShadow){
		draw_shadow_sphere( new_model_matrix );
    } 

    child = node->child;
    while (child != NULL) {

        traverse_dag_for_shadow(child, new_model_matrix);

        child = child->next;
    } 
}

void
draw_shadow_sphere(const pp::Matrix& model_matrix)
{
    double theta, phi, d_theta, d_phi, eps, twopi;
    double x, y, z;
    int div = GameConfig::tuxShadowSphereDivisions;
    
    eps = 1e-15;
    twopi = M_PI * 2.0;

    d_theta = d_phi = M_PI / div;

    for ( phi = 0.0; phi + eps < M_PI; phi += d_phi ) {
	double cos_theta, sin_theta;
	double sin_phi, cos_phi;
	double sin_phi_d_phi, cos_phi_d_phi;

	sin_phi = sin( phi );
	cos_phi = cos( phi );
	sin_phi_d_phi = sin( phi + d_phi );
	cos_phi_d_phi = cos( phi + d_phi );
        
        if ( phi <= eps ) {

            glBegin( GL_TRIANGLE_FAN );
		draw_shadow_vertex( 0., 0., 1., model_matrix );

                for ( theta = 0.0; theta + eps < twopi; theta += d_theta ) {
		    sin_theta = sin( theta );
		    cos_theta = cos( theta );

                    x = cos_theta * sin_phi_d_phi;
		    y = sin_theta * sin_phi_d_phi;
                    z = cos_phi_d_phi;
		    draw_shadow_vertex( x, y, z, model_matrix );
                } 

		x = sin_phi_d_phi;
		y = 0.0;
		z = cos_phi_d_phi;
		draw_shadow_vertex( x, y, z, model_matrix );
            glEnd();

        } else if ( phi + d_phi + eps >= M_PI ) {
            
            gl::Begin( GL_TRIANGLE_FAN );
		draw_shadow_vertex( 0., 0., -1., model_matrix );

                for ( theta = twopi; theta - eps > 0; theta -= d_theta ) {
		    sin_theta = sin( theta );
		    cos_theta = cos( theta );

                    x = cos_theta * sin_phi;
                    y = sin_theta * sin_phi;
                    z = cos_phi;
		    draw_shadow_vertex( x, y, z, model_matrix );
                } 
                x = sin_phi;
                y = 0.0;
                z = cos_phi;
		draw_shadow_vertex( x, y, z, model_matrix );
            gl::End();

        } else {
            
            gl::Begin( GL_TRIANGLE_STRIP );
                
                for ( theta = 0.0; theta + eps < twopi; theta += d_theta ) {
		    sin_theta = sin( theta );
		    cos_theta = cos( theta );

                    x = cos_theta * sin_phi;
                    y = sin_theta * sin_phi;
                    z = cos_phi;
		    draw_shadow_vertex( x, y, z, model_matrix );

                    x = cos_theta * sin_phi_d_phi;
                    y = sin_theta * sin_phi_d_phi;
                    z = cos_phi_d_phi;
		    draw_shadow_vertex( x, y, z, model_matrix );
                } 
                x = sin_phi;
                y = 0.0;
                z = cos_phi;
		draw_shadow_vertex( x, y, z, model_matrix );

                x = sin_phi_d_phi;
                y = 0.0;
                z = cos_phi_d_phi;
		draw_shadow_vertex( x, y, z, model_matrix );

            gl::End();

        } 
    } 

} 

void
draw_shadow_vertex(double x, double y, double z, 
			 const pp::Matrix& model_matrix )
{
    ppogl::Vec3d pt;
    double old_y;
    ppogl::Vec3d nml;

    pt = ppogl::Vec3d( x, y, z );
    pt = model_matrix.transformPoint( pt );

    old_y = pt.y();
    
    nml = find_course_normal( pt.x(), pt.z() );
    pt.y() = find_y_coord( pt.x(), pt.z() ) + SHADOW_HEIGHT;

    if ( pt.y() > old_y ) 
	pt.y() = old_y;

    gl::Normal( nml.x(), nml.y(), nml.z() );
    gl::Vertex( pt.x(), pt.y(), pt.z() );
}
