/******************************************************************************
	created:	2008-12-12   22:59
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFRect.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFRect_2008_12_12__
#define __EFFRect_2008_12_12__


EFFMATH_BEGIN

struct EFFRECT
{
	effFLOAT		left;
	effFLOAT		top;
	effFLOAT		right;
	effFLOAT		bottom;
};

class EFFMATH_API EFFRect : public EFFRECT
{
public:
	EFFRect() {}
	EFFRect(effFLOAT x, effFLOAT y, effFLOAT width, effFLOAT height)
	{
		left = x;
		top = y;
		right = x + width;
		bottom = y + height;
	}

	~EFFRect() {}
public:

};

EFFMATH_END

#endif