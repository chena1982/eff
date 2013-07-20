/******************************************************************************
	created:	2012-10-22   23:37
	file path:	d:\EFF\EFFEngine\Include\MMOEncapsulation\MMOEncapsulationBaseDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __MMOEncapsulationBaseDefines_2012_10_22__
#define __MMOEncapsulationBaseDefines_2012_10_22__

#pragma comment (lib, "dwmapi.lib")

#ifndef MMOENCAPSULATION_EXPORTS
	#ifndef _EFFLIB
		#define MMO_API		__declspec(dllimport)
	#else
		#define MMO_API
	#endif
#else
	#define MMO_API			__declspec(dllexport)
#endif


#endif