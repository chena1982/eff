/******************************************************************************
	created:	2013-02-05   20:19
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFMath\EFFPlane3.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFPlane3_2013_02_05__
#define __EFFPlane3_2013_02_05__

#include "EFFVector3.h"

EFFMATH_BEGIN

class EFFVector4;
class EFFMatrix4;

class EFFPlane3
{
public:
	inline EFFPlane3() {}

	inline EFFPlane3(effFLOAT nx, effFLOAT ny, effFLOAT nz, effFLOAT d)
	{
		Set(nx, ny, nz, d);
	}

	inline EFFPlane3(const EFFVector3 & p, const EFFVector3 & n)
	{
		Set(p, n);
	}

	inline EFFPlane3(const EFFVector3 & p0, const EFFVector3 & p1, const EFFVector3 & p2)
	{ 
		Set(p0, p1, p2);
	}

	inline EFFPlane3(const EFFVector3 & n, effFLOAT d)
	{
		normal = n;
		this->d = d;
	}

	inline EFFPlane3(const EFFPlane3 & plane) : normal(plane.normal), d(plane.d) {}

	inline ~EFFPlane3() {}

	inline EFFPlane3 & Zero()
	{
		normal.Zero();
		d = 0.0f;
		return *this;
	}

	inline EFFPlane3 & Set(effFLOAT nx, effFLOAT ny, effFLOAT nz, effFLOAT d) 
	{
		normal.SetValue(nx, ny, nz);
		this->d = d;
		return *this;
	}

	inline EFFPlane3 & Set(const EFFVector3 & p, const EFFVector3 & n)	
	{
		normal = n;
		d = - p | n;
		return *this;
	}

	inline EFFPlane3 & Set(const EFFVector3 & p0, const EFFVector3 & p1, const EFFVector3 & p2)
	{
		EFFVector3 Edge0 = p1 - p0;
		EFFVector3 Edge1 = p2 - p0;

		normal = Edge0 % Edge1;
		normal.Normalize();

		d = -(p0 | normal);

		return *this;
	}

	inline effFLOAT Distance(const EFFVector3 & p) const
	{
		return (p | normal) + d;
	}

	inline effBOOL Belong(const EFFVector3 & p) const 
	{ 
		return fabsf(Distance(p)) < EFFMATH_EPSILON;
	}

	inline effVOID Normalize()
	{
		effFLOAT Denom = 1.0f / normal.Length();
		normal *= Denom;
		d *= Denom;
	}

	inline EFFVector3 GetNormal() const
	{
		return normal;
	}

	inline effFLOAT GetD()
	{
		return d;
	}

	effVOID ProjPoint(const EFFVector3 & point, EFFVector3 & result);
public:

	inline operator EFFVector3() const
	{
		return normal;
	}

	inline operator EFFVector4() const;

	inline EFFPlane3 operator * (const EFFMatrix4 & m) const
	{
		EFFPlane3 Ret(*this);
		return Ret *= m;
	}

	inline EFFPlane3 & operator *= (const EFFMatrix4 & m);

public:
	EFFVector3		normal;		//!< The normal to the plane
	effFLOAT		d;			//!< The distance from the origin
};

/*------------------------------------------------------------------------------------------------
* Transforms a plane by a 4x4 matrix. Same as Plane * Matrix4x4 operator, but faster.
* warning the plane normal must be unit-length
------------------------------------------------------------------------------------------------*/
inline effVOID TransformPlane(EFFPlane3 & result, const EFFPlane3 & plane, const EFFMatrix4 & transform)
{
	// Rotate the normal using the rotation part of the 4x4 matrix
	result.normal = plane.normal * transform;

	// Compute new d
	result.d = plane.d - (transform.GetTrans() | result.normal);
}

/*------------------------------------------------------------------------------------------------
* Transforms a plane by a 4x4 matrix. Same as Plane * Matrix4x4 operator, but faster.
* warning the plane normal must be unit-length
------------------------------------------------------------------------------------------------*/
inline effVOID TransformPlane(EFFPlane3 & plane, const EFFMatrix4 & transform)
{
	// Rotate the normal using the rotation part of the 4x4 matrix
	plane.normal *= transform;

	// Compute new d
	plane.d -= transform.GetTrans() | plane.normal;
}

/**
* Planes-triangle overlap test.
* clipMask [in] bitmask for active planes
* return effTRUE if triangle overlap planes
* warning THIS IS A CONSERVATIVE TEST !! Some triangles will be returned as intersecting, while they're not!
*/
inline effBOOL EFFPlaneTriOverlap(const EFFPlane3 * p, effDWORD clipMask, const EFFVector3 & v0, const EFFVector3 & v1, const EFFVector3 & v2)
{
	effDWORD Mask = 1;

	while ( Mask <= clipMask )
	{
		if ( clipMask & Mask )
		{
			effFLOAT d0 = p->Distance(v0);
			effFLOAT d1 = p->Distance(v1);
			effFLOAT d2 = p->Distance(v2);
			if ( d0 > 0.0f && d1 > 0.0f && d2 > 0.0f )
			{
				return effFALSE;
			}
		}

		Mask += Mask;
		p++;
	}

	return effTRUE;
}


/**
* Planes-AABB overlap test.
*/
inline effBOOL EFFPlaneAABBOverlap(const EFFPlane3 * p, effDWORD inClipMask, effDWORD & outClipMask, const EFFVector3 & center,
								const EFFVector3 & extent)
{


	// Evaluate through all active frustum planes. We determine the relation 
	// between the AABB and a plane by using the concept of "near" and "far"
	// vertices originally described by Zhang. Our
	// variant here uses 3 fabs ops, 6 muls, 7 adds and two floating point
	// comparisons per plane. The routine early-exits if the AABB is found
	// to be outside any of the planes. The loop also constructs a new output
	// clip mask. Most FPUs have a native single-cycle fabsf() operation.

	effDWORD Mask = 1;					// current mask index (1,2,4,8,..)
	effDWORD TmpOutClipMask	= 0;		// initialize output clip mask into empty. 

	while ( Mask <= inClipMask )		// keep looping while we have active planes left...
	{
		if ( inClipMask & Mask )		// if clip plane is active, process it..
		{               
			effFLOAT NP = extent.x * fabsf(p->normal.x) + extent.y * fabsf(p->normal.y) + extent.z * fabsf(p->normal.z);	// ### fabsf could be precomputed
			effFLOAT MP = center.x * p->normal.x + center.y * p->normal.y + center.z * p->normal.z + p->d;

			if ( NP < MP )					// near vertex behind the clip plane... 
			{
				return effFALSE;			// .. so there is no intersection..
			}

			if ( (-NP) < MP )				// near and far vertices on different sides of plane..
			{
				TmpOutClipMask |= Mask;		// .. so update the clip mask...
			}
		}

		Mask += Mask;						// mk = (1<<plane)
		p++;								// advance to next plane
	}

	outClipMask = TmpOutClipMask;			// copy output value (temp used to resolve aliasing!)
	return effTRUE;							// indicate that AABB intersects frustum
}


EFFMATH_END

#endif
