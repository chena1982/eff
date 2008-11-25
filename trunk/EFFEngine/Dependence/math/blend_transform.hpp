/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef BLEND_TRANSFORM_HPP
#define BLEND_TRANSFORM_HPP

// #include <math/matrix44.hpp>
// #include <math/matrix34.hpp>

#include "matrix.hpp"
#include "quat.hpp"
#include "vector3.hpp"

class BlendTransform
{
public:
	Quaternion	rotate;
	Vector3		scale;
	Vector3		translate;

public:
	const Quaternion& rotation() const { return rotate; };
	const Vector3& scaling() const { return scale; };
	const Vector3& translation() const { return translate; };

	inline void init( const Matrix& ma )
	{
		Matrix m = ma;

		Vector3& row0 = m[0];
		Vector3& row1 = m[1];
		Vector3& row2 = m[2];
		Vector3& row3 = m[3];

		scale.x = XPVec3Length( &row0 );
		scale.y = XPVec3Length( &row1 );
		scale.z = XPVec3Length( &row2 );
		
		row0 *= 1.f / scale.x;
		row1 *= 1.f / scale.y;
		row2 *= 1.f / scale.z;

		Vector3 in;

		XPVec3Cross( &in, &row0, &row1 );
		if( XPVec3Dot( &in, &row2 ) < 0 )
		{
			row2 *= -1;
			scale.z *= -1;
		}

		translate = row3;
		XPQuaternionRotationMatrix( &rotate, &m );
	}

	explicit inline BlendTransform( const Matrix& m )
	{
		init( m );
	}

	inline BlendTransform() :
		rotate( 0, 0, 0, 1 ),
		scale( 1, 1, 1 ),
		translate( 0, 0, 0 )
	{
	}

	inline BlendTransform(
			const Quaternion & irotate,
			const Vector3 & iscale,
			const Vector3 & itranslate ) :
		rotate( irotate ),
		scale( iscale ),
		translate( itranslate )
	{
	}

	~BlendTransform()
	{
	}

	/**
	 *  This function normalises the rotation quaternion.
	 */
	inline void normaliseRotation()
	{
		rotate.normalise();
	}

	inline void blend( float ts, const BlendTransform& bt )
	{
		if ( ts > 0 && ts < 1 )
		{
			XPQuaternionSlerp( &rotate, &rotate, &bt.rotate, ts );
			XPVec3Lerp( &translate, &translate, &bt.translate, ts );
			XPVec3Lerp( &scale, &scale, &bt.scale, ts );
		}
		else if ( ts >= 1 )
		{
			*this = bt;
		}
		/*
		else
		{
			// do nothing
		}
		*/
	}

	inline void output( Matrix & mOut ) const
	{
		XPMatrixRotationQuaternion( &mOut, &rotate );
		mOut[0] *= scale.x;
		mOut[1] *= scale.y;
		mOut[2] *= scale.z;
		mOut[3] = translate;
	}

};

#endif // BLEND_TRANSFORM_HPP
