/********************************************************************
	created:	2012-03-13   21:358
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFMath\EFFVector4.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFVector4_2012_03_13__
#define __EFFVector4_2012_03_13__

EFFMATH_BEGIN

class EFFMATH_API EFFVector4
{
public:
	EFFVector4() {}
	EFFVector4(effFLOAT x, effFLOAT y, effFLOAT z, effFLOAT w) : x(x), y(y), z(z), w(w) {}



public:
	effFLOAT		x;
	effFLOAT		y;
	effFLOAT		z;
	effFLOAT		w;


};

EFFMATH_END

#endif