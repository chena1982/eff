/********************************************************************
	created:	1:12:2008   22:26
	file path:	d:\EFF\EFFEngine\Src\EFF3D.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3D_1_12_2008__
#define __EFF3D_1_12_2008__


#include <Python.h>

#include "EFF3D/EFF3DBaseDefines.h"
#include "EFF3D/EFF3DDevice.h"
#include "EFF3D/EFF3DTexture.h"
#include "EFF3D/EFF3DSurface.h"
#include "EFF3D/EFF3DIndexBuffer.h"
#include "EFF3D/EFF3DVertexBuffer.h"
#include "EFF3D/EFF3DVertexDeclaration.h"
#include "EFF3D/EFF3DEffect.h"
#include "EFF3D/EFF3DMaterial.h"
#include "EFF3D/EFF3DResource.h"
#include "EFF3D/EFF3DResourceManager.h"

#if defined UNICODE || defined _UNICODE
#	ifdef _DEBUG 
#		pragma comment(lib,"EFF3D.lib")
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





