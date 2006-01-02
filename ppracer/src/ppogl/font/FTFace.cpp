
#include "FTFace.h"

#include "FTLibrary.h"

#include FT_TRUETYPE_TABLES_H

namespace ppogl{

FTFace::FTFace(const std::string& fontFilePath)
 : charSize(),
   numGlyphs(0),
   err(0),
   hasKerningTable(false)
{
    const FT_Long DEFAULT_FACE_INDEX = 0;
    ftFace = new FT_Face;

    err = FT_New_Face(*FTLibrary::getInstance().GetLibrary(), fontFilePath.c_str(), DEFAULT_FACE_INDEX, ftFace);

    if(err){
        delete ftFace;
        ftFace = 0;
    }else{
        numGlyphs = (*ftFace)->num_glyphs;
        hasKerningTable = FT_HAS_KERNING((*ftFace));
    }
}

FTFace::~FTFace()
{
    if(ftFace){
        FT_Done_Face(*ftFace);
        delete ftFace;
    }
}

const FTSize&
FTFace::Size(const unsigned int size, const unsigned int res)
{
    charSize.CharSize(ftFace, size, res, res);
    err = charSize.Error();

    return charSize;
}

ppogl::Vec2d
FTFace::KernAdvance(unsigned int index1, unsigned int index2)
{
	ppogl::Vec2d vec;
	
    if(hasKerningTable && index1 && index2){
        FT_Vector kernAdvance;
        kernAdvance.x = kernAdvance.y = 0;

        err = FT_Get_Kerning(*ftFace, index1, index2, ft_kerning_unfitted, &kernAdvance);
        if(!err){   
            vec.x() = kernAdvance.x / 64.0f;
            vec.y() = kernAdvance.y / 64.0f;
        }
    }
    return vec;
}

FT_GlyphSlot
FTFace::Glyph(unsigned int index, FT_Int load_flags)
{
    err = FT_Load_Glyph(*ftFace, index, load_flags);
    if(err){
        return NULL;
    }

    return (*ftFace)->glyph;
}

} // namespace ppogl
