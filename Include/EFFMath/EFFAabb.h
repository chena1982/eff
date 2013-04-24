/******************************************************************************
	created:	2013-02-02   21:55
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFMath\EFFAabb.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFAabb_2013_02_02__
#define __EFFAabb_2013_02_02__


#include "EFFVector3.h"

EFFMATH_BEGIN

class EFFMatrix4;


class EFFAabb
{
public:
	EFFAabb() {}
	~EFFAabb() {}
public:
	inline effVOID SetMinMax(const EFFVector3 & min, const EFFVector3 & max)
	{
		this->min = min;
		this->max = max;
	}

	inline effVOID SetCenterExtent(const EFFVector3 & center, const EFFVector3 & extent)
	{
		min = center - extent;
		max = center + extent;
	}

	inline effVOID SetEmpty()
	{
		EFFVector3 p(0, 0, 0);
		min = -p;
		max = p;
	}

	inline effVOID SetPoint(const EFFVector3 & point)
	{
		min = max = point;
	}

	// Gets the size of the AABB. The size is defined as the longest extent.
	inline effFLOAT GetSize() const
	{
		EFFVector3 e = GetExtent();
		return e.Length();
	}

	inline effVOID SetPoints(const EFFVector3 * points, effINT count)
	{
		SetEmpty();
		for ( effINT i = 0; i < count; i++ )
		{
			Extend(*points);
			points++;
		}
	}

	inline effVOID Extend(const EFFVector3 & point)
	{

		if ( point.x > max.x )
		{
			max.x = point.x;
		}

		if ( point.x < min.x )
		{
			min.x = point.x;
		}

		if ( point.y > max.y )
		{
			max.y = point.y;
		}

		if ( point.y < min.y )
		{
			min.y = point.y;
		}

		if ( point.z > max.z )
		{
			max.z = point.z;
		}

		if ( point.z < min.z )
		{
			min.z = point.z;
		}
	}

	// Data access

	inline EFFVector3 GetMin() const { return min; }
	inline EFFVector3 GetMax() const { return max; }

	inline EFFVector3 GetCenter() const
	{ 
		return (max + min) * 0.5f;	
	}

	inline EFFVector3 GetExtent() const
	{
		return (max - min) * 0.5f;
	}


	//! Get box diagonal
	inline effVOID GetDiagonal(EFFVector3 & diagonal) const { diagonal = max - min; }
	inline effFLOAT GetWidth() const { return max.x - min.x; }
	inline effFLOAT GetHeight() const { return max.y - min.y; }
	inline effFLOAT GetDepth() const { return max.z - min.z; }

	inline effFLOAT GetVolume() const { return GetWidth() * GetHeight() * GetDepth(); }

	inline effBOOL Intersect(const EFFAabb & a) const
	{
		if ( max.x < a.min.x
			|| a.max.x < min.x
			|| max.y < a.min.y
			|| a.max.y < min.y
			|| max.z < a.min.z
			|| a.max.z < min.z )
		{
			return effFALSE;
		}

		return effTRUE;
	}

	inline effBOOL Intersect(const EFFAabb & a, effINT axis) const
	{
		if( max[axis] < a.min[axis] || a.max[axis] < min[axis] )
		{
			return effFALSE;
		}
		return effTRUE;
	}

	inline effVOID Rotate(const EFFMatrix4 & m, EFFAabb & aabb) const
	{
		// The three edges transformed: you can efficiently transform an X-only vector
		// by just getting the "X" column of the matrix
		
		EFFVector3 vx,vy,vz;
		m.GetRow(0, vx);
		vx *= (max.x - min.x);

		m.GetRow(1, vy);
		vy *= (max.y - min.y);

		m.GetRow(2, vz);
		vz *= (max.z - min.z);

		// Transform the min EFFVector3
		aabb.min = aabb.max = min * m;

		// Take the transformed min & axes and find new extents
		// Using CPU code in the right place is faster...
		if ( IS_NEGATIVE_FLOAT(vx.x) )
		{
			aabb.min.x += vx.x;
		}
		else
		{
			aabb.max.x += vx.x;
		}

		if ( IS_NEGATIVE_FLOAT(vx.y) )
		{
			aabb.min.y += vx.y;
		}
		else
		{
			aabb.max.y += vx.y;
		}
		
		if ( IS_NEGATIVE_FLOAT(vx.z) )
		{
			aabb.min.z += vx.z;
		}
		else
		{
			aabb.max.z += vx.z;
		}

		if ( IS_NEGATIVE_FLOAT(vy.x) )
		{
			aabb.min.x += vy.x;
		}
		else
		{
			aabb.max.x += vy.x;
		}

		if ( IS_NEGATIVE_FLOAT(vy.y) )
		{
			aabb.min.y += vy.y;
		}
		else
		{
			aabb.max.y += vy.y;
		}


		if ( IS_NEGATIVE_FLOAT(vy.z) )
		{
			aabb.min.z += vy.z;
		}
		else
		{	aabb.max.z += vy.z;
		}


		if ( IS_NEGATIVE_FLOAT(vz.x) )
		{
			aabb.min.x += vz.x;
		}
		else
		{
			aabb.max.x += vz.x;
		}

		if ( IS_NEGATIVE_FLOAT(vz.y) )
		{
			aabb.min.y += vz.y;
		}
		else
		{
			aabb.max.y += vz.y;
		}

		if ( IS_NEGATIVE_FLOAT(vz.z) )
		{
			aabb.min.z += vz.z;
		}
		else
		{
			aabb.max.z += vz.z;
		}
	}

	inline effBOOL IsValid() const
	{
		// Consistency condition for (Min, Max) boxes: min < max
		if ( min.x > max.x )
		{
			return effFALSE;
		}

		if ( min.y > max.y )
		{
			return effFALSE;
		}

		if ( min.z > max.z )
		{
			return effFALSE;
		}

		return effTRUE;
	}

	//! Operator for AABB *= effFLOAT. Scales the extents, keeps same center.
	inline EFFAabb & operator *= (effFLOAT s)
	{
		EFFVector3 center = GetCenter();
		EFFVector3 extent = GetExtent();
		SetCenterExtent(center, extent * s);

		return *this;
	}

	//! Operator for AABB /= effFLOAT. Scales the extents, keeps same center.
	inline EFFAabb & operator /= (effFLOAT s)
	{
		EFFVector3 center = GetCenter();
		EFFVector3 extent = GetExtent();
		SetCenterExtent(center, extent * (1.0f / s));
		return *this;
	}

	//! Operator for AABB += EFFVector3. Translates the box.
	inline EFFAabb & operator += (const EFFVector3 & trans)
	{
		min += trans;
		max += trans;
		return *this;
	}


protected:
	EFFVector3		min;
	EFFVector3		max;
};

EFFMATH_END

#endif