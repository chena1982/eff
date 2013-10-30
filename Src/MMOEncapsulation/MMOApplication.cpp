/******************************************************************************
	created:	2012-10-22   23:43
	file path:	d:\EFF\EFFEngine\Src\MMOEncapsulation\MMOApplication.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "MMOApplication.h"

#define MAX_LOADSTRING 100
#define MEMFILE_SIZE 256

HINSTANCE hInst;								// 当前实例
HWND hWnd;

TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

effBOOL	appExit = effFALSE;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;



	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		return DefWindowProc(hWnd, message, wParam, lParam);
		break;


    case WM_KEYDOWN:
        {
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			inputManager->SetKeyDown(inputManager->ConvertOsVKey2EFFVKey(wParam));
        }
        break;

    case WM_KEYUP:
        {
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			inputManager->SetKeyUp(inputManager->ConvertOsVKey2EFFVKey(wParam));
        }
        break;

	case WM_MOUSEMOVE:
		{
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			effINT xPos = GET_X_LPARAM(lParam); 
			effINT yPos = GET_Y_LPARAM(lParam);
			inputManager->SetPos(xPos, yPos);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			inputManager->SetLeftButtonDown(effTRUE);
		}
		break;
	case WM_LBUTTONUP:
		{
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			inputManager->SetLeftButtonDown(effFALSE);
		}
		break;
	case WM_MBUTTONDOWN:
		{
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			inputManager->SetMiddleButtonDown(effTRUE);
		}
		break;
	case WM_MBUTTONUP:
		{
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			inputManager->SetMiddleButtonDown(effFALSE);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			inputManager->SetRightButtonDown(effTRUE);
		}
		break;
	case WM_RBUTTONUP:
		{
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			inputManager->SetRightButtonDown(effFALSE);
		}
		break;
	case WM_ACTIVATE:
		{
			EFF3DDevice * device = EFF3DGetDevice();
			if ( device != NULL )
			{
				EFFInputManager * inputManager = device->GetInputManager();
				if ( wParam == WA_INACTIVE )
				{
					inputManager->SetLeftButtonDown(effFALSE);
				}
			}
		}
		break;
	case WM_DESTROY:
		appExit = effTRUE;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}


MMOApplication::MMOApplication()
{
	device = NULL;
	memFile = NULL;
	backGroundColor = 0xFF4c4c52;
}

MMOApplication::~MMOApplication()
{
	SF_RELEASE(device);
    CloseHandle(memFile); 
}


EFF3DTerrain * terrain = NULL;
EFF3DCamera * camera = NULL;


EFF3DFont * font = NULL;

effBOOL MMOApplication::Init(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host)
{
	if ( !CreateAppWindow(window, width, height, multiProcess, host) )
	{
		return effFALSE;
	}

	Create3DDevice(L"EFFD3D9UD.dll", &device, window, hWnd, width, height);

	if ( device == NULL )
	{
		return effFALSE;
	}

	//terrain = TestMyTerrain();

	camera = new EFF3DCamera();
	camera->SetViewParams(EFFVector3(0.0f, 15.0f, -15.0f), EFFVector3(0.0f, 0.0f, 0.0f));

	RECT rect;
	GetClientRect(hWnd, &rect);
	effFLOAT aspect = ((effFLOAT)(rect.bottom - rect.top)) / ((effFLOAT)(rect.right - rect.left));
	camera->SetProjParams(PI * 0.5f, aspect, 1.0f, 500.0f);

	//device->GetSceneManager();
	imguiRenderInit(_effT("Font\\msyh.ttf"));

	return effTRUE;
}


const int TICKS_PER_SECOND = 25;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

effVOID MMOApplication::Run()
{


    effUINT nextGameTick = GetTickCount();
    effINT loops;
    effFLOAT interpolation;

	//这里的主循环是Update限帧，但是Render不限帧
    while ( !appExit )
	{
        loops = 0;
        while ( GetTickCount() > nextGameTick && loops < MAX_FRAMESKIP )
		{
			//先处理所有的windows消息
			MSG msg;
			while ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) )
			{
				/*effTCHAR temp[64];
				swprintf_s(temp, 64, L"receive msg %d.\n", msg.message);
				OutputDebugString(temp);*/

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}

            Update();
            nextGameTick += SKIP_TICKS;
            loops++;
        }

        interpolation = effFLOAT(GetTickCount() + SKIP_TICKS - nextGameTick) / effFLOAT(SKIP_TICKS);
        Render(interpolation);
    }


	return;
}


effVOID	MMOApplication::SetBackGroundColor(effUINT color)
{
	backGroundColor = color;
}

effBOOL MMOApplication::CreateAppWindow(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host)
{
	hInst = GetModuleHandle(NULL);

	_tcscpy_s(szWindowClass, _effT("EFF Application"));

	MyRegisterClass(hInst);

	RECT windowRect;
	SetRect(&windowRect, 0, 0, width, height);
	effUINT style = 0;
	effUINT exStyle = 0;

	if ( multiProcess )
	{
		if ( host )
		{
			style = WS_OVERLAPPEDWINDOW;
		}
		else
		{
			effBOOL comp;
			DwmIsCompositionEnabled(&comp);

			if ( !comp )
			{
				UnregisterClass(szWindowClass, hInst);
				return effFALSE;
			}

			style = WS_POPUP;
			exStyle = WS_EX_COMPOSITED;
		}
	}
	else
	{
		style = WS_OVERLAPPEDWINDOW;
	}



	AdjustWindowRect(&windowRect, style, FALSE);

	hWnd = CreateWindowEx(exStyle, szWindowClass, szTitle, style,
		0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInst, NULL);

	if ( !hWnd )
	{
		return effFALSE;
	}

	if ( multiProcess )
	{
		if ( host )
		{
			CreateMemFile();
		}
		else
		{
			ReadHWNDFromMemFile();
			if ( mpwh.hWndMain != NULL )
			{
				//SetParent(hWnd, mpwh.hWndMain);
			}

			MARGINS mgDWMMargins = {-1, -1, -1, -1};
			DwmExtendFrameIntoClientArea(hWnd, &mgDWMMargins);
		}
	}


	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);


	return effTRUE;
}

effBOOL MMOApplication::CreateMemFile()
{
    //创建一个内存镜像文件 
    memFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MEMFILE_SIZE, _effT("EFFEngine"));

    if ( memFile == NULL )
    {
        return effFALSE; 
    }

    effVOID * address = MapViewOfFile(memFile, FILE_MAP_ALL_ACCESS, 0, 0, MEMFILE_SIZE);

	mpwh.hWndMain = hWnd;
    CopyMemory(address, &mpwh, sizeof(mpwh)); 
    UnmapViewOfFile(address);

	return effTRUE;
}

effVOID MMOApplication::ReadHWNDFromMemFile()
{

    //创建一个内存镜像文件 
    memFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MEMFILE_SIZE, _effT("EFFEngine"));

    if ( memFile == NULL )
    {
        return; 
    }

    effVOID * address = MapViewOfFile(memFile, FILE_MAP_ALL_ACCESS, 0, 0, MEMFILE_SIZE);

    CopyMemory(&mpwh, address, sizeof(mpwh)); 
    UnmapViewOfFile(address); 
}





effVOID	MMOApplication::Update()
{
	EFF3DDevice * device = EFF3DGetDevice();
	if ( device == NULL )
	{
		return;
	}

	EFFInputManager * inputManager = device->GetInputManager();
	inputManager->Update();
}


effVOID MMOApplication::Render(effFLOAT elapsedTime)
{
	device->Clear(0, NULL, EFF3DCLEAR_TARGET | EFF3DCLEAR_ZBUFFER, backGroundColor, 1.0f, 0);
	if ( device->BeginScene() )
	{
		wkeUpdate();
		OnRenderGUI(elapsedTime);

		device->EndScene();
		device->Present(NULL, NULL);
	}
}




