#ifndef __PPOGL_FTLIBRARY_H_
#define __PPOGL_FTLIBRARY_H_

#include <ft2build.h>
#include FT_FREETYPE_H
//#include FT_CACHE_H

#include "FTGL.h"

#include "../base/singleton.h"

namespace ppogl{

/**
 * FTLibrary class is the global accessor for the Freetype library.
 *
 * This class encapsulates the Freetype Library. This is a singleton class
 * and ensures that only one FT_Library is in existence at any one time.
 * All constructors are private therefore clients cannot create or
 * instantiate this class themselves and must access it's methods via the
 * static <code>FTLibrary::Instance()</code> function.
 *
 * Just because this class returns a valid <code>FTLibrary</code> object
 * doesn't mean that the Freetype Library has been successfully initialised.
 * Clients should check for errors. You can initialse the library AND check
 * for errors using the following code...
 * <code>err = FTLibrary::Instance().Error();</code>
 *
 * @see "Freetype 2 Documentation"
 *
 */
class FTLibrary
 : public ppogl::Singleton<FTLibrary>
{
public:
	FTLibrary();

	/**
	 * Destructor
	 *
	 * Disposes of the Freetype library
	 */
	~FTLibrary();
        
	/**
	 * Gets a pointer to the native Freetype library.
	 * 
	 * @return A handle to a FreeType library instance. 
	 */
	const FT_Library* GetLibrary() const {return library;}
        
	/**
	 * Queries the library for errors.
	 *
	 * @return  The current error code.
	 */
	FT_Error Error() const {return err;}
        

private:
	/**
	 * Default constructors.
	 *
	 * Made private to stop clients creating there own FTLibrary
	 * objects.
	 */
	FTLibrary(const FT_Library&):library(0),err(0){}
	FTLibrary& operator=(const FT_Library&){return *this;}
        
	/**
	 * Freetype library handle.
	 */
	FT_Library* library;

	/**
	 * Current error code. Zero means no error.
	 */
	FT_Error err;
};

} // namespace ppogl

#endif // __PPOGL_FTLIBRARY_H_
