
#include "FTCharmap.h"

#include "FTFace.h"

namespace ppogl{

FTCharmap::FTCharmap(FTFace* face)
 : ftFace(*(face->Face())),
   charMap(),
   err(0)
{
	if(!ftFace->charmap){
		err = FT_Set_Charmap(ftFace, ftFace->charmaps[0]);
	}
    
	ftEncoding = ftFace->charmap->encoding;
	
	if(ftEncoding != ft_encoding_unicode){
		err = FT_Select_Charmap(ftFace, ft_encoding_unicode);
    
		if(!err){
			ftEncoding = ft_encoding_unicode;
    	}else{
			ftEncoding = ft_encoding_none;
			PP_WARNING("Unable to set encoding to unicode: Error code -> " << err);
    	}
    	charMap.clear();		
	}
}

unsigned int
FTCharmap::GlyphListIndex(unsigned int characterCode)
{
	return charMap.find(characterCode);
}


unsigned int
FTCharmap::FontIndex(unsigned int characterCode)
{
	return FT_Get_Char_Index(ftFace, characterCode);
}

void
FTCharmap::InsertIndex(const unsigned int characterCode, const unsigned int containerIndex)
{
	charMap.insert(characterCode, containerIndex);
}

} // namespace ppogl
