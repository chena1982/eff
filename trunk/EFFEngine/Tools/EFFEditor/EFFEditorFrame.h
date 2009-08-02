/******************************************************************************
	created:	2008-12-14   0:20
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\EFFEditorFrame.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFEditorFrame_2008_12_14__
#define __EFFEditorFrame_2008_12_14__


class EFFEditorPropertyPanel;
class EFFEditorBaseWindow;

class EFFEditorFrame: public wxFrame
{
public:
	EFFEditorFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~EFFEditorFrame();

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
public:
	wxAuiDockArt *					GetDockArt();

	void										ToggleOneOrMoreWindow(wxWindowID winId);

protected:
    wxAuiManager						m_AuiManager;

	EFFEditorBaseWindow *		m_pTopWindow;
	EFFEditorBaseWindow *		m_pFrontWindow;
	EFFEditorBaseWindow *		m_pRightWindow;
	EFFEditorBaseWindow *		m_pCameraWindow;

	bool										m_bOneWindow;

	EFFEditorPropertyPanel *		m_pPropertyPanel;
	wxPanel *								m_pLayoutPanel;

	std::vector<EFFEditorBaseWindow *>		m_aryWindow;
private:
    DECLARE_EVENT_TABLE()
};




#endif