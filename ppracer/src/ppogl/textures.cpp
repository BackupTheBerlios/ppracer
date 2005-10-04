/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
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

#include "textures.h"

#include "base/assert.h"
#include "images/image.h"
#include "config.h"

namespace ppogl{

Texture::Texture(const std::string& filename, bool repeatable)
 : RefObject(),
   m_id(0),
   m_width(0),
   m_height(0),
   m_repeatable(repeatable)
{
	int filter = translateFilter(
		ppogl::Config::getInstance().getInt("texture_filter"));

	load_texture(filename, filter);
}

Texture::~Texture()
{
	if(m_id){
		gl::DeleteTextures(1, &m_id);
	}	
}


void
Texture::load_texture(const std::string& filename, int filter)
{
	ppogl::Image *image = ppogl::Image::readFile(filename);

	if(image==NULL){
		PP_WARNING("Unable to load texture image: " << filename);
		return;
	}
	
	gl::GenTextures(1, &m_id);
    gl::BindTexture(GL_TEXTURE_2D, m_id);

    gl::PixelStore(GL_UNPACK_ALIGNMENT, 4);

    if(m_repeatable){
		gl::TexParameter( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		gl::TexParameter( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    }else{
		gl::TexParameter( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		gl::TexParameter( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }
    gl::TexParameter( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    gl::TexParameter( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	
	glu::Build2DMipmaps( GL_TEXTURE_2D, *image );

	m_width = image->width;
	m_height = image->height;
		
	delete image;
}

int
Texture::translateFilter(int filter)
{
	switch(filter){
    	case 1:
			return GL_LINEAR;
    	case 2: 
			return GL_NEAREST_MIPMAP_NEAREST;
    	case 3: 
			return GL_LINEAR_MIPMAP_NEAREST;
    	case 4: 
			return GL_NEAREST_MIPMAP_LINEAR;
    	case 5: 
			return GL_LINEAR_MIPMAP_LINEAR;
    	default:
			// this default value is not realy usefull
			return GL_NEAREST;
	}
}

// TextureMgr

PPOGL_SINGLETON(TextureMgr);

TextureRef
TextureMgr::load(const std::string& binding, const std::string& filename, bool repeatable)
{
	PP_LOG(LogTextures, "Loading texture: " << binding << " -> " << filename);
	
	// Create texture and put it in our binding table.
	return m_bindings[binding] = TextureRef(new ppogl::Texture(filename,repeatable));
}

void
TextureMgr::bind(const std::string& binding, const std::string& name)
{
	PP_LOG(LogTextures, "Binding texture: " << binding << " -> " << name);
	
	std::map<std::string, RefPtr<Texture> >::iterator it;
	
	if((it=m_bindings.find(name))!=m_bindings.end()){
		m_bindings[binding]=(*it).second;
	}else{
		PP_WARNING("Cannot find font: " << name);		
	}	
}

int
TextureMgr::sqLoad(ppogl::Script *vm)
{
	bool repeatable=false;
	
	std::string binding = vm->getString(1);
	std::string filename = vm->getString(2);

	if(vm->getTop()==3){
		repeatable = vm->getBool(3);
	}
	
	getInstance().load(binding, filename, repeatable);
	
	return 0;
}

int
TextureMgr::sqBind(ppogl::Script *vm)
{
	std::string binding = vm->getString(1);
	std::string name = vm->getString(2);
	
	getInstance().bind(binding, name);

	return 0;
}


} //namepsace ppogl
