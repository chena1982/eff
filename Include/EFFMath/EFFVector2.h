/********************************************************************
	created:	2013-02-04   22:11
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFVector2.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFVector2_2013_02_04__
#define __EFFVector2_2013_02_04__

EFFMATH_BEGIN



class EFFMATH_API EFFVector2 : public EFFVECTOR2
{
public:
	EFFVector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	EFFVector2(effFLOAT x, effFLOAT y)
	{
		this->x = x; 
		this->y = y;
	}

	EFFVector2(const EFFVector2 & v)
	{
		x = v.x;
		y = v.y;
	}

	inline effVOID SetValue(effFLOAT x, effFLOAT y)
	{
		this->x = x;
		this->y = y;
	}

	static effFLOAT Normalize(EFFVector2 & v)
	{
		return v.Normalize();
	}

	inline effFLOAT Normalize()
	{
		effFLOAT length = Length();
		if ( fabs(length) < EFFMATH_EPSILON )
		{
			return 0.0f;
		}
		else
		{
			x /= length;
			y /= length;

			return length;
		}
	}

	inline effFLOAT Length() const
	{
		return sqrtf(x * x + y * y);  
	}


	inline effFLOAT SquaredLength() const
	{ 
		return x * x + y * y;
	}

	inline effVOID Rotate(effFLOAT angle)
	{
		effFLOAT s = sinf(angle);
		effFLOAT c = cosf(angle);
		effFLOAT nx = x * c + y * s;
		y = -x * s + y * c;
		x = nx;
	}

	EFFVector2 & operator += (const EFFVector2 & v)
	{ 
		x += v.x;
		y += v.y;
		
		return *this; 
	}

	inline EFFVector2 & operator -= (const EFFVector2 & v)
	{ 
		x -= v.x;
		y -= v.y;
		
		return *this; 
	}

	inline EFFVector2 & operator *= (effFLOAT f)
	{ 
		x *= f;
		y *= f;
		
		return *this; 
	}

	inline EFFVector2 & operator /= (effFLOAT f)
	{
		x /= f;
		y /= f;
		
		return *this; 
	}

	inline EFFVector2 operator + () const
	{ 
		return EFFVector2(x, y); 
	}

	inline EFFVector2 operator - () const
	{ 
		return EFFVector2(-x, -y); 
	}

	friend inline EFFVector2 operator + (const EFFVector2 & v1, const EFFVector2 & v2)
	{ 
		return EFFVector2(v1.x + v2.x, v1.y + v2.y); 
	}

	friend inline EFFVector2 operator - (const EFFVector2 & v1, const EFFVector2 & v2)
	{ 
		return EFFVector2(v1.x - v2.x, v1.y - v2.y); 
	}

	friend inline effFLOAT operator * (const EFFVector2 & v1, const EFFVector2 & v2)
	{ 
		return v1.x * v2.x + v1.y * v2.y; 
	}

	friend inline EFFVector2 operator * (const EFFVector2 & v, effFLOAT f)
	{ 
		return EFFVector2(v.x * f, v.y * f); 
	}

	friend inline EFFVector2 operator * (effFLOAT f, const EFFVector2 & v)
	{ 
		return EFFVector2(v.x * f, v.y * f); 
	}

	friend inline EFFVector2 operator / (const EFFVector2 & v, effFLOAT f)
	{ 
		return EFFVector2(v.x / f, v.y / f); 
	}



	friend inline bool operator < (const EFFVector2 & v, effFLOAT f)
	{ 
		return fabs(v.x) < f && fabs(v.y) < f; 
	}

	friend inline bool operator > (effFLOAT f, const EFFVector2 & v)
	{ 
		return fabs(v.x) < f && fabs(v.y) < f; 
	}

};


EFFMATH_END

#endif