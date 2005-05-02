/* 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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
 
#ifndef _PP_DEFS_H
#define _PP_DEFS_H

#include <math.h>

#ifndef M_PI
#   define M_PI 3.1415926535
#endif

#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))

#define EPS 1e-13

#define ROUND_TO_NEAREST(x) ( int((x)+0.5) )

/// convert angles to radians
#define ANGLES_TO_RADIANS(x) ( M_PI / 180.0 * (x) )

/// convert radioans to angles
#define RADIANS_TO_ANGLES(x) ( 180.0 / M_PI * (x) )

/// Returns the number of ellements in an array.
/// This only works for static arrays in the same context
#define PP_NUM_ELEMENTS(array) (sizeof(array) / sizeof ((array)[0]))

#endif // PP_DEFS_H
