/******************************************************************************
	created:	2013-7-12   20:37
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFOSVersion.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFOSVersion.h"


EFFBASE_BEGIN

#ifdef _WIN32

EFFOSVERSION GetOSVersion()
{
	if (IsWindowsXPOrGreater())
	{
		return WinXP;
	}
	else if (IsWindows8OrGreater())
	{
		return Win8;
	}
	else if (IsWindows7OrGreater())
	{
		return Win7;
	}

	return OS_Unknown;
}

#endif

EFFBASE_END

