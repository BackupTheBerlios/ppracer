/* 
 * PPRacer 
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

#include "lights.h"
#include "gl_util.h"
#include "tcl_util.h"

static light_t course_lights[NUM_COURSE_LIGHTS];

light_t* get_course_lights() { 
    return course_lights; 
}


void reset_lights()
{
    for (int i=0; i<NUM_COURSE_LIGHTS; i++) {
		// Note: we initialize the lights to default OpenGL values
        // EXCEPT that light 0 isn't treated differently than the
        // others 
		course_lights[i].is_on = false;
		course_lights[i].spot_direction.z=-1.0;
		course_lights[i].spot_exponent = 0.0;
		course_lights[i].spot_cutoff = 180.0;
		course_lights[i].constant_attenuation = 1.0;
		course_lights[i].linear_attenuation = 0.0;
		course_lights[i].quadratic_attenuation = 0.0;
    }

    // Turn off global ambient light
    gl::LightModel(GL_LIGHT_MODEL_AMBIENT, pp::Color::black);
}

void setup_course_lighting()
{
    light_t *course_lights;

    course_lights = get_course_lights();

    for (int i=0; i<NUM_COURSE_LIGHTS; i++) {
		if ( !course_lights[i].is_on ) {
			gl::Disable( GL_LIGHT0 + i );
			continue;
		}
		gl::Enable( GL_LIGHT0 + i );

		gl::Light( GL_LIGHT0 + i, GL_AMBIENT, course_lights[i].ambient );
		gl::Light( GL_LIGHT0 + i, GL_DIFFUSE, course_lights[i].diffuse );
		gl::Light( GL_LIGHT0 + i, GL_SPECULAR, course_lights[i].specular );
		gl::Light( GL_LIGHT0 + i, GL_POSITION, course_lights[i].position );
		gl::Light( GL_LIGHT0 + i, GL_SPOT_DIRECTION, course_lights[i].spot_direction );
		gl::Light( GL_LIGHT0 + i, GL_SPOT_EXPONENT, 
			  course_lights[i].spot_exponent );
		gl::Light( GL_LIGHT0 + i, GL_SPOT_CUTOFF, 
			  course_lights[i].spot_cutoff );
		gl::Light( GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, 
			  course_lights[i].constant_attenuation );
		gl::Light( GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, 
			  course_lights[i].linear_attenuation );
		gl::Light( GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, 
			  course_lights[i].quadratic_attenuation );
	}
}

static int course_light_cb (ClientData cd, Tcl_Interp *ip, 
			    int argc, CONST84 char **argv) 
{
    int light_num;
    double tmp_arr[4];
    double tmp_dbl;
    bool error = false;
    
    if (argc < 3) {
	error = true;
    }

    NEXT_ARG;

    if ( Tcl_GetInt( ip, *argv, &light_num ) == TCL_ERROR ) {
	error = true;
    }

    if ( light_num < 0 || light_num >= NUM_COURSE_LIGHTS ) {
	error = true;
    }

    NEXT_ARG;

    while ( !error && argc > 0 ) {
	if ( strcmp( "-on", *argv ) == 0 ) {
	    course_lights[light_num].is_on = true;
	} else if ( strcmp( "-off", *argv ) == 0 ) {
	    course_lights[light_num].is_on = false;
	} else if ( strcmp( "-ambient", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    if ( get_tcl_tuple ( ip, *argv, tmp_arr, 4 ) == TCL_ERROR ) {
		error = true;
		break;
	    }
		course_lights[light_num].ambient.set(tmp_arr);
		
	} else if ( strcmp( "-diffuse", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    if ( get_tcl_tuple ( ip, *argv, tmp_arr, 4 ) == TCL_ERROR ) {
		error = true;
		break;
	    }
		course_lights[light_num].diffuse.set(tmp_arr);

	} else if ( strcmp( "-specular", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    if ( get_tcl_tuple ( ip, *argv, tmp_arr, 4 ) == TCL_ERROR ) {
		error = true;
		break;
	    }
		course_lights[light_num].specular.set(tmp_arr);
		
	} else if ( strcmp( "-position", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
			error = true;
			break;
	    }
	    if ( get_tcl_tuple ( ip, *argv, tmp_arr, 4 ) == TCL_ERROR ) {
			error = true;
			break;
	    }
		course_lights[light_num].position.set(tmp_arr);
		
	} else if ( strcmp( "-spot_direction", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
			error = true;
			break;
	    }
	    if ( get_tcl_tuple ( ip, *argv, tmp_arr, 3 ) == TCL_ERROR ) {
			error = true;
			break;
	    }
		course_lights[light_num].spot_direction.set(tmp_arr);

	} else if ( strcmp( "-spot_exponent", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    if ( Tcl_GetDouble ( ip, *argv, &tmp_dbl ) == TCL_ERROR ) {
		error = true;
		break;
	    }
	    course_lights[light_num].spot_exponent = tmp_dbl;
	} else if ( strcmp( "-spot_cutoff", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    if ( Tcl_GetDouble ( ip, *argv, &tmp_dbl ) == TCL_ERROR ) {
		error = true;
		break;
	    }
	    course_lights[light_num].spot_cutoff = tmp_dbl;
	} else if ( strcmp( "-constant_attenuation", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    if ( Tcl_GetDouble ( ip, *argv, &tmp_dbl ) == TCL_ERROR ) {
		error = true;
		break;
	    }
	    course_lights[light_num].constant_attenuation = tmp_dbl;
	} else if ( strcmp( "-linear_attenuation", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    if ( Tcl_GetDouble ( ip, *argv, &tmp_dbl ) == TCL_ERROR ) {
		error = true;
		break;
	    }
	    course_lights[light_num].linear_attenuation = tmp_dbl;
	} else if ( strcmp( "-quadratic_attenuation", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    if ( Tcl_GetDouble ( ip, *argv, &tmp_dbl ) == TCL_ERROR ) {
		error = true;
		break;
	    }
	    course_lights[light_num].quadratic_attenuation = tmp_dbl;
	} else {
	    print_warning( TCL_WARNING, "tux_course_light: unrecognized "
			   "parameter `%s'", *argv );
	}

	NEXT_ARG;
    }

    if ( error ) {
	print_warning( TCL_WARNING, "error in call to tux_course_light" );
	Tcl_AppendResult(
	    ip, 
	    "\nUsage: tux_course_light <light_number> [-on|-off] "
	    "[-ambient { r g b a }] "
	    "[-diffuse { r g b a }] "
	    "[-specular { r g b a }] "
	    "[-position { x y z w }] "
	    "[-spot_direction { x y z }] "
	    "[-spot_exponent <value>] "
	    "[-spot_cutoff <value>] "
	    "[-constant_attenuation <value>] "
	    "[-linear_attenuation <value>] "
	    "[-quadratic_attenuation <value>] ",
	    (char *) 0 );
	return TCL_ERROR;
    }
    
    return TCL_OK;
}

void register_course_light_callbacks( Tcl_Interp *ip )
{
    Tcl_CreateCommand (ip, "tux_course_light", course_light_cb, 0,0);
}
