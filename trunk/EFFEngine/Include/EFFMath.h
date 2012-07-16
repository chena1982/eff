/******************************************************************************
	created:	2008-12-12   23:03
	file path:	d:\EFF\EFFEngine\Include\EFFMath.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMath_2008_12_12__
#define __EFFMath_2008_12_12__

#include ".\EFFMath\EFFMathBaseDefines.h"
#include ".\EFFMath\EFFRect.h"
#include ".\EFFMath\EFFMatrix4.h"
#include ".\EFFMath\EFFVector3.h"
#include ".\EFFMath\EFFVector4.h"

#if defined UNICODE || defined _UNICODE
	#ifdef _DEBUG 
		#pragma comment(lib,"EFFMathUD.lib")
	#else
		#pragma comment(lib,"EFFMathU.lib")
	#endif
#else
	#if _DEBUG 
		#pragma comment(lib,"EFFMathD.lib")
	#else
		#pragma comment(lib,"EFFMath.lib")
	#endif
#endif

#endif