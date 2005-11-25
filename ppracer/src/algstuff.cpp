/* 
 * PlanetPenguin Racer
 * Copyright (C) 2004-2005 Volker Stroebel <mmv1@planetpenguin.de>
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
 
#include "algstuff.h"

#include "ppogl/base/defs.h"


static unsigned short order(double *matrix, int n, int pivot);
static void elim(double *matrix, int n, int pivot);
static void backsb(double *matrix, int n, double *soln);

/*
  gauss function -- takes a two dimensional array as its arg,
  of dimension n x (n+1)
*/

static int gauss(double *matrix, int n, double *soln)
{
    int pivot=0;
    unsigned short error=0;


    while ((pivot<(n-1)) && (!error))
    {
	if(!(error = order(matrix,n,pivot)))
	{
	    elim(matrix,n,pivot);
	    pivot++;
	}
    }

    if (error)
    {
	return 1;
    }
    else
    {
	backsb(matrix, n, soln);
    }
	
    return 0;
}


static unsigned short order( double *matrix, int n, int pivot)
{
    int row, rmax, k;
    double temp;
    unsigned short error=0;

    rmax = pivot;

    for (row=pivot+1; row<n; row++)
    {
	if (fabs(*(matrix+row*(n+1)+pivot)) > fabs(*(matrix+rmax*(n+1)+pivot)))
	    rmax = row;
    }

    if (fabs(*(matrix+rmax*(n+1)+pivot)) < EPS )
	error = 1;
    else if (rmax != pivot)
    {
	for (k=0; k<(n+1); k++)
	{
	    temp = *(matrix+rmax*(n+1)+k);
	    *(matrix+rmax*(n+1)+k) = *(matrix+pivot*(n+1)+k);
	    *(matrix+pivot*(n+1)+k) = temp;
	}
    }

    return error;

}

static void elim(double *matrix, int n, int pivot)
{
    int row, col;
    double factor;

    for (row = pivot+1; row < n; row++)
    {
	factor = (*(matrix+row*(n+1)+pivot))/(*(matrix+pivot*(n+1)+pivot));
	*(matrix+row*(n+1)+pivot)=0.0;
	for (col=pivot+1l; col<n+1; col++)
	{
	    *(matrix+row*(n+1)+col) = *(matrix+row*(n+1)+col) - 
		(*(matrix+pivot*(n+1)+col))*factor;
	}
    }
}


static void backsb(double *matrix, int n, double *soln)
{
    int row, col;


    for (row = n-1; row >=0; row--)
    {
	for (col = n-1; col >= row+1; col--)
	{
	    *(matrix+row*(n+1)+(n)) = *(matrix+row*(n+1)+n) - 
		(*(soln+col))*(*(matrix+row*(n+1)+col));
	}
	*(soln+row) = (*(matrix+row*(n+1)+n))/(*(matrix+row*(n+1)+row));
    }

}

namespace pp {
	
Matrix::Matrix(const Quat& quat)
{
    data[0][0] = 1.0 - 2.0 * ( quat.y * quat.y + quat.z * quat.z );
    data[1][0] =       2.0 * ( quat.x * quat.y - quat.w * quat.z );
    data[2][0] =       2.0 * ( quat.x * quat.z + quat.w * quat.y );

    data[0][1] =       2.0 * ( quat.x * quat.y + quat.w * quat.z );
    data[1][1] = 1.0 - 2.0 * ( quat.x * quat.x + quat.z * quat.z );
    data[2][1] =       2.0 * ( quat.y * quat.z - quat.w * quat.x );

	data[0][2] =       2.0 * ( quat.x * quat.z - quat.w * quat.y );
    data[1][2] =       2.0 * ( quat.y * quat.z + quat.w * quat.x );
    data[2][2] = 1.0 - 2.0 * ( quat.x * quat.x + quat.y * quat.y );

    data[3][0] = data[3][1] = data[3][2] = 0.0;
    data[0][3] = data[1][3] = data[2][3] = 0.0;
    data[3][3] = 1.0;
}

void
Matrix::makeIdentity(void)
{
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			data[i][j]=(i==j);	
		}
	}
}	

void
Matrix::makeRotation(const double angle, const char axis)
{
	double sinv = sin( ANGLES_TO_RADIANS( angle ) );
    double cosv = cos( ANGLES_TO_RADIANS( angle ) );

    makeIdentity();

    switch( axis ) {
    case 'x':
        data[1][1] = cosv;
        data[2][1] = -sinv;
        data[1][2] = sinv;
        data[2][2] = cosv;
        break;

    case 'y':
        data[0][0] = cosv;
        data[2][0] = sinv;
        data[0][2] = -sinv;
        data[2][2] = cosv;
        break;

    case 'z': 
        data[0][0] = cosv;
        data[1][0] = -sinv;
        data[0][1] = sinv;
        data[1][1] = cosv;
        break;

    default:
		{
        //code_not_reached();  // shouldn't get here
		}
    }
}

void
Matrix::makeTranslation(const double x, const double y, const double z)
{
	makeIdentity();
    data[3][0] = x;
    data[3][1] = y;
    data[3][2] = z;
}

void
Matrix::makeScaling(const double x, const double y, const double z )
{
	makeIdentity();
    data[0][0] = x;
    data[1][1] = y;
    data[2][2] = z;
}

void
Matrix::makeRotationAboutVector(const ppogl::Vec3d& u, const double angle )
{
	Matrix rx,irx, ry, iry;
	
    double a = u.x();
    double b = u.y();
    double c = u.z();
	double d = sqrt( b*b + c*c );

    if ( d < EPS ) {
        if ( a < 0 ) 
            makeRotation( -angle, 'x' );
        else
            makeRotation( angle, 'x' );
        return;
    } 

    rx.makeIdentity();
    irx.makeIdentity();
    ry.makeIdentity();
    iry.makeIdentity();

    rx.data[1][1] = c/d;
    rx.data[2][1] = -b/d;
    rx.data[1][2] = b/d;
    rx.data[2][2] = c/d;

    irx.data[1][1] = c/d;
    irx.data[2][1] = b/d;
    irx.data[1][2] = -b/d;
    irx.data[2][2] = c/d;

    ry.data[0][0] = d;
    ry.data[2][0] = -a;
    ry.data[0][2] = a;
    ry.data[2][2] = d;

    iry.data[0][0] = d;
    iry.data[2][0] = a;
    iry.data[0][2] = -a;
    iry.data[2][2] = d;

    makeRotation( angle, 'z' );

    *this=(*this)*ry;
    *this=(*this)*rx;
    *this=iry*(*this);
    *this=irx*(*this);
}


void
Matrix::transpose(const Matrix& matrix)
{
	for( int i= 0 ; i< 4 ; i++ ){
		for( int j= 0 ; j< 4 ; j++ ){
	    	data[j][i] = matrix.data[i][j];
		}
	}
}

Matrix
Matrix::operator*(const Matrix& matrix) const
{
	Matrix ret;
 
    for( int i= 0 ; i< 4 ; i++ )
	for( int j= 0 ; j< 4 ; j++ )
	    ret.data[j][i]=
		data[0][i] * matrix.data[j][0] +
		data[1][i] * matrix.data[j][1] +
		data[2][i] * matrix.data[j][2] +
		data[3][i] * matrix.data[j][3];

   	return ret;
}

ppogl::Vec3d
Matrix::transformVector(const ppogl::Vec3d& v) const
{   
	return ppogl::Vec3d(
		v.x() * data[0][0] + v.y() * data[1][0] + v.z() * data[2][0],
    	v.x() * data[0][1] + v.y() * data[1][1] + v.z() * data[2][1],
    	v.x() * data[0][2] + v.y() * data[1][2] + v.z() * data[2][2]
	);
}
	
		
ppogl::Vec3d
Matrix::transformPoint(const ppogl::Vec3d& p) const
{
	return ppogl::Vec3d(
    	p.x() * data[0][0] + p.y() * data[1][0] + p.z() * data[2][0] + data[3][0],
    	p.x() * data[0][1] + p.y() * data[1][1] + p.z() * data[2][1] + data[3][1],
    	p.x() * data[0][2] + p.y() * data[1][2] + p.z() * data[2][2] + data[3][2]
    );
}

void
Matrix::makeChangeOfBasisMatrix(Matrix& mat,
				Matrix& invMat,
				const ppogl::Vec3d& w1,
				const ppogl::Vec3d& w2,
				const ppogl::Vec3d& w3)
{
	mat.makeIdentity();
    mat.data[0][0] = w1.x();
    mat.data[0][1] = w1.y();
    mat.data[0][2] = w1.z();
    mat.data[1][0] = w2.x();
    mat.data[1][1] = w2.y();
    mat.data[1][2] = w2.z();
    mat.data[2][0] = w3.x();
    mat.data[2][1] = w3.y();
    mat.data[2][2] = w3.z();

    invMat.makeIdentity();
    invMat.data[0][0] = w1.x();
    invMat.data[1][0] = w1.y();
    invMat.data[2][0] = w1.z();
    invMat.data[0][1] = w2.x();
    invMat.data[1][1] = w2.y();
    invMat.data[2][1] = w2.z();
    invMat.data[0][2] = w3.x();
    invMat.data[1][2] = w3.y();
    invMat.data[2][2] = w3.z();
}

Plane::Plane(const double x, const double y, const double z, const double _d)
 : nml(x, y ,z),
   d(_d)
{
}

double
Plane::distance(const ppogl::Vec3d& point) const
{
	return 	nml.x() * point.x() +
			nml.y() * point.y() +
			nml.z() * point.z() +
			d;	
}


bool
Plane::intersect( const Plane& s1, const Plane& s2, const Plane& s3, ppogl::Vec3d *p )
{
	double A[3][4];
    double x[3];
    double retval;

    A[0][0] =  s1.nml.x();
    A[0][1] =  s1.nml.y();
    A[0][2] =  s1.nml.z();
    A[0][3] = -s1.d;

    A[1][0] =  s2.nml.x();
    A[1][1] =  s2.nml.y();
    A[1][2] =  s2.nml.z();
    A[1][3] = -s2.d;

    A[2][0] =  s3.nml.x();
    A[2][1] =  s3.nml.y();
    A[2][2] =  s3.nml.z();
    A[2][3] = -s3.d;

    retval = gauss( reinterpret_cast<double*>(A), 3, x);

    if ( retval != 0 ) {
		// Matrix is singular 
		return false;
    } else {
		// Solution found
		p->x() = x[0];
		p->y() = x[1];
		p->z() = x[2];
		return true;
    }
}

Quat::Quat(const double _x, const double _y, const double _z, const double _w)
 : x(_x), y(_y), z(_z), w(_w)
{
}	
	
Quat::Quat(const ppogl::Vec3d& s, const ppogl::Vec3d& t)
{
    ppogl::Vec3d u = s^t;
	if (u.normalize() < EPS ){
		x=0.0;
		y=0.0;
		z=0.0;
		w=1.0;
    }else{
		double cos2phi = s*t;
		double sinphi = sqrt( ( 1 - cos2phi ) / 2.0 );
		double cosphi = sqrt( ( 1 + cos2phi ) / 2.0 );

		x = sinphi * u.x();
		y = sinphi * u.y();
		z = sinphi * u.z();
		w = cosphi;
    }
}

// source:
// http://www.gamasutra.com/features/19980703/quaternions_01.htm
//
Quat::Quat(const Matrix& matrix)
{
    static int nxt[3] = {1, 2, 0};
    double tr = matrix.data[0][0] + matrix.data[1][1] + matrix.data[2][2];

    // check the diagonal
   	if (tr > 0.0) {
		double s = sqrt (tr + 1.0);
		w = 0.5 * s;
		s = 0.5 / s;
		x = (matrix.data[1][2] - matrix.data[2][1]) * s;
		y = (matrix.data[2][0] - matrix.data[0][2]) * s;
		z = (matrix.data[0][1] - matrix.data[1][0]) * s;
    } else {                
		// diagonal is negative
		int i = 0;
		if (matrix.data[1][1] > matrix.data[0][0]) i = 1;
		if (matrix.data[2][2] > matrix.data[i][i]) i = 2;
		int j = nxt[i];
		int k = nxt[j];

		double s = sqrt (matrix.data[i][i] - matrix.data[j][j] - matrix.data[k][k] + 1.0);
        
		double q[4];		
		q[i] = s * 0.5;
                             
		if (s != 0.0) s = 0.5 / s;

		q[3] = (matrix.data[j][k] - matrix.data[k][j]) * s;
		q[j] = (matrix.data[i][j] + matrix.data[j][i]) * s;
		q[k] = (matrix.data[i][k] + matrix.data[k][i]) * s;

		x = q[0];
		y = q[1];
		z = q[2];
		w = q[3];
	}
}



void
Quat::set(const double _x, const double _y, const double _z, const double _w)
{
	x=_x;
	y=_y;
	z=_z;
	w=_w;
}


Quat
Quat::conjugate() const
{
	return Quat(-x, -y, -z, w);	
}

ppogl::Vec3d
Quat::rotate(const ppogl::Vec3d& v) const
{
    Quat p(v.x(),v.y(),v.z(),1.0);
    Quat res_q = (*this)*(p*conjugate());
    
	return ppogl::Vec3d(res_q.x,res_q.y,res_q.z);
}

Quat
Quat::operator*(const Quat& quat) const{
	return Quat(
		y * quat.z - z * quat.y + quat.w * x + w * quat.x,
		z * quat.x - x * quat.z + quat.w * y + w * quat.y,
		x * quat.y - y * quat.x + quat.w * z + w * quat.z,
		w * quat.w - x * quat.x - y * quat.y - z * quat.z
	);
}



Quat 
Quat::interpolate(const Quat& q, Quat r, double t )
{
    double cosphi;
    double sinphi;
    double phi;
    double scale0, scale1;

    cosphi = q.x * r.x + q.y * r.y + q.z * r.z + q.w * r.w;

    // adjust signs (if necessary) 
    if ( cosphi < 0.0 ) {
	cosphi = -cosphi;
	r.x = -r.x;
	r.y = -r.y;
	r.z = -r.z;
	r.w = -r.w;
    }

    if ( 1.0 - cosphi > EPS ) {
	// standard case -- slerp 
	phi = acos( cosphi );
	sinphi = sin( phi );
	scale0 = sin( phi * ( 1.0 - t ) ) / sinphi;
	scale1 = sin( phi * t ) / sinphi;
    } else {
	// use linear interpolation to avoid division by zero
	scale0 = 1.0 - t;
	scale1 = t;
    }

	return Quat(
    	scale0 * q.x + scale1 * r.x,
    	scale0 * q.y + scale1 * r.y,
    	scale0 * q.z + scale1 * r.z,
    	scale0 * q.w + scale1 * r.w
	);
}

} //namespace pp
