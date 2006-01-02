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

#include "gl_util.h"

#include "gameconfig.h"
#include "ppogl/base/glwrappers.h"

void
set_gl_options(const RenderMode mode) 
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
		if(GameConfig::enableStencilBuffer){
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

    default:
		PP_NOT_REACHED();
    } 
} 

static struct{
    const char *name;
    GLenum value;
    GLenum type;
} glValues[] = {
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
    { "stencil bits", GL_STENCIL_BITS, GL_INT }
};

void
print_gl_info()
{
	std::ostream& stream =
			ppogl::Log::Instance()->getStream();
	stream << "OpenGL information:" << std::endl;
	stream << "  vendor: " << gl::GetString(GL_VENDOR) << std::endl;
	stream << "  renderer: " << gl::GetString(GL_RENDERER) << std::endl;
	stream << "  version: " << gl::GetString(GL_VERSION) << std::endl;

	const char* extensions = gl::GetString(GL_EXTENSIONS);
	
	char* oldp = new char[strlen(extensions)+1];
	strcpy(oldp, extensions);
	extensions = oldp;
	
    stream << "  extensions:" << std::endl;
	
	char *p;
    while((p=strchr(oldp,' '))){
		*p='\0';
		stream << "    " << oldp << std::endl;
		oldp = p+1;
    }
	
	if(*oldp){
		stream << "    " << oldp << std::endl;
    }
	
	delete extensions;
	
    for(unsigned int i=0; i<sizeof(glValues)/sizeof(glValues[0]); i++){
		stream << "  " << glValues[i].name << ": ";

		switch(glValues[i].type){
		case GL_INT:
	    	GLint int_val;
			gl::GetValue(glValues[i].value, &int_val);
	    	stream << int(int_val);
	    	break;

		case GL_FLOAT:
			GLfloat float_val;
	    	gl::GetValue(glValues[i].value, &float_val);
			stream << float(float_val);
			break;

		case GL_UNSIGNED_BYTE:
		    GLboolean boolean_val;
			gl::GetValue(glValues[i].value, &boolean_val);
	    	if(boolean_val){
				stream << "true";
			}else{	
				stream << "false";
	    	}
			break;

		default:
	    	PP_NOT_REACHED();
		}

		stream << std::endl;
    }
	stream << std::endl;
}
