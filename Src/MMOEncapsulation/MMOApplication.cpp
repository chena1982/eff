/******************************************************************************
	created:	2012-10-22   23:43
	file path:	d:\EFF\EFFEngine\Src\MMOEncapsulation\MMOApplication.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "MMOApplication.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;								// ��ǰʵ��
HWND hWnd;

TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

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

effBOOL MMOApplication::CreateAppWindow(effBOOL window, effINT width, effINT height)
{


	hInst = GetModuleHandle(NULL);

	_tcscpy_s(szWindowClass, _effT("EFF Application"));

	MyRegisterClass(hInst);

	RECT windowRect;
	SetRect(&windowRect, 0, 0, width, height);
	effUINT style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&windowRect, style, FALSE);

	hWnd = CreateWindow(szWindowClass, szTitle, style,
		0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInst, NULL);

	if ( !hWnd )
	{
		return effFALSE;
	}

   ShowWindow(hWnd, SW_SHOW);
   UpdateWindow(hWnd);

	return effTRUE;
}

MMOApplication::MMOApplication()
{
	device = NULL;
}

MMOApplication::~MMOApplication()
{
	SF_RELEASE(device);
}

EFF3DTerrain * terrain = NULL;
EFF3DCamera * camera = NULL;


EFF3DFont * font = NULL;

effBOOL MMOApplication::Init(effBOOL window, effINT width, effINT height)
{

	if ( !CreateAppWindow(window, width, height) )
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

	EFF3DFontManager * fontManager = EFFNEW EFF3DFontManager();
	font = fontManager->CreateFromFile(_effT("Font\\msyh.ttf"), 16);

	device->SetRenderState(EFF3DRS_LIGHTING, effFALSE);

	return effTRUE;
}

effVOID	MMOApplication::Update()
{
}


effVOID MMOApplication::Render(effFLOAT elapsedTime)
{
	static UINT color = 0xFF4c4c52;
	//color += 10;
	device->Clear(0, NULL, EFF3DCLEAR_TARGET | EFF3DCLEAR_TARGET, color, 1.0f, 0);
	if ( device->BeginScene() )
	{
		EFFMatrix4 world;
		device->SetTransform(EFF3DTS_WORLD, &world);
		device->SetTransform(EFF3DTS_VIEW, &camera->GetViewMatirx());
		device->SetTransform(EFF3DTS_PROJECTION, &camera->GetProjMatrix());

		//terrain->Render(device);
		/*imguiBeginFrame(0, 0, 0, 0);

		effINT toolsScroll = 0;
		imguiBeginScrollArea(_effT("Tools"), 0, 0, 300, 500, &toolsScroll);
		imguiButton(L"");
		imguiEndScrollArea();

		imguiEndFrame();
		imguiRenderDraw();*/

		effINT x = 10;
		effINT y = 100;
		//font->DrawText(_effT("��"));
		//font->DrawText(_effT("��������"));
		//font->DrawText(_effT("abcdefg"));
		//font->DrawText(_effT("��г�磬���հ��������������Σ��Ҳ������Ǹ���,��ǰ���¹⣬���Ƶ���˪����ͷ�����£���ͷ˼����"));
		font->DrawText(_effT("������"), x, y);

		y += 20;
		font->DrawText(_effT("�ⲿ�ִ��벻����ͣ�ֻ����ʾλͼ���ݣ�����face->glyph->bitmap��û�е�ɫ���1λλͼ��Դ��ʹ��FT_RENDER_MODE_MONO��Ⱦģʽ"), x, y);
		//font->DrawText(_effT("�򵥵�����һ��freetype��ʹ�����̣�����ϸ�ĺ���˵������������ġ�freetype2�������š��� �����д��ĵ�������Ȥ���Կ�����"));

		device->EndScene();
		device->Present(NULL, NULL);
	}
}


const int TICKS_PER_SECOND = 25;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

effVOID MMOApplication::Run()
{


    effUINT nextGameTick = GetTickCount();
    effINT loops;
    effFLOAT interpolation;

	//�������ѭ����Update��֡������Render����֡
    while( !appExit )
	{
        loops = 0;
        while ( GetTickCount() > nextGameTick && loops < MAX_FRAMESKIP )
		{
			//�ȴ������е�windows��Ϣ
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


