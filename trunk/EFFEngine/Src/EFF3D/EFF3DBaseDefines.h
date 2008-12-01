/******************************************************************************
	created:	2008-12-1   22:37
	file path:	d:\EFF\EFFEngine\Src\EFF3D.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DBaseDefines_2008_12_1__
#define __EFF3DBaseDefines_2008_12_1__



#ifndef EFF3D_EXPORTS
	#ifndef _EFFLIB
		#define EFF3D_API		__declspec(dllimport)
	#else
		#define EFF3D_API
	#endif
#else
	#define EFF3D_API		__declspec(dllexport)
#endif

#endif