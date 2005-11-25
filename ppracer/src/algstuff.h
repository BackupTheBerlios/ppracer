/* 
 * PlanetPenguin Racer
 * Copyright (C) 2004-2005 Volker Stroebel <mmv1@planetpenguin.de>
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
 
/*
 * This file contains some classes from ppgltk/alg.
 * They will be replaced as soon as there are useable replacements in ppogl
 */
 
#ifndef _ALGSTUFF_H_
#define _ALGSTUFF_H_

#include "ppogl/base/vec3d.h"

///things to replace 
namespace pp{

class Quat;

/// Matrix class
class Matrix
{
public:
	union{
		double data[4][4];
		double rawdata[16];
	};

	Matrix(){};
	Matrix(const Quat& quat);

	Matrix operator*(const Matrix& matrix) const;
		
	void makeIdentity(void);
	void makeRotation(const double angle, const char axis);
	void makeTranslation(const double x, const double y, const double z);
	void makeScaling(const double x, const double y, const double z );
	void makeRotationAboutVector(const ppogl::Vec3d& u, const double angle );

	void transpose(const Matrix& mat);
		
	ppogl::Vec3d  transformVector(const ppogl::Vec3d& v) const;
	ppogl::Vec3d  transformPoint(const ppogl::Vec3d& p) const;
		
	static void makeChangeOfBasisMatrix(Matrix& mat,
				Matrix& invMat,
				const ppogl::Vec3d& w1,
				const ppogl::Vec3d& w2,
				const ppogl::Vec3d& w3);	
};

/// Plane class
class Plane
{
public:
	Plane(){};
	Plane(const double x, const double y, const double z, const double _d);
	
	double distance(const ppogl::Vec3d& point) const;
	static bool intersect( const Plane& s1, const Plane& s2, const Plane& s3, ppogl::Vec3d *p );
		
	ppogl::Vec3d nml;
    double d;
};

///Quaternion class
class Quat
{
public:
	double x, y ,z, w;

	Quat(){};
	Quat(const double x, const double y, const double z, const double w);	
	Quat(const ppogl::Vec3d& s, const ppogl::Vec3d& t);
	Quat(const Matrix& matrix);
	
	
	void set(const double x, const double y, const double z, const double w);	

	Quat conjugate(void) const;
	ppogl::Vec3d rotate( const ppogl::Vec3d& v ) const;
	
	Quat operator*(const Quat& quat) const;

	static Quat interpolate(const Quat& q, Quat r,double t );		
};

} //namespace pp

//todo: move this to glwrappers.h in ppogl

#ifdef _WIN32
	#include <WTypes.h>
#endif

#include <GL/gl.h>

namespace gl{
	
	inline void MultMatrix(const pp::Matrix& matrix){glMultMatrixd(matrix.rawdata);}
	
} //namespace gl

#endif // _ALGSTUFF_H
