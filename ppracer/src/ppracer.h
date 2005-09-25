/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * Copyright (C) 1999-2001 Jasmin F. Patry
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _PPRACER_H_
#define _PPRACER_H_

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#if defined ( __MWERKS__ ) || defined( _MSC_VER )
#   define NATIVE_WIN32_COMPILER 1
#else
/* Assume UNIX compatible by default */
#   define COMPILER_IS_UNIX_COMPATIBLE 1
#endif

#if defined( WIN32 ) || defined( __CYGWIN__ ) || \
    defined ( NATIVE_WIN32_COMPILER )
#  ifndef WIN32
#     define WIN32
#  endif
#  include <windows.h>
#endif 

#if defined( NATIVE_WIN32_COMPILER )
/* Need to manually define some things that autoconf defines for
   us in config.h */
#   define VERSION "0.5alpha"
#   define HAVE_SDL_MIXER 1
#   define HAVE_SDL_JOYSTICKOPEN 1

#   define HAVE__ISNAN

#   ifdef _MSC_VER
#       pragma warning (disable:4244) /* conversion from double to float */
#       pragma warning (disable:4305) /* truncation from const dbl to float */
#       pragma warning (disable:4761) /* integral size mismatch */
#   endif /* _MSC_VER */

#   ifdef __MWERKS__
        /* Codewarrior 4 seems to need this... */
        int _isnan( double x );
#   endif /* __MWERKS__ */
#endif

/* Include all (or most) system include files here.  This slows down
   compilation but makes maintenance easier since system-dependent
   #ifdefs are centralized. */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>

/* Macros and include files for non-standard math routines */
#ifdef HAVE_IEEEFP_H
#   include <ieeefp.h>
#endif
#include <float.h>

/* System-dependent includes */
#if defined( NATIVE_WIN32_COMPILER )
#  include <io.h>
#  include <direct.h>
#endif

#if defined( COMPILER_IS_UNIX_COMPATIBLE )
#   include <unistd.h>
#   include <sys/types.h>
#	ifndef WIN32
#   	include <pwd.h>
#	endif
#   include <dirent.h>
#   include <sys/time.h>
#   include <sys/types.h>
#   include <dirent.h>
#endif

/* Some versions of sys/stat.h don't define S_ISDIR */
#ifndef S_ISDIR
#   define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif /* S_ISDIR */

/* Header files to include by default */

#include "ppogl/base/errors.h"
#include "ppogl/base/assert.h"

#include "ppogl/config.h"
#define PPConfig ppogl::Config::getInstance()

#define PROG_NAME "ppracer"

#if defined( HAVE_FINITE )
#   define FINITE(x) (finite(x))
#elif defined( HAVE__FINITE )
#   define FINITE(x) (_finite(x))
#elif defined( HAVE_ISNAN )
#   define FINITE(x) (!isnan(x))
#elif defined( HAVE__ISNAN )
#   define FINITE(x) (!_isnan(x))
#else
#   error "You don't have finite(), _finite(), isnan(), or _isnan() on your system!"
#endif


/* Macros for swapping bytes */
#define SWAP_WORD(x) \
{ \
unsigned long tmp; \
tmp  = ((x) >> 24) & 0x000000ff; \
tmp |= ((x) >> 8)  & 0x0000ff00; \
tmp |= ((x) << 8)  & 0x00ff0000; \
tmp |= ((x) << 24) & 0xff000000; \
(x) = tmp; \
}

#define SWAP_SHORT(x) \
{ \
unsigned short tmp; \
tmp  = ((x) << 8)  & 0xff00; \
tmp |= ((x) >> 8)  & 0x00ff; \
(x) = tmp; \
}

#define BUFF_LEN 512

/// Number of lives players get to complete a cup
#define INIT_NUM_LIVES 4

/// The custom log modes 
enum DebugMode{
    DEBUG_ODE       = 1,
    DEBUG_QUADTREE,
    DEBUG_CONTROL,
    DEBUG_SOUND,
    DEBUG_TEXTURE,
    DEBUG_VIEW,
    DEBUG_GL_EXT,
    DEBUG_FONT,
    DEBUG_UI,
    DEBUG_GAME_LOGIC,
    DEBUG_SAVE,
    DEBUG_JOYSTICK,
    DEBUG_GL_INFO,
    NUM_DEBUG_MODES
};

/// multiplier for scaling the textures to match the terrains
#define TEX_SCALE 6

#include "ppogl/ppogl_script.h"

/// global instance of the lua interpreter
extern ppogl::Script script;

///release name
#define PP_RELEASE_NAME "0.5alpha (Den Blembel kamma faj ned dringg'n)"

#endif // _PPRACER_H_
