/******************************************************************************
	created:	2008-12-2   0:18
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9BaseDefines_2008_12_2__
#define __EFFD3D9BaseDefines_2008_12_2__

#	pragma comment (lib,"dxguid.lib")
#	pragma comment (lib,"dxerr9.lib")
# pragma comment (lib,"DxErr.lib")
#	pragma comment (lib,"d3d9.lib")
#	pragma comment (lib,"D3dx9.lib")
#	pragma comment (lib,"d3dxof.lib")


#ifndef EFFD3D9_EXPORTS
#ifndef _EFFLIB
#define EFFD3D9_API		__declspec(dllimport)
#else
#define EFFD3D9_API
#endif
#else
#define EFFD3D9_API		__declspec(dllexport)
#endif

#endif