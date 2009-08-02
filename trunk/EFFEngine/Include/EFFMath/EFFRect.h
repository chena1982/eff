/******************************************************************************
	created:	2008-12-12   22:59
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFRect.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFRect_2008_12_12__
#define __EFFRect_2008_12_12__


struct EFFRECT
{
	effLONG		nLeft;
	effLONG		nTop;
	effLONG		nRight;
	effLONG		nBottom;
};

class EFFMATH_API EFFRect : public EFFRECT
{
public:
	EFFRect() {}
	~EFFRect() {}
public:

};

#endif