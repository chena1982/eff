/******************************************************************************
	created:	2012-10-23   0:04
	file path:	d:\EFF\EFFEngine\Include\MMOEncapsulation.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __MMOEncapsulation_2012_10_23__
#define __MMOEncapsulation_2012_10_23__

#include "MMOEncapsulation/MMOEncapsulationBaseDefines.h"
#include "MMOEncapsulation/MMOApplication.h"

#if defined UNICODE || defined _UNICODE
#	ifdef _DEBUG 
#		pragma comment(lib,"MMOEncapsulationUD.lib")
#	else
#		pragma comment(lib,"MMOEncapsulationU.lib")
#	endif
#else
#	if _DEBUG 
#		pragma comment(lib,"MMOEncapsulationD.lib")
#	else
#		pragma comment(lib,"MMOEncapsulation.lib")
#	endif
#endif

#endif



