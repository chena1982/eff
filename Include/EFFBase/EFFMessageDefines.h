/******************************************************************************
	created:	2014-12-06   23:18
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFMessageDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMessageDefines_2014_12_06__
#define __EFFMessageDefines_2014_12_06__


#define RequestGameWindowPosAndSize 0


#define SendGameWindowPosAndSize 1

struct GameWindowPosAndSize
{
	effINT id;
	effINT x;
	effINT y;
	effINT width;
	effINT height;
};


#endif