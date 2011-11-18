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

#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <iostream>

#include "mathdef.hpp"
#include "vector3.hpp"
#include "xp_math.hpp"

#include "cstdmf/stdmf.hpp"


/**
 *	This class implements a vector of four floats.
 *
 *	@ingroup Math
 */
class Vector4 : public Vector4Base
{
public:
	Vector4();
	Vector4( float a, float b, float c, float d );
	explicit Vector4( const Vector4Base & v );
	Vector4( const Vector3 & v, float w );

	// Use the default compiler implementation for these methods. It is faster.
	//	Vector4( const Vector4& v);
	//	Vector4& operator = ( const Vector4& v );

	void setZero();
	void set( float a, float b, float c, float d ) ;
	void scale( const Vector4& v, float s );
	void scale( float s );
	void parallelMultiply( const Vector4& v );
	float length() const;
	float lengthSquared() const;
	void normalise();
	Outcode calculateOutcode() const;

	float dotProduct( const Vector4& v ) const;

	void operator += ( const Vector4& v);
	void operator -= ( const Vector4& v);
	void operator *= ( const Vector4& v);
	void operator /= ( const Vector4& v);
	void operator *= ( float s);

	static const Vector4 & zero()	{ return s_zero; }

private:
    friend std::ostream& operator <<( std::ostream&, const Vector4& );
    friend std::istream& operator >>( std::istream&, Vector4& );

	// This is to prevent construction like:
	//	Vector4( 0 );
	// It would interpret this as a float * and later crash.
	Vector4( int value );

	static Vector4	s_zero;
};

Vector4 operator +( const Vector4& v1, const Vector4& v2 );
Vector4 operator -( const Vector4& v1, const Vector4& v2 );
Vector4 operator *( const Vector4& v1, const Vector4& v2 );
Vector4 operator /( const Vector4& v1, const Vector4& v2 );
Vector4 operator *( const Vector4& v, float s );
Vector4 operator *( float s, const Vector4& v );
Vector4 operator /( const Vector4& v, float s );
bool operator   ==( const Vector4& v1, const Vector4& v2 );
bool operator   !=( const Vector4& v1, const Vector4& v2 );
bool operator   < ( const Vector4& v1, const Vector4& v2 );

typedef Vector4 Vector4;

#ifdef CODE_INLINE
#include "vector4.ipp"
#endif

#endif // VECTOR4_HPP

// vector4.hpp
