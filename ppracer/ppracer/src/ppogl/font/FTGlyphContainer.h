#ifndef __PPOGL_FTGLYPHCONTAINER_H_
#define __PPOGL_FTGLYPHCONTAINER_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "FTGL.h"
#include "FTVector.h"

#include "../base/vec2d.h"

namespace ppogl{
	
class FTFace;
class FTGlyph;
class FTCharmap;

/**
 * FTGlyphContainer holds the post processed FTGlyph objects.
 *
 * @see FTGlyph
 */
class FTGlyphContainer
{
	typedef FTVector<FTGlyph*> GlyphVector;
public:
	/**
	 * Constructor
	 *
	 * @param face      The Freetype face
	 */
	FTGlyphContainer(FTFace* face);

	/**
	 * Destructor
	 */
	~FTGlyphContainer();

	/**
	 * Get the font index of the input character.
	 *
	 * @param characterCode The character code of the requested glyph in the
	 *                      current encoding eg apple roman.
	 * @return      The font index for the character.
	 */
	unsigned int FontIndex(const unsigned int characterCode) const;
        
	/**
	 * Adds a glyph to this glyph list.
	 *
	 * @param glyph         The FTGlyph to be inserted into the container
	 * @param characterCode The char code of the glyph NOT the glyph index.
	 */
	void Add(FTGlyph* glyph, const unsigned int characterCode);

	/**
	 * Get a glyph from the glyph list
	 *
	 * @param characterCode The char code of the glyph NOT the glyph index      
	 * @return              An FTGlyph or <code>null</code> is it hasn't been
	 * loaded.
	 */
	const FTGlyph* Glyph(const unsigned int characterCode) const;
        
	/**
	 * Returns the kerned advance width for a glyph.
	 *
	 * @param characterCode     glyph index of the character
	 * @param nextCharacterCode the next glyph in a string
	 * @return                  advance width
	 */
	float Advance(const unsigned int characterCode, const unsigned int nextCharacterCode);
        
	/**
	 * Renders a character
	 * @param characterCode      the glyph to be Rendered
	 * @param nextCharacterCode  the next glyph in the string. Used for kerning.
	 * @param penPosition        the position to Render the glyph
	 * @return                   The distance to advance the pen position after Rendering
	 */
	ppogl::Vec2d Render(const unsigned int characterCode, const unsigned int nextCharacterCode, const ppogl::Vec2d& penPosition);
        
	/**
	 * Queries the Font for errors.
	 *
	 * @return  The current error code.
	 */
	FT_Error Error() const {return err;}

private:
	/**
	 * The FTGL face
	 */
	FTFace* face;

	/**
	 * The Character Map object associated with the current face
	 */
	FTCharmap* charMap;

	/**
	 * A structure to hold the glyphs
	 */
	GlyphVector glyphs;

	/**
	 * Current error code. Zero means no error.
	 */
	FT_Error err;
};

} // namespace ppogl

#endif // __PPOGL_FTGLYPHCONTAINER_H_
