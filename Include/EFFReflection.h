/******************************************************************************
created:	2017-07-27   01:28
file path:	d:\EFF\Include\EFFReflection\EFFReflection.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFReflection_2017_07_27__
#define __EFFReflection_2017_07_27__

#include <Meta.h>
using namespace ursine;
using namespace meta;

#include <EFFReflectionModule.h>



#if defined UNICODE || defined _UNICODE
	#ifdef _DEBUG 
		#pragma comment(lib, "EFFReflection_d.lib")
	#else
		#pragma comment(lib, "EFFReflection.lib")
	#endif
#else
	#if _DEBUG 
		#pragma comment(lib, "EFFReflection_d.lib")
	#else
		#pragma comment(lib, "EFFReflection.lib")
	#endif
#endif


#endif