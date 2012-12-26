/********************************************************************
	created:	30:12:2009   23:33
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFVector3.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFVector3_2009_12_30__
#define __EFFVector3_2009_12_30__


EFFMATH_BEGIN


class EFFMATH_API EFFVector3
{
	RTTI_DECLARE_BASE(EFFVector3)
public:
	EFFVector3() {}
	EFFVector3(effFLOAT x, effFLOAT y, effFLOAT z) : x(x), y(y), z(z) {}

	inline effVOID Normalize()
	{
		effFLOAT sum = x * x + y * y + z * z;
		sum = 1.0f / sqrtf(sum);
		x *= sum;
		y *= sum;
		z *= sum;
	}

	inline effVOID Set(effFLOAT x, effFLOAT y, effFLOAT z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	EFFVector3 & Cross(const EFFVector3 * v);

	effFLOAT Dot(const EFFVector3 * v);

	inline EFFVector3 & operator - (const EFFVector3 * v)
	{
		x -= v->x;
		y -= v->y;
		z -= v->z;
		return *this;
	}

public:
	effFLOAT		x;
	effFLOAT		y;
	effFLOAT		z;


};

inline EFFMATH_API EFFVector3 * EFFVec3Cross(EFFVector3 * out,const EFFVector3 * v1,const EFFVector3 * v2)
{
	EFFVector3 temp;
	temp.x = v1->y * v2->z - v1->z * v2->y;
	temp.y = v1->z * v2->x - v1->x * v2->z;
	temp.z = v1->x * v2->y - v1->y * v2->x;
	*out = temp;
	return out;
}

inline EFFMATH_API effFLOAT EFFVec3Dot(const EFFVector3 * v1,const EFFVector3 * v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

inline EFFMATH_API EFFVector3 * EFFVec3Subtract(EFFVector3 * out,const EFFVector3 * v1,const EFFVector3 * v2)
{
	EFFVector3 temp;
	temp.x = v1->x - v2->x;
	temp.y = v1->y - v2->y;
	temp.z = v1->z - v2->z;

	*out = temp;
	return out;
}

inline EFFVector3 & EFFVector3::Cross(const EFFVector3 * v)
{
	return *EFFVec3Cross(this, this, v);
}

inline effFLOAT EFFVector3::Dot(const EFFVector3 * v)
{
	return EFFVec3Dot(this, v);
}



EFFMATH_END


#endif




