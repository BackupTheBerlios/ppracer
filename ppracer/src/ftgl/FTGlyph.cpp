#include    "FTGlyph.h"


FTGlyph::FTGlyph( FT_GlyphSlot glyph, bool useList)
 : err(0),
   useDisplayList(useList)
{
    if( glyph)
    {
        bBox = FTBBox( glyph);
        advance = FTPoint( glyph->advance.x / 64.0f, glyph->advance.y / 64.0f, 0.0f);
    }
}


FTGlyph::~FTGlyph()
{}
