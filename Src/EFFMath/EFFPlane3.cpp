/******************************************************************************
	created:	2013-02-05   20:23
	file path:	d:\EFF\EFFEngine\Src\EFFMath\EFFPlane3.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/
#include "stdafx.h"

#include "EFFMathFunction.h"
#include "EFFPlane3.h"
#include "EFFVector4.h"
#include "EFFMatrix4.h"


EFFMATH_BEGIN


effVOID EFFPlane3::ProjPoint(const EFFVector3 & point, EFFVector3 & result)
{
	effFLOAT distance = Distance(point);
	result = point - distance * normal;
}

inline EFFPlane3::operator EFFVector4() const
{
	return EFFVector4(normal, d);
}

inline EFFPlane3 & EFFPlane3::operator *= (const EFFMatrix4 & m)
{
	EFFVector3 n2 = EFFVector4(normal, 0.0f) * m;
	d = -((EFFVector3)(EFFVector4(-d * normal, 1.0f ) * m) | n2);
	normal = n2;
	return *this;
}

EFFMATH_END