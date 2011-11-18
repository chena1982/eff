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

#ifdef CODE_INLINE
#define INLINE    inline
#else
	/// INLINE macro.
#define INLINE
#endif


/**
 *	This constructor creates a zero matrix. That is, a matrix whose elements are
 *	all 0.
 */
INLINE
Matrix::Matrix()
{
	this->setZero();
}


/**
 *	This method sets this matrix to the zero matrix. The zero matrix is the
 *	matrix whose elements are all 0.
 */
INLINE
void Matrix::setZero()
{
	memset( this, 0, sizeof( *this ) );
}


/**
 *	This method returns the determinant of this matrix. For a non-scaling,
 *	non-skewing transform, this should be 1.
 */
INLINE
float Matrix::getDeterminant() const
{
#ifdef EXT_MATH
	return XPMatrixfDeterminant( this );
#else
	// TODO: This only considers the 3x3 part of the matrix.
	float det = 0;

	det += m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
	det -= m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
	det += m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

	return det;
#endif
}


/**
 *	This method implements an operator that can be used to access individual
 *	elements of this matrix.
 *
 *	@param column	The column number of the desired element. This value must be
 *					in the range [0, 3].
 *	@param row		The row number of the desired element. This value must be in
 *					the range [0, 3].
 *
 *	@return A reference to the specified element.
 */
INLINE
float& Matrix::operator ()( uint32 column, uint32 row )
{

	return m[column][row];
}


/**
 *	This method implements an operator that can be used to access individual
 *	elements of this matrix.
 *
 *	@param column	The column number of the desired element. This value must be
 *					in the range [0, 3].
 *	@param row		The row number of the desired element. This value must be in
 *					the range [0, 3].
 *
 *	@return A copy of the specified element.
 */
INLINE
float Matrix::operator ()( uint32 column, uint32 row ) const
{

	return m[column][row];
}


/**
 *	This method pre multiplies this matrix by the matrix that translates by the
 *	input amount.
 *
 *	@param v	The amount to translate by.
 */
INLINE
void Matrix::preTranslateBy(const Vector3 & v)
{
	(*this)[3] += this->applyVector(v);
}


/**
 *	This method post multiplies this matrix by the matrix that translates by the
 *	input amount.
 *
 *	@param v	The amount to translate by.
 */
INLINE
void Matrix::postTranslateBy(const Vector3 & v)
{
	(*this)[3] += v;
}


/**
 *	This method multiplies the point represented by the input vector with this
 *	matrix. The input vector is on the left of the multiplication. i.e. it
 *	produces vM, where v is the input point and M is this matrix.
 *
 *	@return The resulting point represented by a Vector3.
 */
INLINE
Vector3 Matrix::applyPoint( const Vector3& v ) const
{
#ifdef EXT_MATH
	Vector3 result;
	XPVec3TransformCoord( &result, &v, this );
	return result;
#else
	return Vector3(
		v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
		v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
		v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]);
#endif
}


/**
 *	This method multiplies the point represented by the input v2 with this
 *	matrix. The input vector is on the left of the multiplication. i.e. it
 *	produces vM, where v is the input point and M is this matrix. The resulting
 *	point is placed in v.
 *
 *	@param v	The vector to place the resulting point in.
 *	@param v2	The vector representing the point to be transformed.
 */
INLINE
void Matrix::applyPoint( Vector3& v, const Vector3& v2) const
{
#ifdef EXT_MATH
	XPVec3TransformCoord( &v, &v2, this );
#else
	v.x = v2.x * m[0][0] + v2.y * m[1][0] + v2.z * m[2][0] + m[3][0];
	v.y = v2.x * m[0][1] + v2.y * m[1][1] + v2.z * m[2][1] + m[3][1];
	v.z = v2.x * m[0][2] + v2.y * m[1][2] + v2.z * m[2][2] + m[3][2];
#endif
}


/**
 *	This method applies this matrix to an input Vector3 and produces a resulting
 *	Vector4.
 *
 *	@param v1	The Vector4 that will be set to the result.
 *	@param v2	The Vector3 that the matrix will be applied to.
 */
INLINE
void Matrix::applyPoint( Vector4& v1, const Vector3& v2 ) const
{
#ifdef EXT_MATH
	XPVec3Transform( &v1, &v2, this );
#else
#if defined( CODE_INLINE ) && defined( _WIN32 ) && !defined( __BORLANDC__ )
	// c-style cast used to fix C++/asm problem
	float *pM = (float *)m;

	_asm
	{
		push eax
		push ebx

		mov eax, v2
		mov ebx, pM	// use to read: 'mov ebx, m_' where m_ is a private member

		fld dword ptr [eax+8]
		fld dword ptr [eax+4]
		fld dword ptr [eax]
		//first row
		fld st
		fmul dword ptr [ebx]

		fld st(1)
		fmul dword ptr [ebx+4]

		fld st(2)
		fmul dword ptr [ebx+8]

		fxch st(3)
		fmul dword ptr [ebx+12]
		//second row
		fld st(4)
		fmul dword ptr [ebx+16]
		faddp st(3),st

		fld st(4)
		fmul dword ptr [ebx+20]
		faddp st(2),st

		fld st(4)
		fmul dword ptr [ebx+24]
		faddp st(4),st

		fxch st(4)
		fmul dword ptr [ebx+28]
		faddp st(4),st
		//third row
		fld st(4)
		fmul dword ptr [ebx+32]
		faddp st(2),st

		fld st(4)
		fmul dword ptr [ebx+36]
		faddp st(1),st

		fld st(4)
		fmul dword ptr [ebx+40]
		faddp st(3),st

		fxch st(4)
		fmul dword ptr [ebx+44]
		faddp st(3),st
		//fourth row
		fadd dword ptr [ebx+48]
		fxch st(3)
		fadd dword ptr [ebx+52]
		fxch st(1)
		fadd dword ptr [ebx+56]
		fxch st(2)
		fadd dword ptr [ebx+60]

		mov eax, v1

		fstp dword ptr [eax+12]
		fstp dword ptr [eax+4]
		fstp dword ptr [eax+8]
		fstp dword ptr [eax]

		pop ebx
		pop eax
	}

#else
	v1[0] = v2[0] * m[0][0] + v2[1] * m[1][0] + v2[2] * m[2][0] + m[3][0];
	v1[1] = v2[0] * m[0][1] + v2[1] * m[1][1] + v2[2] * m[2][1] + m[3][1];
	v1[2] = v2[0] * m[0][2] + v2[1] * m[1][2] + v2[2] * m[2][2] + m[3][2];
	v1[3] = v2[0] * m[0][3] + v2[1] * m[1][3] + v2[2] * m[2][3] + m[3][3];
#endif

#endif // EXT_MATH
}


/**
 *	This method applies this matrix to an input Vector4 and produces a resulting
 *	Vector4.
 *
 *	@param v1	The Vector4 that will be set to the result.
 *	@param v2	The Vector4 that the matrix will be applied to.
 */
INLINE
void Matrix::applyPoint( Vector4& v1, const Vector4& v2 ) const
{
#ifdef EXT_MATH
	XPVec4Transform( &v1, &v2, this );
#else
	v1[0] = v2[0] * m[0][0] + v2[1] * m[1][0] + v2[2] * m[2][0] + v2[3] * m[3][0];
	v1[1] = v2[0] * m[0][1] + v2[1] * m[1][1] + v2[2] * m[2][1] + v2[3] * m[3][1];
	v1[2] = v2[0] * m[0][2] + v2[1] * m[1][2] + v2[2] * m[2][2] + v2[3] * m[3][2];
	v1[3] = v2[0] * m[0][3] + v2[1] * m[1][3] + v2[2] * m[2][3] + v2[3] * m[3][3];
#endif
}


/**
 *	This method applies the transform to a vector. It is the same as apply point
 *	except that it does not add the translation. The v is on the left of the
 *	multiplication. i.e. it produces vM, where M is this matrix.
 *
 *	@see applyPoint
 */
INLINE
Vector3 Matrix::applyVector( const Vector3& v ) const
{
#ifdef EXT_MATH
	Vector3 vout;
	XPVec3TransformNormal( &vout, &v, this );
	return vout;
#else
	return Vector3(
		v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
		v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
		v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]);
#endif
}

/**
 *	This method applies the transform to a vector. It is the same as apply point
 *	except that it does not add the translation. The v is on the left of the
 *	multiplication. i.e. it produces vM, where M is this matrix.
 *
 *	@param v	The vector that is set to the resulting vector.
 *	@param v2	The vector that is to be transformed.
 *
 *	@see applyPoint
 */
INLINE
void Matrix::applyVector( Vector3& v, const Vector3& v2 ) const
{
#ifdef EXT_MATH
	XPVec3TransformNormal( &v, &v2, this );
#else
	v.set(
		v2.x * m[0][0] + v2.y * m[1][0] + v2.z * m[2][0],
		v2.x * m[0][1] + v2.y * m[1][1] + v2.z * m[2][1],
		v2.x * m[0][2] + v2.y * m[1][2] + v2.z * m[2][2]);
#endif
}


/**
 *	This method returns the vector that would result from applying the transform
 *	to the unit vector along the input axis. This corresponds to one of the
 *	columns of the matrix.
 */
INLINE
const Vector3 & Matrix::applyToUnitAxisVector(int axis) const
{
	return *reinterpret_cast<const Vector3 *>( m[axis] );
}


/**
 *	This method returns the point that would result from applying the transform
 *	to the point at the origin. This is the same as the translation part of the
 *	matrix.
 */
INLINE
const Vector3 & Matrix::applyToOrigin() const
{
	return (*this)[3];
}


/**
 *	This method returns a row of the matrix as a Vector4. The rows are indexed
 *	from 0 to 3.
 *
 *	@param i	The index of the desired row.
 *
 *	@return The row with the input index.
 */
INLINE
Vector4 & Matrix::row( int i )
{
#ifndef __BORLANDC__
	MF_ASSERT_DEBUG( 0 <= i && i < 4 );
#endif
	return *reinterpret_cast<Vector4 *>( m[i] );
}


/**
 *	This method returns a row of the matrix as a Vector4. The rows are indexed
 *	from 0 to 3.
 *
 *	@param i	The index of the desired row.
 *
 *	@return The row with the input index.
 */
INLINE
const Vector4 & Matrix::row( int i ) const
{
#ifndef __BORLANDC__
	MF_ASSERT_DEBUG( 0 <= i && i < 4 );
#endif
	return *reinterpret_cast< const Vector4 * >( m[i] );
}


/**
 *	This method returns a row of the matrix as a Vector3. The rows are indexed
 *	from 0 to 3.
 *
 *	@param i	The index of the desired row.
 *
 *	@return The row with the input index.
 */
INLINE
Vector3 & Matrix::operator []( int i )
{
#ifndef __BORLANDC__
	MF_ASSERT_DEBUG( 0 <= i && i < 4 );
#endif
	return *reinterpret_cast<Vector3 *>( m[i] );
}


/**
 *	This method returns a row of the matrix as a Vector3. The rows are indexed
 *	from 0 to 3.
 *
 *	@param i	The index of the desired row.
 *
 *	@return The row with the input index.
 */
INLINE
const Vector3 & Matrix::operator []( int i ) const
{
#ifndef __BORLANDC__
	MF_ASSERT_DEBUG( 0 <= i && i < 4 );
#endif
	return *reinterpret_cast< const Vector3 * >( m[i] );
}


/**
 *	This method sets this matrix to a scaling matrix. It does not contain any
 *	rotation and translation.
 *
 *	@param	scale	The amount to scale in each coordinate.
 */
INLINE
void Matrix::setScale( const Vector3 & scale )
{
	this->setScale( scale.x, scale.y, scale.z );
}


/**
 *	This method sets this matrix to be a translation matrix. It contains no
 *	rotation or scaling.
 *
 *	@param pos	The amount to translate by.
 */
INLINE
void Matrix::setTranslate( const Vector3 & pos )
{
	this->setTranslate( pos.x, pos.y, pos.z );
}


/**
 *	This method inverts this matrix. It only works on matrices that are
 *	orthonormal. That is, the axis vectors must be orthogonal and have unit
 *	length. This matrix is set to the result.
 */
INLINE
void Matrix::invertOrthonormal()
{
	const Matrix m( *this );
	this->invertOrthonormal( m );
}


/**
 *	This method inverts this matrix. This matrix is set to the result.
 *
 *	@warning	The determinant of this matrix should not be 0.
 */
INLINE
void Matrix::invert()
{
#ifdef EXT_MATH
	XPMatrixInverse( this, NULL, this );
#else
	const Matrix m( *this );
	this->invert( m );
#endif
}


/**
 *	This method transposes this matrix
 */
INLINE
void Matrix::transpose()
{
#ifdef EXT_MATH
	XPMatrixTranspose( this, this );
#else
	const Matrix m( *this );
	this->transpose( m );
#endif
}



/**
 *	This method sets this matrix to a left-handed orthogonal projection matrix.
 */
INLINE
void Matrix::orthogonalProjection( float w, float h, float zn, float zf )
{
#ifdef EXT_MATH
	XPMatrixOrthoLH( this, w, h, zn, zf );
#else
	this->row(0) = Vector4( 2.f/w,	0.f,	0.f,			0.f );
	this->row(1) = Vector4( 0.f,	2.f/h,	0.f,			0.f );
	this->row(2) = Vector4( 0.f,	0.f,	1.f/(zf-zn),	0.f );
	this->row(3) = Vector4( 0.f,	0.f,	zn/(zn-zf),		1.f );	
#endif
}


/**
 *	This method sets this matrix to a left-handed perspective projection matrix.
 */
INLINE
void Matrix::perspectiveProjection( float fov, float aspectRatio,
	float nearPlane, float farPlane )
{
#ifdef EXT_MATH
	XPMatrixPerspectiveFovLH( this, fov, aspectRatio, nearPlane, farPlane );
#else
	float cot = 1 / (float)tan(fov * 0.5f);
	float rcp = 1 / (farPlane - nearPlane);

	m[0][0] = (cot / aspectRatio);
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;

	m[1][0] = 0;
	m[1][1] = cot;
	m[1][2] = 0;
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = rcp * farPlane;
	m[2][3] = 1;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = - rcp  * farPlane * nearPlane;
	m[3][3] = 0;
#endif
}


/**
 *	This method sets the translation component of this matrix.
 */
INLINE
void Matrix::translation( const Vector3& v )
{
	(*this)[3] = v;
	m[3][3] = 1;
}


/**
 *	This method sets this matrix to the identity matrix.
 */
INLINE
void Matrix::setIdentity()
{
#ifdef EXT_MATH
	XPMatrixIdentity( this );
#else
	(*this) = Matrix::identity;
#endif
}


/**
 *	Return the yaw of the rotation part of this matrix
 */
INLINE float Matrix::yaw() const
{
	Vector3 zdir = this->applyToUnitAxisVector(2);
	zdir.normalise();
#ifdef __BORLANDC__
	return atan2( zdir.x, zdir.z );
#else
	return atan2f( zdir.x, zdir.z );
#endif
}


/**
 *	Return the pitch of the rotation part of this matrix
 */
INLINE float Matrix::pitch() const
{
	Vector3 zdir = this->applyToUnitAxisVector(2);
	zdir.normalise();
#ifdef __BORLANDC__
	return -asin( zdir.y );
#else
	return -asinf( zdir.y );
#endif
}


/**
 *	Return the roll of the rotation part of this matrix
 */
INLINE float Matrix::roll() const
{
	float roll;

	Vector3 xdir = this->applyToUnitAxisVector(0);
	Vector3 zdir = this->applyToUnitAxisVector(2);
	xdir.normalise();
	zdir.normalise();

	const float zdirxzlen = sqrtf( zdir.z*zdir.z + zdir.x*zdir.x );
	const float acarg = (zdir.z * xdir.x - zdir.x * xdir.z) / zdirxzlen;
	if (zdirxzlen == 0.f || acarg > 1.f) return 0.f;
#ifdef __BORLANDC__
	roll = acos( acarg );
#else
	roll = acosf( acarg );
#endif

	return xdir.y < 0.f ? -roll : roll;
}



// -----------------------------------------------------------------------------
// Section: DirectX inline methods
// -----------------------------------------------------------------------------

#ifdef EXT_MATH

/**
 *	This method sets this matrix to a scaling matrix. It does not contain any
 *	rotation and translation.
 *
 *	@param	x	The amount to scale the x-coordinate.
 *	@param	y	The amount to scale the y-coordinate.
 *	@param	z	The amount to scale the z-coordinate.
 */
INLINE
void Matrix::setScale( const float x, const float y, const float z )
{
	XPMatrixScaling( this, x, y, z );
}


/**
 *	This method sets this matrix to be a translation matrix. It contains no
 *	rotation or scaling.
 *
 *	@param x	The amount to translate along the x-axis.
 *	@param y	The amount to translate along the y-axis.
 *	@param z	The amount to translate along the z-axis.
 */
INLINE
void Matrix::setTranslate( const float x, const float y, const float z )
{
	XPMatrixTranslation( this, x, y, z );
}


/**
 *	This method sets this matrix to the result of multiplying the two input
 *	matrices.
 *
 *	@param m1	The left-hand side of the multiplication.
 *	@param m2	The right-hand side of the multiplication.
 */
INLINE
void Matrix::multiply( const Matrix& m1, const Matrix& m2 )
{
	XPMatrixMultiply( this, &m1, &m2 );
}


/**
 *	This method sets this matrix to the result of multiply the input matrix by
 *	this matrix. The input matrix is on the left-hand side of the multiplication
 *	and this matrix is on the right.
 *
 *	@param m	The matrix to pre-multiply this matrix by.
 */
INLINE
void Matrix::preMultiply( const Matrix& m )
{
	XPMatrixMultiply( this, &m, this );
}


/**
 *	This method sets this matrix to the result of multiply this matrix by the
 *	input matrix. The input matrix is on the right-hand side of the
 *	multiplication and this matrix is on the left.
 *
 *	@param m	The matrix to post-multiply this matrix by.
 */
INLINE
void Matrix::postMultiply( const Matrix& m )
{
	XPMatrixMultiply( this, this, &m );
}


/**
 *	This method sets this matrix to be a rotation matrix around the X axis. A
 *	positive value rotates the Y axis towards the Z axis.
 *
 *	@param angle	The angle in radians to rotate by.
 */
INLINE
void Matrix::setRotateX( const float angle )
{
	XPMatrixRotationX( this, angle );
}


/**
 *	This method sets this matrix to be a rotation matrix around the Y axis. A
 *	positive value rotates the Z axis towards the X axis.
 *
 *	@param angle	The angle in radians to rotate by.
 */
INLINE
void Matrix::setRotateY( const float angle )
{
	XPMatrixRotationY( this, angle );
}


/**
 *	This method sets this matrix to be a rotation matrix around the Z axis. A
 *	positive rotates the X axis towards the Y axis.
 *
 *	@param angle	The angle in radians to rotate by.
 */
INLINE
void Matrix::setRotateZ( const float angle )
{
	XPMatrixRotationZ( this, angle );
}


/**
 *	This method sets this matrix to be the rotation matrix equivalent to the
 *	input quaternion.
 */
INLINE
void Matrix::setRotate( const Quaternion & q )
{
	XPMatrixRotationQuaternion( this, (const QuaternionBase *)&q );
}


/**
 *	This method sets this matrix to be the rotation matrix equivalent to the
 *	input yaw, pitch and roll values.
 */
INLINE void Matrix::setRotate( float yaw, float pitch, float roll )
{
	XPMatrixRotationYawPitchRoll( this, yaw, pitch, roll );
}


/**
 *	This method inverts the input matrix. This matrix is set to the result.
 *
 *	@warning	The determinant of the input matrix should not be 0.
 */
INLINE void Matrix::invert( const Matrix & matrix )
{
	XPMatrixInverse( this, NULL, &matrix );
}


/**
 *	This method transposes the input matrix
 */
INLINE void Matrix::transpose( const Matrix & matrix )
{
	XPMatrixTranspose( this, &matrix );
}


/**
 *	This method creates a <i>look at</i> matrix from two directions and a
 *	position.
 *
 *	@param postion		The position to look from.
 *	@param direction	The direction to look.
 *	@param up			The direction of <i>"up"</i>.
 */
INLINE
void Matrix::lookAt( const Vector3& position, const Vector3& direction,
		const Vector3& up )
{
	Vector3 at = position + direction;
	XPMatrixLookAtLH( this, &position, &at, &up );
}


/**
 *	This method post multiplies this matrix by the matrix that rotates around
 *	the X axis by the input amount.
 *
 *	@param angle	The angle in radians to rotate by.
 */
INLINE
void Matrix::postRotateX( const float angle )
{
	Matrix m;
	XPMatrixRotationX( &m, angle );
	this->postMultiply( m );
}


/**
 *	This method post multiplies this matrix by the matrix that rotates around
 *	the Y axis by the input amount.
 *
 *	@param angle	The angle in radians to rotate by.
 */
INLINE
void Matrix::postRotateY(const float angle)
{
	Matrix m;
	XPMatrixRotationY( &m, angle );
	this->postMultiply( m );
}


/**
 *	This method post multiplies this matrix by the matrix that rotates around
 *	the Z axis by the input amount.
 *
 *	@param angle	The angle in radians to rotate by.
 */
INLINE
void Matrix::postRotateZ(const float angle)
{
	Matrix m;
	XPMatrixRotationZ( &m, angle );
	this->postMultiply( m );
}


/**
 *	This method pre multiplies this matrix by the matrix that rotates around
 *	the X axis by the input amount.
 *
 *	@param angle	The angle in radians to rotate by.
 */
INLINE
void Matrix::preRotateX(const float angle)
{
	Matrix m;
	XPMatrixRotationX( &m, angle );
	this->preMultiply( m );
}


/**
 *	This method pre multiplies this matrix by the matrix that rotates around
 *	the Y axis by the input amount.
 *
 *	@param angle	The angle in radians to rotate by.
 */
INLINE
void Matrix::preRotateY(const float angle)
{
	Matrix m;
	XPMatrixRotationY( &m, angle );
	this->preMultiply( m );
}


/**
 *	This method pre multiplies this matrix by the matrix that rotates around
 *	the Y axis by the input amount.
 *
 *	@param angle	The angle in radians to rotate by.
 */
INLINE
void Matrix::preRotateZ(const float angle)
{
	Matrix m;
	XPMatrixRotationZ( &m, angle );
	this->preMultiply( m );
}

#endif // EXT_MATH

// matrix.ipp
