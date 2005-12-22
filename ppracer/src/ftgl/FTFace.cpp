#include "FTFace.h"
#include "FTLibrary.h"

#include FT_TRUETYPE_TABLES_H

FTFace::FTFace(const std::string& fontFilePath)
:   charSize(),
	numGlyphs(0),
    fontEncodingList(0),
	err(0),
	hasKerningTable(false)
{
    const FT_Long DEFAULT_FACE_INDEX = 0;
    ftFace = new FT_Face;

    err = FT_New_Face( *FTLibrary::Instance().GetLibrary(), fontFilePath.c_str(), DEFAULT_FACE_INDEX, ftFace);

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
    if(ftFace!=NULL){
        FT_Done_Face(*ftFace);
        delete ftFace;
        ftFace = 0;
    }
}

const FTSize& FTFace::Size( const unsigned int size, const unsigned int res)
{
    charSize.CharSize( ftFace, size, res, res);
    err = charSize.Error();

    return charSize;
}

FTPoint FTFace::KernAdvance( unsigned int index1, unsigned int index2)
{
    float x, y;
    x = y = 0.0f;

    if( hasKerningTable && index1 && index2)
    {
        FT_Vector kernAdvance;
        kernAdvance.x = kernAdvance.y = 0;

        err = FT_Get_Kerning( *ftFace, index1, index2, ft_kerning_unfitted, &kernAdvance);
        if( !err)
        {   
            x = static_cast<float>( kernAdvance.x) / 64.0f;
            y = static_cast<float>( kernAdvance.y) / 64.0f;
        }
    }
    
    return FTPoint( x, y, 0.0);
}


FT_GlyphSlot FTFace::Glyph( unsigned int index, FT_Int load_flags)
{
    err = FT_Load_Glyph( *ftFace, index, load_flags);
    if( err)
    {
        return NULL;
    }

    return (*ftFace)->glyph;
}
