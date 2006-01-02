
#include "FTGlyph.h"
#include FT_OUTLINE_H

namespace ppogl{

GLint FTGlyph::activeTextureID = 0;
 
FTGlyph::FTGlyph(FT_GlyphSlot glyph, int id, int xOffset, int yOffset, GLsizei width, GLsizei height)
 : err(0),
   destWidth(0),
   destHeight(0),
   glTextureID(id)
{
    if(glyph){
        //bBox = FTBBox(glyph);
		FT_Outline_Get_CBox(&(glyph->outline), &bBox);
			
        advance = ppogl::Vec2d(glyph->advance.x / 64.0f, glyph->advance.y / 64.0f);
    }
	
	err = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
    if(err || glyph->format != ft_glyph_format_bitmap){
        return;
    }

    FT_Bitmap bitmap = glyph->bitmap;

    destWidth  = bitmap.width;
    destHeight = bitmap.rows;
    
    if(destWidth && destHeight){
        gl::PushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
        gl::PixelStore(GL_UNPACK_LSB_FIRST, GL_FALSE);
        gl::PixelStore(GL_UNPACK_ROW_LENGTH, 0);
        gl::PixelStore(GL_UNPACK_ALIGNMENT, 1);

        gl::BindTexture(GL_TEXTURE_2D, glTextureID);
        gl::TexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, destWidth, destHeight, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.buffer);

        gl::PopClientAttrib();
    }

//      0    
//      +----+
//      |    |
//      |    |
//      |    |
//      +----+
//           1
    
    uv[0].x() = static_cast<float>(xOffset) / static_cast<float>(width);
    uv[0].y() = static_cast<float>(yOffset) / static_cast<float>(height);
    uv[1].x() = static_cast<float>(xOffset + destWidth) / static_cast<float>(width);
    uv[1].y() = static_cast<float>(yOffset + destHeight) / static_cast<float>(height);
    
    pos.x() = glyph->bitmap_left;
    pos.y() = glyph->bitmap_top;
}

const ppogl::Vec2d&
FTGlyph::Render(const ppogl::Vec2d& pen)
{
    if(activeTextureID != glTextureID){
        gl::BindTexture(GL_TEXTURE_2D, GLuint(glTextureID));
        activeTextureID = glTextureID;
    }
    
    gl::Translate(pen);

    gl::Begin(GL_QUADS);
        gl::TexCoord(uv[0]);
        gl::Vertex(pos);

        gl::TexCoord(uv[0].x(), uv[1].y());
        gl::Vertex(pos.x(), pos.y() - destHeight);

        gl::TexCoord(uv[1]);
        gl::Vertex(destWidth + pos.x(), pos.y() - destHeight);
        
        gl::TexCoord(uv[1].x(), uv[0].y());
        gl::Vertex(destWidth + pos.x(), pos.y());
    gl::End();

    return advance;
}

} // namespace ppogl
