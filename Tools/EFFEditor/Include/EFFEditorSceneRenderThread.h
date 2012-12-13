/********************************************************************
	created:	13:11:2010   20:31
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\Include\EFFEditorSceneRenderThread.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorSceneRenderThread_2012_03_15__
#define __EFFEditorSceneRenderThread_2012_03_15__


struct RenderThreadStartParam
{
	HWND hWnd;
	effINT width;
	effINT height;
};

effUINT WINAPI RenderThread(effVOID * arglist);


#endif