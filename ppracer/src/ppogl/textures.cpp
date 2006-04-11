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

Texture::Texture(const std::string& filename, int mode)
 : RefObject(),
   m_id(0),
   m_width(0),
   m_height(0)
{
	int filter = translateFilter(
		ppogl::Config::getInstance().getInt("texture_filter"));

	load_texture(filename, filter, mode);
}

Texture::~Texture()
{
	if(m_id){
		gl::DeleteTextures(1, &m_id);
	}	
}

void
Texture::load_texture(const std::string& filename, int filter, int mode)
{
	ppogl::Image *image = ppogl::Image::readFile(filename);

	if(image==NULL){
		PP_WARNING("Unable to load texture image: " << filename);
		return;
	}
	
	gl::GenTextures(1, &m_id);
    gl::BindTexture(GL_TEXTURE_2D, m_id);

    gl::PixelStore(GL_UNPACK_ALIGNMENT, 4);

    if(mode & REPEATABLE){
		gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }else{
		// use GL_CLAMP_TO_EDGE to prevent some bad behavior at texture/polygon edges
		gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
	
	if(mode & UNFILTERED){
		// use GL_NEAREST for textures that don't need a real filter
		gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    	gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}else{
		gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	}
		
	// Only build mipmaps if we need them
	if(mode & NO_MIPMAP){
		gl::TexImage2D(GL_TEXTURE_2D, *image);
	}else{	
		glu::Build2DMipmaps(GL_TEXTURE_2D, *image);
	}
		
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
TextureMgr::load(const std::string& binding, const std::string& filename, int mode)
{
	PP_LOG(LogTextures, "Loading texture: " << binding << " -> " << filename);
		
	// Create texture and put it in our binding table.
	return m_bindings[binding] = TextureRef(new ppogl::Texture(filename, mode));
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
	std::string binding = vm->getStringFromTable("name");
	std::string filename = vm->getStringFromTable("file");
	
	if(binding.empty() || filename.empty()){
		PP_WARNING("Textures.load: Please specify name and file");
		return vm->defaultError();
	}
	
	int mode =0;
	
	// is texture repeatable
	if(vm->isKeyInTable("repeatable") && vm->getBoolFromTable("repeatable") ){
		mode |= Texture::REPEATABLE;			
	}
		
	if(vm->isKeyInTable("scalable") && !vm->getBoolFromTable("scalable") ){
		//  we don't need mipmaps and filtersbecause texture isn't scaled
		mode |= Texture::UNFILTERED;
		mode |= Texture::NO_MIPMAP;
	}	
	
	getInstance().load(binding, filename, mode);
	
	return 0;
}

int
TextureMgr::sqBind(ppogl::Script *vm)
{
	std::string name = vm->getStringFromTable("name");
	std::string  texture = vm->getStringFromTable("texture");
	
	if(name.empty() || texture.empty()){
		PP_WARNING("Textures.load: Please specify name and texture");
		return vm->defaultError();
	}
	
	getInstance().bind(name, texture);

	return 0;
}


} //namepsace ppogl
