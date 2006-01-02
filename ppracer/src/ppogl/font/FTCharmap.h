
#ifndef __PPOGL_FTCHARMAP_H_
#define __PPOGL_FTCHARMAP_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "FTCharToGlyphIndexMap.h"

#include "FTGL.h"

namespace ppogl{

/**
 * FTCharmap takes care of specifying the encoding for a font and mapping
 * character codes to glyph indices.
 *
 * It doesn't preprocess all indices, only on an as needed basis. This may
 * seem like a performance penalty but it is quicker than using the 'raw'
 * freetype calls and will save significant amounts of memory when dealing
 * with unicode encoding
 *
 * @see "Freetype 2 Documentation" 
 *
 */

class FTFace;

class FTCharmap
{
public:
	/**
	 * Constructor
	 */
	FTCharmap(FTFace* face);

	/**
	 * Destructor
	 */
	~FTCharmap()
	{
		charMap.clear();
	};

	/**
     * Queries for the current character map code.
	 *
	 * @return  The current character map code.
	 */
	FT_Encoding Encoding() const {return ftEncoding;}
       
	/**
	 * Get the FTGlyphContainer index of the input character.
	 *
	 * @param characterCode The character code of the requested glyph in
	 *                      the current encoding eg apple roman.
	 * @return      The FTGlyphContainer index for the character or zero
	 *              if it wasn't found
	 */
	unsigned int GlyphListIndex( const unsigned int characterCode);

	/**
	 * Get the font glyph index of the input character.
	 *
	 * @param characterCode The character code of the requested glyph in
	 *                      the current encoding eg apple roman.
	 * @return      The glyph index for the character.
	 */
	unsigned int FontIndex(const unsigned int characterCode);

	/**
	 * Set the FTGlyphContainer index of the character code.
	 *
	 * @param characterCode  The character code of the requested glyph in
	 *                       the current encoding eg apple roman.
	 * @param containerIndex The index into the FTGlyphContainer of the
	 *                       character code.
	 */
	void InsertIndex(const unsigned int characterCode, const unsigned int containerIndex);

	/**
	 * Queries for errors.
	 *
	 * @return  The current error code. Zero means no error.
	 */
	FT_Error Error() const {return err;}
        
private:
	/**
	 * Current character map code.
	 */
	FT_Encoding ftEncoding;
        
	/**
	 * The current Freetype face.
	 */
	const FT_Face ftFace;
        
	/**
	 * A structure that maps glyph indices to character codes
	 *
	 * < character code, face glyph index>
	 */
	typedef FTCharToGlyphIndexMap CharacterMap;
	CharacterMap charMap;
        
	/**
	 * Current error code.
	 */
	FT_Error err;  
};

} //namespace ppogl

#endif // __PPOGL_FTCHARMAP_H_
