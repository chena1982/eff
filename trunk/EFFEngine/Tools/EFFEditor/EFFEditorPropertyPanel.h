/******************************************************************************
	created:	2009-3-15   0:14
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\EFFEditorPropertyPanel.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFEditorPropertyPanel_2009_3_15__
#define __EFFEditorPropertyPanel_2009_3_15__

#include "EFFEditorFrame.h"

class EFFEditorPropertyPanel : public wxPanel
{
public:
	EFFEditorPropertyPanel(wxWindow* pParent,EFFEditorFrame * pFrame);
	virtual ~EFFEditorPropertyPanel();

protected:
	wxGrid *								m_pPropertyGrid;
	EFFEditorFrame *					m_pFrame;

	DECLARE_EVENT_TABLE()
};

#endif