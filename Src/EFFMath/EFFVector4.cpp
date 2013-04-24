/********************************************************************
	created:	2012-03-13   21:358
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFMath\EFFVector4.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/
#include "stdafx.h"

#include "EFFMathFunction.h"
#include "EFFVector4.h"
#include "EFFVector3.h"

EFFMATH_BEGIN

inline EFFVector4::EFFVector4(const EFFVector3 & p, effFLOAT w)
{
	x = p.x;
	y = p.y;
	z = p.z;
	this->w = w;
}


inline EFFVector4 EFFVector4::operator * (const EFFMatrix4 & m) const
{
	return EFFVector4(x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0],
					  x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1],
					  x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2],
					  x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3]);
}

inline EFFVector4 & EFFVector4::operator *= (const EFFMatrix4 & m)
{
	float xp = x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0];
	float yp = x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1];
	float zp = x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2];
	float wp = x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3];

	x = xp;
	y = yp;
	z = zp;
	w = wp;

	return *this;
}

inline EFFVector4::operator EFFVector3() const
{
	return EFFVector3(x, y, z);
}

EFFMATH_END