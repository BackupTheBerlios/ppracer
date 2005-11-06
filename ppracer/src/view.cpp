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

#include "view.h"
#include "phys_sim.h"
#include "tux.h"
#include "hier.h"
#include "stuff.h"

#include "ppogl/base/defs.h"
#include "ppogl/base/glwrappers.h"

/** This defines the camera height "target" for all cameras; 
   cameras can go below this because of interpolation (m) */
#define MIN_CAMERA_HEIGHT  1.5

/// Absolute lower bound on camera height above terrain (m)
#define ABSOLUTE_MIN_CAMERA_HEIGHT  0.3

/// Distance of camera from player (m)
#define CAMERA_DISTANCE 4.0

/// Angle that camera should look above the slope of the terrain (deg)
#define CAMERA_ANGLE_ABOVE_SLOPE 10

/// Angle at which to position player in camera below the horizontal (deg)
#define PLAYER_ANGLE_IN_CAMERA 20

/// Maximum downward pitch of camera (deg)
#define MAX_CAMERA_PITCH 40

/// Time constant for interpolation of camera position in "behind" mode (s)
#define BEHIND_ORBIT_TIME_CONSTANT 0.06

/** Time constant for interpolation of camera orientation in 
   "behind" mode (s) */
#define BEHIND_ORIENT_TIME_CONSTANT 0.06

/// Time constant for interpolation of camera position in "follow" mode (s)
#define FOLLOW_ORBIT_TIME_CONSTANT 0.06

/** Time constant for interpolation of orientation position in 
   "follow" mode (s) */
#define FOLLOW_ORIENT_TIME_CONSTANT 0.06

/** This places an upper limit on the interpolation, which prevent excessive
   "jumping" of the camera during the occaisonal slow frame */
#define MAX_INTERPOLATION_VALUE 0.3

/// Speed above which interpolation is "normal" (m/s)
#define BASELINE_INTERPOLATION_SPEED 4.5

/// Speed below which no camera position interpolation occurs (m/s)
#define NO_INTERPOLATION_SPEED 2.0

static ppogl::Vec3d tux_eye_pts[2];
//static ppogl::Vec3d tux_view_pt;
static ppogl::Vec3d tux_view_pt[2];

void
set_view_mode(Player& plyr, ViewMode mode)
{
    plyr.view.mode = mode;
    PP_LOG( DEBUG_VIEW, "View mode: " << plyr.view.mode );
} 

ppogl::Vec3d
interpolate_view_pos(const ppogl::Vec3d& plyr_pos1, const ppogl::Vec3d& plyr_pos2,
			      double max_vec_angle,
			      const ppogl::Vec3d& pos1, const ppogl::Vec3d& pos2,
			      double dist, double dt,
			      double time_constant )
{
    static ppogl::Vec3d y_vec(0.0, 1.0, 0.0);

    pp::Quat q1, q2;
    ppogl::Vec3d vec1, vec2;
    double alpha;
    double theta;
    pp::Matrix rot_mat;
    ppogl::Vec3d axis;

    vec1 = pos1 - plyr_pos1;
    vec2 = pos2 - plyr_pos2;

    vec1.normalize();
    vec2.normalize();

    q1 = pp::Quat( y_vec, vec1 );
    q2 = pp::Quat( y_vec, vec2 );

    alpha = MIN( MAX_INTERPOLATION_VALUE,
		 1.0 - exp ( -dt / time_constant ) );

    q2 = pp::Quat::interpolate( q1, q2, alpha );

    vec2 = q2.rotate( y_vec );

    /* Constrain angle with x-z plane */
    theta = RADIANS_TO_ANGLES( M_PI/2 - acos(vec2*y_vec) );

    if ( theta > max_vec_angle )
    {
	axis = y_vec^vec2;
	axis.normalize();

	rot_mat.makeRotationAboutVector( axis, 
					   theta-max_vec_angle );
	vec2 = rot_mat.transformVector( vec2 );
    }

    return plyr_pos2+dist*vec2;

}


/*! 
  Interpolates between camera orientations
  \pre     p_up2, p_dir2 != NULL; *p_up2 is the target up vector; 
	   *p_dir2 is the target view direction
  \arg \c  up1 original up vector
  \arg \c  dir1 original view direction
  \arg \c  p_up2 pointer to target up vector; is overwritten with 
           interpolated up vector
  \arg \c  p_dir2 pointer to target view direction; is overwritten with 
           interpolated view direction
  \arg \c  dt time step
  \arg \c  time_constant interpolation time constant

  \return  none
  \author  jfpatry
  \date    Created:  2000-08-26
  \date    Modified: 2000-08-26
*/
void
interpolate_view_frame( ppogl::Vec3d up1, ppogl::Vec3d dir1,
			     ppogl::Vec3d *p_up2, ppogl::Vec3d *p_dir2,
			     double dt, double time_constant )
{
    pp::Quat q1, q2;
    double alpha;
    ppogl::Vec3d x1, y1, z1;
    ppogl::Vec3d x2, y2, z2;
    pp::Matrix cob_mat1, inv_cob_mat1;
    pp::Matrix inv_cob_mat2;

    /* Now interpolate between camera orientations */
    z1 = -1.0*dir1;
    z1.normalize();

    y1 = projectIntoPlane( z1, up1 );
    y1.normalize();

    x1 = y1^z1;

    pp::Matrix::makeChangeOfBasisMatrix( cob_mat1, inv_cob_mat1,
				 x1, y1, z1 );

    q1 = pp::Quat( cob_mat1 );

    z2 = -1.0*(*p_dir2);
    z2.normalize();

    y2 = projectIntoPlane( z2, *p_up2 );
    y2.normalize();

    x2 = y2^z2;
	{
		pp::Matrix cob_mat2;
    	pp::Matrix::makeChangeOfBasisMatrix( cob_mat2, inv_cob_mat2,
				 x2, y2, z2 );

    	q2 = pp::Quat( cob_mat2 );
	}
    alpha = MIN( MAX_INTERPOLATION_VALUE, 
		 1.0 - exp( -dt / time_constant ) );

    q2 = pp::Quat::interpolate( q1, q2, alpha );

    pp::Matrix cob_mat2( q2 );

    p_up2->x() = cob_mat2.data[1][0];
    p_up2->y() = cob_mat2.data[1][1];
    p_up2->z() = cob_mat2.data[1][2];

    p_dir2->x() = -cob_mat2.data[2][0];
    p_dir2->y() = -cob_mat2.data[2][1];
    p_dir2->z() = -cob_mat2.data[2][2];
}

void
setup_view_matrix(Player& plyr) 
{
    ppogl::Vec3d view_x, view_y, view_z;
    pp::Matrix view_mat;
    ppogl::Vec3d viewpt_in_view_frame;

    view_z = -1*plyr.view.dir;
    view_x = plyr.view.up^view_z;
    view_y = view_z^view_x;
    view_z.normalize();
    view_x.normalize();
    view_y.normalize();

    plyr.view.inv_view_mat.makeIdentity();

    plyr.view.inv_view_mat.data[0][0] = view_x.x();
    plyr.view.inv_view_mat.data[0][1] = view_x.y();
    plyr.view.inv_view_mat.data[0][2] = view_x.z();

    plyr.view.inv_view_mat.data[1][0] = view_y.x();
    plyr.view.inv_view_mat.data[1][1] = view_y.y();
    plyr.view.inv_view_mat.data[1][2] = view_y.z();

    plyr.view.inv_view_mat.data[2][0] = view_z.x();
    plyr.view.inv_view_mat.data[2][1] = view_z.y();
    plyr.view.inv_view_mat.data[2][2] = view_z.z();

    plyr.view.inv_view_mat.data[3][0] = plyr.view.pos.x();
    plyr.view.inv_view_mat.data[3][1] = plyr.view.pos.y();
    plyr.view.inv_view_mat.data[3][2] = plyr.view.pos.z();
    plyr.view.inv_view_mat.data[3][3] = 1;
    
    view_mat.transpose(plyr.view.inv_view_mat);

    view_mat.data[0][3] = 0;
    view_mat.data[1][3] = 0;
    view_mat.data[2][3] = 0;
    
    viewpt_in_view_frame = view_mat.transformPoint( plyr.view.pos );
    
    view_mat.data[3][0] = -viewpt_in_view_frame.x();
    view_mat.data[3][1] = -viewpt_in_view_frame.y();
    view_mat.data[3][2] = -viewpt_in_view_frame.z();
    
    gl::LoadIdentity();
    gl::MultMatrix( reinterpret_cast<double *>(view_mat.data) );
}

/*! 
  Updates camera and sets the view matrix
  \pre     plyr != NULL, plyr has been initialized with position & 
           velocity info., plyr->view.mode has been set
  \arg \c  plyr pointer to player data
  \arg \c  dt time step size  

  \return  none
  \author  jfpatry
  \date    Created:  2000-08-26
  \date    Modified: 2000-08-26
*/
void
update_view(Player& plyr, float dt)
{
    ppogl::Vec3d view_pt;
    ppogl::Vec3d view_dir, up_dir, vel_dir, view_vec;
    float ycoord;
    float course_angle;
    ppogl::Vec3d axis;
    pp::Matrix rot_mat;
    ppogl::Vec3d y_vec;
    ppogl::Vec3d mz_vec;
    ppogl::Vec3d vel_proj;
    pp::Quat rot_quat;
    float speed;
    ppogl::Vec3d vel_cpy;
    float time_constant_mult;

    vel_cpy = plyr.vel;
    speed = vel_cpy.normalize();

    time_constant_mult = 1.0 /
	MIN( 1.0, 
	     MAX( 0.0, 
		  ( speed - NO_INTERPOLATION_SPEED ) /
		  ( BASELINE_INTERPOLATION_SPEED - NO_INTERPOLATION_SPEED )));

    up_dir = ppogl::Vec3d( 0, 1, 0 );

    vel_dir = plyr.vel;
    vel_dir.normalize();

    course_angle = Course::getAngle();

    switch( plyr.view.mode ) {

    case BEHIND:
    {
	/* Camera-on-a-string mode */

	/* Construct vector from player to camera */
	view_vec = ppogl::Vec3d( 0, 
				sin( ANGLES_TO_RADIANS( 
				    course_angle -
				    CAMERA_ANGLE_ABOVE_SLOPE + 
				    PLAYER_ANGLE_IN_CAMERA ) ),
				cos( ANGLES_TO_RADIANS( 
				    course_angle -
				    CAMERA_ANGLE_ABOVE_SLOPE + 
				    PLAYER_ANGLE_IN_CAMERA ) ) );

	view_vec = CAMERA_DISTANCE*view_vec;

	y_vec = ppogl::Vec3d( 0.0, 1.0, 0.0 );
	mz_vec = ppogl::Vec3d( 0.0, 0.0, -1.0 );
	vel_proj = projectIntoPlane( y_vec, vel_dir );

	vel_proj.normalize();

	/* Rotate view_vec so that it places the camera behind player */
	rot_quat = pp::Quat( mz_vec, vel_proj );

	view_vec = rot_quat.rotate(view_vec);


	/* Construct view point */
	view_pt = plyr.pos - view_vec;

	/* Make sure view point is above terrain */
        ycoord = find_y_coord( view_pt.x(), view_pt.z() );

        if ( view_pt.y() < ycoord + MIN_CAMERA_HEIGHT ) {
            view_pt.y() = ycoord + MIN_CAMERA_HEIGHT;
        } 

	/* Interpolate view point */
	if ( plyr.view.initialized ) {
	    /* Interpolate twice to get a second-order filter */
	    int i;
	    for (i=0; i<2; i++) {
		view_pt = 
		    interpolate_view_pos( plyr.pos, plyr.pos, 
					  MAX_CAMERA_PITCH, plyr.view.pos, 
					  view_pt, CAMERA_DISTANCE, dt,
					  BEHIND_ORBIT_TIME_CONSTANT * 
					  time_constant_mult );
	    }
	}

	/* Make sure interpolated view point is above terrain */
        ycoord = find_y_coord( view_pt.x(), view_pt.z() );

        if ( view_pt.y() < ycoord + ABSOLUTE_MIN_CAMERA_HEIGHT ) {
            view_pt.y() = ycoord + ABSOLUTE_MIN_CAMERA_HEIGHT;
        } 

	/* Construct view direction */
	view_vec = view_pt - plyr.pos;
	
	axis = y_vec^view_vec;
	axis.normalize();
	
	rot_mat.makeRotationAboutVector( axis,
					   PLAYER_ANGLE_IN_CAMERA );
	view_dir = -1.0*rot_mat.transformVector( view_vec );

	/* Interpolate orientation of camera */
	if ( plyr.view.initialized ) {
	    /* Interpolate twice to get a second-order filter */
	    int i;
	    for (i=0; i<2; i++) {
		interpolate_view_frame( plyr.view.up, plyr.view.dir,
					&up_dir, &view_dir, dt,
					BEHIND_ORIENT_TIME_CONSTANT );
		up_dir = ppogl::Vec3d( 0.0, 1.0, 0.0 );
	    }
	}

        break;
    }

    case FOLLOW: 
    {
	/* Camera follows player (above and behind) */

	up_dir = ppogl::Vec3d( 0, 1, 0 );

	/* Construct vector from player to camera */
	view_vec = ppogl::Vec3d( 0, 
				sin( ANGLES_TO_RADIANS( 
				    course_angle -
				    CAMERA_ANGLE_ABOVE_SLOPE +
				    PLAYER_ANGLE_IN_CAMERA ) ),
				cos( ANGLES_TO_RADIANS( 
				    course_angle -
				    CAMERA_ANGLE_ABOVE_SLOPE + 
				    PLAYER_ANGLE_IN_CAMERA ) ) );
	view_vec = CAMERA_DISTANCE*view_vec;

	y_vec = ppogl::Vec3d( 0.0, 1.0, 0.0 );
	mz_vec = ppogl::Vec3d( 0.0, 0.0, -1.0 );
	vel_proj = projectIntoPlane( y_vec, vel_dir );

	vel_proj.normalize();

	/* Rotate view_vec so that it places the camera behind player */
	rot_quat = pp::Quat( mz_vec, vel_proj );

	view_vec = rot_quat.rotate( view_vec );


	/* Construct view point */
	view_pt = plyr.pos + view_vec;


	/* Make sure view point is above terrain */
        ycoord = find_y_coord( view_pt.x(), view_pt.z() );

        if ( view_pt.y() < ycoord + MIN_CAMERA_HEIGHT ) {
            view_pt.y() = ycoord + MIN_CAMERA_HEIGHT;
	}

	/* Interpolate view point */
	if ( plyr.view.initialized ) {
	    /* Interpolate twice to get a second-order filter */
	    int i;
	    for ( i=0; i<2; i++ ) {
		view_pt = 
		    interpolate_view_pos( plyr.view.plyr_pos, plyr.pos, 
					  MAX_CAMERA_PITCH, plyr.view.pos, 
					  view_pt, CAMERA_DISTANCE, dt,
					  FOLLOW_ORBIT_TIME_CONSTANT *
					  time_constant_mult );
	    }
	}

	/* Make sure interpolate view point is above terrain */
        ycoord = find_y_coord( view_pt.x(), view_pt.z() );

        if ( view_pt.y() < ycoord + ABSOLUTE_MIN_CAMERA_HEIGHT ) {
            view_pt.y() = ycoord + ABSOLUTE_MIN_CAMERA_HEIGHT;
        } 

	/* Construct view direction */
	view_vec = view_pt - plyr.pos;
	
	axis = y_vec^view_vec;
	axis.normalize();
	
	rot_mat.makeRotationAboutVector( axis, PLAYER_ANGLE_IN_CAMERA );
	view_dir = -1.0*rot_mat.transformVector( view_vec );

	/* Interpolate orientation of camera */
	if ( plyr.view.initialized ) {
	    /* Interpolate twice to get a second-order filter */
	    int i;
	    for ( i=0; i<2; i++ ) {
		interpolate_view_frame( plyr.view.up, plyr.view.dir,
					&up_dir, &view_dir, dt,
					FOLLOW_ORIENT_TIME_CONSTANT );
		up_dir = ppogl::Vec3d( 0.0, 1.0, 0.0 );
	    }
	}

        break;
    }

    case ABOVE:
    {
	/* Camera always uphill of player */

	up_dir = ppogl::Vec3d( 0, 1, 0 );


	/* Construct vector from player to camera */
	view_vec = ppogl::Vec3d( 0, 
				sin( ANGLES_TO_RADIANS( 
				    course_angle - 
				    CAMERA_ANGLE_ABOVE_SLOPE+
				    PLAYER_ANGLE_IN_CAMERA ) ),
				cos( ANGLES_TO_RADIANS( 
				    course_angle - 
				    CAMERA_ANGLE_ABOVE_SLOPE+ 
				    PLAYER_ANGLE_IN_CAMERA ) ) );
	view_vec = CAMERA_DISTANCE*view_vec;

	
	/* Construct view point */
	view_pt = plyr.pos + view_vec;


	/* Make sure view point is above terrain */
        ycoord = find_y_coord( view_pt.x(), view_pt.z() );

        if ( view_pt.y() < ycoord + MIN_CAMERA_HEIGHT ) {
            view_pt.y() = ycoord + MIN_CAMERA_HEIGHT;
	}

	/* Construct view direction */
	view_vec = view_pt - plyr.pos;

	rot_mat.makeRotation( PLAYER_ANGLE_IN_CAMERA, 'x' );
	view_dir = -1.0*rot_mat.transformVector( view_vec );

        break;
    }

    default:
	PP_NOT_REACHED();
    } 

    /* Create view matrix */
    plyr.view.pos = view_pt;
    plyr.view.dir = view_dir;
    plyr.view.up = up_dir;
    plyr.view.plyr_pos = plyr.pos;
    plyr.view.initialized = true;
}
