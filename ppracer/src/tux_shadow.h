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

#ifndef _TUX_SHADOW_H_
#define _TUX_SHADOW_H_

#include "hier_util.h"

void draw_tux_shadow(int player);
void traverse_dag_for_shadow(SceneNode *node, const pp::Matrix& model_matrix);
void draw_shadow_sphere(const pp::Matrix& model_matrix);
void draw_shadow_vertex(double x, double y, double z, 
			 const pp::Matrix& model_matrix );

#endif // _TUX_SHADOW_H_
