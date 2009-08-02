/******************************************************************************
	created:	2009-3-22   23:08
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\EFFEditorBaseWindow.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFEditorBaseWindow_2009_3_22__
#define __EFFEditorBaseWindow_2009_3_22__

class EFFEditorFrame;

class EFFEditorBaseWindow : public wxPanel
{
public:
	EFFEditorBaseWindow(EFFEditorFrame * pFrame,wxWindowID id = wxID_ANY);
	~EFFEditorBaseWindow();
public:
	void OnSize(wxSizeEvent& event);
	void OnToggleOneOrMoreWindow(wxCommandEvent& event);
protected:
	wxToolBar *						m_pToolBar;
	wxButton *						m_pToogleOneOrMoreWindowButton;
	EFFEditorFrame *				m_pFrame;
private:
    DECLARE_EVENT_TABLE()
};

#endif