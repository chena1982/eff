/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\EFFEngine\Src\EFFD3D12.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D12BaseDefines_2019_03_23__
#define __EFFD3D12BaseDefines_2019_03_23__

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "dxguid.lib")


#ifndef EFFD3D12_EXPORTS
    #define EFFD3D12_API		__declspec(dllimport)
#else
    #define EFFD3D12_API		__declspec(dllexport)
#endif


// Primitives supported by draw-primitive API
typedef enum _D3DPRIMITIVETYPE {
	D3DPT_POINTLIST = 1,
	D3DPT_LINELIST = 2,
	D3DPT_LINESTRIP = 3,
	D3DPT_TRIANGLELIST = 4,
	D3DPT_TRIANGLESTRIP = 5,
	D3DPT_TRIANGLEFAN = 6,
	D3DPT_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
} D3DPRIMITIVETYPE;

struct PrimInfo
{
	D3DPRIMITIVETYPE type;
	effUINT min;
	effUINT div;
	effUINT sub;
};

static const PrimInfo s_primInfo[] =
{
	{ D3DPT_TRIANGLELIST,  3, 3, 0 },
	{ D3DPT_TRIANGLESTRIP, 3, 1, 2 },
	{ D3DPT_LINELIST,      2, 2, 0 },
	{ D3DPT_LINESTRIP,     2, 1, 1 },
	{ D3DPT_POINTLIST,     1, 1, 0 },
	{ D3DPRIMITIVETYPE(0), 0, 0, 0 },
};

#endif