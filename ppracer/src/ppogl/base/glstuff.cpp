/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005-2006 Volker Stroebel <volker@planetpenguin.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
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
 
#include "glstuff.h"

#include "glwrappers.h"

namespace ppogl{
	
void
drawRectangle(const ppogl::Vec2d& position,
			  const ppogl::Vec2d& size,
			  const ppogl::Color& color)
{
	gl::PushMatrix();
	{
		gl::Disable( GL_TEXTURE_2D );
    	gl::Color(color);
    
		gl::Rect(position, position+size);
	
		gl::Enable( GL_TEXTURE_2D );
	}
	gl::PopMatrix();
}

void
drawRectangle(const ppogl::Vec2d& position,
			  const ppogl::Vec2d& size,
			  const ppogl::TextureRef& texture,
			  const ppogl::Color& color)
{
	gl::PushMatrix();
	{
		gl::Enable(GL_TEXTURE_2D);
    	gl::BindTexture(GL_TEXTURE_2D, texture);
    	gl::Color(color);

    	gl::Begin(GL_QUADS);
    	{
			gl::TexCoord(0, 0);
			gl::Vertex( position);

			gl::TexCoord(0, 1);
			gl::Vertex( position.x(),
				        position.y() + size.y());

			gl::TexCoord(1, 1);
			gl::Vertex( position.x() + size.x(),
			    		position.y() + size.y());

			gl::TexCoord(1, 0);
			gl::Vertex( position.x() + size.x(),
			    		position.y());
    	}
    	gl::End();
		gl::Disable(GL_TEXTURE_2D);
	}
	gl::PopMatrix();
}

void
drawRectangle(	const ppogl::Vec2d& position,
				const ppogl::Vec2d& size,
				const ppogl::TextureRef& texture,
				const ppogl::Vec4f& texcoords,
				const ppogl::Color& color)
{
	gl::PushMatrix();
	{
		gl::Enable(GL_TEXTURE_2D);
    	gl::BindTexture(GL_TEXTURE_2D, texture);
    	gl::Color(color);

    	gl::Begin(GL_QUADS);
    	{
			gl::TexCoord(texcoords.values[0], texcoords.values[1]);
			gl::Vertex( position);

			gl::TexCoord(texcoords.values[0], texcoords.values[3]);
			gl::Vertex( position.x(),
				        position.y() + size.y());

			gl::TexCoord(texcoords.values[2], texcoords.values[3]);
			gl::Vertex( position.x() + size.x(),
			    		position.y() + size.y());

			gl::TexCoord(texcoords.values[2], texcoords.values[1]);
			gl::Vertex( position.x() + size.x(),
			    		position.y());
    	}
    	gl::End();
		gl::Disable(GL_TEXTURE_2D);
	}
	gl::PopMatrix();	
}


} //namepsace ppogl
