#include    "FTFace.h"
#include    "FTFont.h"
#include    "FTGlyphContainer.h"
#include    "FTBBox.h"

void 
FTFont::reference()
{
	m_references++;	
}

void 
FTFont::unreference()
{
	m_references--;
	if(m_references<=0){
		delete this;	
	}	
}

FTFont::FTFont(const char* fontFilePath)
 : face( fontFilePath),
   m_references(0), 
   glyphList(0),
   useDisplayLists(true)
{
    err = face.Error();
    if( err == 0)
    {
        glyphList = new FTGlyphContainer( &face);
    }
}

FTFont::~FTFont()
{
	delete glyphList;
}

bool FTFont::FaceSize( const unsigned int size, const unsigned int res )
{
    charSize = face.Size( size, res);
    err = face.Error();
    
    if( err != 0)
    {
        return false;
    }
    
    if( glyphList != NULL)
    {
        delete glyphList;
    }
    
    glyphList = new FTGlyphContainer( &face);
    return true;
}

bool FTFont::CharMap( FT_Encoding encoding)
{
    bool result = glyphList->CharMap( encoding);
    err = glyphList->Error();
    return result;
}

float FTFont::Ascender() const
{
    return charSize.Ascender();
}

float FTFont::Descender() const
{
    return charSize.Descender();
}

float FTFont::Advance( const wchar_t* string)
{
    const wchar_t* c = string;
    float width = 0.0f;

    while( *c)
    {
        if(CheckGlyph( *c))
        {
            width += glyphList->Advance( *c, *(c + 1));
        }
        ++c;
    }
    
    return width;
}

void FTFont::Render( const wchar_t* string )
{
    const wchar_t* c = string;
    pen.X(0); pen.Y(0);

    while( *c)
    {
        if(CheckGlyph( *c))
        {
            pen = glyphList->Render( *c, *(c + 1), pen);
        }
        ++c;
    }
}

bool FTFont::CheckGlyph( const unsigned int characterCode)
{
    if( NULL == glyphList->Glyph( characterCode))
    {
        unsigned int glyphIndex = glyphList->FontIndex( characterCode);
        FTGlyph* tempGlyph = MakeGlyph( glyphIndex);
        if( NULL == tempGlyph)
        {
            if( 0 == err)
            {
                err = 0x13;
            }
            
            return false;
        }
        glyphList->Add( tempGlyph, characterCode);
    }
    
    return true;
}
