/******************************************************************************
	created:	2012-10-22   23:37
	file path:	d:\EFF\EFFEngine\Include\MMOEncapsulation\MMOApplication.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __MMOApplication_2012_10_22__
#define __MMOApplication_2012_10_22__


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


class MMO_API MMOApplication
{
public:
	MMOApplication();
	~MMOApplication();

public:
	effBOOL Init(effBOOL window, effINT width, effINT height, effBOOL multiProcess, effBOOL host);
	effVOID Run();

	effVOID	SetBackGroundColor(effUINT color);

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
};


#endif
