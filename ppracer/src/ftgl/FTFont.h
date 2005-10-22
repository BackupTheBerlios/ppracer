#ifndef    __FTFont__
#define    __FTFont__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "FTFace.h"
#include "FTGL.h"

class FTGlyphContainer;
class FTGlyph;


/**
 * FTFont is the public interface for the FTGL library.
 *
 * Specific font classes are derived from this class. It uses the helper
 * classes FTFace and FTSize to access the Freetype library. This class
 * is abstract and deriving classes must implement the protected
 * <code>MakeGlyph</code> function to create glyphs of the
 * appropriate type.
 *
 * It is good practice after using these functions to test the error
 * code returned. <code>FT_Error Error()</code>. Check the freetype file fterrdef.h
 * for error definitions.
 *
 * @see     FTFace
 * @see     FTSize
 * @see     FTGlyphContainer
 * @see     FTGlyph
 */
class FTGL_EXPORT FTFont
{
    public:
		// increment reference counter   
		void reference();
	
		// decrement reference counter   
		void unreference();
	
		/**
         * Open and read a font file. Sets Error flag.
         *
         * @param fontFilePath  font file path.
         */
        FTFont( const char* fontFilePath);
        
        /**
         * Destructor
         */
        virtual ~FTFont();
        
        /**
         * Set the character map for the face.
         *
         * @param encoding      Freetype enumerate for char map code.
         * @return              <code>true</code> if charmap was valid and
         *                      set correctly
         */
        bool CharMap( FT_Encoding encoding );
       
        /**
         * Set the char size for the current face.
         *
         * @param size      the face size in points (1/72 inch)
         * @param res       the resolution of the target device.
         * @return          <code>true</code> if size was set correctly
         */
        virtual bool FaceSize( const unsigned int size, const unsigned int res = 72);
                
        /**
         * Get the global ascender height for the face.
         *
         * @return  Ascender height
         */
        float Ascender() const;
        
        /**
         * Gets the global descender height for the face.
         *
         * @return  Descender height
         */
        float Descender() const;
               
        /**
         * Get the advance width for a string.
         *
         * @param string    a wchar_t string
         * @return      advance width
         */
        float Advance( const wchar_t* string);

        /**
         * Render a string of characters
         * 
         * @param string    wchar_t string to be output.     
         */
        virtual void Render( const wchar_t* string );

        /**
         * Queries the Font for errors.
         *
         * @return  The current error code.
         */
        FT_Error Error() const { return err;}

    protected:
        /**
         * Construct a glyph of the correct type.
         *
         * Clients must overide the function and return their specialised
         * FTGlyph.
         *
         * @param g The glyph index NOT the char code.
         * @return  An FT****Glyph or <code>null</code> on failure.
         */
        virtual FTGlyph* MakeGlyph( unsigned int g) = 0;
        
        /**
         * Current face object
         */
        FTFace face;
        
        /**
         * Current size object
         */
        FTSize charSize;

        /**
         * Current error code. Zero means no error.
         */
        FT_Error err;
		
	private:        
    	
		// reference counter;
		int m_references;
	
		/**
         * Check that the glyph at <code>chr</code> exist. If not load it.
         *
         * @param chr  character index
         * @return <code>true</code> if the glyph can be created.
         */
        inline bool CheckGlyph( const unsigned int chr);

        /**
         * An object that holds a list of glyphs
         */
        FTGlyphContainer* glyphList;
        
        /**
         * Current pen or cursor position;
         */
        FTPoint pen;
		
	protected:
		/**
         * Flag to enable or disable the use of Display Lists inside FTGL
         * <code>true</code> turns ON display lists.
         * <code>false</code> turns OFF display lists.
         */
        bool useDisplayLists;
        
};


#endif  //  __FTFont__
