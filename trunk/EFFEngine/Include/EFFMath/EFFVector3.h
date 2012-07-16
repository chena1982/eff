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
public:
	EFFVector3() {}
	EFFVector3(effFLOAT x,effFLOAT y,effFLOAT z) : x(x), y(y), z(z) {}

	effVOID Normalize()
	{
		effFLOAT fSum = x*x + y*y + z*z;
		fSum = 1.0f / sqrtf(fSum);
		x *= fSum;
		y *= fSum;
		z *= fSum;
	}

	effVOID Set(effFLOAT x,effFLOAT y,effFLOAT z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	EFFVector3 & Cross(const EFFVector3 * pV);

	effFLOAT Dot(const EFFVector3 * pV);

	EFFVector3 & operator - (const EFFVector3 * pV)
	{
		x -= pV->x;
		y -= pV->y;
		z -= pV->z;
		return *this;
	}

public:
	effFLOAT		x;
	effFLOAT		y;
	effFLOAT		z;


};

inline EFFMATH_API EFFVector3 * EFFVec3Cross(EFFVector3 * pOut,const EFFVector3 * pV1,const EFFVector3 * pV2)
{
	EFFVector3 out;
	out.x = pV1->y * pV2->z - pV1->z * pV2->y;
	out.y = pV1->z * pV2->x - pV1->x * pV2->z;
	out.z = pV1->x * pV2->y - pV1->y * pV2->x;
	*pOut = out;
	return pOut;
}

inline EFFMATH_API effFLOAT EFFVec3Dot(const EFFVector3 * pV1,const EFFVector3 * pV2)
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

inline EFFMATH_API EFFVector3 * EFFVec3Subtract(EFFVector3 * pOut,const EFFVector3 * pV1,const EFFVector3 * pV2)
{
	EFFVector3 out;
	out.x = pV1->x - pV2->x;
	out.y = pV1->y - pV2->y;
	out.z = pV1->z - pV2->z;

	*pOut = out;
	return pOut;
}

inline EFFVector3 & EFFVector3::Cross(const EFFVector3 * pV)
{
	return *EFFVec3Cross(this,this,pV);
}

inline effFLOAT EFFVector3::Dot(const EFFVector3 * pV)
{
	return EFFVec3Dot(this,pV);
}


EFFMATH_END


#endif




