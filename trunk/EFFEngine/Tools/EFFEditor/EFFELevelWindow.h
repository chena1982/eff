/******************************************************************************
	created:	2008-12-24   1:27
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\EFFELevelWindow.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFELevelWindow_2008_12_24__
#define __EFFELevelWindow_2008_12_24__


class EFFELevelWindow
{
public:
	EFFELevelWindow();
	~EFFELevelWindow();
public:
	effBOOL					Init(HWND hWnd);
protected:
private:
	EFF3DDevice *			m_pDevice;
	EFF3DMaterial *		m_pMaterial;
};

#endif