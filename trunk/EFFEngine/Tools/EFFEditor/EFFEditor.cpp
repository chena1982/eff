/******************************************************************************
	created:	2008-12-14   0:19
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\EFFEditor.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFEditor.h"
#include "EFFEditorFrame.h"



IMPLEMENT_APP(EFFEditorApp)


bool EFFEditorApp::OnInit()
{
	EFFEditorFrame *frame = new EFFEditorFrame(_T("EFFEditor"), wxPoint(50,50), wxSize(800,600) );
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}



