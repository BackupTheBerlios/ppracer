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

#include "ppracer.h"
#include "nmrcl.h"

static const double ode45_time_step_mat[] = 
{ 0., 1./5., 3./10., 4./5., 8./9., 1., 1. };

static const double ode45_coeff_mat[][7] = {
    { 0, 1./5., 3./40.,  44./45.,  19372./6561.,   9017./3168.,     35./384. },
    { 0, 0.,    9./40., -56./15., -25360./2187.,     -355./33.,           0. },
    { 0, 0.,         0,   32./9.,  64448./6561.,  46732./5247.,   500./1113. },
    { 0, 0.,         0,        0,    -212./729.,      49./176.,    125./192. },
    { 0, 0.,         0,        0,             0, -5103./18656., -2187./6784. },
    { 0, 0.,         0,        0,             0,             0,      11./84. },
    { 0, 0.,         0,        0,             0,             0,           0. }
};

static const double ode45_error_mat[] = 
{ 71./57600., 0., -71./16695., 71./1920., -17253./339200., 22./525., -1./40.};

void
ODESolver::initODEData(double _init_val, double _h)
{
    init_val = _init_val;
    h = _h;
}

double
ODESolver::nextVal(int step)
{
    double val = init_val;
    for(int i=0; i<step; i++ ) {
		val += ode45_coeff_mat[i][step] * k[i];
    }
    return val;
}

void
ODESolver::updateEstimate(int step, double val)
{
    k[step] = h * val;
}

double
ODESolver::finalEstimate()
{
	double val = init_val;
	for(int i=0; i<6; i++ ){
		val += ode45_coeff_mat[i][6] * k[i];
    }
    return val;
}

double
ODESolver::estimateError()
{
    double err=0.;
    for(int i=0; i<7; i++){
		err += ode45_error_mat[i] * k[i];
    }
    return fabs(err);
}


double
lin_interp(const double x[], const double y[], double val, int n)
{
    PP_REQUIRE(n>=2, "linear interpolation requires at least two data points");

	int i;
	
	if(val < x[0]){
		i = 0;
	}else if(val >= x[n-1]){
		i = n-2;
	}else{
		// should replace with binary search if we ever use large tables
		for(i=0; i<n-1; i++){
			if(val < x[i+1]) break;
		}
    }
	
    const double m = ( y[i+1] - y[i] ) / ( x[i+1] - x[i] );
    const double b = y[i] - m * x[i];

    return m * val + b;
}
