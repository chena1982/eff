/********************************************************************
	created:	2013-02-04   22:57
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFSegment3.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFSegment3_2013_02_04__
#define __EFFSegment3_2013_02_04__

#include "EFFVector3.h"

EFFMATH_BEGIN

class EFFSegment3
{
public:
	EFFSegment3() {}
	~EFFSegment3() {}

	EFFSegment3(const EFFVector3 & s, const EFFVector3 & e) : start(s), end(e) {}

	EFFSegment3(const EFFSegment3 & seg)
	{
		start = seg.start;
		end = seg.end;
	}
public:
	inline EFFVector3 ComputeDirection() const
	{
		return end - start;
	}

	inline effFLOAT Length()
	{ 
		return (end - start).Length(); 
	}



	inline EFFVector3 GetCenter()
	{
		return (start + end) * 0.5f;
	}

	effFLOAT SquareDistance(const EFFVector3 & point) const;

	effFLOAT Distance(const EFFVector3 & point) const
	{
		return sqrtf(SquareDistance(point));
	}

public:
	EFFVector3 start;
	EFFVector3 end;
};

EFFMATH_END

#endif