/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <iostream>

#include "xp_math.hpp"
#include "cstdmf/stdmf.hpp"

/**
 *	This class implements a vector of three floats.
 *
 *	@ingroup Math
 */
class Vector3 : public Vector3Base
{

public:
	Vector3();
	Vector3( float a, float b, float c );
	explicit Vector3( const Vector3Base & v );

	// Use the default compiler implementation for these methods. It is faster.
	//	Vector3( const Vector3& v);
	//	Vector3& operator  = ( const Vector3& v );

	void setZero();
	void set( float a, float b, float c );
//	void scale( const Vector3& v, float s );
	void setPitchYaw( float pitchInRadians, float yawInRadians );

	float dotProduct( const Vector3& v ) const;
	void crossProduct( const Vector3& v1, const Vector3& v2 );
	Vector3 crossProduct( const Vector3 & v ) const;
	void lerp( const Vector3 & v1, const Vector3 & v2, float alpha );

	void projectOnto( const Vector3& v1, const Vector3& v2 );
	Vector3 projectOnto( const Vector3 & v ) const;

	float length() const;
	float lengthSquared() const;
	void normalise();

	void operator += ( const Vector3& v );
	void operator -= ( const Vector3& v );
	void operator *= ( float s );
	void operator /= ( float s );
	Vector3 operator-() const;

	float yaw() const;
	float pitch() const;

	static const Vector3 & zero()		{ return s_zero; }

private:
    friend std::ostream& operator <<( std::ostream&, const Vector3& );
    friend std::istream& operator >>( std::istream&, Vector3& );

	// This is to prevent construction like:
	//	Vector3( 0 );
	// It would interpret this as a float * and later crash.
	Vector3( int value );

	static Vector3 s_zero;
};

Vector3 operator +( const Vector3& v1, const Vector3& v2 );
Vector3 operator -( const Vector3& v1, const Vector3& v2 );
Vector3 operator *( const Vector3& v, float s );
Vector3 operator *( float s, const Vector3& v );
Vector3 operator *( const Vector3& v1, const Vector3& v2 );
Vector3 operator /( const Vector3& v, float s );
bool operator   ==( const Vector3& v1, const Vector3& v2 );
bool operator   !=( const Vector3& v1, const Vector3& v2 );
bool operator   < ( const Vector3& v1, const Vector3& v2 );

inline bool almostEqual( const Vector3& v1, const Vector3& v2, const float epsilon = 0.0004f )
{
	return almostEqual( v1.x, v2.x, epsilon ) &&
		almostEqual( v1.y, v2.y, epsilon ) &&
		almostEqual( v1.z, v2.z, epsilon );
}

// typedef Vector3 Vector3;

// Vector3 pitchYawToVector3( float pitchInRadians, float yawInRadians );

#ifdef CODE_INLINE
#include "vector3.ipp"
#endif

#endif // VECTOR3_HPP
