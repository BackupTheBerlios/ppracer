/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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

#ifndef _NMRCL_H_
#define _NMRCL_H_

#include "ppogl/base/defs.h"

class ODESolver
{
	double k[7];
	double init_val;
	double h;
public:
	void initODEData(double init_val, double h);
	double nextVal(int step);
	void updateEstimate(int step, double val);
	double finalEstimate();
	double estimateError();

	static double timeStepExponent(){return 1.0/5.0;}
	static int numEstimates(){return 7;}
};

double lin_interp( const double x[], const double y[], double val, int n );

#endif // _NMRCL_H_
