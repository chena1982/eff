/******************************************************************************
	created:	2012-10-22   23:37
	file path:	d:\EFF\EFFEngine\Include\EFFFramework\EFFApplication.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFApplication_2012_10_22__
#define __EFFApplication_2012_10_22__


struct MuiltiProcessWndHandles
{
	MuiltiProcessWndHandles()
	{
		hWndMain = NULL;
		hWndTab = NULL;
	}

    HWND hWndMain; 
    HWND hWndTab;
};


class EFFF_API EFFApplication
{
public:
	EFFApplication();
	~EFFApplication();

public:
	effBOOL Init(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host);
	effVOID Run();
    effVOID Quit();

	effVOID	SetBackGroundColor(effUINT color);

	effVOID MoveWindow(effINT x, effINT y, effINT width, effINT height);

	HWND GetHWND() { return hWnd; }

	effBOOL IsWindowSizeChanged(effINT width, effINT height);

	effVOID SetWindowMinimized(effBOOL minimized);
protected:
	effBOOL	CreateAppWindow(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host);

	effVOID	Update();
	effVOID Render(effFLOAT elapsedTime);

	effBOOL	CreateMemFile();
	effVOID ReadHWNDFromMemFile();

	effVOID InitGui();
	effVOID CreateGui();

	effVOID WindowResized(effINT width, effINT height);

public:
    EFFEvent                    OnUpdate;
	EFFEvent					OnRenderGUI;
	EFFEvent					OnWindowMove;
	EFFEvent					OnWindowResize;
protected:
	EFF3DDevice *				device;
	HANDLE						memFile;
	MuiltiProcessWndHandles		mpwh;
	effUINT						backGroundColor;
	effBOOL						host;
	effBOOL						window;

	HWND						hWnd;

	effINT						width;
	effINT						height;

	effBOOL						minimized;
};

EFFF_API EFFApplication * EFFGetApplication();
#endif
