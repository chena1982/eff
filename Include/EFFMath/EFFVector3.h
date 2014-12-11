/********************************************************************
	created:	30:12:2009   23:33
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFVector3.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFVector3_2009_12_30__
#define __EFFVector3_2009_12_30__

#include "EFFMatrix4.h"

EFFMATH_BEGIN

class EFFAabb;



class EFFMATH_API EFFVector3 : public EFFVECTOR3
{
	//RTTI_DECLARE_BASE(EFFVector3)
public:
	inline EFFVector3() { memset(this, 0, sizeof(EFFVector3)); }

	inline EFFVector3(effFLOAT x, effFLOAT y, effFLOAT z)
	{ 
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline EFFVector3(const EFFVector3 & rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z; 
	}

	inline effVOID SetValue(effFLOAT x, effFLOAT y, effFLOAT z)
	{ 
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline EFFVector3 & Zero()
	{
		x = y = z = 0;
		return *this;
	}

	inline effFLOAT Distance(const EFFVector3 & b) const
	{
		return sqrtf((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y) + (z - b.z) * (z - b.z));
	}

	inline effFLOAT SquareDistance(const EFFVector3 & b) const
	{
		return ((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y) + (z - b.z) * (z - b.z));
	}

	inline VectorComponent SmallestAxis() const
	{
		const effFLOAT * Vals = &x;
		VectorComponent m = X;
		
		if ( fabs(Vals[Y]) < fabs(Vals[m]) )
		{
			m = Y;
		}
		
		if ( fabs(Vals[Z]) < fabs(Vals[m]) )
		{
			m = Z;
		}

		return m;
	}

	inline VectorComponent LargestAxis() const
	{
		const effFLOAT * Vals = &x;
		VectorComponent m = X;

		if ( fabs(Vals[Y]) > fabs(Vals[m]) )
		{
			m = Y;
		}

		if ( fabs(Vals[Z]) > fabs(Vals[m]) )
		{
			m = Z;
		}

		return m;
	}


	friend inline EFFVector3 operator + (const EFFVector3 & v1, const EFFVector3 & v2);
	friend inline EFFVector3 operator - (const EFFVector3 & v1, const EFFVector3 & v2);
	friend inline EFFVector3 operator * (const EFFVector3 & v, const effFLOAT value);
	friend inline EFFVector3 operator * (const effFLOAT value, const EFFVector3 & v);
	friend inline EFFVector3 operator / (const EFFVector3 & v, const effFLOAT value);

	friend inline effFLOAT operator | (const EFFVector3 & v1, const EFFVector3 & v2);
	friend inline EFFVector3 operator % (const EFFVector3 & v1, const EFFVector3 & v2);


	inline const EFFVector3 operator + () const
	{ 
		return EFFVector3(x, y, z); 
	}

	inline const EFFVector3 operator - () const
	{ 
		return EFFVector3(-x, -y, -z); 
	}


	EFFVector3 & operator += (const EFFVector3 & v);
	EFFVector3 & operator -= (const EFFVector3 & v);
	EFFVector3 & operator *= (effFLOAT value);
	EFFVector3 & operator /= (effFLOAT value);

	inline	EFFVector3 & operator *= (const EFFMatrix4 & m)
	{

		effFLOAT xp = x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0];
		effFLOAT yp = x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1];
		effFLOAT zp = x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2];
		
		x = xp;
		y = yp;
		z = zp;

		return *this;
	}

    //inline EFFVector3 & operator *= (const EFFQuaternion & q);

	inline operator const effFLOAT *() const { return &x; }
	inline operator effFLOAT *() { return &x; }

	inline effFLOAT Min() const
	{
		return MIN(x, MIN(y, z));
	}

	inline effFLOAT Max() const
	{
		return MAX(x, MAX(y, z));
	}

	inline EFFVector3 & Min(const EFFVector3 & p)
	{
		x = MIN(x, p.x);
		y = MIN(y, p.y);
		z = MIN(z, p.z);
		return *this;
	}

	inline EFFVector3 & Max(const EFFVector3 & p)
	{
		x = MAX(x, p.x);
		y = MAX(y, p.y);
		z = MAX(z, p.z);
		return *this;
	}

	//求V1在V2上的垂足向量
	inline EFFVector3 operator >> (const EFFVector3 & v)  const
	{ 
		return v * ( ((*this) | v) / (v | v) ); 
	}

	inline effBOOL operator == (const EFFVector3 & v) const
	{ 
		return fabs(x - v.x) <= EFFMATH_EPSILON
			 && fabs(y - v.y) <= EFFMATH_EPSILON
			 && fabs(z - v.z) <= EFFMATH_EPSILON; 
	}

	inline effBOOL operator != (const EFFVector3 & v) const
	{ 
		return fabs(x - v.x) > EFFMATH_EPSILON
			  || fabs(y-v.y) > EFFMATH_EPSILON
			  || fabs(z-v.z) > EFFMATH_EPSILON; 
	}

	inline effFLOAT Length() const
	{ 
		return sqrtf(x * x + y * y + z * z); 
	}

	inline effFLOAT SquareLength() const
	{
		return x * x + y * y + z * z;
	}

	inline effVOID Normalize();

	//Linear interpolate between two vectors: this = a + t * (b - a)
	inline EFFVector3 & Lerp(const EFFVector3 & a, const EFFVector3 & b, effFLOAT t)
	{
		x = a.x + t * (b.x - a.x);
		y = a.y + t * (b.y - a.y);
		z = a.z + t * (b.z - a.z);
		return *this;
	}

	//得到该向量的规格化向量
	EFFVector3 GetUnitVector()
	{
		effFLOAT length = Length();
		if ( length > 0.0f )
		{
			return (*this / length);
		}
		else
		{
			return EFFVector3(0.0f, 0.0f, 0.0f);
		}
	}

	//effVOID ProjectToScreen(effFLOAT screenWidth, effFLOAT screenHeight, const EFFMatrix4 & m, EFFVector4 & projected) const;
	//effVOID	PorjectToPlane(EFFPlane & plane, EFFVector3 & direction, EFFVector3 * out);


};


inline EFFVector3 operator + (const EFFVector3 & v1, const EFFVector3 & v2)
{
	EFFVector3 result;
	EFFVector3Add(&result, &v1, &v2);
	return result;
}

inline EFFVector3 operator - (const EFFVector3 & v1, const EFFVector3 & v2)
{
	EFFVector3 result;
	EFFVector3Sub(&result, &v1, &v2);
	return result;
}



inline EFFVector3 operator * (const EFFVector3 & v, const effFLOAT value)
{
	EFFVector3 result;
	EFFVector3Mul(&result, value, &v);
	return result;
}

inline EFFVector3 operator * (const effFLOAT value, const EFFVector3 & v)
{
	EFFVector3 result;
	EFFVector3Mul(&result, value, &v);
	return result;
}

inline EFFVector3 operator / (const EFFVector3 & v, const effFLOAT value)
{
	EFFVector3 result;
	EFFVector3Mul(&result, 1.0f / value, &v);
	return result;
}


inline effFLOAT operator | (const EFFVector3 & v1, const EFFVector3 & v2)
{
	return EFFVector3Dot(&v1, &v2);
}

inline EFFVector3 operator % (const EFFVector3 & v1, const EFFVector3 & v2)
{
	EFFVector3 result;
	EFFVector3Cross(&result, &v1, &v2);
	return result;
}

inline EFFVector3 operator * (const EFFVector3 & v, const EFFMatrix4 & m)
{
	return EFFVector3(m.m11 * v.x + m.m21 * v.y + m.m31 * v.z + m.m41,
					  m.m12 * v.x + m.m22 * v.y + m.m32 * v.z + m.m42,
					  m.m13 * v.x + m.m23 * v.y + m.m33 * v.z + m.m43);
}

/*inline effVOID EFFVector3::operator *= (const EFFQuaternion & q)
{
	EFFQuaternion temp(-q.x, -q.y, -q.z, q.w);
	temp *= *this;
	temp *= q;

	x = temp.x;
	y = temp.y;
	z = temp.z;
}*/

inline EFFVector3 & EFFVector3::operator *= (const effFLOAT value)
{ 
	EFFVector3Mul(this, value, this);
	return *this;
}

inline EFFVector3& EFFVector3::operator /= (const effFLOAT value)
{ 
	EFFVector3Mul(this, 1.0f / value, this);
	return *this;
}

inline EFFVector3& EFFVector3::operator += (const EFFVector3 & v)
{ 
	EFFVector3Add(this, this, &v);
	return *this;
}

inline EFFVector3 & EFFVector3::operator -= (const EFFVector3 & v)
{ 
	EFFVector3Sub(this, this, &v);
	return *this;
}


inline effVOID EFFVector3::Normalize()
{
	EFFVector3Normalize(this, this);
}

EFFMATH_API effVOID EFFVector3TransformNormal(EFFVector3 * out, EFFVector3 & normal, const EFFMatrix4 & m);
EFFMATH_API effFLOAT EFFPointAABBSqrDist(const EFFVector3 & point, const EFFAabb & aabb);
EFFMATH_API effFLOAT EFFPointAABBSqrDist(const EFFVector3 & point, const EFFVector3 & center,const EFFVector3 & extents);
EFFMATH_API effFLOAT EFFPointTriangleSqrDist(const EFFVector3 & point, const EFFVector3 & p0, const EFFVector3 & p1, const EFFVector3 & p2);



//变换坐标，结果向量被作了透视除法
EFFMATH_API effVOID EFFVector3TransformCoordArray(EFFVector3 * out, effINT outStride, const EFFVector3 * v, effINT vStride, const EFFMatrix4 * m, effINT n);


EFFMATH_END

/*namespace YAML
{
	template<>
	struct convert<EFFMATH::EFFVector3>
	{
		static Node encode(const EFFMATH::EFFVector3 & rhs)
		{
			Node node = YAML::Load("{}");
			node["x"] = rhs.x;
			node["y"] = rhs.y;
			node["z"] = rhs.z;

			return node;
		}
		
		static bool decode(const Node & node, EFFMATH::EFFVector3 & rhs)
		{
			if ( !node.IsMap() || node.size() != 3 )
            {
				return false;
			}
			
			rhs.x = node["x"].as<float>();
			rhs.y = node["y"].as<float>();
			rhs.z = node["z"].as<float>();
			return true;
		}
	};
}*/


#endif




