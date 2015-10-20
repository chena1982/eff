/******************************************************************************
	created:	2008-12-13   0:48
	file path:	d:\EFF\EFFEngine\Src\EFFMath\EFFMatrix4.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/
#include "EFFMathPCH.h"

#include "EFFMathFunction.h"
#include "EFFVector3.h"
#include "EFFVector4.h"
#include "EFFMatrix4.h"
#include "D3dx9math.h"
//#define new EFFNEW

EFFMATH_BEGIN



/*inline EFFMatrix4::EFFMatrix4(const EFFQuaternion & q)
{

	effFLOAT xx2 = q.x * q.x * 2.0f;
	effFLOAT yy2 = q.y * q.y * 2.0f;
	effFLOAT zz2 = q.z * q.z * 2.0f;
	effFLOAT xy2 = q.x * q.y * 2.0f;
	effFLOAT zw2 = q.z * q.w * 2.0f;
	effFLOAT xz2 = q.x * q.z * 2.0f;
	effFLOAT yw2 = q.y * q.w * 2.0f;
	effFLOAT yz2 = q.y * q.z * 2.0f;
	effFLOAT xw2 = q.x * q.w * 2.0f;

	m11 = 1.0f - yy2 - zz2;
	m12 = xy2 + zw2;
	m13 = xz2 - yw2;

	m21 = xy2 - zw2;
	m22 = 1.0f - xx2 - zz2;
	m23 = yz2 + xw2;

	m31 = xz2 + yw2;
	m32 = yz2 - xw2;
	m33 = 1.0f - xx2 - yy2;

	m14 = m24 = m34 = m41 = m42 = m43 = 0.0f;
	m44 = 1.0f;

	EFFMatrix4FromQuaternion(this, q);
}*/

inline effVOID EFFMatrix4::GetRow(const effINT r, EFFVector4 & p) const
{
	p.x = m[r][0];
	p.y = m[r][1];
	p.z = m[r][2];
	p.w = m[r][3];
}

inline effVOID EFFMatrix4::GetRow(const effINT r, EFFVector3 & p) const
{
	p.x = m[r][0];
	p.y = m[r][1];
	p.z = m[r][2];
}

inline effVOID EFFMatrix4::SetRow(const effINT r, const EFFVector4 & p)
{
	m[r][0] = p.x;
	m[r][1] = p.y;
	m[r][2] = p.z;
	m[r][3] = p.w;
}

inline effVOID EFFMatrix4::SetRow(const effINT r, const EFFVector3 & p)
{
	m[r][0] = p.x;
	m[r][1] = p.y;
	m[r][2] = p.z;
	m[r][3] = (r != 3) ? 0.0f : 1.0f;
}

inline effVOID EFFMatrix4::GetCol(const effINT c, EFFVector4 & p) const
{
	p.x = m[0][c];
	p.y = m[1][c];
	p.z = m[2][c];
	p.w = m[3][c];
}

inline effVOID EFFMatrix4::GetCol(const effINT c, EFFVector3 & p) const
{
	p.x = m[0][c];
	p.y = m[1][c];
	p.z = m[2][c];
}

inline effVOID EFFMatrix4::SetCol(const effINT c, const EFFVector4 & p)
{
	m[0][c] = p.x;
	m[1][c] = p.y;
	m[2][c] = p.z;
	m[3][c] = p.w;
}

inline effVOID EFFMatrix4::SetCol(const effINT c, const EFFVector3 & p)
{
	m[0][c] = p.x;
	m[1][c] = p.y;
	m[2][c] = p.z;
	m[3][c] = (c != 3) ? 0.0f : 1.0f;
}

inline effVOID EFFMatrix4::GetTrans(EFFVector3 & p) const
{
	p.x = m[3][0];
	p.y = m[3][1];
	p.z = m[3][2];
}

inline effVOID EFFMatrix4::SetTrans(const EFFVector3 & p)
{
	m[3][0] = p.x;
	m[3][1] = p.y;
	m[3][2] = p.z;
}

inline effVOID EFFMatrix4::Trans(const EFFVector3 & p)
{
	m[3][0] += p.x;
	m[3][1] += p.y;
	m[3][2] += p.z;
}

inline effVOID EFFMatrix4::SetTrans(const EFFVector4 & p)
{
	m[3][0] = p.x;
	m[3][1] = p.y;
	m[3][2] = p.z;
	m[3][3] = p.w;
}

inline EFFMatrix4 EFFMatrix4::operator * (const EFFMatrix4 & m) const
{
	EFFMatrix4 temp;
	EFFMatrix4Mul(&temp, this, &m);
	return temp;
}

inline EFFMatrix4 EFFMatrix4::operator + (const EFFMatrix4 & m) const
{
	EFFMatrix4 temp;
	EFFMatrix4Add(&temp, this, &m);
	return temp;
}

inline EFFMatrix4 EFFMatrix4::operator - (const EFFMatrix4 & m) const
{
	EFFMatrix4 temp;
	EFFMatrix4Sub(&temp, this, &m);
	return temp;
}

inline EFFMatrix4& EFFMatrix4::operator *= (const EFFMatrix4 & m)
{
	EFFMatrix4 temp = *this;
	return *(EFFMatrix4 *)EFFMatrix4Mul(this, &temp, &m);
}

inline EFFMatrix4& EFFMatrix4::operator += (const EFFMatrix4 & m)
{
	return *(EFFMatrix4 *)EFFMatrix4Add(this, this, &m);
}

inline EFFMatrix4& EFFMatrix4::operator -= (const EFFMatrix4 & m)
{
	return *(EFFMatrix4 *)EFFMatrix4Sub(this, this, &m);
}

inline EFFMatrix4 EFFMatrix4::operator * (effFLOAT f) const
{
	EFFMatrix4 m;
	EFFMatrix4Mul(&m, this, f);
	return m;
}

inline EFFMatrix4 EFFMatrix4::operator / (effFLOAT f) const
{
	assert(f != 0.0f);
	EFFMatrix4 m;
	EFFMatrix4Mul(&m, this, 1.0f / f);
	return m;
}

inline EFFMatrix4 & EFFMatrix4::operator *= (effFLOAT f)
{
	return *(EFFMatrix4 *)EFFMatrix4Mul(this, this, f);
}

inline EFFMatrix4& EFFMatrix4::operator /= (effFLOAT f)
{
	return *(EFFMatrix4 *)EFFMatrix4Mul(this, this, 1.0f / f);
}

inline EFFMatrix4 operator *(effFLOAT f, const EFFMatrix4 & m)
{ 
	return m.operator * (f); 
}


inline effVOID EFFMatrix4::ZeroMatrix()
{
	memset(m, 0, sizeof(EFFMatrix4));
}

//得到单位矩阵
inline effVOID EFFMatrix4::Identity()
{
	m12 = m13 = m14 =
	m21 = m23 = m24 =
	m31 = m32 = m34 =
	m41 = m42 = m43 = 0.0f;
	m11 = m22 = m33 = m44 = 1.0f;
}

//得到平移矩阵
inline effVOID EFFMatrix4::MoveMatrix(effFLOAT x, effFLOAT y, effFLOAT z)
{
	m12 = m13 = m14 =
	m21 = m23 = m24 =
	m31 = m32 = m34 = 0.0f;
	m11 = m22 = m33 = m44 = 1.0f;

	m41 = x, m42 = y, m43 = z;
}



inline effVOID EFFMatrix4::ScaleMatrix(effFLOAT x, effFLOAT y, effFLOAT z)
{
	m12 = m13 = m14 =
	m21 = m23 = m24 =
	m31 = m32 = m34 =
	m41 = m42 = m43 = 0.0f;

	m11 = x, m22 = y, m33 = z, m44 = 1.0f;
}

inline effVOID EFFMatrix4::RotationMatrixX(effFLOAT angle)
{
	m12 = m13 = m14 = 
	m21 = m24 = 
	m31 = m34 = 
	m41 = m42 = m43 = 0.0f;

	m11 = m44 = 1.0f;

	m22 = m33 = cosf(angle);
	m32 = sinf(angle);
	m23 = -m32;

	/*EFFMatrix4 result;
	D3DXMatrixRotationX((D3DXMATRIX *)&result, angle);*/
}


inline effVOID EFFMatrix4::RotationMatrixY(effFLOAT angle)
{
	m12 = m14 = 
	m21 = m23 = m24 = 0.0f;
	m32 = m34 = 
	m41 = m42 = m43 = 0.0f;     

	m22 = m44 = 1.0f;

	m11 = m33 = cosf(angle);
	m31 = sinf(angle); 
	m13 = -m31;
}


inline effVOID EFFMatrix4::RotationMatrixZ(effFLOAT angle)
{
	m13 = m14 = 
	m23 = m24 = 
	m31 = m32 = m34 = 
	m41 = m42 = m43 = 0.0f;

	m33 = m44 = 1.0f;

	m11 = m22 = cosf(angle);
	m12 = sinf(angle); 
	m21 = -m12;
}

inline effVOID EFFMatrix4::RotationAxis(const EFFVector3 * axis, effFLOAT angle)
{
	//D3DXMatrixRotationAxis((D3DXMATRIX *)this, (D3DXVECTOR3 *)axis, angle);

	/*EFFQuaternion quaternion;
	effFLOAT halfAngle = angle * 0.5f;
	quaternion.x = axis->x * sinf(halfAngle);
	quaternion.y = axis->y * sinf(halfAngle);
	quaternion.z = axis->z * sinf(halfAngle);
	quaternion.w = cosf(halfAngle);*/

	//this = quaternion;
}

inline effVOID EFFMatrix4::Transpose()
{
	EFFMatrix4 temp;
	memcpy(&temp, this, sizeof(EFFMatrix4));

	m12 = temp.m21;
	m13 = temp.m31;
	m14 = temp.m41;

	m21 = temp.m12;
	m23 = temp.m32;
	m24 = temp.m42;

	m31 = temp.m13;
	m32 = temp.m23;
	m34 = temp.m43;

	m41 = temp.m14;
	m42 = temp.m24;
	m43 = temp.m34;
}

inline effVOID EFFMatrix4::Transpose(EFFMatrix4 & transpose) const
{
	transpose.m12 = m21;
	transpose.m13 = m31;
	transpose.m14 = m41;

	transpose.m21 = m12;
	transpose.m23 = m32;
	transpose.m24 = m24;

	transpose.m31 = m13;
	transpose.m32 = m23;
	transpose.m34 = m43;

	transpose.m41 = m14;
	transpose.m42 = m24;
	transpose.m43 = m34;
}

/*------------------------------------------------------------------------------------------------
*	Inverts a PR matrix. (which only contains a rotation and a translation)
*	This is faster and less subject to FPU errors than the generic inversion code.
*
* param dest [out] destination matrix
* param src [in] source matrix
------------------------------------------------------------------------------------------------*/
effVOID EFFMatrix4InvertPR(EFFMatrix4 & dest, const EFFMatrix4 & src)
{
	dest.m[0][0] = src.m[0][0];
	dest.m[1][0] = src.m[0][1];
	dest.m[2][0] = src.m[0][2];
	dest.m[3][0] = -(src.m[3][0] * src.m[0][0] + src.m[3][1] * src.m[0][1] + src.m[3][2] * src.m[0][2]);

	dest.m[0][1] = src.m[1][0];
	dest.m[1][1] = src.m[1][1];
	dest.m[2][1] = src.m[1][2];
	dest.m[3][1] = -(src.m[3][0] * src.m[1][0] + src.m[3][1] * src.m[1][1] + src.m[3][2] * src.m[1][2]);

	dest.m[0][2] = src.m[2][0];
	dest.m[1][2] = src.m[2][1];
	dest.m[2][2] = src.m[2][2];
	dest.m[3][2] = -(src.m[3][0] * src.m[2][0] + src.m[3][1] * src.m[2][1] + src.m[3][2] * src.m[2][2]);

	dest.m[0][3] = 0.0f;
	dest.m[1][3] = 0.0f;
	dest.m[2][3] = 0.0f;
	dest.m[3][3] = 1.0f;
}

/*------------------------------------------------------------------------------------------------
* Compute the cofactor of the Matrix at a specified location
------------------------------------------------------------------------------------------------*/
effFLOAT EFFMatrix4::CoFactor(effINT row, effINT col) const
{
	return ( (m[(row+1)&3][(col+1)&3] * m[(row+2)&3][(col+2)&3] * m[(row+3)&3][(col+3)&3] +
			  m[(row+1)&3][(col+2)&3] * m[(row+2)&3][(col+3)&3] * m[(row+3)&3][(col+1)&3] +
			  m[(row+1)&3][(col+3)&3] * m[(row+2)&3][(col+1)&3] * m[(row+3)&3][(col+2)&3] ) -
			 (m[(row+3)&3][(col+1)&3] * m[(row+2)&3][(col+2)&3] * m[(row+1)&3][(col+3)&3] +
			  m[(row+3)&3][(col+2)&3] * m[(row+2)&3][(col+3)&3] * m[(row+1)&3][(col+1)&3] +
			  m[(row+3)&3][(col+3)&3] * m[(row+2)&3][(col+1)&3] * m[(row+1)&3][(col+2)&3]) ) * ((row + col) & 1 ? -1.0f : +1.0f);
}

effFLOAT EFFMatrix4::Determinant() const
{
	return m[0][0] * CoFactor(0, 0) +
		   m[0][1] * CoFactor(0, 1) +
		   m[0][2] * CoFactor(0, 2) +
		   m[0][3] * CoFactor(0, 3);
}

effVOID EFFMatrix4Invert(EFFMatrix4 & out, effFLOAT * determinant, const EFFMatrix4 & in)
{
	effFLOAT det = in.Determinant();
	if ( determinant != NULL )
	{
		*determinant = det;
	}

	EFFMatrix4 temp;
	if ( fabsf(det) < EFFMATH_EPSILON )
	{
		out = in;

		// The matrix is not invertible! Singular case!
		return;
	}

	effFLOAT IDet = 1.0f / det;

	temp.m[0][0] = in.CoFactor(0, 0) * IDet;
	temp.m[1][0] = in.CoFactor(0, 1) * IDet;
	temp.m[2][0] = in.CoFactor(0, 2) * IDet;
	temp.m[3][0] = in.CoFactor(0, 3) * IDet;
	temp.m[0][1] = in.CoFactor(1, 0) * IDet;
	temp.m[1][1] = in.CoFactor(1, 1) * IDet;
	temp.m[2][1] = in.CoFactor(1, 2) * IDet;
	temp.m[3][1] = in.CoFactor(1, 3) * IDet;
	temp.m[0][2] = in.CoFactor(2, 0) * IDet;
	temp.m[1][2] = in.CoFactor(2, 1) * IDet;
	temp.m[2][2] = in.CoFactor(2, 2) * IDet;
	temp.m[3][2] = in.CoFactor(2, 3) * IDet;
	temp.m[0][3] = in.CoFactor(3, 0) * IDet;
	temp.m[1][3] = in.CoFactor(3, 1) * IDet;
	temp.m[2][3] = in.CoFactor(3, 2) * IDet;
	temp.m[3][3] = in.CoFactor(3, 3) * IDet;

	out = temp;

	return;
}

/*effVOID EFFMatrix4Decompose(EFFVector3 * scale, EFFVector3 * trans, EFFQuaternion * rotation, const EFFMatrix4 * m)
{

	m->GetTrans(*trans);

	scale->x = sqrtf(m->m11 * m->m11 + m->m12 * m->m12 + m->m13 * m->m13);
	scale->y = sqrtf(m->m21 * m->m21 + m->m22 * m->m22 + m->m23 * m->m23);
	scale->z = sqrtf(m->m31 * m->m31 + m->m32 * m->m32 + m->m33 * m->m33);

	EFFMatrix4 mat = *m;	
	mat.m11 /= scale->x;
	mat.m12 /= scale->x;
	mat.m13 /= scale->x;

	mat.m21 /= scale->y;
	mat.m22 /= scale->y;
	mat.m23 /= scale->y;

	mat.m31 /= scale->z;
	mat.m32 /= scale->z;
	mat.m33 /= scale->z;

	EFFMatrix4ToQuat(&mat, rotation);

	//uc3dxQuaternion qD3D;
	//EFFVector3 vD3D,vScaleD3D;
	//D3DXMatrixDecompose((D3DXVECTOR3 *)&vScaleD3D,(D3DXQUATERNION *)&qD3D,(D3DXVECTOR3 *)&vD3D,(const D3DXMATRIX *)pM);
}*/

EFFMatrix4 EFFMatrix4CubeVeiw(effINT face)
{
	switch(face)
	{
	case 0://POSITIVE_X
 		return EFFMatrix4(
			0, 0, 1, 0,
			0, 1, 0, 0,
			-1, 0, 0, 0,
			0, 0, 0, 1);
	case 1://NEGATIVE_X
		return EFFMatrix4(
			0, 0, -1, 0,
			0, 1, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 1);
	case 2://POSITIVE_Y
		return EFFMatrix4(
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, -1, 0, 0,
			0, 0, 0, 1);
	case 3://NEGATIVE_Y
		return EFFMatrix4(
			1, 0, 0, 0,
			0, 0, -1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1);
	case 4://POSITIVE_Z
		return EFFMatrix4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	default://NEGATIVE_Z
		return EFFMatrix4(
			-1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, -1, 0,
			0, 0, 0, 1);
	}
}

/*inline EFFMatrix4 * EFFMatrix4FromQuaternion(EFFMatrix4 * out, const EFFQuaternion * q)
{
	effFLOAT xx = q->x * q->x;
	effFLOAT xy = q->x * q->y;
	effFLOAT xz = q->x * q->z;
	effFLOAT xw = q->x * q->w;
	effFLOAT yy = q->y * q->y;
	effFLOAT yz = q->y * q->z;
	effFLOAT yw = q->y * q->w;
	effFLOAT zz = q->z * q->z;
	effFLOAT zw = q->z * q->w;

	out->m11 = 1.0f - 2.0f * (yy + zz);
	out->m12 = 2.0f * (xy + zw);
	out->m13 = 2.0f * (xz - yw);
	out->m21 = 2.0f * (xy - zw);
	out->m22 = 1.0f - 2.0f * (xx + zz);
	out->m23 = 2.0f * (yz + xw);
	out->m31 = 2.0f * (xz + yw);
	out->m32 = 2.0f * (yz - xw);
	out->m33 = 1.0f - 2.0f * (xx + yy);

	out->m41 = out->m42 = out->m43 = out->m14 = out->m24 = out->m34 = 0.0f;
	out->m44 = 1.0f;

	//EFFMatrix4 matD3D;
	//D3DXMatrixRotationQuaternion((D3DXMATRIX *)&matD3D,(const D3DXQUATERNION *)pQ);
	return out;
}*/

EFFMatrix4 * EFFMatrixPerspectiveFovLH(EFFMatrix4 * out, effFLOAT fovy, effFLOAT aspect, effFLOAT zn, effFLOAT zf)
{
	/*
	xScale     0          0               0
	0        yScale       0               0
	0          0       zf/(zf-zn)         1
	0          0       -zn*zf/(zf-zn)     0
	where:
	yScale = cot(fovY/2)

	xScale = yScale / aspect ratio

	*/

	effFLOAT yScale = 1.0f / tanf(fovy * 0.5f);
	effFLOAT xScale = yScale / aspect;

	out->m11 = xScale;
	out->m12 = 0.f;
	out->m13 = 0.f;
	out->m14 = 0.f;

	out->m21 = 0.f;
	out->m22 = yScale;
	out->m23 = 0.f;
	out->m24 = 0.f;

	out->m31 = 0.f;
	out->m32 = 0.f;
	out->m33 = zf / (zf - zn);
	out->m34 = 1.f;

	out->m41 = 0.f;
	out->m42 = 0.f;
	out->m43 = zn * zf / (zn - zf);
	out->m44 = 0.f;

	return out;
}

EFFMatrix4 * EFFMatrixLookAtLH(EFFMatrix4 * out, const EFFVector3 * eye, const EFFVector3 * lookAt, const EFFVector3 * up)
{
	/*zaxis = normal(At - Eye)
	xaxis = normal(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)

	xaxis.x           yaxis.x           zaxis.x          0
	xaxis.y           yaxis.y           zaxis.y          0
	xaxis.z           yaxis.z           zaxis.z          0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye) 1.f*/

	EFFVector3 zaxis;
	EFFVector3Sub(&zaxis, lookAt, eye);

	//EFFVector3 z;
	//D3DXVec3Normalize((D3DXVECTOR3 *)&z, (const D3DXVECTOR3 *)&zaxis);

	zaxis.Normalize();



	EFFVector3 xaxis;
	EFFVector3Cross(&xaxis, up, &zaxis);

	//EFFVector3 x;
	//D3DXVec3Cross((D3DXVECTOR3 *)&x, (const D3DXVECTOR3 *)up, (const D3DXVECTOR3 *)&zaxis);
	xaxis.Normalize();

	EFFVector3 yaxis;
	EFFVector3Cross(&yaxis, &zaxis, &xaxis);

	//EFFVector3 y;
	//D3DXVec3Cross((D3DXVECTOR3 *)&y, (const D3DXVECTOR3 *)&zaxis, (const D3DXVECTOR3 *)&xaxis);

	out->m11 = xaxis.x;
	out->m12 = yaxis.x;
	out->m13 = zaxis.x;
	out->m14 = 0.f;

	out->m21 = xaxis.y;
	out->m22 = yaxis.y;
	out->m23 = zaxis.y;
	out->m24 = 0.f;

	out->m31 = xaxis.z;
	out->m32 = yaxis.z;
	out->m33 = zaxis.z;
	out->m34 = 0.f;

	out->m41 = -1.f * EFFVector3Dot(&xaxis, eye);
	out->m42 = -1.f * EFFVector3Dot(&yaxis, eye);
	out->m43 = -1.f * EFFVector3Dot(&zaxis, eye);
	out->m44 = 1.f;


	return out;
}


EFFMATH_END