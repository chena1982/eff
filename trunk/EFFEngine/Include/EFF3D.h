/********************************************************************
	created:	1:12:2008   22:26
	file path:	d:\EFF\EFFEngine\Src\EFF3D.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3D_1_12_2008__
#define __EFF3D_1_12_2008__


#include "EFF3D/EFF3DBaseDefines.h"
#include "EFF3D/EFF3DDevice.h"

#if defined UNICODE || defined _UNICODE
#	if _DEBUG 
#		pragma comment(lib,"EFF3DUD.lib")
#	else
#		pragma comment(lib,"EFF3DU.lib")
#	endif
#else
#	if _DEBUG 
#		pragma comment(lib,"EFF3DD.lib")
#	else
#		pragma comment(lib,"EFF3D.lib")
#	endif
#endif

#endif





