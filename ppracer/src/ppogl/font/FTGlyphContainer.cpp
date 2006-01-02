#include    "FTGlyphContainer.h"
#include    "FTGlyph.h"
#include    "FTFace.h"
#include    "FTCharmap.h"

namespace ppogl{

FTGlyphContainer::FTGlyphContainer(FTFace* f)
 : face(f),
   err(0)
{
    glyphs.push_back(NULL);
    charMap = new FTCharmap(face);
}


FTGlyphContainer::~FTGlyphContainer()
{
    GlyphVector::iterator glyphIterator;
    for(glyphIterator = glyphs.begin(); glyphIterator != glyphs.end(); ++glyphIterator){
        delete *glyphIterator;
    }
    
    glyphs.clear();
    delete charMap;
}

unsigned int
FTGlyphContainer::FontIndex(const unsigned int characterCode) const
{
    return charMap->FontIndex(characterCode);
}

void
FTGlyphContainer::Add(FTGlyph* tempGlyph, const unsigned int characterCode)
{
    charMap->InsertIndex(characterCode, glyphs.size());
    glyphs.push_back(tempGlyph);
}

const FTGlyph*
FTGlyphContainer::Glyph(const unsigned int characterCode) const
{
    signed int index = charMap->GlyphListIndex(characterCode);
    return glyphs[index];
}

float
FTGlyphContainer::Advance(const unsigned int characterCode, const unsigned int nextCharacterCode)
{
    unsigned int left = charMap->FontIndex(characterCode);
    unsigned int right = charMap->FontIndex(nextCharacterCode);

    float width = face->KernAdvance(left, right).x();
    width += glyphs[charMap->GlyphListIndex( characterCode)]->Advance().x();
    
    return width;
}

ppogl::Vec2d
FTGlyphContainer::Render(const unsigned int characterCode, const unsigned int nextCharacterCode, const ppogl::Vec2d& penPosition)
{
    ppogl::Vec2d kernAdvance, advance;
    
    unsigned int left = charMap->FontIndex( characterCode);
    unsigned int right = charMap->FontIndex( nextCharacterCode);

    kernAdvance = face->KernAdvance( left, right);
        
    if(!face->Error()){
        advance = glyphs[charMap->GlyphListIndex( characterCode)]->Render( penPosition);
    }
    
    kernAdvance += advance;
    return kernAdvance;
}

} // namespace ppogl
