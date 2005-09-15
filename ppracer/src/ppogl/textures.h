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

#ifndef _PPOGL_TEXTURES_H_
#define _PPOGL_TEXTURES_H_

#include "base/refptr.h"
#include "base/singleton.h"
#include "base/resourcemgr.h"
#include "base/glwrappers.h"
#include "ppogl_script.h"

namespace ppogl{
	
///A texture
class Texture : public RefObject
{
	GLuint m_id;
	int m_width, m_height;
	bool m_repeatable;
		
	void load_texture(const std::string& filename, int filter);

	static int translateFilter(int filter);
	
public:	
	Texture(const std::string& filename, bool repeatable=true);
	~Texture();

	inline GLuint getID(){return m_id;};
	inline int getWidth(){return m_width;};
	inline int getHeight(){return m_height;};
};

///A reference pointer to a texture
typedef RefPtr<Texture> TextureRef;


///The texture manager singleton
class TextureMgr
 : public Singleton<TextureMgr>,
   public ResourceMgr<Texture>
{
public:
	TextureMgr(){};	

	TextureRef load(const std::string& binding, const std::string& filename, bool repeatable=true); 
	void bind(const std::string& binding, const std::string& name);
			
	static int sqLoad(ppogl::Script *vm);
	static int sqBind(ppogl::Script *vm);	
};

} //namepsace ppogl

namespace gl{
	inline void BindTexture(GLenum target, ppogl::TextureRef texture)
	{
		if(texture){
			glBindTexture(target, texture->getID());
		}else{
			glBindTexture(target, 0);
		}
	}
} // namespace gl

#endif // _PPOGL_TEXTURE_H_
