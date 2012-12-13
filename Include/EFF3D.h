/********************************************************************
	created:	1:12:2008   22:26
	file path:	d:\EFF\EFFEngine\Src\EFF3D.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3D_1_12_2008__
#define __EFF3D_1_12_2008__


//#include <Python.h>
//#include "WebCore.h"

#include "EFF3D/EFF3DBaseDefines.h"
#include "EFF3D/EFF3DObject.h"
#include "EFF3D/EFF3DDevice.h"
#include "EFF3D/EFF3DTexture.h"
#include "EFF3D/EFF3DSurface.h"
#include "EFF3D/EFF3DIndexBuffer.h"
#include "EFF3D/EFF3DVertexBuffer.h"
#include "EFF3D/EFF3DVertexDeclaration.h"
#include "EFF3D/EFF3DShader.h"
#include "EFF3D/EFF3DMaterial.h"
#include "EFF3D/EFF3DResource.h"
#include "EFF3D/EFF3DResourceManager.h"
#include "EFF3D/EFF3DAsyncLoader.h"
#include "EFF3D/EFF3DTerrain.h"
//#include "EFF3D/EFF3DForm.h"

#if defined UNICODE || defined _UNICODE
#	ifdef _DEBUG 
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

/*#if defined _DEBUG
#	pragma comment(lib,"Awesomium_d.lib")
#else
#	pragma comment(lib,"Awesomium.lib")
#endif*/

#endif





