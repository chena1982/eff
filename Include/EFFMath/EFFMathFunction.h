/******************************************************************************
	created:	2013-02-02   23:33
	file path:	d:\EFF\EFFEngine\Include\EFFMathFunction.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMath_2013_02_02__
#define __EFFMath_2013_02_02__


EFFMATH_BEGIN

inline effFLOAT frand()
{
	return rand() / (effFLOAT)RAND_MAX;
}

inline effFLOAT randFloat(effFLOAT lower, effFLOAT upper)
{
	return lower + (upper - lower) * (rand() / (effFLOAT)RAND_MAX);
}

inline effINT randInt(effINT lower, effINT upper)
{
	return lower + (effINT)((upper + 1 - lower) * frand());
}

//左手坐标系换成右手坐标系
inline effVOID EFFProjLHtoRH(effFLOAT * d, const effFLOAT * s)
{
	d[0] = -s[0];
	d[1] = -s[1];
	d[2] = -s[2];
	d[3] = -s[3];
	
	d[4] = s[4];
	d[5] = s[5];
	d[6] = s[6];
	d[7] = s[7];
	
	d[8] = -s[8];
	d[9] = -s[9];
	d[10] = -s[10];
	d[11] = -s[11];
	
	d[12] = s[12];
	d[13] = s[13];
	d[14] = s[14];
	d[15] = s[15];
}

//右手坐标系换成左手坐标系
inline void EFFViewLHtoRH(effFLOAT *d, const effFLOAT *s)
{
	d[0] = -s[0];
	d[1] = s[1];
	d[2] = -s[2];
	d[3] = s[3];

	d[4] = -s[4];
	d[5] = s[5];
	d[6] = -s[6];
	d[7] = s[7];

	d[8] = -s[8];
	d[9] = s[9];
	d[10] = -s[10];
	d[11] = s[11];

	d[12] = -s[12];
	d[13] = s[13];
	d[14] = -s[14];
	d[15] = s[15];
}

//2d向量加
inline EFFVECTOR2 * EFFVector2Add(EFFVECTOR2 * out, const EFFVECTOR2 * p1, const EFFVECTOR2 * p2)
{
	out->x = p1->x + p2->x;
	out->y = p1->y + p2->y;
	return out;
}

//2d向量减
inline EFFVECTOR2 * EFFVector2Sub(EFFVECTOR2 * out, const EFFVECTOR2 * p1, const EFFVECTOR2 * p2)
{
	out->x = p1->x - p2->x;
	out->y = p1->y - p2->y;
	return out;
}

//2d向量乘
inline EFFVECTOR2 * EFFVector2Mul(EFFVECTOR2 * out, effFLOAT f, const EFFVECTOR2 * p)
{
	out->x = f * p->x;
	out->y = f * p->y;
	return out;
}

//2d向量规格化
inline EFFVECTOR2 * EFFVector2Normalize(EFFVECTOR2 * out, const EFFVECTOR2 * p)
{
	if ( p->x == 0.0f && p->y == 0.0f )
	{
		out->x = 0.0f;
		out->y = 0.0f;
		return out;
	}

	effFLOAT length = sqrtf(p->x * p->x + p->y * p->y);
	out->x = p->x / length;
	out->y = p->y / length;
	return out;
}

//2d向量长度
inline effFLOAT EFFVector2Length(const EFFVECTOR2 * p)
{
	return sqrtf(p->x * p->x + p->y * p->y);
}

//2d向量长度平方
inline effFLOAT EFFVector2LengthSq(const EFFVECTOR2 * p)
{
	return (p->x * p->x + p->y * p->y);
}

//求两个3d向量的点积
inline effFLOAT EFFVector3Dot(const EFFVECTOR3 * v1, const EFFVECTOR3 * v2)
{ 
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z; 
}

//求两个3d向量的叉积
inline EFFVECTOR3 * EFFVector3Cross(EFFVECTOR3 * out, const EFFVECTOR3 * p1, const EFFVECTOR3* p2)
{
	out->x = p1->y * p2->z - p1->z * p2->y;
	out->y = p1->z * p2->x - p1->x * p2->z;
	out->z = p1->x * p2->y - p1->y * p2->x;
	return out;
}

//3d向量加
inline EFFVECTOR3 * EFFVector3Add(EFFVECTOR3 * out, const EFFVECTOR3 * p1, const EFFVECTOR3 * p2)
{
	out->x = p1->x + p2->x;
	out->y = p1->y + p2->y;
	out->z = p1->z + p2->z;
	return out;
}

//3d向量减
inline EFFVECTOR3 * EFFVector3Sub(EFFVECTOR3 * out, const EFFVECTOR3 * p1, const EFFVECTOR3 * p2)
{
	out->x = p1->x - p2->x;
	out->y = p1->y - p2->y;
	out->z = p1->z - p2->z;
	return out;
}

//3D向量乘
inline EFFVECTOR3 * EFFVector3Mul(EFFVECTOR3 * out, effFLOAT f, const EFFVECTOR3 * p)
{
	out->x = f * p->x;
	out->y = f * p->y;
	out->z = f * p->z;
	return out;
}

//规格化向量
inline EFFVECTOR3 * EFFVector3Normalize(EFFVECTOR3 * out, const EFFVECTOR3 * p)
{
	if ( p->x == 0.0f && p->y == 0.0f && p->z == 0.0f )
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		return out;
	}

	effFLOAT length = sqrtf(p->x * p->x + p->y * p->y + p->z * p->z);
	out->x = p->x / length;
	out->y = p->y / length;
	out->z = p->z / length;
	return out;
}

//4D向量长度平方
inline effFLOAT EFFVector4Dot(EFFVECTOR4 * v1, const EFFVECTOR4 * v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w; 
}

//4D向量规格化
inline EFFVECTOR4 * EFFVector4Normalize(EFFVECTOR4 * out, const EFFVECTOR4 * p)
{
	if ( p->x == 0.0f && p->y == 0.0f && p->z == 0.0f && p->w == 0.0f )
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->w = 0.0f;
		return out;
	}

	effFLOAT length = sqrtf(p->x * p->x + p->y * p->y + p->z * p->z + p->w * p->w);
	out->x = p->x / length;
	out->y = p->y / length;
	out->z = p->z / length;
	out->w = p->w / length;
	return out;
}

//3D向量长度
inline effFLOAT EFFVector3Length(const EFFVECTOR3 * p)
{

	return sqrtf(p->x * p->x + p->y * p->y + p->z * p->z);
}

//3D向量长度平方
inline effFLOAT EFFVector3LengthSq(const EFFVECTOR3 * p)
{
	return (p->x * p->x + p->y * p->y + p->z * p->z);
}

//矩阵乘法
inline EFFMATRIX4 * EFFMatrix4Mul(EFFMATRIX4 * out, const EFFMATRIX4 * mat1, const EFFMATRIX4 * mat2)
{
	EFFMATRIX4 temp;
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			temp.m[i][j] = mat1->m[i][0] * mat2->m[0][j] +
						   mat1->m[i][1]*mat2->m[1][j] +
						   mat1->m[i][2]*mat2->m[2][j] +
						   mat1->m[i][3]*mat2->m[3][j];
		}
	}

	memcpy(out, &temp, sizeof(EFFMATRIX4));
	return out;
}

//矩阵乘法，放大
inline EFFMATRIX4 * EFFMatrix4Mul(EFFMATRIX4 * out, const EFFMATRIX4 * mat1, effFLOAT scale)
{
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			out->m[i][j] = mat1->m[i][j] * scale;
		}
	}

	return out;
}

//矩阵加法
inline EFFMATRIX4 * EFFMatrix4Add(EFFMATRIX4 * out, const EFFMATRIX4 * mat1, const EFFMATRIX4 * mat2)
{
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			out->m[i][j] = mat1->m[i][j] + mat2->m[i][j];
		}
	}

	return out;
}

//矩阵减法
inline EFFMATRIX4 * EFFMatrix4Sub(EFFMATRIX4 * out, const EFFMATRIX4 * mat1, const EFFMATRIX4 * mat2)
{
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			out->m[i][j] = mat1->m[i][j] - mat2->m[i][j];
		}
	}

	return out;
}

//生成什么也不改变的矩阵
inline EFFMATRIX4 * EFFMatrix4Identity(EFFMATRIX4 * out)
{
	out->m11 = 1.0f;
	out->m12 = 0.0f;
	out->m13 = 0.0f;
	out->m14 = 0.0f;

	out->m21 = 0.0f;
	out->m22 = 1.0f;
	out->m23 = 0.0f;
	out->m24 = 0.0f;

	out->m31 = 0.0f;
	out->m32 = 0.0f;
	out->m33 = 1.0f;
	out->m34 = 0.0f;

	out->m41 = 0.0f;
	out->m42 = 0.0f;
	out->m43 = 0.0f;
	out->m44 = 1.0f;

	return out;
}

//平移矩阵
inline EFFMATRIX4 * EFFMatrix4Translation(EFFMATRIX4 * out, effFLOAT x, effFLOAT y, effFLOAT z)
{
	out->m11 = 1.0f;
	out->m12 = 0.0f;
	out->m13 = 0.0f;
	out->m14 = 0.0f;

	out->m21 = 0.0f;
	out->m22 = 1.0f;
	out->m23 = 0.0f;
	out->m24 = 0.0f;

	out->m31 = 0.0f;
	out->m32 = 0.0f;
	out->m33 = 1.0f;
	out->m34 = 0.0f;

	out->m41 = x;
	out->m42 = y;
	out->m43 = z;
	out->m44 = 1.0f;

	return out;
}

//放缩矩阵
inline EFFMATRIX4 * EFFMatrix4Scale(EFFMATRIX4 * out, effFLOAT x, effFLOAT y, effFLOAT z)
{
	out->m11 = x;
	out->m12 = 0.0f;
	out->m13 = 0.0f;
	out->m14 = 0.0f;

	out->m21 = 0.0f;
	out->m22 = y;
	out->m23 = 0.0f;
	out->m24 = 0.0f;

	out->m31 = 0.0f;
	out->m32 = 0.0f;
	out->m33 = z;
	out->m34 = 0.0f;

	out->m41 = 0.0f;
	out->m42 = 0.0f;
	out->m43 = 0.0f;
	out->m44 = 1.0f;

	return out;
}

//X旋转矩阵
inline EFFMATRIX4 * EFFMatrix4RotationX(EFFMATRIX4 * out, effFLOAT angle)
{
	out->m11 = 1.0f;
	out->m12 = 0.0f;
	out->m13 = 0.0f;
	out->m14 = 0.0f;

	out->m21 = 0.0f;
	out->m22 = cosf(angle);
	out->m23 = -sinf(angle);
	out->m24 = 0.0f;

	out->m31 = 0.0f;
	out->m32 = sinf(angle);
	out->m33 =  cosf(angle);
	out->m34 = 0.0f;

	out->m41 = 0.0f;
	out->m42 = 0.0f;
	out->m43 = 0.0f;
	out->m44 = 1.0f;

	return out;
}

//Y旋转矩阵
inline EFFMATRIX4 * EFFMatrix4RotationY(EFFMATRIX4 * out, effFLOAT angle)
{
	out->m11 = cosf(angle);
	out->m12 = 0.0f;
	out->m13 = -sinf(angle);
	out->m14 = 0.0f;

	out->m21 = 0.0f;
	out->m22 = 1.0f;
	out->m23 = 0.0f;
	out->m24 = 0.0f;

	out->m31 = sinf(angle);
	out->m32 = 0.0f;
	out->m33 = cosf(angle);
	out->m34 = 0.0f;

	out->m41 = 0.0f;
	out->m42 = 0.0f;
	out->m43 = 0.0f;
	out->m44 = 1.0f;

	return out;
}

//Z旋转矩阵
inline EFFMATRIX4 * EFFMatrix4RotationZ(EFFMATRIX4 * out, effFLOAT angle)
{
	out->m11 = cosf(angle);
	out->m12 =  sinf(angle);
	out->m13 = 0.0f;
	out->m14 = 0.0f;

	out->m21 = -sinf(angle);
	out->m22 =  cosf(angle);
	out->m23 = 0.0f;
	out->m24 = 0.0f;

	out->m31 = 0.0f;
	out->m32 = 0.0f;
	out->m33 = 1.0f;
	out->m34 = 0.0f;

	out->m41 = 0.0f;
	out->m42 = 0.0f;
	out->m43 = 0.0f;
	out->m44 = 1.0f;

	return out;
}

//用指定的绕Y轴角度，绕X轴角度和Z轴角度创建旋转矩阵
inline EFFMATRIX4 * uc3dxMatrixRotationYawPitchRoll(EFFMATRIX4 * out, effFLOAT Yaw, effFLOAT Pitch, effFLOAT Roll)
{
	EFFMATRIX4 mOut;
	EFFMatrix4RotationZ(out, Roll);
	EFFMatrix4RotationX(&mOut, -Pitch);
	EFFMatrix4Mul(out, out, &mOut);
	EFFMatrix4RotationY(&mOut, Yaw);
	EFFMatrix4Mul(out, out, &mOut);
	return out;
}

//变换矩阵
inline EFFMATRIX4 * EFFMatrix4Transpose(EFFMATRIX4 * out, const EFFMATRIX4 * mat)
{
	assert(out != mat);

	out->m11 = mat->m11;
	out->m12 =  mat->m21;
	out->m13 = mat->m31;
	out->m14 = mat->m41;

	out->m21 = mat->m12;
	out->m22 =  mat->m22;
	out->m23 = mat->m32;
	out->m24 = mat->m24;

	out->m31 = mat->m13;
	out->m32 =  mat->m23;
	out->m33 = mat->m33;
	out->m34 = mat->m43;

	out->m41 = mat->m14;
	out->m42 = mat->m24;
	out->m43 = mat->m34;
	out->m44 = mat->m44;

	return out;
}


inline EFFVECTOR4 * EFFVector3TransformFull(EFFVECTOR4 * out, const EFFVECTOR3 * v, const EFFMATRIX4 * mat)
{
	out->x = mat->m11 * v->x + mat->m21 * v->y + mat->m31 * v->z + mat->m41;
	out->y = mat->m12 * v->x + mat->m22 * v->y + mat->m32 * v->z + mat->m42;
	out->z = mat->m13 * v->x + mat->m23 * v->y + mat->m33 * v->z + mat->m43;
	out->w = mat->m14 * v->x + mat->m24 * v->y + mat->m34 * v->z + mat->m44;
	
	return out;
}

//求向量乘矩阵(假设向量w = 1.0f，矩阵最后一列(0.f, 0.f, 0.f, 1.f)，只适合计算坐标变换)
inline EFFVECTOR3 * EFFVector3Transform(EFFVECTOR3 * out, const EFFVECTOR3 * v, const EFFMATRIX4 * mat)
{
	if ( v == out )
	{
		EFFVECTOR3 temp = *v;
		out->x = mat->m11 * temp.x + mat->m21 * temp.y + mat->m31 * temp.z + mat->m41;
		out->y = mat->m12 * temp.x + mat->m22 * temp.y + mat->m32 * temp.z + mat->m42;
		out->z = mat->m13 * temp.x + mat->m23 * temp.y + mat->m33 * temp.z + mat->m43;
	}
	else
	{
		out->x = mat->m11 * v->x + mat->m21 * v->y + mat->m31 * v->z + mat->m41;
		out->y = mat->m12 * v->x + mat->m22 * v->y + mat->m32 * v->z + mat->m42;
		out->z = mat->m13 * v->x + mat->m23 * v->y + mat->m33 * v->z + mat->m43;
	}
	return out;
}

inline EFFVECTOR3 * EFFVector3TransformCoord(EFFVECTOR3 * out, const EFFVECTOR3 * v, const EFFMATRIX4 * mat)
{
	effFLOAT w;
	if ( v == out )
	{
		EFFVECTOR3 temp = *v;
		out->x = mat->m11 * temp.x + mat->m21 * temp.y + mat->m31 * temp.z + mat->m41;
		out->y = mat->m12 * temp.x + mat->m22 * temp.y + mat->m32 * temp.z + mat->m42;
		out->z = mat->m13 * temp.x + mat->m23 * temp.y + mat->m33 * temp.z + mat->m43;
		w = mat->m14 * temp.x + mat->m24 * temp.y + mat->m34 * temp.z + mat->m44;

		w = 1.0f / w;
		out->x *= w;
		out->y *= w;
		out->z *= w;
	}
	else
	{
		out->x = mat->m11 * v->x + mat->m21 * v->y + mat->m31 * v->z + mat->m41;
		out->y = mat->m12 * v->x + mat->m22 * v->y + mat->m32 * v->z + mat->m42;
		out->z = mat->m13 * v->x + mat->m23 * v->y + mat->m33 * v->z + mat->m43;
		w = mat->m14 * v->x + mat->m24 * v->y + mat->m34 * v->z + mat->m44;

		w = 1.0f / w;
		out->x *= w;
		out->y *= w;
		out->z *= w;
	}

	return out;
}


inline effFLOAT EFFMatrix4Determinant(const EFFMATRIX4 * m)
{
	effFLOAT det;
	det = m->m11 * ( m->m22 * m->m33 * m->m44 
					- m->m24 * m->m33 * m->m42
					+ m->m32 * m->m43 * m->m24
					- m->m23 * m->m32 * m->m44
					+ m->m42 * m->m23 * m->m34
					- m->m22 * m->m34 * m->m43);

	det -= m->m12 * ( m->m21 * m->m33 * m->m44
					- m->m23 * m->m31 * m->m44
					+ m->m41 * m->m23 * m->m34 
					- m->m24 * m->m33 * m->m41
					+ m->m31 * m->m43 * m->m24
					- m->m21 * m->m34 * m->m43);

	det += m->m13 * ( m->m21 * m->m32 * m->m44
					- m->m24 * m->m32 * m->m41
					+ m->m31 * m->m42 * m->m24
					- m->m22 * m->m31 * m->m44
					+ m->m41 * m->m22 * m->m34
					- m->m21 * m->m34 * m->m42);

	det -= m->m14 * ( m->m21 * m->m32 * m->m43
					- m->m23 * m->m32 * m->m41
					+ m->m31 * m->m42 * m->m23
					- m->m22 * m->m31 * m->m43
					+ m->m41 * m->m22 * m->m33
					- m->m21 * m->m33 * m->m42);

	return det;
}

inline EFFMATRIX4 * EFFMatrix4Inverse_CXX(EFFMATRIX4 * out, const EFFMATRIX4 * mat, effFLOAT * pDet)
{
	effFLOAT tmp[12];	/* temp array for pairs */
	effFLOAT src[16];	/* array of transpose source matrix */
	effFLOAT * dst = out->m[0];
	effFLOAT det;		/* determinant */
	
	/* transpose matrix */
	for ( int i = 0; i < 4; i++ )
	{
		src[i + 0] = mat->m[i][0];
		src[i + 4] = mat->m[i][1];
		src[i + 8] = mat->m[i][2];
		src[i + 12] = mat->m[i][3];
	}

	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];

	/* calculate first 8 elements (cofactors) */
	dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];
	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];

	/* calculate second 8 elements (cofactors) */
	dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	dst[10]-= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	dst[11]-= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	dst[12]-= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	dst[13]-= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	dst[14]-= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	dst[15]-= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

	/* calculate determinant */
	det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

	/* calculate matrix inverse */
	det = 1.0f / det;
	for ( int j = 0; j < 16; j++ )
	{
		dst[j] *= det;
	}

	if ( pDet != NULL )
	{
		*pDet = det;
	}

	return out;
}


#ifdef SYSTEM_WIN32
__declspec(naked)
inline EFFMATRIX4 * EFFMatrix4Inverse_SSE(EFFMATRIX4 * out, const EFFMATRIX4 * mat)
{
	//#if !defined(__VFX_ICL) && (__CPL_FULL_VER < 13000000)
	//#pragma message("You may need 'visual c++ processor patch'")
	//#pragma message("http://download.microsoft.com/download/vb60ent/Update/6/W9X2KXP/EN-US/vcpp5.exe")
	//#endif
	__asm
	{
        push      ebx
        mov       ebx, esp                                      
        and       esp, -16                                      
        sub       esp, 80                                       
        mov       eax, [ebx+8]                        
        mov       ecx, [ebx+12]                       
		;
        movlps    xmm0, [ecx]                         
        movhps    xmm0, [ecx+16]
        movaps    xmm1, xmm0                                    
        movlps    xmm3, [ecx+32] 
        movhps    xmm3, [ecx+48]                      
        shufps    xmm1, xmm3, 136                               
        shufps    xmm3, xmm0, 221                               
		;
        movaps    xmm4, xmm1                                    
        movaps    xmm6, xmm3                                    
        movlps    xmm0, [ecx+8]                       
        movhps    xmm0, [ecx+24]                      
        movaps    xmm2, xmm0                                    
        movlps    xmm5, [ecx+40]                      
        movhps    xmm5, [ecx+56]                      
        shufps    xmm2, xmm5, 136                               
        shufps    xmm5, xmm0, 221                               
        movaps    xmm7, xmm2                                    
        mulps     xmm7, xmm5                                    
        shufps    xmm7, xmm7, 177                               
        movaps    xmm0, xmm3                                    
        mulps     xmm0, xmm7                                    
        mulps     xmm4, xmm7                                    
        shufps    xmm7, xmm7, 78                                
        mulps     xmm6, xmm7                                    
        subps     xmm6, xmm0                                    
        mulps     xmm7, xmm1                                    
        subps     xmm7, xmm4                                    
        shufps    xmm7, xmm7, 78                                
        movaps    [esp+16], xmm7                    
		;
        movaps    xmm0, xmm3                                    
        mulps     xmm0, xmm2                                    
        shufps    xmm0, xmm0, 177                               
        movaps    xmm4, xmm5                                    
        mulps     xmm4, xmm0                                    
        addps     xmm4, xmm6                                    
        movaps    xmm7, xmm1                                    
        mulps     xmm7, xmm0                                    
        shufps    xmm0, xmm0, 78                                
        movaps    xmm6, xmm5                                    
        mulps     xmm6, xmm0                                    
        subps     xmm4, xmm6                                    
        mulps     xmm0, xmm1                                    
        subps     xmm0, xmm7                                    
        shufps    xmm0, xmm0, 78                                
        movaps    [esp+32], xmm0                    
		;
        movaps    xmm0, xmm3                                    
        shufps    xmm0, xmm0, 78                                
        mulps     xmm0, xmm5                                    
        shufps    xmm0, xmm0, 177                               
        shufps    xmm2, xmm2, 78                                
        movaps    xmm6, xmm2                                    
        mulps     xmm6, xmm0                                    
        addps     xmm6, xmm4                                    
        movaps    xmm7, xmm1                                    
        mulps     xmm7, xmm0                                    
        shufps    xmm0, xmm0, 78                                
        movaps    xmm4, xmm2                                    
        mulps     xmm4, xmm0                                    
        subps     xmm6, xmm4                                    
        movaps    [esp+48], xmm6                    
        mulps     xmm0, xmm1                                    
        subps     xmm0, xmm7                                    
        shufps    xmm0, xmm0, 78                                
		;
        movaps    xmm7, xmm1                                    
        mulps     xmm7, xmm3                                    
        shufps    xmm7, xmm7, 177                               
        movaps    xmm6, xmm5                                    
        mulps     xmm6, xmm7                                    
        addps     xmm6, xmm0                                    
        movaps    xmm0, xmm2                                    
        mulps     xmm0, xmm7                                    
        subps     xmm0, [esp+32]                    
        shufps    xmm7, xmm7, 78                                
        movaps    xmm4, xmm5                                    
        mulps     xmm4, xmm7                                    
        subps     xmm4, xmm6                                    
        mulps     xmm7, xmm2                                    
        subps     xmm0, xmm7                                    
        movaps    [esp+64], xmm0                    
		;
        movaps    xmm7, xmm1                                    
        mulps     xmm7, xmm5                                    
        shufps    xmm7, xmm7, 177                               
        movaps    xmm0, xmm2                                    
        mulps     xmm0, xmm7                                    
        movaps    xmm6, [esp+16]                    
        subps     xmm6, xmm0                                    
        movaps    xmm0, xmm3                                    
        mulps     xmm0, xmm7                                    
        addps     xmm0, xmm4                                    
        shufps    xmm7, xmm7, 78                                
        movaps    xmm4, xmm2                                    
        mulps     xmm4, xmm7
        addps     xmm4, xmm6                                    
        mulps     xmm7, xmm3                                    
        subps     xmm0, xmm7                                    
		;
        mulps     xmm2, xmm1                                    
        shufps    xmm2, xmm2, 177                               
        movaps    xmm6, xmm5                                    
        mulps     xmm6, xmm2                                    
        addps     xmm6, xmm4                                    
        movaps    xmm4, xmm3                                    
        mulps     xmm4, xmm2                                    
        movaps    xmm7, [esp+64]                    
        subps     xmm7, xmm4                                    
        shufps    xmm2, xmm2, 78                                
        mulps     xmm5, xmm2                                    
        subps     xmm6, xmm5                                    
        mulps     xmm3, xmm2                                    
        addps     xmm3, xmm7                                    
		;
        movaps    xmm2, [esp+48]                    
        mulps     xmm1, xmm2                                    
        movaps    xmm4, xmm1                                    
        shufps    xmm4, xmm4, 78                                
        addps     xmm4, xmm1                                    
        movaps    xmm5, xmm4                                    
        shufps    xmm5, xmm5, 177                               
        addss     xmm5, xmm4                                    
        movaps    xmm1, xmm5
        rcpss     xmm1, xmm1                                    
        movaps    xmm4, xmm1                                    
        addss     xmm4, xmm4                                    
        mulss     xmm1, xmm1                                    
        mulss     xmm5, xmm1                                    
        subss     xmm4, xmm5                                    
        shufps    xmm4, xmm4, 0                                 
        mulps     xmm2, xmm4                                    
		;
        mulps     xmm6, xmm4                                    
        movlps    [eax], xmm2                         
        mulps     xmm0, xmm4                                    
        mulps     xmm4, xmm3                                    
        movhps    [eax+8], xmm2                       
        movlps    [eax+16], xmm6                      
        movhps    [eax+24], xmm6                      
        movlps    [eax+32], xmm0                      
        movhps    [eax+40], xmm0                      
        movlps    [eax+48], xmm4                      
        movhps    [eax+56], xmm4                      
		;
        mov       esp, ebx                                      
        pop       ebx                                           
        ret                                                 
	}
}
#endif

inline EFFMATRIX4 * EFFMatrix4Inverse(EFFMATRIX4 * out, const EFFMATRIX4 * mat, effFLOAT * det)
{
	return EFFMatrix4Inverse_CXX(out, mat, det);
}

inline EFFMATRIX4 * EFFMatrix4RotationAxis(EFFMATRIX4 * out, const EFFVECTOR3 * axis, effFLOAT theta)
{
	/* This function performs an axis/angle rotation. (x,y,z) is any 
		vector on the axis. For greater speed, always use a unit vector 
		(length = 1). In this version, we will assume an arbitrary 
		length and normalize. */

	// normalize
	effFLOAT length = sqrtf(axis->x * axis->x + axis->y * axis->y + axis->z * axis->z);

	// too close to 0, can't make a normalized vector
	if ( length < EFFMATH_EPSILON )
	{
		return NULL;
	}

	effFLOAT x = axis->x / length;
	effFLOAT y = axis->y / length;
	effFLOAT z = axis->z / length;

	// do the trig
	effFLOAT c = cosf(-theta);
	effFLOAT s = sinf(-theta);
	effFLOAT t = 1.0f - c;   

	// build the rotation matrix
	out->m11 = t * x * x + c;
	out->m12 = t * x * y - s * z;
	out->m13 = t * x * z + s * y;
	out->m14 = 0;

	out->m21 = t * x * y + s * z;
	out->m22 = t * y * y + c;
	out->m23 = t * y * z - s * x;
	out->m24 = 0;

	out->m31 = t * x * z - s * y;
	out->m32 = t * y * z + s * x;
	out->m33 = t * z * z + c;
	out->m34 = 0;

	out->m41 = out->m42 = out->m43 = 0.0f;
	out->m44 = 1.0f;

	return out;
}

//设置一个左手定则的视矩阵
inline EFFMATRIX4 * EFFMatrix4View(EFFMATRIX4 * mat, const EFFVECTOR3 * pos, const EFFVECTOR3 * dir, const EFFVECTOR3 * up, const EFFVECTOR3 * right)
{
	mat->m11 = right->x;
	mat->m12 = up->x;
	mat->m13 = dir->x;

    mat->m21 = right->y;
	mat->m22 = up->y;
	mat->m23 = dir->y;

    mat->m31 = right->z;
	mat->m32 = up->z;
	mat->m33 = dir->z;

	mat->m41 = -EFFVector3Dot(pos, right);
    mat->m42 = -EFFVector3Dot(pos, up);
    mat->m43 = -EFFVector3Dot(pos, dir);

	mat->m14 = 0.f;
	mat->m24 = 0.f;
	mat->m34 = 0.f;
	mat->m44 = 1.f;

	return mat;
}

inline EFFMATRIX4* EFFMatrix4LookAtLH(EFFMATRIX4 * out, const EFFVECTOR3 * pos, const EFFVECTOR3 * lookAt, const EFFVECTOR3 * up)
{
	EFFVECTOR3 dir, right, vUp;
	EFFVector3Sub(&dir, lookAt, pos);
	EFFVector3Normalize(&dir, &dir);
	
	EFFVector3Cross(&right, up, &dir);
	EFFVector3Normalize(&right, &right);
	EFFVector3Cross(&vUp, &dir, &right);

	return EFFMatrix4View(out, pos, &dir, &vUp, &right);
}

EFFMATH_API EFFMATRIX4 * EFFMatrix4PerspectiveEx(EFFMATRIX4 * out, const effFLOAT nearPlane, const effFLOAT farPlane, const effFLOAT fovHoriz,
												const effFLOAT fovVert);


EFFMATH_API EFFMATRIX4 * EFFMatrix4PerspectiveLH(EFFMATRIX4 * out, effFLOAT w, effFLOAT h, effFLOAT zn, effFLOAT zf);

EFFMATH_API EFFMATRIX4 * EFFMatrix4PerspectiveFovLH(EFFMATRIX4 * out, effFLOAT fovy, effFLOAT aspect, effFLOAT zn, effFLOAT zf);

EFFMATH_API EFFMATRIX4* EFFMatrix4OrthoLH(EFFMATRIX4 * out, effFLOAT w, effFLOAT h, effFLOAT zn, effFLOAT zf);

EFFMATH_API EFFMATRIX4 * EFFMatrix4OrthoOffCenterLH(EFFMATRIX4 * out, effFLOAT l, effFLOAT r, effFLOAT b, effFLOAT t, effFLOAT zn, effFLOAT zf);

/*
检测一个方阵是否是positive definite matrix，m是矩阵的首地址，n是矩阵的维度，取值为3或者4
具体的算法参见http://en.wikipedia.org/wiki/Cholesky_decomposition
*/
EFFMATH_API effBOOL EFFMatrixIsPositiveDefinite(const effFLOAT * m, effINT n);

EFFMATH_API effBOOL EFFMatrixCanCholeskyDecomposition(effFLOAT * m, effINT n);

/* solve for x: L*L'*x = b, and put the result back into x.
* L is size n*n, b is size n*1. only the lower triangle of L is considered.
*/
EFFMATH_API effVOID EFFMatrixSolveCholesky(const effFLOAT * l, effFLOAT * b, effINT n);

EFFMATH_API effVOID EFFReflect(EFFMATRIX4 * m, const EFFPLANE * plane);

EFFMATH_API effVOID EFFShadow(EFFMATRIX4 * m, const EFFVECTOR4 * l, const EFFPLANE * plane);


/**
   检测一个点在一个3d平面的哪边
   如果点在0-v1-v2面的后面，返回-1
   如果点在0-v1-v2面的前面，返回1
   如果点正好在面上面，返回0
   这个面是由<0,0,0>-v1-v2组成
**/
inline effINT EFFWhichSide3D(const EFFVECTOR3 * p, const EFFVECTOR3 * v1, const EFFVECTOR3 * v2)
{
	// V1 % V2 得到 V1-V2-0 面法向量，它与p-0向量点积得到夹角余玄
	effFLOAT s = p->x * (v1->y * v2->z - v1->z * v2->y) + p->y * (v1->z * v2->x - v1->x * v2->z) + 
				 p->z * (v1->x * v2->y - v1->y * v2->x);

	if ( s < -EFFMATH_EPSILON ) 
	{
		return -1;
	}
	else if ( s > EFFMATH_EPSILON ) 
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

/**
   计算给定的点在直线的哪一边
   如果点是在s1-s2线段左边，返回-1
   如果是在右边，返回1
   正好在线段上返回0
*/
inline effINT EFFWhichSide2D(const EFFVECTOR2 * v, const EFFVECTOR2 * s1, const EFFVECTOR2 * s2)
{
	effFLOAT k  = (s1->y - v->y) * (s2->x - s1->x);
	effFLOAT k1 = (s1->x - v->x) * (s2->y - s1->y);
	if ( k < k1 ) 
	{
		return -1;
	}
	else if ( k > k1 ) 
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

/**
   计算3个向量组成的平面的法线
   返回值存放在norm里面
**/
inline effVOID EFFVector3CalcNormal(EFFVECTOR3 * normal, const EFFVECTOR3 * v1, const EFFVECTOR3 * v2, const EFFVECTOR3 * v3, effBOOL normalize = effFALSE)
{
	EFFVECTOR3 temp1, temp2;
	EFFVector3Sub(&temp1, v1, v3);
	EFFVector3Sub(&temp2, v2, v3);
	EFFVector3Cross(normal, &temp1, &temp2);
	if ( normalize )
	{
		EFFVector3Normalize(normal, normal);
	}
}

//计算两个点的距离的平方
inline effFLOAT EFFVector2CalSquareDistance(const EFFVECTOR2 * v1, const EFFVECTOR2 * v2)
{	
	return ( (v1->x - v2->x) * (v1->x - v2->x) + (v1->y - v2->y) * (v1->y - v2->y) );
}

inline effFLOAT EFFVector3CalSquareDistance(const EFFVECTOR3 * v1, const EFFVECTOR3 * v2)
{
	return ( (v1->x - v2->x) * (v1->x - v2->x) + (v1->y - v2->y) * (v1->y - v2->y) + (v1->z - v2->z) * (v1->z - v2->z) );
}

//点线距离平方
EFFMATH_API effFLOAT EFFPointLineSquareDistance(const EFFVECTOR3 * p, const EFFVECTOR3 * l1, const EFFVECTOR3 * l2);

//点到平面距离，平面不一定要规格化，内部自动计算平面的法方程
EFFMATH_API effFLOAT EFFPointPlaneDistance(const EFFVECTOR3 * p, EFFPLANE & plane);


EFFMATH_API effFLOAT EFFArea3D(const EFFVECTOR3 * a, const EFFVECTOR3 * b, const EFFVECTOR3 * c);

//得到向量绕Y，X，Z轴的角度
EFFMATH_API effFLOAT EFFVector3CalAngleXZ(const EFFVECTOR3 & v);
EFFMATH_API effFLOAT EFFVector3CalAngleYZ(const EFFVECTOR3 & v);
EFFMATH_API effFLOAT EFFVector3CalAngleXY(const EFFVECTOR3 & v);


//检测相交数学工具类
class EFFAabb;
class EFFSegment3;

//计算射线是否和三角形相交，并且求交点，只计算正面
EFFMATH_API effBOOL EFFRayIntersectPlane(effFLOAT * t, EFFVECTOR3 * point, const EFFVECTOR3 * pos, const EFFVECTOR3 * dir, 
										const EFFVECTOR3 * a, const EFFVECTOR3 * b, const EFFVECTOR3 * c);


//计算线段和三角形的交点，只计算正面
EFFMATH_API effBOOL EFFLineIntersectPlane(effFLOAT * t, EFFVECTOR3 * point, const EFFVECTOR3 * from, const EFFVECTOR3 * length,
										const EFFVECTOR3 * a, const EFFVECTOR3 * b, const EFFVECTOR3 * c);

//计算线段和三角形的交点
EFFMATH_API effUINT EFFLineIntersectPlane(EFFVECTOR3 * point, const EFFVECTOR3 * from, const EFFVECTOR3 * length,
										const EFFVECTOR3 * a, const EFFVECTOR3 * b, const EFFVECTOR3 * c);

//只计算正面
EFFMATH_API effBOOL EFFLineIntersectTriangle(effFLOAT * t, EFFVECTOR3 * point, const EFFVECTOR3 * from, const EFFVECTOR3 * length,
										const EFFVECTOR3 * a, const EFFVECTOR3 * b, const EFFVECTOR3 * c, const EFFAabb * lineBox = NULL,
										const EFFAabb * triBox = NULL, const EFFVECTOR3 * normal = NULL, effFLOAT precision = EFFMATH_EPSILON);


EFFMATH_API effBOOL EFFLineIntersectBox3(effFLOAT * normal, EFFVECTOR3 * pointNormal, effFLOAT * pos, EFFVECTOR3 * pointPos,
										const EFFVECTOR3 * from, const EFFVECTOR3 * dir, const EFFAabb * box);

//计算线段和三角形的交点，实用的witchside测试
EFFMATH_API effBOOL EFFSegIntersectTriangle(const EFFVECTOR3 & v1, const EFFVECTOR3 & v2, const EFFVECTOR3 & v3, const EFFSegment3 & seg,
										EFFVECTOR3 & sect);


//EFFPlane3版本
EFFMATH_API effBOOL EFFLineIntersectPlane(const EFFVECTOR3 & v1, const EFFVECTOR3 & v2, effFLOAT A, effFLOAT B, effFLOAT C, effFLOAT D,
										EFFVECTOR3 & sect, effFLOAT & dist);

EFFMATH_API effBOOL EFFLineIntersectPlane(const EFFVECTOR3 & v1, const EFFVECTOR3 & v2, const EFFPLANE & p, EFFVECTOR3 & sect,
										effFLOAT & dist);

/**
计算多边形poly和平面plane的交点，如果没有交点，返回false，有的话segment
就是交线，当segment的start，end相同，表示只有一个交点
*/
//EFFMATH_API effBOOL EFFPlaneIntersectPolygon(const EFFPLANE & plane, EFFPolygon3 * poly, EFFSegment3 & seg);



EFFMATH_END


#endif