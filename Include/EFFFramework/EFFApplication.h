/******************************************************************************
	created:	2012-10-22   23:37
	file path:	d:\EFF\EFFEngine\Include\EFFFramework\EFFApplication.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFApplication_2012_10_22__
#define __EFFApplication_2012_10_22__


EFFFRAMEWORK_BEGIN

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


class EFFFRAMEWORK_API EFFApplication
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

	effBOOL IsWindowSizeChanged(effINT width, effINT height);

	effVOID SetWindowMinimized(effBOOL minimized);

	effVOID ReceiveMsg();
	effVOID SendMsg(effINT id, effVOID * buffer, effINT size);
	effVOID SendCmd(effINT id);
public:
	EFFNetServer * GetServer() { return server; }
	EFFNetClient * GetClient() { return client; }
	HWND GetHWND() { return hWnd; }

protected:
	effBOOL	CreateAppWindow(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host);

	effVOID	Update();
	effVOID Render(effFLOAT elapsedTime);

    effVOID OnNotifyHostStartRendering(effUINT index);
	effBOOL	SendSharedTextureIndexFromMemFile(effUINT index);
    effUINT ReadSharedTextureIndexFromMemFile();

	effVOID InitGui();
	effVOID CreateGui();

	effVOID InitServer();
	effVOID InitClient();
	effVOID ShutdownServerAndClient();

	effVOID WindowResized(effINT width, effINT height);

public:
    EFFEvent                    OnUpdate;
	EFFEvent					OnRenderGUI;
	EFFEvent					OnWindowMove;
	EFFEvent					OnWindowResize;

    EFFEvent                    OnQuit();

	effBOOL                     connectedToHost;
	effBOOL						connectedToClient;
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

	EFFNetClient *				client;
	EFFNetServer *				server;
};

EFFFRAMEWORK_API EFFApplication * EFFGetApplication();


EFFFRAMEWORK_END

#endif
