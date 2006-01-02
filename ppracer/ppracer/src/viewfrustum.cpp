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

#include "viewfrustum.h"

#include "loop.h"
#include "ppogl/base/defs.h"

ViewFrustum viewFrustum;

/* This will be used as a bitfield to select the "n" and "p" vertices
of the bounding boxes wrt to each plane.  
*/
bool ViewFrustum::p_vertex_code_x[6];
bool ViewFrustum::p_vertex_code_y[6];
bool ViewFrustum::p_vertex_code_z[6];

void
ViewFrustum::setup(const Player& plyr,
			const double near_dist, const double far_dist,
			const int multiscreen)
{
    const double aspect = double(GameMode::resolution.x()) /
					double(GameMode::resolution.y());

    double half_fov = ANGLES_TO_RADIANS( GameConfig::fov * 0.5 );
    double half_fov_horiz = atan(tan(half_fov) * aspect); 
	
	if(multiscreen>=0){
		//in multiscreen mode we have to double the horizontal aspect
		half_fov_horiz*=2; 	
	}

    // create frustum in viewing coordinates

    // near
    planes[0] = pp::Plane(0, 0, 1, near_dist);
    
    // far
    planes[1] = pp::Plane(0, 0, -1, -far_dist);

    // left
    planes[2] = pp::Plane( -cos(half_fov_horiz), 0, 
				    sin(half_fov_horiz), 0 );

    // right
    planes[3] = pp::Plane( cos(half_fov_horiz), 0, 
				    sin(half_fov_horiz), 0 );

    // top
    planes[4] = pp::Plane( 0, cos(half_fov),  
				    sin(half_fov), 0 );

    // bottom
    planes[5] = pp::Plane( 0, -cos(half_fov),  
				    sin(half_fov), 0 );

    // We now transform frustum to world coordinates
    for(int i=0; i<6; i++){
		const ppogl::Vec3d pt = plyr.view.inv_view_mat.transformPoint(
	    	ppogl::Vec3d::origin() + (-planes[i].d*planes[i].nml) );

		planes[i].nml = plyr.view.inv_view_mat.transformVector(planes[i].nml );

		planes[i].d = -(planes[i].nml*(pt-ppogl::Vec3d::origin()));
    }

    for(int i=0; i<6; i++){
		p_vertex_code_x[i] = false;
		p_vertex_code_y[i] = false;
		p_vertex_code_z[i] = false;
		
		
		if(planes[i].nml.x() > 0){
			p_vertex_code_x[i] = true;
		}
		if(planes[i].nml.y() > 0){
			p_vertex_code_y[i] = true;
		}
		if(planes[i].nml.z() > 0){
			p_vertex_code_z[i] = true;
		}
    }
}

/** View frustum clipping for AABB (axis-aligned bounding box). See
   Assarsson, Ulf and Tomas M\"oller, "Optimized View Frustum Culling
   Algorithms", unpublished, http://www.ce.chalmers.se/staff/uffe/ .  */
ViewFrustum::ClipResult
ViewFrustum::clip(const ppogl::Vec3d& min, const ppogl::Vec3d& max)
{
    ppogl::Vec3d n, p;
    ClipResult intersect = NoClip;

    for(int i=5; i>=0; i--){
		p = min;
		n = max;
		
		if(p_vertex_code_x[i]){
		    p.x() = max.x();
		    n.x() = min.x();
		}

		if(p_vertex_code_y[i]){
		    p.y() = max.y();
		    n.y() = min.y();
		}

		if(p_vertex_code_z[i]){
		    p.z() = max.z();
		    n.z() = min.z();
		}

		if(n*planes[i].nml +
		     planes[i].d > 0)
		{
		    return NotVisible;
		}

		if(p*planes[i].nml +
		     planes[i].d > 0)
		{
			intersect = SomeClip;
		}
    }	
    return intersect;
}
