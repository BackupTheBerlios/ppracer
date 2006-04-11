/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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

#ifndef _PPOGL_IMAGE_H_
#define _PPOGL_IMAGE_H_

#include "../base/assert.h"
#include "../base/color.h"

namespace ppogl {

class Image
{
public:
	Image();
	Image(unsigned short width, unsigned short height, unsigned char depth=3);
	virtual ~Image();	


	unsigned char* data;
	unsigned short width, height;
	unsigned char depth;
		
	bool writeToFile(const std::string& filename) const;
		
	static Image* readFile(const std::string& filename);


	class iterator
	{
		Image* m_image;
		int m_position; 
		
	public:
		iterator(int position=0):m_image(NULL), m_position(position){};
		iterator(Image* image, int position=0) : m_image(image), m_position(position){};
	
		int getPosition(){return m_position;};
		int getY(){return m_position / m_image->width;};	
		int getX(){return m_position - (getY() * m_image->width);};
			
		ppogl::Color4c& operator*()
		{	
			PP_REQUIRE(m_image!=NULL,"IteratorRGB not initialized");
			PP_REQUIRE(m_position>=0,"Illegal position: " << m_position);
			PP_REQUIRE(m_position < m_image->width*m_image->height,"Illegal position: " << m_position << "\nWidth: " << m_image->width << "\nHeight: " << m_image->height);
			return *(reinterpret_cast<ppogl::Color4c*>(m_image->data+(m_position * m_image->depth)));
		};
		
		iterator operator++(int)
		{
			PP_REQUIRE(m_image!=NULL,"IteratorRGB not initialized");
			iterator it = *this;
			m_position++;
			return it;
		};
		
		iterator operator--(int)
		{			
			PP_REQUIRE(m_image!=NULL,"IteratorRGB not initialized");
			iterator it = *this;
			m_position--;
			return it;
		};
		
		bool operator==(const iterator& it)
		{			
			PP_REQUIRE(m_image!=NULL,"IteratorRGB not initialized");
			PP_REQUIRE(it.m_image!=NULL,"IteratorRGB not initialized");
			return (m_position==it.m_position) && (m_image==it.m_image);
		};	
		
		bool operator!=(const iterator& it)
		{
			PP_REQUIRE(m_image!=NULL,"IteratorRGB not initialized");
			PP_REQUIRE(it.m_image!=NULL,"IteratorRGB not initialized");
			return (m_position!=it.m_position) || (m_image!=it.m_image);	
		};
	
	}; // iterator

	iterator begin(){return iterator(this, 0);};
	iterator end(){return iterator(this, width*height);};	
	
}; // Image

} //namespace ppogl

#ifdef _WIN32
	#include <WTypes.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace gl{
	inline void TexImage2D(GLenum target, const ppogl::Image& image){glTexImage2D(target, 0, image.depth, image.width, image.height, 0, (image.depth == 3)?(GL_RGB):(GL_RGBA), GL_UNSIGNED_BYTE, image.data);}		
} // namespace gl

namespace glu{
	inline GLint Build2DMipmaps(GLenum target, const ppogl::Image& image){return gluBuild2DMipmaps(target, image.depth, image.width, image.height, (image.depth == 3)?(GL_RGB):(GL_RGBA), GL_UNSIGNED_BYTE, image.data);}
} // namespace glu





#endif // _PPOGL_IMAGE_H_
