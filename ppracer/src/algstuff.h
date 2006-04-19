/* 
 * PlanetPenguin Racer
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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
#include "ppogl/base/defs.h"


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
		
	void makeIdentity()
	{
		for(int i=0; i<4; i++){
			for(int j=0; j<4; j++){
				data[i][j]=(i==j);	
			}
		}
	}
		
	void makeRotation(const double angle, const ppogl::Axis axis);
	
	
	void makeTranslation(const double x, const double y, const double z)
	{
		makeIdentity();
	    data[3][0] = x;
	    data[3][1] = y;
	    data[3][2] = z;
	}
	
	void makeScaling(const double x, const double y, const double z )
	{
		makeIdentity();
	    data[0][0] = x;
	    data[1][1] = y;
	    data[2][2] = z;
	}	
	
	void transpose(const Matrix& matrix)
	{
		for( int i= 0 ; i< 4 ; i++ ){
			for( int j= 0 ; j< 4 ; j++ ){
		    	data[j][i] = matrix.data[i][j];
			}
		}
	}

	void makeRotationAboutVector(const ppogl::Vec3d& u, const double angle );
	
	ppogl::Vec3d transformVector(const ppogl::Vec3d& v) const;
	ppogl::Vec3d transformPoint(const ppogl::Vec3d& p) const;
		
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
	Plane(){}
	Plane(const double x, const double y, const double z, const double _d)
	 : nml(x, y ,z),
	   d(_d)
	{
	}
	
	Plane(const ppogl::Vec3d& _nml, const double _d)
	 : nml(_nml),
	   d(_d)
	{
	}
		
	double distance(const ppogl::Vec3d& point) const
	{
		return 	nml.x() * point.x() +
				nml.y() * point.y() +
				nml.z() * point.z() +
				d;	
	}
	
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
	Quat(const double _x, const double _y, const double _z, const double _w)
	 : x(_x), y(_y), z(_z), w(_w)
	{
	}	
	
	Quat(const ppogl::Vec3d& s, const ppogl::Vec3d& t);
	Quat(const Matrix& matrix);
		
	void set(const double _x, const double _y, const double _z, const double _w)
	{
		x=_x;
		y=_y;
		z=_z;
		w=_w;
	}
		
	Quat conjugate() const
	{
		return Quat(-x, -y, -z, w);	
	}
	
	ppogl::Vec3d rotate(const ppogl::Vec3d& v) const
	{
	    Quat p(v.x(),v.y(),v.z(),1.0);
	    Quat res_q = (*this)*(p*conjugate());
    
		return ppogl::Vec3d(res_q.x,res_q.y,res_q.z);
	}
		
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
