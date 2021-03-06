
#ifndef __PPOGL_FTFace_H_
#define __PPOGL_FTFace_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "FTGL.h"
#include "FTSize.h"

#include "../base/vec2d.h"

#include <string>

namespace ppogl{

/**
 * FTFace class provides an abstraction layer for the Freetype Face.
 *
 * @see "Freetype 2 Documentation"
 *
 */
class FTFace
{
public:
	/**
	 * Opens and reads a face file. Error is set.
	 *
	 * @param fontFilePath  font file path.
	 */
	FTFace(const std::string& fontFilePath);

	/**
	 * Destructor
	 *
	 * Disposes of the current Freetype Face.
	 */
	~FTFace();

	/**
	 * Get the freetype face object..
	 *
	 * @return pointer to an FT_Face.
	 */
	FT_Face* Face() const {return ftFace;}
        
	/**
	 * Sets the char size for the current face.
	 *
	 * This doesn't guarantee that the size was set correctly. Clients
	 * should check errors.
	 *
	 * @param size      the face size in points (1/72 inch)
	 * @param res       the resolution of the target device.
	 * @return          <code>FTSize</code> object
	 */
	const FTSize& Size(const unsigned int size, const unsigned int res);
        
	/**
	 * Gets the kerning vector between two glyphs
	 */
	ppogl::Vec2d KernAdvance(unsigned int index1, unsigned int index2);

	/**
	 * Loads and creates a Freetype glyph.
	 */
	FT_GlyphSlot Glyph( unsigned int index, FT_Int load_flags);

	/**
	 * Gets the number of glyphs in the current face.
	 */
	unsigned int GlyphCount() const {return numGlyphs;}

	/**
	 * Queries for errors.
	 *
	 * @return  The current error code.
	 */
	FT_Error Error() const { return err; }
        
private:
	/**
	 * The Freetype face
	 */
	FT_Face* ftFace;

	/**
	 * The size object associated with this face
	 */
	FTSize  charSize;
        
	/**
	 * The number of glyphs in this face
	 */
	int numGlyphs;

	/**
	 * Current error code. Zero means no error.
	 */
	FT_Error err;
		
	/**
	 * This face has kerning tables
	 */
	bool hasKerningTable;
};

} //namespace ppogl

#endif // __PPOGL_FTFace_H_
