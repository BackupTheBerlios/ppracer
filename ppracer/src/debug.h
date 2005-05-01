/* 
 * PPRacer 
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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "ppgltk/alg/errors.h"

typedef enum {
    DEBUG_ODE,
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
} debug_mode_t;

#endif /* _DEBUG_H_ */
