
#ifndef __PPOGL_FTGL_H_
#define __PPOGL_FTGL_H_

#ifndef FT_RENDER_MODE_MONO
    #define FT_RENDER_MODE_MONO ft_render_mode_mono
#endif

#ifndef FT_RENDER_MODE_NORMAL
    #define FT_RENDER_MODE_NORMAL ft_render_mode_normal
#endif

// Compiler-specific conditional compilation
#ifdef _MSC_VER // MS Visual C++ 
    // Disable various warning.
    // 4786: template name too long
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4786 )
#endif  

#endif // __PPOGL_FTGL_H_
