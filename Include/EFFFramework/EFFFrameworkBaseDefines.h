/******************************************************************************
	created:	2012-10-22   23:37
	file path:	d:\EFF\EFFEngine\Include\EFFFramework\EFFFrameworkBaseDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFFrameworkBaseDefines_2012_10_22__
#define __EFFFrameworkBaseDefines_2012_10_22__

#pragma comment (lib, "dwmapi.lib")


#define EFFFRAMEWORK_BEGIN		namespace EFFFramework {
#define EFFFRAMEWORK_END		}
#define USE_EFFFRAMEWORK		using namespace EFFFramework;

#ifndef EFFFRAMEWORK_EXPORTS
	#define EFFFRAMEWORK_API	__declspec(dllimport)
#else
	#define EFFFRAMEWORK_API	__declspec(dllexport)
#endif



#endif