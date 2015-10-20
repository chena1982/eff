/******************************************************************************
	created:	2013-7-12   20:37
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFOSVersion.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFOSVersion_2013_7_12__
#define __EFFOSVersion_2013_7_12__



EFFBASE_BEGIN

enum EFFOSVERSION
{
	WinXP,
	Win2003,
	Win7,
	Win8,
	OS_Unknown
};



#ifdef _WIN32
	EFFBASE_API EFFOSVERSION GetOSVersion();
#endif

EFFBASE_END


#endif



