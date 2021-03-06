/******************************************************************************
	created:	2012-10-22   23:43
	file path:	d:\EFF\EFFEngine\Src\EFFFramework\EFFApplication.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFFrameworkPCH.h"
#include "EFFApplication.h"
#include "EFFFrameworkMessageManager.h"

EFFFRAMEWORK_BEGIN

#define MAX_LOADSTRING 100
#define MEMFILE_SIZE 256

HINSTANCE hInst;								// 当前实例

TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

effBOOL	appExit = effFALSE;

EFFApplication * application = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	static effBOOL moveorsizing = effFALSE;
	static effINT width = 0;
	static effINT height = 0;

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
			inputManager->SetKeyDown(inputManager->ConvertOsVKey2EFFVKey((effUINT)wParam));
        }
        break;

    case WM_KEYUP:
        {
			EFFInputManager * inputManager = EFF3DGetDevice()->GetInputManager();
			inputManager->SetKeyUp(inputManager->ConvertOsVKey2EFFVKey((effUINT)wParam));
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
				if (wParam == WA_INACTIVE)
				{
					inputManager->SetLeftButtonDown(effFALSE);
				}
			}
		}
		break;
	case WM_MOVE:
		{
			EFFApplication * application = EFFGetApplication();

			application->OnWindowMove();
		}
		break;
	case WM_SIZE:
		{
			EFFApplication * application = EFFGetApplication();

			if (SIZE_MAXIMIZED == wParam || SIZE_RESTORED == wParam)
			{
				width = LOWORD(lParam);
				height = HIWORD(lParam);

				application->SetWindowMinimized(effFALSE);

				if (!moveorsizing && application->IsWindowSizeChanged(width, height))
				{
					application->OnWindowResize(width, height);
				}
			}
			else if (SIZE_MINIMIZED == wParam)
			{
				application->SetWindowMinimized(effTRUE);
			}
		}
		break;
	case WM_ENTERSIZEMOVE:
		{
			moveorsizing = effTRUE;
		}
		break;
	case WM_EXITSIZEMOVE:
		{
			moveorsizing = effFALSE;

			EFFApplication * application = EFFGetApplication();
			if (application->IsWindowSizeChanged(width, height))
			{
				application->OnWindowResize(width, height);
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



EFFApplication::EFFApplication()
{
	device = NULL;
	memFile = NULL;
	//backGroundColor = 0xFF4c4c52;
	backGroundColor = 0x80000000;
	host = effFALSE;
    connectedToHost = effFALSE;
	connectedToClient = effFALSE;
	window = effFALSE;
	hWnd = NULL;

	width = 0;
	height = 0;
	minimized = effFALSE;

	server = NULL;
	client = NULL;

	application = this;
}

EFFApplication::~EFFApplication()
{
	SF_RELEASE(device);
    CloseHandle(memFile);

	ShutdownServerAndClient();
}


EFF3DTerrain * terrain = NULL;
EFF3DCamera * camera = NULL;


EFF3DFont * font = NULL;

effBOOL EFFApplication::Init(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host)
{
	if ( !CreateAppWindow(window, width, height, multiProcess, host) )
	{
		return effFALSE;
	}

#ifdef _DEBUG
	Create3DDevice(L"EFFD3D12_d.dll", &device, window, host, hWnd, width, height);
#else
	Create3DDevice(L"EFFD3D12.dll", &device, window, host, hWnd, width, height);
#endif

	if ( device == NULL )
	{
		return effFALSE;
	}

	this->window = window;
    this->host = host;


	//terrain = TestMyTerrain();

	camera = new EFF3DCamera();
	camera->SetViewParams(EFFVector3(16.0f, 460.0f, -32.0f), EFFVector3(16.0f, 435.0f, -16.0f));

	RECT rect;
	GetClientRect(hWnd, &rect);
	effFLOAT aspect = ((effFLOAT)(rect.bottom - rect.top)) / ((effFLOAT)(rect.right - rect.left));
	camera->SetProjParams(PI * 0.5f, aspect, 1.0f, 500.0f);

	//device->GetSceneManager();
	//imguiRenderInit(_effT("Font\\msyh.ttf"));

    device->OnNotifyHostStartRendering += EFFEventCall(this, &EFFApplication::OnNotifyHostStartRendering);

	InitServer();
	InitClient();

	return effTRUE;
}


const int TICKS_PER_SECOND = 25;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

effVOID EFFApplication::Run()
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


		if (host && connectedToClient)
		{
			EFF3DSharedTexture * sharedTexture = device->GetSharedRenderTarget();
			if (sharedTexture != NULL)
			{
				sharedTexture->NotifyClientStartRendering();
			}
		}

		if (!host && connectedToHost)
		{
			EFF3DSharedTexture * sharedTexture = device->GetSharedRenderTarget();

			sharedTexture->ClientWaitToStartRendering();

			EFF3DTextureHandle shared3DTexture = sharedTexture->GetClientTexture();
			device->SetRenderTarget(0, shared3DTexture);
		}

		if (!minimized && !appExit)
		{
			Render(interpolation);
			EFF3DTimeQuery * timeQuery = EFF3DGPUProfiler::GlobalProfiler.EndFrame();

			if (timeQuery != NULL && !host && connectedToHost)
			{
				device->GetSharedRenderTarget()->NotifyHostStartRendering(timeQuery->currentFrame);
			}
		}

        //让client渲染最后一帧，否则有可能client正在wait，这样的话client无法退出
        if (host && connectedToClient && appExit)
        {
            EFF3DSharedTexture * sharedTexture = device->GetSharedRenderTarget();
            if (sharedTexture != NULL)
            {
                sharedTexture->NotifyClientStartRendering();
            }
        }
    }

	return;
}

effVOID EFFApplication::Quit()
{
    appExit = effTRUE;
}


effVOID	EFFApplication::SetBackGroundColor(effUINT color)
{
	backGroundColor = color;
}

effBOOL EFFApplication::CreateAppWindow(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host)
{
	hInst = GetModuleHandle(NULL);

	_tcscpy_s(szWindowClass, _effT("EFF Application"));

	MyRegisterClass(hInst);

	RECT windowRect;
	SetRect(&windowRect, 0, 0, width, height);
	effUINT style = 0;
	effUINT exStyle = 0;

	/*if ( multiProcess )
	{
		if ( host )
		{
			style = WS_OVERLAPPEDWINDOW;
			this->host = effTRUE;
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
	else*/

	if (host)
	{
		style = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		style = WS_POPUP;
	}

	AdjustWindowRect(&windowRect, style, FALSE);

	hWnd = CreateWindowEx(exStyle, szWindowClass, szTitle, style,
		0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInst, NULL);

	if ( !hWnd )
	{
		return effFALSE;
	}

	/*if ( multiProcess )
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
				GetClientRect(mpwh.hWndMain, &windowRect);
				ClientToScreen(mpwh.hWndMain, reinterpret_cast<POINT *>(&windowRect.left));
				ClientToScreen(mpwh.hWndMain, reinterpret_cast<POINT *>(&windowRect.right));
				::MoveWindow(hWnd, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, TRUE);
				
				SetParent(hWnd, mpwh.hWndMain);
			}

			MARGINS mgDWMMargins = {-1, -1, -1, -1};
			//MARGINS mgDWMMargins = {0, 0, 0, 25};
			DwmExtendFrameIntoClientArea(hWnd, &mgDWMMargins);

			//DWM_BLURBEHIND bb = {0};

			//HRGN rect = CreateRectRgn(-1, -1, 0, 0);

			//// Enable Blur Behind and apply to the entire client area
			//bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
			//bb.fEnable = FALSE;
			//bb.hRgnBlur = rect;
			//bb.fTransitionOnMaximized = 0;

			//// Apply Blur Behind
			//DwmEnableBlurBehindWindow(hWnd, &bb);

		}
	}*/

	if (host)
	{
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}


	OnWindowResize += EFFEventCall(this, &EFFApplication::WindowResized);

	return effTRUE;
}

effVOID EFFApplication::MoveWindow(effINT x, effINT y, effINT width, effINT height)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	if ( rc.right - rc.left == width && rc.bottom - rc.top == height )
	{
		//::MoveWindow(hWnd, x, y, width, height, effFALSE);
		::SetWindowPos(hWnd, HWND_TOPMOST, x, y, width, height, SWP_NOSIZE);
		SendWindowPosAndSize();
	}
	else
	{
		::MoveWindow(hWnd, x, y, width, height, effFALSE);
		SendWindowPosAndSize();
		device->Reset(window, width, height);
	}
}

effVOID EFFApplication::WindowResized(effINT width, effINT height)
{
	this->width = width;
	this->height = height;

	SendWindowPosAndSize();

	device->Reset(window, width, height);
}

effBOOL EFFApplication::IsWindowSizeChanged(effINT width, effINT height)
{
	return this->width != width || this->height != height;
}

effVOID EFFApplication::SetWindowMinimized(effBOOL minimized)
{
	this->minimized = minimized;
}

effVOID EFFApplication::OnNotifyHostStartRendering(effUINT index)
{
    SendSharedTextureIndexFromMemFile(index);
}

effBOOL EFFApplication::SendSharedTextureIndexFromMemFile(effUINT index)
{
    //创建一个内存镜像文件 
    memFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MEMFILE_SIZE, _effT("EFFEngine"));

    if ( memFile == NULL )
    {
        return effFALSE; 
    }

    effVOID * address = MapViewOfFile(memFile, FILE_MAP_ALL_ACCESS, 0, 0, MEMFILE_SIZE);

	//mpwh.hWndMain = hWnd;
    //CopyMemory(address, &mpwh, sizeof(mpwh)); 
    
    CopyMemory(address, &index, sizeof(index));
    
    UnmapViewOfFile(address);

	return effTRUE;
}

effUINT EFFApplication::ReadSharedTextureIndexFromMemFile()
{
    if ( memFile == NULL )
    {
        //创建一个内存镜像文件 
        memFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MEMFILE_SIZE, _effT("EFFEngine"));
    }

    if (memFile == NULL)
    {
        return -1;
    }

    effVOID * address = MapViewOfFile(memFile, FILE_MAP_ALL_ACCESS, 0, 0, MEMFILE_SIZE);

    effUINT index;

    CopyMemory(&index, address, sizeof(index));
    UnmapViewOfFile(address); 

    return index;
}





effVOID	EFFApplication::Update()
{
	EFF3DDevice * device = EFF3DGetDevice();
	if ( device == NULL )
	{
		return;
	}

	device->Update();

    OnUpdate();
}


effVOID EFFApplication::Render(effFLOAT elapsedTime)
{
    EFF3DGPUProfileBlock profileBlock(_effT("Render"));

	device->ResetCommandBuffer();

	device->Clear(0, NULL, EFF3D_CLEAR_COLOR | EFF3D_CLEAR_DEPTH, backGroundColor, 1.0f, 0);
	if ( device->BeginScene() )
	{
		EFFMatrix4 rotate;
		rotate.RotationMatrixX(PI);
		rotate.Identity();
		//device->SetTransform(EFF3DTS_WORLD, &rotate);
		//device->SetTransform(EFF3DTS_VIEW, &camera->GetViewMatirx());
		//device->SetTransform(EFF3DTS_PROJECTION, &camera->GetProjMatrix());
		//device->SetRenderState(EFF3DRS_CULLMODE, EFF3DCULL_NONE);
		//device->SetRenderState(EFF3DRS_ALPHABLENDENABLE, effFALSE);

		EFFVector4 v(0.0f, 442.770f, 32, 1);
		v *= camera->GetViewMatirx();
		v *= camera->GetProjMatrix();

		if (!host && connectedToHost)
		{
			//terrain->Render(device);
		}

        if (host && connectedToClient)
        {
            EFF3DSharedTexture * sharedTexture = device->GetSharedRenderTarget();
            if (sharedTexture != NULL)
            {
                sharedTexture->HostWaitToStartRendering();

				EFF3DTextureHandle clientTexture = sharedTexture->GetClientTexture();
				if (sharedTexture != NULL)
				{
					device->DrawQuad(NULL, clientTexture, effFALSE);
                }
            }
        }

		//wkeUpdate();
		OnRenderGUI(elapsedTime);


		device->SubmitCommandBuffer();

		device->EndScene();


        device->Present(NULL, NULL);
	}
}


effVOID EFFApplication::InitGui()
{

}


effVOID EFFApplication::InitServer()
{
	if (host)
	{
		server = new EFFNetServer();
		server->Init();
		server->Bind(_effT("tcp://127.0.0.1:5555"));
	}
}

effVOID EFFApplication::InitClient()
{
	if (!host)
	{
		client = EFFNEW EFFNetClient();
		client->Init();
		client->Connect(_effT("tcp://127.0.0.1:5555"));
	}
}

effVOID EFFApplication::ShutdownServerAndClient()
{
	if (server != NULL)
	{
		server->Shutdown();
	}

	SF_DELETE(server);

	if (client != NULL)
	{
		client->Shutdown();
	}

	SF_DELETE(client);
}

effVOID EFFApplication::ReceiveMsg()
{
	EFFFramework::ReceiveMsg(server, client);
}

effVOID EFFApplication::SendMsg(effINT id, effVOID * buffer, effINT size)
{
	if (server != NULL)
	{
		server->SendMsg(id, buffer, size);
	}

	if (client != NULL)
	{
		client->SendMsg(id, buffer, size);
	}
}

effVOID EFFApplication::SendCmd(effINT id)
{
	if (server != NULL)
	{
		server->SendCmd(id);
	}

	if (client != NULL)
	{
		client->SendCmd(id);
	}
}

EFFApplication * EFFGetApplication()
{
	return application;
}

EFFFRAMEWORK_END