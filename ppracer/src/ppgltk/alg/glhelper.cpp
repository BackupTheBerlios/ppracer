/* 
 * PPRacer 
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
 
#include "glhelper.h"

#include "../../textures.h"

namespace ppGL {

namespace draw {
	

void
rect(pp::Color color, pp::Vec2d startPos, pp::Vec2d endPos)
{
	gl::Disable( GL_TEXTURE_2D );
    gl::Color(color);
    gl::Rect(startPos, endPos);
	gl::Enable( GL_TEXTURE_2D );
}	
	
void
rect(const char* binding, pp::Vec2d pos, pp::Vec2d size )
{
	GLuint texobj;
	
    if ( !get_texture_binding( binding, &texobj ) ) {
		texobj = 0;
    }
		
    gl::PushMatrix();
    {	
		gl::BindTexture( GL_TEXTURE_2D, texobj );
		gl::Enable( GL_TEXTURE_2D );
		gl::Color(pp::Color::white);
		gl::Translate(pos);
		gl::Begin( GL_QUADS );
		{
	    	gl::TexCoord( 0.0, 0.0 );
	    	gl::Vertex( 0.0, 0.0 );
	    
	    	gl::TexCoord( 1.0, 0.0 );
			gl::Vertex( size.x, 0.0 );
	    
	    	gl::TexCoord( 1.0, 1.0 );
	    	gl::Vertex( size.x, size.y );
	    
	    	gl::TexCoord( 0.0, 1.0 );
	    	gl::Vertex( 0.0, size.y );
		}
		gl::End();
    }
    gl::PopMatrix();
}

} //namespace draw
	
} //namespace ppGL
