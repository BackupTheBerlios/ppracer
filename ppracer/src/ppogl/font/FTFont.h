
#ifndef __PPOGL_FTFONT_H_
#define __PPOGL_FTFONT_H_

#include "FTVector.h"
#include "FTGL.h"
#include "FTGlyph.h"
#include "FTGlyphContainer.h"
#include "FTFace.h"

#include "../base/refptr.h"
#include "../base/vec2d.h"

namespace ppogl{

class FTTextureGlyph;
	
/**
 * FTGLFont is a class for handling
 * Texture mapped fonts
 *
 * @see     FTFont
 */
class FTFont
 : public ppogl::RefObject
{
public:
	/**
	 * Open and read a font file. Sets Error flag.
	 *
	 * @param fontFilePath  font file path.
	 */
	FTFont(const std::string& fontFilePath);
        
	/**
	 * Destructor
	 */
	~FTFont();

	/**
	 * Set the char size for the current face.
	 *
	 * @param size      the face size in points (1/72 inch)
	 * @param res       the resolution of the target device.
	 * @return          <code>true</code> if size was set correctly
	 */
	bool FaceSize( const unsigned int size, const unsigned int res = 72);
       
	/**
	 * Renders a string of characters
	 * 
	 * @param string    wchar_t string to be output.     
	 */
	void Render( const wchar_t* string);

	/**
	 * Get the global ascender height for the face.
	 *
	 * @return  Ascender height
	 */
	float Ascender() const
	{
		return charSize.Ascender();
	}
		
	/**
	 * Gets the global descender height for the face.
	 *
	 * @return  Descender height
	 */
	float Descender() const
	{
		return charSize.Descender();
	}

 	/**
	 * Get the advance width for a string.
	 *
	 * @param string    a wchar_t string
	 * @return      advance width
	 */
	float Advance(const wchar_t* string);
	
	/**
	 * Queries the Font for errors.
	 *
	 * @return  The current error code.
	 */
	FT_Error Error() const {return err;}
		
private:
	/**
	 * Construct a FTTextureGlyph.
	 *
	 * @param glyphIndex The glyph index NOT the char code.
 	 * @return  An FTTextureGlyph or <code>null</code> on failure.
 	 */
	FTGlyph* MakeGlyph( unsigned int glyphIndex);
    
	/**
 	 * Get the size of a block of memory required to layout the glyphs
	 *
	 * Calculates a width and height based on the glyph sizes and the
	 * number of glyphs. It over estimates.
	 */
	void CalculateTextureSize();

	/**
	 * Creates a 'blank' OpenGL texture object.
	 *
	 * The format is GL_ALPHA and the params are
	 * GL_TEXTURE_WRAP_S = GL_CLAMP
	 * GL_TEXTURE_WRAP_T = GL_CLAMP
	 * GL_TEXTURE_MAG_FILTER = GL_LINEAR
	 * GL_TEXTURE_MIN_FILTER = GL_LINEAR
	 * Note that mipmapping is NOT used
	 */
	GLuint CreateTexture();
        		
	/**
	 * Check that the glyph at <code>chr</code> exist. If not load it.
	 *
	 * @param chr  character index
 	 * @return <code>true</code> if the glyph can be created.
	 */
	bool CheckGlyph( const unsigned int chr);
								
	/**
	 * The maximum texture dimension on this OpenGL implemetation
	 */
	GLsizei maximumGLTextureSize;
        
	/**
	 * The minimum texture width required to hold the glyphs
	 */
	GLsizei textureWidth;
        
	/**
	 * The minimum texture height required to hold the glyphs
	 */
	GLsizei textureHeight;
        
	/**
	 *An array of texture ids
	 */
	FTVector<GLuint> textureIDList;
        
	/**
	 * The max height for glyphs in the current font
	 */
	int glyphHeight;

	/**
	 * The max width for glyphs in the current font
	 */
	int glyphWidth;

	/**
	 * A value to be added to the height and width to ensure that
	 * glyphs don't overlap in the texture
	 */
 	unsigned int padding;
        
	unsigned int numGlyphs;
        
	unsigned int remGlyphs;

	int xOffset;

	int yOffset;
		 
	/**
	 * An object that holds a list of glyphs
	 */
	FTGlyphContainer* glyphList;
		          
	/**
	 * Current pen or cursor position;
	 */
	ppogl::Vec2d pen;
       
	/**
	 * Current face object
	 */
	FTFace face;
		
	/**
	 * Current size object
	 */
	FTSize charSize;	
		
	/**
	 * Current error code. Zero means no error.
	 */
	FT_Error err;
};

} //namespace ppogl

#endif // __PPOGL_FTFONT_H_
