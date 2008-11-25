/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

/**
 *	@file
 *
 *	@ingroup Math
 */

#include "pch.hpp"
#include <stdio.h>

#include <math.h>

#include "matrix.hpp"

#include "quat.hpp"

#ifndef CODE_INLINE
#include "quat.ipp"
#endif

#ifndef EXT_MATH
/**
 *	This method slerps from qStart when t = 0 to qEnd when t = 1. It is safe for
 *	the result quaternion (i.e. 'this') to be one of the parameters.
 *
 *	@param qStart	The quaternion to start from.
 *	@param qEnd		The quaternion to end at from.
 *	@param t		A value in the range [0, 1] that indicates the fraction to
 *					slerp between to to input quaternions.
 */
void Quaternion::slerp( const Quaternion &qStart, const Quaternion &qEnd,
		float t )
{
    // Compute dot product (equal to cosine of the angle between quaternions)
	float cosTheta = qStart.v().dotProduct( qEnd.v() ) + qStart.w * qEnd.w;

	bool invert = false;

    if( cosTheta < 0.0f )
    {
        // If so, flip one of the quaterions
        cosTheta = -cosTheta;

		invert = true;
	}

    // Set factors to do linear interpolation, as a special case where the
    // quaternions are close together.
    float  t2 = 1.0f - t;

    // If the quaternions aren't close, proceed with spherical interpolation
    if( 1.0f - cosTheta > 0.001f )
    {
        float theta = acosf( cosTheta );

        t2  = sinf( theta * t2 ) / sinf( theta);
        t = sinf( theta * t ) / sinf( theta);
    }

	if( invert )
		t = -t;

    // Do the interpolation

	this->v() = qStart.v() * t2 + qEnd.v() * t;
	w = qStart.w * t2 + qEnd.w * t;
}


/**
 *	This method initialises this quaternion base on the input angle and axis.
 *	The input parameters describe a rotation in 3 dimensions.
 *
 *	@param angle	The angle in radians to rotate around the input axis.
 *	@param axis		The axis to rotate around.
 */
void Quaternion::fromAngleAxis( float angle, const Vector3 &axis )
{
	this->v() = axis;
	this->v().normalise();

	this->v() *= sinf( angle * 0.5f );
	w = cosf( angle * 0.5f );
}


/**
 *	This method initialises this quaternion based on the rotation in the input
 *	matrix. The translation present in the matrix is not used.
 *
 *	@param m	The matrix to set this quaternion from.
 */
void Quaternion::fromMatrix( const Matrix &m )
{
//	float tr, s, q[4];
//	int j, k;
//	int nxt[3] = { 1, 2, 0 };

	float tr = m[0][0] + m[1][1] + m[2][2];

	if( tr > 0 )
	{
		float s = sqrtf( tr + 1.f );
		w = s * 0.5f;

		s = 0.5f / s;

		this->v()[0] = ( m[1][2] - m[2][1] ) * s;
		this->v()[1] = ( m[2][0] - m[0][2] ) * s;
		this->v()[2] = ( m[0][1] - m[1][0] ) * s;

	}
	else
	{
		int i = 0;

		if( m[1][1] > m[0][0] )
			i = 1;
		if( m[2][2] > m[i][i] )
			i = 2;

		int j = ( i + 1 ) % 3;
		int k = ( j + 1 ) % 3;

		float s = sqrtf( (m[i][i] - ( m[j][j] + m[k][k] ) ) + 1.f );

		this->v()[i] = s * 0.5f;

		if( s != 0.f )
			s = 0.5f / s;

		w    = ( m[j][k] - m[k][j] ) * s;
		this->v()[j] = ( m[i][j] + m[j][i] ) * s;
		this->v()[k] = ( m[i][k] + m[k][i] ) * s;

	}

}


/**
 *	This method sets this quaternion to be the rotation that would result by
 *	applying the rotation of q1 and then the rotation of q2.
 */
void Quaternion::multiply( const Quaternion& q1, const Quaternion& q2 )
{
	float localw = q2.w * q1.w - q2.v().dotProduct( q1.v() );
	Vector3 v;
	v.crossProduct( q2.v(), q1.v() );
	this->v() = ( q2.w * q1.v() ) + ( q1.w * q2.v() ) + v;
	w = localw;
}


/**
 *	This method normalises this quaternion. That is, it makes sure that the
 *	quaternion has a length of 1.
 */
void Quaternion::normalise()
{
	float invd = 1.f / sqrtf( this->v().dotProduct( this->v() ) + w * w );

	this->v() *= invd;
	w *= invd;
}


/**
 *	This method inverts this quaternion.
 */
void Quaternion::invert()
{
	float oodivisor = 1.f / ( this->v().lengthSquared() + ( w * w ) );
	this->v() *= -1 * oodivisor;
	w *= oodivisor;
}

#endif // !EXT_MATH


#include <stdio.h>

/**
 *	This function implements the output streaming operator for Quaternion.
 *
 *	@relates Quaternion
 */
std::ostream& operator<<(std::ostream& o, const Quaternion& t)
{
	Vector3	vn = t.v();
	vn.normalise();
	float	angle = acosf( t.w )*360.0f/MATH_PI;

	char	sprintfRulesBuffer[128];
	sprintf( sprintfRulesBuffer, "%.3f\0260 around (%.3f,%.3f,%.3f)",
		angle, vn[0], vn[1], vn[2] );

	return o << sprintfRulesBuffer;
}

/*quat.cpp*/
