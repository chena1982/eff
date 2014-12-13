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
protected:
	effBOOL	CreateAppWindow(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host);
	effVOID	Update();
	effVOID Render(effFLOAT elapsedTime);

	effBOOL	CreateMemFile();
	effVOID ReadHWNDFromMemFile();
public:
	EFFEvent					OnRenderGUI;
protected:
	EFF3DDevice *				device;
	HANDLE						memFile;
	MuiltiProcessWndHandles		mpwh;
	effUINT						backGroundColor;
	effBOOL						host;
	effBOOL						window;
};


#endif
