/********************************************************************
	created:	2012-03-13   21:358
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFMath\EFFVector4.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFVector4_2012_03_13__
#define __EFFVector4_2012_03_13__

EFFMATH_BEGIN

class EFFVector3;
class EFFMatrix4;

class EFFMATH_API EFFVector4 : public EFFVECTOR4
{
public:
	inline EFFVector4() {}

	inline EFFVector4(effFLOAT x, effFLOAT y, effFLOAT z, effFLOAT w = 0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	inline EFFVector4(const effFLOAT f[4])
	{
		x = f[X];
		y = f[Y];
		z = f[Z];
		w = f[W]; 
	}

	inline EFFVector4(const EFFVector3 & p, effFLOAT w = 0.0f);

	inline EFFVector4(const EFFVector4 & p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
		w = p.w;
	}

	~EFFVector4() {}

	inline EFFVector4 & Zero()
	{
		x = y = z = w = 0.0f;
		return *this;
	}

	inline EFFVector4 & Set(effFLOAT x, effFLOAT y, effFLOAT z, effFLOAT w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;

		return *this;
	}

	inline	EFFVector4 & Set(const effFLOAT f[4])
	{
		x  = f[X];
		y  = f[Y];
		z  = f[Z];
		w  = f[W];

		return *this;
	}

	inline	EFFVector4 & Set(const EFFVector4 & src)
	{ 
		x  = src.x;
		y  = src.y;
		z  = src.z;
		w = src.w;

		return *this;
	}

	inline	EFFVector4 & Add(effFLOAT _x, effFLOAT _y, effFLOAT _z, effFLOAT _w )
	{
		x += _x;	
		y += _y;
		z += _z;
		w += _w;

		return *this;
	}

	inline	EFFVector4 & Add(const EFFVector4 & v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	inline EFFVector4 & Sub(effFLOAT x, effFLOAT y, effFLOAT z, effFLOAT w )
	{
		this->x -= x;	
		this->y -= y;	
		this->z -= z;	
		this->w -= w;

		return *this;
	}
	inline	EFFVector4 &	 Sub(const EFFVector4 & v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;	
		return *this;	
	}

	inline	EFFVector4 &	 Mul(effFLOAT s) 
	{ 
		x *= s;
		y *= s;	
		z *= s;
		w *= s;	
		return *this;
	}

	effFLOAT Min() const
	{
		return MIN(x, MIN(y, MIN(z, w)));
	}

	effFLOAT Max() const
	{
		return MAX(x, MAX(y, MAX(z, w)));
	}

	EFFVector4 & Min(const EFFVector4 & p) 
	{ 
		x = MIN(x, p.x);
		y = MIN(y, p.y);
		z = MIN(z, p.z); 
		w = MIN(w, p.w);

		return *this;
	}

	EFFVector4 & Max(const EFFVector4& p) 
	{ 
		x = MAX(x, p.x);
		y = MAX(y, p.y);
		z = MAX(z, p.z);
		w = MAX(w, p.w);

		return *this;
	}

	//! Computes square magnitude
	inline effFLOAT SquareLength() const
	{
		return x * x + y * y + z * z + w * w;
	}

	//! Computes magnitude
	inline effFLOAT Length() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	inline EFFVector4 & Normalize()
	{
		effFLOAT M = Length();
		if ( M > EFFMATH_EPSILON )
		{
			M = 1.0f / M;
			x *= M;
			y *= M;
			z *= M;
			w *= M;
		}

		return *this;
	}


	inline EFFVector4 operator - () const
	{
		return EFFVector4(-x, -y, -z, -w);
	}

	inline EFFVector4 operator + (const EFFVector4 & p) const
	{
		return EFFVector4(x + p.x, y + p.y, z + p.z, w + p.w);
	}

	inline EFFVector4 operator - (const EFFVector4 & p) const
	{
		return EFFVector4(x - p.x, y - p.y, z - p.z, w - p.w);
	}

	inline EFFVector4 operator * (const EFFVector4 & p) const
	{
		return EFFVector4(x * p.x, y * p.y, z * p.z, w * p.w);
	}

	inline	EFFVector4 operator * (effFLOAT s) const
	{
		return EFFVector4(x * s, y * s, z * s, w * s);
	}

	inline friend EFFVector4 operator * (effFLOAT s, const EFFVector4 & p)
	{
		return EFFVector4(s * p.x, s * p.y, s * p.z, s * p.w);
	}

	inline EFFVector4 operator / (const EFFVector4 & p) const
	{
		return EFFVector4(x / p.x, y / p.y, z / p.z, w / p.w);
	}

	inline EFFVector4 operator / (effFLOAT s) const
	{
		s = 1.0f / s;
		return EFFVector4(x * s, y * s, z * s, w * s);
	}

	inline friend EFFVector4 operator / (effFLOAT s, const EFFVector4 & p)
	{
		return EFFVector4(s / p.x, s / p.y, s / p.z, s / p.w);
	}

	inline effFLOAT operator | (const EFFVector4 & p) const
	{
		return x * p.x + y * p.y + z * p.z + w * p.w;
	}


	inline EFFVector4 & operator += (const EFFVector4 & p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		w += p.w;
		
		return *this;
	}

	inline EFFVector4 & operator += (effFLOAT s)
	{
		x += s;
		y += s;
		z += s;
		w += s;
		
		return *this;
	}

	inline EFFVector4 & operator -= (const EFFVector4 & p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;
		w -= p.w;
		
		return *this;
	}

	inline EFFVector4 & operator -= (effFLOAT s)
	{
		x -= s;
		y -= s;
		z -= s;
		w -= s;
		
		return *this;
	}

	inline EFFVector4 & operator *= (const EFFVector4 & p)
	{
		x *= p.x;
		y *= p.y;
		z *= p.z;
		w *= p.w;
		
		return *this;
	}

	inline EFFVector4 & operator *= (effFLOAT s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		
		return *this;
	}

	inline EFFVector4 & operator /= (const EFFVector4 & p)
	{
		x /= p.x;
		y /= p.y;
		z /= p.z;
		w /= p.w;
		
		return *this;
	}

	inline EFFVector4 & operator /= (effFLOAT s)
	{
		s = 1.0f / s;
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		
		return *this;
	}

	EFFVector4 operator * (const EFFMatrix4 & m) const;

	EFFVector4 & operator *= (const EFFMatrix4 & m);


	inline operator EFFVector3() const;

};

EFFMATH_END

#endif