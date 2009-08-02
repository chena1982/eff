/******************************************************************************
	created:	2008-12-13   0:32
	file path:	d:\EFF\EFFEngine\Include\EFFMath\EFFMathBaseDefines.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMathBaseDefines_2008_12_13__
#define __EFFMathBaseDefines_2008_12_13__



#ifndef EFFMATH_EXPORTS
	#ifndef _EFFMATHLIB
		#define EFFMATH_API		__declspec(dllimport)
	#else
		#define EFFMATH_API
	#endif
#else
	#define EFFMATH_API		__declspec(dllexport)
#endif


#endif
