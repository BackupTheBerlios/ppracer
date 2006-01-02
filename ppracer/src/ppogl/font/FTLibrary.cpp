
#include "FTLibrary.h"

namespace ppogl{

PPOGL_SINGLETON(FTLibrary);

FTLibrary::FTLibrary()
{
	library = new FT_Library;
    err = FT_Init_FreeType(library);
    if(err){
        delete library;
        library = NULL;
    }
}

FTLibrary::~FTLibrary()
{
    if(library!=NULL){
        FT_Done_FreeType(*library);
        delete library;
    }
}

} // namespace ppogl
