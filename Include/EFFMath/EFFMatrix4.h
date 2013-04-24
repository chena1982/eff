/******************************************************************************
	created:	2008-12-13   0:42
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFMatrix4.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMatrix4_2008_12_13__
#define __EFFMatrix4_2008_12_13__



EFFMATH_BEGIN

class EFFVector3;
class EFFVector4;

class EFFMATH_API EFFMatrix4 : public EFFMATRIX4
{
public:
	EFFMatrix4() {}
	~EFFMatrix4() {}

	EFFMatrix4(const effFLOAT * src)
	{	
		memcpy(m, src, sizeof(effFLOAT) * 16); 
	}


	EFFMatrix4(const EFFMatrix4 & m)
	{
		memcpy(this, &m, sizeof(EFFMatrix4));
	}

	EFFMatrix4(effFLOAT _11, effFLOAT _12, effFLOAT _13, effFLOAT _14,
					   effFLOAT _21, effFLOAT _22, effFLOAT _23, effFLOAT _24,
					   effFLOAT _31, effFLOAT _32, effFLOAT _33, effFLOAT _34,
					   effFLOAT _41, effFLOAT _42, effFLOAT _43, effFLOAT _44)
	{
		m11 = _11, m12 = _12, m13 = _13, m14 = _14,
		m21 = _21, m22 = _22, m23 = _23, m24 = _24,
		m31 = _31, m32 = _32, m33 = _33, m34 = _34,
		m41 = _41, m42 = _42, m43 = _43, m44 = _44;
	}

	inline effVOID Set(effFLOAT _11, effFLOAT _12, effFLOAT _13, effFLOAT _14,
					   effFLOAT _21, effFLOAT _22, effFLOAT _23, effFLOAT _24,
					   effFLOAT _31, effFLOAT _32, effFLOAT _33, effFLOAT _34,
					   effFLOAT _41, effFLOAT _42, effFLOAT _43, effFLOAT _44)
	{
		m11 = _11, m12 = _12, m13 = _13, m14 = _14,
		m21 = _21, m22 = _22, m23 = _23, m24 = _24,
		m31 = _31, m32 = _32, m33 = _33, m34 = _34,
		m41 = _41, m42 = _42, m43 = _43, m44 = _44;
	}


	//EFFMatrix4(const uc3dxQuaternion & q);

	EFFMatrix4 operator * ( const EFFMatrix4 & m) const;
	EFFMatrix4 operator + ( const EFFMatrix4 & m) const;
	EFFMatrix4 operator - ( const EFFMatrix4 & m) const;

	EFFMatrix4 & operator *= ( const EFFMatrix4 & m);
	EFFMatrix4 & operator += ( const EFFMatrix4 & m);
	EFFMatrix4 & operator -= ( const EFFMatrix4 & m);

	EFFMatrix4 operator * ( effFLOAT f) const;
	EFFMatrix4 operator / ( effFLOAT f) const;

	friend inline EFFMatrix4 operator * (effFLOAT f, const EFFMatrix4 & m);

	EFFMatrix4 & operator *= ( effFLOAT f);
	EFFMatrix4 & operator /= ( effFLOAT f);

	inline effVOID GetRow(const effINT r, EFFVector4 & p) const;
	inline effVOID GetRow(const effINT r, EFFVector3 & p) const;

	inline const EFFVector4 & GetRow(const effINT r) const
	{
		return *(const EFFVector4 *)&m[r][0];
	}

	inline EFFVector4 & GetRow(const effINT r)
	{
		return *(EFFVector4 *)&m[r][0];
	}

	inline effVOID SetRow(const effINT r, const EFFVector4 & p);

	inline effVOID SetRow(const effINT r, const EFFVector3 & p);

	inline effVOID GetCol(const effINT c, EFFVector4 & p) const;

	inline effVOID GetCol(const effINT c, EFFVector3 & p) const;

	inline effVOID SetCol(const effINT c, const EFFVector4 & p);

	inline effVOID SetCol(const effINT c, const EFFVector3 & p);



	/*inline const EFFVector4 & GetTrans() const
	{
		return *((const EFFVector4 *)&m[3][0]);
	}*/

	inline const EFFVector3 & GetTrans() const
	{
		return *((const EFFVector3 *)&m[3][0]);
	}

	inline effVOID GetTrans(EFFVector3 & p) const;

	inline effVOID SetTrans(const EFFVector4 & p);

	inline effVOID SetTrans(const EFFVector3 & p);

	inline effVOID SetTrans(effFLOAT x, effFLOAT y, effFLOAT z)
	{
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
	}

	inline effVOID Trans(const EFFVector3 & p);


	effVOID ZeroMatrix();

	//得到单位矩阵
	effVOID Identity();
	//得到平移矩阵
	effVOID MoveMatrix(effFLOAT x, effFLOAT y, effFLOAT z);

	effVOID ScaleMatrix(effFLOAT x, effFLOAT y, effFLOAT z);

	effVOID RotationMatrixX(effFLOAT angle);
	effVOID RotationMatrixY(effFLOAT angle);
	effVOID RotationMatrixZ(effFLOAT angle);

	effVOID RotationAxis(const EFFVector3 * axis, effFLOAT angle);
	effVOID Transpose();
	effVOID Transpose(EFFMatrix4 & transpose) const;

	effFLOAT Determinant() const;
	effFLOAT CoFactor(effINT row, effINT col) const;

	/*inline operator uc3dxMatrix3() const
	{
		return uc3dxMatrix3(
			m[0][0],	m[0][1],	m[0][2],
			m[1][0],	m[1][1],	m[1][2],
			m[2][0],	m[2][1],	m[2][2]);
	}*/
};

inline EFFMATH_API const EFFMatrix4 & EFFGetIdentityMatrix4()
{
	static EFFMatrix4 MATRIX4_IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
									   0.0f, 1.0f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   0.0f, 0.0f, 0.0f, 1.0f);
	return MATRIX4_IDENTITY;
}

EFFMATH_API effVOID EFFMatrix4Invert(EFFMatrix4 & out, effFLOAT * determinant, const EFFMatrix4 & in);
EFFMATH_API effVOID EFFMatrix4InvertPR(EFFMatrix4 & dest, const EFFMatrix4 & src);
//EFFMATH_API effVOID EFFMatrix4Decompose(EFFVector3 * scale, EFFVector3 * trans, EFFQuaternion * rotation, const EFFMatrix4 * m);
EFFMATH_API EFFMatrix4 EFFMatrix4CubeVeiw(effINT face);
//EFFMATH_API EFFMatrix4 * EFFMatrix4FromQuaternion(EFFMatrix4 * out, const EFFQuaternion * q);



EFFMATH_API EFFMatrix4 * EFFMatrixPerspectiveFovLH(EFFMatrix4 * out, effFLOAT fovy, effFLOAT aspect, effFLOAT zn, effFLOAT zf);
EFFMATH_API EFFMatrix4 * EFFMatrixLookAtLH(EFFMatrix4 * out, const EFFVector3 * eye, const EFFVector3 * lookAt, const EFFVector3 * up);

EFFMATH_END

#endif