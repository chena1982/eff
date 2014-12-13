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



#include <EFFBase.h>
USE_EFFBASE

#include <EFFMath.h>
USE_EFFMATH

#include <wke\wke.h>

#include "EFF3D\EFF3DBaseDefines.h"
#include "EFF3D\EFF3DObject.h"
#include "EFF3D\EFF3DDevice.h"
#include "EFF3D\EFF3DTexture.h"
#include "EFF3D\EFF3DSurface.h"
#include "EFF3D\EFF3DIndexBuffer.h"
#include "EFF3D\EFF3DVertexBuffer.h"
#include "EFF3D\EFF3DVertexDeclaration.h"
#include "EFF3D\EFF3DShader.h"
#include "EFF3D\EFF3DMaterial.h"
#include "EFF3D\EFF3DResource.h"
#include "EFF3D\EFF3DResourceManager.h"
#include "EFF3D\EFF3DAsyncLoader.h"
#include "EFF3D\EFF3DTerrain.h"
#include "EFF3D\EFF3DCamera.h"
//#include "EFF3D/EFF3DForm.h"
#include "EFF3D\EFF3DImgui.h"
#include "EFF3D\EFF3DImguiRender.h"
#include "EFF3D\EFF3DFont.h"
#include "EFF3D\EFF3DWebGui.h"
#include "EFF3D\EFF3DHtmlWindow.h"

#if defined UNICODE || defined _UNICODE
#	ifdef _DEBUG 
#		pragma comment(lib,"EFF3D_d.lib")
#	else
#		pragma comment(lib,"EFF3D.lib")
#	endif
#else
#	if _DEBUG 
#		pragma comment(lib,"EFF3D_d.lib")
#	else
#		pragma comment(lib,"EFF3D.lib")
#	endif
#endif

/*#if defined _DEBUG
#	pragma comment(lib,"Awesomium_d.lib")
#else
#	pragma comment(lib,"Awesomium.lib")
#endif*/

#if defined _DEBUG
#	pragma comment(lib,"wke.lib")
#else
#	pragma comment(lib,"wke.lib")
#endif

#endif





