/* 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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

#include "callbacks.h"

#include "tcl_util.h"
#include "ppgltk/font.h"
#include "ppgltk/alg/color.h"

#include "ppgltk/audio/audio.h"
#include "ppgltk/audio/audio_data.h"


static int
register_font_cb ( ClientData cd, Tcl_Interp *ip, 
			  int argc, CONST84 char *argv[])
{
	bool error = false;
	double tmp_dbl;

    CONST84 char *binding = NULL;
    CONST84 char *fileName = NULL;
    pp::Color color = pp::Color::white;
    unsigned int size = 30;

	if ( argc < 7 ) {
		Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " -binding <binding> -font <ttf file> -size <fontsize> [-color <color>]",
			 NULL );
        return TCL_ERROR;
    } 
	    
    NEXT_ARG;

    while ( !error && argc > 0 ) {

	if ( strcmp( "-binding", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    binding = *argv;
	} else if ( strcmp( "-font", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    fileName = *argv;
	} else if ( strcmp( "-size", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    if ( Tcl_GetDouble ( ip, *argv, &tmp_dbl ) == TCL_ERROR ) {
		error = true;
		break;
	    }
	    size = int(tmp_dbl);
	} else if ( strcmp( "-color", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
		double tmp_color[4];
	    if ( get_tcl_tuple( ip, *argv, tmp_color, 4 ) == 
		 TCL_ERROR ) 
	    {
		error = true;
		break;
	    }
		color.set(tmp_color);
		
	} else {
	    PP_WARNING( "pp_load_font: unrecognized "
			   "parameter `%s'", *argv );
	}

	NEXT_ARG;
    }

    if ( binding == NULL || fileName == NULL ){
		error = true;
    }

    if ( error ) {
		return TCL_ERROR;
    }

	pp::Font::registerFont(binding, fileName, size, color);

    return TCL_OK;
}

static int
bind_font_cb ( ClientData cd, Tcl_Interp *ip, 
			  int argc, CONST84 char *argv[])
{
	bool error = false;

    CONST84 char *binding = NULL;
    CONST84 char *fontName = NULL;
    pp::Color *color = NULL;

	NEXT_ARG;

    while ( !error && argc > 0 ) {

	if ( strcmp( "-binding", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
		error = true;
		break;
	    }
	    binding = *argv;
	} else if ( strcmp( "-font", *argv ) == 0 ) {
	    NEXT_ARG;
		if ( argc == 0 ) {
			error = true;
			break;
	    }
	    fontName = *argv;
	} else if ( strcmp( "-color", *argv ) == 0 ) {
	    NEXT_ARG;
	    if ( argc == 0 ) {
			error = true;
			break;
	    }
		
		double tmp_color[4];		
		
	    if ( get_tcl_tuple( ip, *argv, tmp_color, 4 ) == 
		 TCL_ERROR ) 
	    {
			error = true;
			break;
	    }
		
		color = new pp::Color(tmp_color);	
		
	} else {
	    PP_WARNING( "pp_bind_font: unrecognized "
			   "parameter `%s'", *argv );
	}

	NEXT_ARG;
    }

    if ( binding == NULL || fontName == NULL ){
		error = true;
    }

    if ( error ) {
		return TCL_ERROR;
    }

	if(color){
		pp::Font::bindFont(binding, fontName, *color);
		delete color;
	}else{
		pp::Font::bindFont(binding, fontName);
	}   
    return TCL_OK;
}

static int
load_sound_cb( ClientData cd, Tcl_Interp *ip, 
			  int argc, CONST84 char *argv[]) 
{
	Tcl_Obj *result;

    if ( argc != 3 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <name> <sound file>",
			 NULL );
        return TCL_ERROR;
    } 

    result = Tcl_NewBooleanObj( load_sound( argv[1], argv[2] ) );
    Tcl_SetObjResult( ip, result );
    return TCL_OK;

} 

static int
load_music_cb( ClientData cd, Tcl_Interp *ip, 
			  int argc, CONST84 char *argv[]) 
{
    Tcl_Obj *result;

    if ( argc != 3 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <name> <sound file>",
			 NULL );
        return TCL_ERROR;
    } 

    result = Tcl_NewBooleanObj( load_music( argv[1], argv[2] ) );
    Tcl_SetObjResult( ip, result );
    return TCL_OK;
}

/*! 
  Tcl callback for tux_bind_sounds
  \author  jfpatry
  \date    Created:  2000-08-14
  \date    Modified: 2000-08-14
*/
static int
bind_sounds_cb( ClientData cd, Tcl_Interp *ip, 
			   int argc, CONST84 char *argv[]) 
{
    if ( argc < 2 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <sound context> <sound name>"
			 " [<sound name> ...]",
			 NULL );
        return TCL_ERROR;
    } 

    bind_sounds_to_context( argv[1], argv+2, argc-2 );
    return TCL_OK;
} 

/*! 
  Tcl callback for tux_bind_music
  \author  jfpatry
  \date    Created:  2000-08-14
  \date    Modified: 2000-08-14
*/
static int
bind_music_cb( ClientData cd, Tcl_Interp *ip, 
			  int argc, CONST84 char *argv[]) 
{
    int loops;

    if ( argc != 4 ) {
        Tcl_AppendResult(ip, argv[0], ": invalid number of arguments\n", 
			 "Usage: ", argv[0], " <music context> <music name> "
			 "<loops>",
			 NULL );
        return TCL_ERROR;
    } 

    if ( Tcl_GetInt( ip, argv[3], &loops ) ) {
		Tcl_AppendResult(
	    ip, "invalid value for loops parameter", NULL );
		return TCL_ERROR;
    }

    bind_music_to_context( argv[1], argv[2], loops );

    return TCL_OK;
} 



void
register_common_callbacks( Tcl_Interp *ip )
{
    Tcl_CreateCommand (ip, "pp_load_font", register_font_cb,   0,0);
    Tcl_CreateCommand (ip, "pp_bind_font", bind_font_cb,   0,0);

	
	Tcl_CreateCommand (ip, "tux_load_sound", load_sound_cb,  0,0);
    Tcl_CreateCommand (ip, "tux_load_music", load_music_cb,  0,0);

    Tcl_CreateCommand (ip, "tux_bind_sounds", bind_sounds_cb,  0,0);
    Tcl_CreateCommand (ip, "tux_bind_music", bind_music_cb,  0,0);	
}
