
#ifndef __PPOGL_FTGLYPH_H_
#define __PPOGL_FTGLYPH_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "FTGL.h"

#include "../base/vec2d.h"
#include "../base/glwrappers.h"

namespace ppogl{

/**
 * FTGlyph is the base class for FTGL glyphs.
 *
 * It provides the interface between Freetype glyphs and their openGL
 * renderable counterparts. This is an abstract class and derived classes
 * must implement the <code>render</code> function. 
 * 
 * @see FTGlyphContainer
 * @see FTBBox
 * @see FTPoint
 *
 */
class FTGlyph
{
public:
	/**
	 * Constructor
	 *
	 * @param glyph     The Freetype glyph to be processed
	 * @param id        The id of the texture that this glyph will be
	 *                  drawn in
	 * @param xOffset   The x offset into the parent texture to draw
	 *                  this glyph
	 * @param yOffset   The y offset into the parent texture to draw
	 *                  this glyph
	 * @param width     The width of the parent texture
	 * @param height    The height (number of rows) of the parent texture
	 */
	FTGlyph(FT_GlyphSlot glyph, int id, int xOffset, int yOffset, GLsizei width, GLsizei height);

	/**
	 * Renders this glyph at the current pen position.
	 *
	 * @param pen   The current pen position.
	 * @return      The advance distance for this glyph.
	 */
	 const ppogl::Vec2d& Render(const ppogl::Vec2d& pen);
        
	/**
	 * Return the advance width for this glyph.
	 *
	 * @return  advance width.
	 */
	const ppogl::Vec2d& Advance() const {return advance;}
        
	/**
	 * Return the bounding box for this glyph.
	 *
	 * @return  bounding box.
	 */
	const FT_BBox& BBox() const {return bBox;}
        	
	/**
	 * Reset the currently active texture to zero to get into a known state before
	 * drawing a string. This is to get round possible threading issues.
	 */
	static void ResetActiveTexture(){activeTextureID = 0;}
       		
	/**
	 * Queries for errors.
	 *
	 * @return  The current error code.
	 */
	FT_Error Error() const {return err;}
        
protected:
	/**
	 * The advance distance for this glyph
	 */
	ppogl::Vec2d advance;

	/**
	 * The bounding box of this glyph.
	 */
	FT_BBox bBox;
	    	
	/**
	 * Current error code. Zero means no error.
	 */
	FT_Error err;
        
	private: 
	/**
	 * The width of the glyph 'image'
	 */
	int destWidth;

	/**
	 * The height of the glyph 'image'
	 */
	int destHeight;

	/**
	 * Vector from the pen position to the topleft corner of the pixmap
	 */
	ppogl::Vec2d pos;
        
	/**
	 * The texture co-ords of this glyph within the texture.
	 */
	ppogl::Vec2d uv[2];
        
	/**
	 * The texture index that this glyph is contained in.
	 */
	int glTextureID;

	/**
	 * The texture index of the currently active texture
	 *
	 * We keep track of the currently active texture to try to reduce the number
	 * of texture bind operations.
	 */
	static GLint activeTextureID;
};

} // namespace ppogl

#endif // __PPOGL_FTGLYPH_H_
