/********************************************************************
	created:	03:15:2012   23:11
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\Src\EFFEditorSceneRenderThread.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorSceneRenderThread.h"

effBOOL g_bExitRenderThread = effFALSE;

effUINT WINAPI RenderThread(effVOID * arglist)
{
	RenderThreadStartParam * param = (RenderThreadStartParam *)arglist;
	MMOApplication app;
	app.Init(effTRUE, param->hWnd, param->width, param->height);

	while ( !g_bExitRenderThread )
	{
		app.Update();
		app.Render();
		Sleep(100);
	}


	return 0;
}

