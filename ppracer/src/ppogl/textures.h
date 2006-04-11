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
public:
	enum{
		DEFAULT = 0,
		NO_MIPMAP = 1<<0,
		REPEATABLE = 1 << 1,
		UNFILTERED = 1 << 2
};

private:
	GLuint m_id;
	int m_width, m_height;
	
	void load_texture(const std::string& filename, int filter, int mode);

	static int translateFilter(int filter);
	
public:	
	Texture(const std::string& filename, int mode = 0);
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
	TextureRef load(const std::string& binding, const std::string& filename, int mode=0); 

	void bind(const std::string& binding, const std::string& name);

	static int sqLoad(ppogl::Script *vm);
	static int sqBind(ppogl::Script *vm);	
};

} //namepsace ppogl

namespace gl{
	inline void BindTexture(GLenum target, const ppogl::TextureRef& texture)
	{
		if(texture.isNull()){
			glBindTexture(target, 0);
		}else{
			glBindTexture(target, texture->getID());
		}
	}
} // namespace gl

#endif // _PPOGL_TEXTURE_H_
