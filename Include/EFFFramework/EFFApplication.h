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

	effVOID	SetBackGroundColor(effUINT color);

	effVOID MoveWindow(effINT x, effINT y, effINT width, effINT height);

	HWND GetHWND() { return hWnd; }
protected:
	effBOOL	CreateAppWindow(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host);

	effVOID	Update();
	effVOID Render(effFLOAT elapsedTime);

	effBOOL	CreateMemFile();
	effVOID ReadHWNDFromMemFile();

	effVOID InitGui();
	effVOID CreateGui();
public:
	EFFEvent						OnRenderGUI;
	static EFFEvent					OnWindowMove;
	static EFFEvent					OnWindowResize;
protected:
	EFF3DDevice *				device;
	HANDLE						memFile;
	MuiltiProcessWndHandles		mpwh;
	effUINT						backGroundColor;
	effBOOL						host;
	effBOOL						window;

	HWND						hWnd;
};


#endif
