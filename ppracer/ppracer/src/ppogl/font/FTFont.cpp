#include <cassert>
#include <string>

#include "FTFont.h"
#include "FTGlyph.h"

inline GLuint
NextPowerOf2(GLuint in)
{
     in -= 1;

     in |= in >> 16;
     in |= in >> 8;
     in |= in >> 4;
     in |= in >> 2;
     in |= in >> 1;

     return in + 1;
}

namespace ppogl{

FTFont::FTFont(const std::string& fontFilePath)
 : maximumGLTextureSize(0),
   textureWidth(0),
   textureHeight(0),
   glyphHeight(0),
   glyphWidth(0),
   padding(3),
   xOffset(0),
   yOffset(0),
   glyphList(NULL),
   face(fontFilePath)
{	
	err = face.Error();
    if(err == 0){
        glyphList = new FTGlyphContainer(&face);
    }
    remGlyphs = numGlyphs = face.GlyphCount();
}

FTFont::~FTFont()
{
    gl::DeleteTextures(textureIDList.size(), static_cast<const GLuint*>(&textureIDList[0]));
	    
	if(glyphList != NULL){
        delete glyphList;
    }
}

FTGlyph*
FTFont::MakeGlyph(unsigned int glyphIndex)
{
    FT_GlyphSlot ftGlyph = face.Glyph(glyphIndex, FT_LOAD_NO_HINTING);
    
    if(ftGlyph){
        glyphHeight = static_cast<int>(charSize.Height());
        glyphWidth = static_cast<int>(charSize.Width());
        
        if(textureIDList.empty()){
            textureIDList.push_back(CreateTexture());
            xOffset = yOffset = padding;
        }
        
        if(xOffset > (textureWidth - glyphWidth)){
            xOffset = padding;
            yOffset += glyphHeight;
            
            if(yOffset > (textureHeight - glyphHeight)){
                textureIDList.push_back(CreateTexture());
                yOffset = padding;
            }
        }
        
        FTGlyph* tempGlyph = new FTGlyph(ftGlyph, textureIDList[textureIDList.size() - 1],
                                                        xOffset, yOffset, textureWidth, textureHeight);
        xOffset += static_cast<int>(tempGlyph->BBox().xMax/64.0 - tempGlyph->BBox().xMin/64.0 + padding);
        
        --remGlyphs;
        return tempGlyph;
    }
    
    err = face.Error();
    return NULL;
}


void
FTFont::CalculateTextureSize()
{
    if(!maximumGLTextureSize){
		gl::GetValue(GL_MAX_TEXTURE_SIZE, static_cast<GLint*>(&maximumGLTextureSize));
       		
		assert(maximumGLTextureSize); // If you hit this then you have an invalid OpenGL context.
    }
    
    textureWidth = NextPowerOf2((remGlyphs * glyphWidth) + ( padding * 2));
    textureWidth = textureWidth > maximumGLTextureSize ? maximumGLTextureSize : textureWidth;
    
    int h = static_cast<int>((textureWidth - ( padding * 2)) / glyphWidth);
        
    textureHeight = NextPowerOf2((( numGlyphs / h) + 1) * glyphHeight);
    textureHeight = textureHeight > maximumGLTextureSize ? maximumGLTextureSize : textureHeight;
}

GLuint
FTFont::CreateTexture()
{   
    CalculateTextureSize();
    
    int totalMemory = textureWidth * textureHeight;
    unsigned char* textureMemory = new unsigned char[totalMemory];
    memset(textureMemory, 0, totalMemory);

    GLuint textID;
    gl::GenTextures(1, static_cast<GLuint*>(&textID));

    gl::BindTexture(GL_TEXTURE_2D, textID);
    gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl::TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    gl::TexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, textureWidth, textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, textureMemory);

    delete [] textureMemory;

    return textID;
}

bool
FTFont::FaceSize(const unsigned int size, const unsigned int res)
{
    if(!textureIDList.empty()){
        gl::DeleteTextures(textureIDList.size(), static_cast<const GLuint*>(&textureIDList[0]));
        textureIDList.clear();
        remGlyphs = numGlyphs = face.GlyphCount();
    }
	
	charSize = face.Size( size, res);
    err = face.Error();
    
    if(err != 0){
        return false;
    }
    
    if(glyphList != NULL){
        delete glyphList;
    }
    
    glyphList = new FTGlyphContainer(&face);
    return true;
}

void
FTFont::Render(const wchar_t* string)
{   
    gl::PushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
    
    gl::Enable(GL_BLEND);
    gl::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE
    
    FTGlyph::ResetActiveTexture();
     
	const wchar_t* c = string;
    pen.x()=0;
	pen.y()=0;

    while(*c){
        if(CheckGlyph(*c)){
            pen = glyphList->Render(*c, *(c + 1), pen);
        }
        ++c;
    }

    gl::PopAttrib();
}

float
FTFont::Advance(const wchar_t* string)
{
    const wchar_t* c = string;
    float width = 0.0f;

    while( *c){
        if(CheckGlyph(*c)){
            width += glyphList->Advance( *c, *(c + 1));
        }
        ++c;
    }
    
    return width;
}

bool
FTFont::CheckGlyph(const unsigned int characterCode)
{
    if(NULL == glyphList->Glyph( characterCode)){
        unsigned int glyphIndex = glyphList->FontIndex( characterCode);
        FTGlyph* tempGlyph = MakeGlyph( glyphIndex);
        if(NULL == tempGlyph){
            if(0 == err){
                err = 0x13;
            }
            
            return false;
        }
        glyphList->Add(tempGlyph, characterCode);
    }
    
    return true;
}

} // namespace ppogl
