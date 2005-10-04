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
 
#include "course_load.h"
#include "quadtree.h"
#include "gameconfig.h"

#include "ppogl/base/defs.h"

#define CULL_DETAIL_FACTOR 25

static quadsquare *root = NULL;
static quadcornerdata root_corner_data = { NULL };


void
reset_course_quadtree()
{
    delete root;
    root = NULL;
}

static int
get_root_level(int nx, int nz)
{
    PP_REQUIRE( nx > 0, "heightmap has x dimension of 0 size" );
    PP_REQUIRE( nz > 0, "heightmap has z dimension of 0 size" );
	
	int xlev, zlev;

    xlev = int( log(double(nx) ) / log (double(2.0) ) );
    zlev = int( log(double(nz) ) / log (double(2.0) ) );

    /* Check to see if nx, nz are powers of 2 
     */

    if ( ( nx >> xlev ) << xlev == nx ) {
	/* do nothing */
    } else {
	nx += 1;
    }

    if ( ( nz >> zlev ) << zlev == nz ) {
	/* do nothing */
    } else {
	nz += 1;
    }

    return MAX( xlev, zlev );
}

static void
Vec3fo_float_array(float dest[3],const ppogl::Vec3d& src)
{
    dest[0] = src.x();
    dest[1] = src.y();
    dest[2] = src.z();
}

void
init_course_quadtree(const float *elevation, int nx, int nz, 
			   double scalex, double scalez,
			   const ppogl::Vec3d& view_pos)
{
	HeightMapInfo hm;

	hm.data = elevation;
	hm.xOrigin = 0;
	hm.zOrigin = 0;
	hm.xSize = nx;
	hm.zSize = nz;
	hm.rowWidth = hm.xSize;
	hm.scale = 0;

    root_corner_data.square = NULL;
    root_corner_data.childIndex = 0;
    root_corner_data.level = get_root_level( nx, nz );
    root_corner_data.xorg = 0;
    root_corner_data.zorg = 0;

    for(int i=0; i<4; i++){
		root_corner_data.verts[i] = 0;
		root_corner_data.verts[i] = 0;
    }

    root = new quadsquare( &root_corner_data );

    root->addHeightMap( root_corner_data, hm );
    root->setScale( scalex, scalez );
    root->setTerrain(Course::getTerrainData() );

	//update static configuration
	GameConfig::update();	
	
    // Debug info.
    //print_debug( DEBUG_QUADTREE, "nodes = %d\n", root->CountNodes());
    PP_LOG( DEBUG_QUADTREE, "max error = " <<
		 root->recomputeError(root_corner_data));

    // Get rid of unnecessary nodes in flat-ish areas.
    PP_LOG( DEBUG_QUADTREE, 
		 "Culling unnecessary nodes (detail factor = " << CULL_DETAIL_FACTOR << ")...");
    root->staticCullData(root_corner_data, CULL_DETAIL_FACTOR);

    // Post-cull debug info.
    PP_LOG( DEBUG_QUADTREE, "nodes = " << root->countNodes());
    PP_LOG( DEBUG_QUADTREE, "max error = " << root->recomputeError(root_corner_data));


    // Run the update function a few times before we start rendering
    // to disable unnecessary quadsquares, so the first frame won't
    // be overloaded with tons of triangles.

    float ViewerLoc[3];
    Vec3fo_float_array( ViewerLoc, view_pos );

    for(int i = 0; i < 10; i++){
		root->update(root_corner_data, reinterpret_cast<const float*>(ViewerLoc));
    }
}


void
update_course_quadtree(const ppogl::Vec3d& view_pos)
{
    float ViewerLoc[3];

    Vec3fo_float_array(ViewerLoc, view_pos);

    root->update(root_corner_data, ViewerLoc);
}

void
render_course_quadtree()
{
    GLubyte *vnc_array;

    Course::getGLArrays(&vnc_array);

    root->render(root_corner_data, vnc_array);
}
