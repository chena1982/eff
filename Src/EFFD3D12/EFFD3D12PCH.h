// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once



#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include <cstdio>

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�



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