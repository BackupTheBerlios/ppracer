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


#include "SDL.h"


#if defined( HAVE_GL_GLX_H )
#   include <GL/glx.h>
#endif // defined( HAVE_GL_GLX_H ) 

#include "gl_util.h"
#include "game_config.h"

#include "ppgltk/alg/glhelper.h"

void set_gl_options( const RenderMode mode ) 
{
    /* Must set the following options:
         Enable/Disable:
	   GL_TEXTURE_2D
	   GL_DEPTH_TEST
	   GL_CULL_FACE
	   GL_LIGHTING
	   GL_NORMALIZE
	   GL_ALPHA_TEST
	   GL_BLEND
	   GL_STENCIL_TEST
	   GL_TEXTURE_GEN_S
	   GL_TEXTURE_GEN_T
	   GL_COLOR_MATERIAL
           
	 Other Functions:
	   glDepthMask
	   glShadeModel
	   glDepthFunc
    */

    /*
     * Modify defaults based on rendering mode
     * 
     * This could could be improved if it stored state and avoided
     * redundant state changes, which are costly (or so I've heard)...  
     */
    switch( mode ) {
    case GUI:
        gl::Enable(GL_TEXTURE_2D);
        gl::Disable(GL_DEPTH_TEST);
        gl::Disable(GL_CULL_FACE);
		gl::Disable(GL_LIGHTING);
		gl::Disable(GL_NORMALIZE);
		gl::Disable(GL_ALPHA_TEST);
        gl::Enable(GL_BLEND);
		gl::Disable(GL_STENCIL_TEST);
		gl::Disable(GL_TEXTURE_GEN_S);
		gl::Disable(GL_TEXTURE_GEN_T);
		gl::Disable(GL_COLOR_MATERIAL);
		gl::DepthMask(GL_TRUE);
		gl::ShadeModel(GL_SMOOTH);
		gl::DepthFunc(GL_LESS);
		gl::Disable(GL_FOG);
        break;
    case GAUGE_BARS:
        gl::Enable(GL_TEXTURE_2D);
        gl::Disable(GL_DEPTH_TEST);
        gl::Disable(GL_CULL_FACE);
		gl::Disable(GL_LIGHTING);
		gl::Disable(GL_NORMALIZE);
		gl::Disable(GL_ALPHA_TEST);
        gl::Enable(GL_BLEND);
		gl::Disable(GL_STENCIL_TEST);
		gl::Enable(GL_TEXTURE_GEN_S);
		gl::Enable(GL_TEXTURE_GEN_T);
		gl::Disable(GL_COLOR_MATERIAL);
		gl::DepthMask(GL_TRUE);
		gl::ShadeModel(GL_SMOOTH);
		gl::DepthFunc(GL_LESS);

		gl::TexGen(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		gl::TexGen(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        break;

    case TEXFONT:
        gl::Enable(GL_TEXTURE_2D);
        gl::Disable(GL_DEPTH_TEST);
        gl::Disable(GL_CULL_FACE);
		gl::Disable(GL_LIGHTING);
		gl::Disable(GL_NORMALIZE);
		gl::Disable(GL_ALPHA_TEST);
        gl::Enable(GL_BLEND);
		gl::Disable(GL_STENCIL_TEST);
		gl::Disable(GL_TEXTURE_GEN_S);
		gl::Disable(GL_TEXTURE_GEN_T);
		gl::Disable(GL_COLOR_MATERIAL);
		gl::DepthMask(GL_TRUE);
		gl::ShadeModel(GL_SMOOTH);
		gl::DepthFunc(GL_LESS);
        break;

    case TEXT:
        gl::Disable(GL_TEXTURE_2D);
        gl::Disable(GL_DEPTH_TEST);
        gl::Disable(GL_CULL_FACE);
		gl::Disable(GL_LIGHTING);
		gl::Disable(GL_NORMALIZE);
		gl::Disable(GL_ALPHA_TEST);
        gl::Enable(GL_BLEND);
		gl::Disable(GL_STENCIL_TEST);
		gl::Disable(GL_TEXTURE_GEN_S);
		gl::Disable(GL_TEXTURE_GEN_T);
		gl::Disable(GL_COLOR_MATERIAL);
		gl::DepthMask(GL_TRUE);
		gl::ShadeModel(GL_SMOOTH);
		gl::DepthFunc(GL_LESS);
        break;

    case SPLASH_SCREEN:
        gl::Disable( GL_TEXTURE_2D );
        gl::Disable( GL_DEPTH_TEST );
        gl::Disable( GL_CULL_FACE );
		gl::Disable( GL_LIGHTING );
		gl::Disable( GL_NORMALIZE );
		gl::Disable( GL_ALPHA_TEST );
        gl::Enable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Disable( GL_TEXTURE_GEN_S );
		gl::Disable( GL_TEXTURE_GEN_T );
		gl::Disable( GL_COLOR_MATERIAL );
		gl::DepthMask( GL_TRUE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LESS );
        break;

    case COURSE:
		gl::Enable( GL_TEXTURE_2D );
		gl::Enable( GL_DEPTH_TEST );
		gl::Enable( GL_CULL_FACE );
		gl::Enable( GL_LIGHTING );
		gl::Disable( GL_NORMALIZE );
		gl::Disable( GL_ALPHA_TEST );
		gl::Enable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Enable( GL_TEXTURE_GEN_S );
		gl::Enable( GL_TEXTURE_GEN_T );
		gl::Enable( GL_COLOR_MATERIAL );
		gl::DepthMask( GL_TRUE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LEQUAL );

		gl::TexGen( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
		gl::TexGen( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
		break;

    case TREES:
		gl::Enable( GL_TEXTURE_2D );
		gl::Enable( GL_DEPTH_TEST );
        gl::Disable( GL_CULL_FACE );
		gl::Enable( GL_LIGHTING );
		gl::Disable( GL_NORMALIZE );
        gl::Enable( GL_ALPHA_TEST );
		gl::Enable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Disable( GL_TEXTURE_GEN_S );
		gl::Disable( GL_TEXTURE_GEN_T );
		gl::Disable( GL_COLOR_MATERIAL );
		gl::DepthMask( GL_TRUE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LESS );

        gl::AlphaFunc( GL_GEQUAL, 0.5 );
        break;
        
    case PARTICLES:
        gl::Enable( GL_TEXTURE_2D );
		gl::Enable( GL_DEPTH_TEST );
        gl::Disable( GL_CULL_FACE );
		gl::Disable( GL_LIGHTING );
		gl::Disable( GL_NORMALIZE );
		gl::Enable( GL_ALPHA_TEST );
        gl::Enable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Disable( GL_TEXTURE_GEN_S );
		gl::Disable( GL_TEXTURE_GEN_T );
		gl::Disable( GL_COLOR_MATERIAL );
		gl::DepthMask( GL_TRUE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LESS );

        gl::AlphaFunc( GL_GEQUAL, 0.5 );
        break;    
        
    case PARTICLE_SHADOWS:
        gl::Disable( GL_TEXTURE_2D );
		gl::Enable( GL_DEPTH_TEST );
        gl::Disable( GL_CULL_FACE );
		gl::Disable( GL_LIGHTING );
		gl::Disable( GL_NORMALIZE );
		gl::Disable( GL_ALPHA_TEST );
        gl::Enable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Disable( GL_TEXTURE_GEN_S );
		gl::Disable( GL_TEXTURE_GEN_T );
		gl::Disable( GL_COLOR_MATERIAL );
		gl::DepthMask( GL_TRUE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LESS );
        break;

    case SKY:
		gl::Enable( GL_TEXTURE_2D );
		gl::Disable( GL_DEPTH_TEST );
		gl::Disable( GL_CULL_FACE ); 
		gl::Disable( GL_LIGHTING );
		gl::Disable( GL_NORMALIZE );
		gl::Disable( GL_ALPHA_TEST );
		gl::Disable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Disable( GL_TEXTURE_GEN_S );
		gl::Disable( GL_TEXTURE_GEN_T );
		gl::Disable( GL_COLOR_MATERIAL );
		gl::DepthMask( GL_FALSE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LESS );
		gl::DepthFunc( GL_ALWAYS );
		break;
 	
    case FOG_PLANE:
		gl::Disable( GL_TEXTURE_2D );
		gl::Enable( GL_DEPTH_TEST );
		gl::Disable( GL_CULL_FACE ); 
		gl::Disable( GL_LIGHTING );
		gl::Disable( GL_NORMALIZE );
		gl::Disable( GL_ALPHA_TEST );
		gl::Enable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Disable( GL_TEXTURE_GEN_S );
		gl::Disable( GL_TEXTURE_GEN_T );
		gl::Disable( GL_COLOR_MATERIAL );
		gl::DepthMask( GL_TRUE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LESS );
		break;

    case TUX:
        gl::Disable( GL_TEXTURE_2D );
		gl::Enable( GL_DEPTH_TEST );
		gl::Enable( GL_CULL_FACE );
        gl::Enable( GL_LIGHTING );
		gl::Enable( GL_NORMALIZE );
		gl::Disable( GL_ALPHA_TEST );
		gl::Enable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Disable( GL_TEXTURE_GEN_S );
		gl::Disable( GL_TEXTURE_GEN_T );
		gl::Disable( GL_COLOR_MATERIAL );
		gl::DepthMask( GL_TRUE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LESS );
        break;

    case TUX_SHADOW:
		if(getparam_stencil_buffer()){
			gl::Disable( GL_TEXTURE_2D );
			gl::Enable( GL_DEPTH_TEST );
			gl::Disable( GL_CULL_FACE );
			gl::Disable( GL_LIGHTING );
			gl::Disable( GL_NORMALIZE );
			gl::Disable( GL_ALPHA_TEST );
			gl::Enable( GL_BLEND );
			gl::Enable( GL_STENCIL_TEST );
			gl::Disable( GL_COLOR_MATERIAL );
			gl::DepthMask( GL_FALSE );
			gl::ShadeModel( GL_SMOOTH );
			gl::DepthFunc( GL_LESS );
			gl::StencilFunc( GL_EQUAL, 0, ~0 );
			gl::StencilOp( GL_KEEP, GL_KEEP, GL_INCR );
		}else{
			gl::Disable( GL_TEXTURE_2D );
			gl::Enable( GL_DEPTH_TEST );
			gl::Enable( GL_CULL_FACE );
			gl::Disable( GL_LIGHTING );
			gl::Disable( GL_NORMALIZE );
			gl::Disable( GL_ALPHA_TEST );
			gl::Enable( GL_BLEND );
			gl::Disable( GL_STENCIL_TEST );
			gl::Disable( GL_COLOR_MATERIAL );
			gl::DepthMask( GL_TRUE );
			gl::ShadeModel( GL_SMOOTH );
			gl::DepthFunc( GL_LESS );
		}
		break;

    case TRACK_MARKS:
		gl::Enable( GL_TEXTURE_2D );
		gl::Enable( GL_DEPTH_TEST );
		gl::Disable( GL_CULL_FACE );
		gl::Enable( GL_LIGHTING );
		gl::Disable( GL_NORMALIZE );
		gl::Disable( GL_ALPHA_TEST );
		gl::Enable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Disable( GL_COLOR_MATERIAL );
		gl::Disable( GL_TEXTURE_GEN_S );
		gl::Disable( GL_TEXTURE_GEN_T );
		gl::DepthMask( GL_FALSE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LEQUAL );
		break;

    case OVERLAYS:
        gl::Enable( GL_TEXTURE_2D );
        gl::Disable( GL_DEPTH_TEST );
        gl::Disable( GL_CULL_FACE );
		gl::Disable( GL_LIGHTING );
		gl::Disable( GL_NORMALIZE );
		gl::Enable( GL_ALPHA_TEST );
        gl::Enable( GL_BLEND );
		gl::Disable( GL_STENCIL_TEST );
		gl::Disable( GL_TEXTURE_GEN_S );
		gl::Disable( GL_TEXTURE_GEN_T );
		gl::Disable( GL_COLOR_MATERIAL );
		gl::DepthMask( GL_TRUE );
		gl::ShadeModel( GL_SMOOTH );
		gl::DepthFunc( GL_LESS );

        gl::AlphaFunc( GL_GEQUAL, 0.5 );
        break;

    default:
		PP_NOT_REACHED();
    } 
} 

/* Checking for GL errors is really just another type of assertion, so we
   turn off the check if TUXRACER_NO_ASSERT is defined */
#if defined( TUXRACER_NO_ASSERT )
void check_gl_error()
{
}
#else 
void check_gl_error()
{
    GLenum error;
    error = glGetError();
    if ( error != GL_NO_ERROR ) {
		print_warning( CRITICAL_WARNING, 
		       "OpenGL Error: %s", gluErrorString( error ) );
		fflush( stderr );
    }
}
#endif /* defined( TUXRACER_NO_ASSERT ) */

void copy_to_glfloat_array( GLfloat dest[], double src[], int n )
{
    for (int i=0; i<n; i++) {
		dest[i] = src[i];
    }
}

void init_glfloat_array( int num, GLfloat arr[], ... )
{
    va_list args;
    va_start( args, arr );

    for (int i=0; i<num; i++) {
		arr[i] = va_arg(args, double);
    }

    va_end( args );
}

/* Extension func ptrs *must* be initialized to NULL */
PFNGLLOCKARRAYSEXTPROC glLockArraysEXT_p = NULL;
PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT_p = NULL;

typedef void (*(*get_gl_proc_fptr_t)(const GLubyte *))(); 

void init_opengl_extensions()
{
    get_gl_proc_fptr_t get_gl_proc;

    get_gl_proc = reinterpret_cast<get_gl_proc_fptr_t>(SDL_GL_GetProcAddress);

    if ( get_gl_proc ) {
		glLockArraysEXT_p = reinterpret_cast<PFNGLLOCKARRAYSEXTPROC> 
		((*get_gl_proc)( reinterpret_cast<const GLubyte*>("glLockArraysEXT") ));
	
		glUnlockArraysEXT_p = reinterpret_cast<PFNGLUNLOCKARRAYSEXTPROC> 
		((*get_gl_proc)( reinterpret_cast<const GLubyte*>("glUnlockArraysEXT") ));
	
		if ( glLockArraysEXT_p != NULL && glUnlockArraysEXT_p != NULL ) {
			PP_LOG( DEBUG_GL_EXT, 
				 "GL_EXT_compiled_vertex_array extension "
				 "supported" );
		} else {
			PP_LOG( DEBUG_GL_EXT, 
				 "GL_EXT_compiled_vertex_array extension "
				 "NOT supported" );
			glLockArraysEXT_p = NULL;
			glUnlockArraysEXT_p = NULL;
		}
    } else {
		PP_LOG( DEBUG_GL_EXT, 
		     "No function available for obtaining GL proc addresses" );
    }
}



/*---------------------------------------------------------------------------*/
/*! 
  Prints information about the current OpenGL implemenation.
  \author  jfpatry
  \date    Created:  2000-10-20
  \date    Modified: 2000-10-20
*/
typedef struct {
    char *name;
    GLenum value;
    GLenum type;
} gl_value_t;

/* Add more things here as needed */
gl_value_t gl_values[] = {
    { "maximum lights", GL_MAX_LIGHTS, GL_INT },
    { "modelview stack depth", GL_MAX_MODELVIEW_STACK_DEPTH, GL_INT },
    { "projection stack depth", GL_MAX_PROJECTION_STACK_DEPTH, GL_INT },
    { "max texture size", GL_MAX_TEXTURE_SIZE, GL_INT },
    { "double buffering", GL_DOUBLEBUFFER, GL_UNSIGNED_BYTE },
    { "red bits", GL_RED_BITS, GL_INT },
    { "green bits", GL_GREEN_BITS, GL_INT },
    { "blue bits", GL_BLUE_BITS, GL_INT },
    { "alpha bits", GL_ALPHA_BITS, GL_INT },
    { "depth bits", GL_DEPTH_BITS, GL_INT },
    { "stencil bits", GL_STENCIL_BITS, GL_INT } };

void print_gl_info()
{
    const char *extensions;
    char *p, *oldp;
    unsigned int i;
    GLint int_val;
    GLfloat float_val;
    GLboolean boolean_val;

    fprintf( stderr,
	     "  vendor: %s\n", 
	     gl::GetString(GL_VENDOR) );

    fprintf( stderr,
	     "  renderer: %s\n", 
	     gl::GetString(GL_RENDERER) );

    fprintf( stderr,
	     "  version: %s\n", 
	     gl::GetString(GL_VERSION) );

	//extensions = string_copy( (char*) glGetString( GL_EXTENSIONS ) );
	//oldp = extensions;
	
	extensions = gl::GetString(GL_EXTENSIONS);

	oldp = new char[strlen(extensions)+1];
	extensions = oldp;
	strcpy(oldp, extensions);

    fprintf( stderr, "  extensions:\n" );
	
    while ( (p=strchr(oldp,' ')) ) {
		*p='\0';
		fprintf( stderr, "    %s\n", oldp );
		oldp = p+1;
    }
	
	if ( *oldp ) {
		fprintf( stderr, "    %s\n", oldp );
    }
	
	delete extensions;
	
    for ( i=0; i<sizeof(gl_values)/sizeof(gl_values[0]); i++) {
	fprintf( stderr, "  %s: ", gl_values[i].name );

	switch( gl_values[i].type ) {
	case GL_INT:
	    gl::GetValue(gl_values[i].value, &int_val);
	    fprintf( stderr, "%d", int_val );
	    break;

	case GL_FLOAT:
	    gl::GetValue(gl_values[i].value, &float_val);
	    fprintf( stderr, "%f", float_val );
	    break;

	case GL_UNSIGNED_BYTE:
	    gl::GetValue(gl_values[i].value, &boolean_val);
	    fprintf( stderr, "%d", boolean_val );
	    break;

	default:
	    PP_NOT_REACHED();
	}

	fprintf( stderr, "\n" );
    }


    fprintf( stderr, "\n" );
}
