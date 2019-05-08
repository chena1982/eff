// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once



#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <cstdio>

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

// TODO: 在此处引用程序需要的其他头文件



//#include <Cg/cg.h>
//#include <Cg/cgD3D9.h>

#define D3D_DEBUG_INFO

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
//#include <DxErr.h>

// D3D12 extension library.
#include "d3dx12.h"


#include "EFF3D.h"
USE_EFF3D

#include "EFFD3D12BaseDefines.h"