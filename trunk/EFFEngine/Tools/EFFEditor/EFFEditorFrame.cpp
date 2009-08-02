/******************************************************************************
	created:	2008-12-14   0:21
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\EFFEditorFrame.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFEditorFrame.h"

#include "EFFELevelWindow.h"
#include "EFFEditorPropertyPanel.h"
#include "EFFEditorBaseWindow.h"


BEGIN_EVENT_TABLE(EFFEditorFrame, wxFrame)
	EVT_MENU(wxID_EXIT,  EFFEditorFrame::OnQuit)
	EVT_MENU(wxID_ABOUT, EFFEditorFrame::OnAbout)
END_EVENT_TABLE()


static const long ID_TOP_WINDOW = wxNewId();
static const long ID_FRONT_WINDOW = wxNewId();
static const long ID_RIGHT_WINDOW = wxNewId();
static const long ID_CAMERA_WINDOW = wxNewId();

EFFEditorFrame::EFFEditorFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, -1, title, pos, size)
{
	wxMenu *menuFile = new wxMenu;

	menuFile->Append(wxID_ABOUT,_T("&About..."));
	menuFile->Append(wxID_EXIT,_T("E&xit"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile,_T("&File"));

	SetMenuBar(menuBar);

	//CreateStatusBar();
	//SetStatusText(_T("Welcome to wxWidgets!"));

	//wxPanel * panel = new wxPanel(this, -1);
	//panel->SetBackgroundColour(wxColour(0xFF00FFFF));
	//panel->SetSize(100,100);


	//wxBoxSizer * vBox = new wxBoxSizer(wxVERTICAL);
	//m_AuiManager.SetManagedWindow(this);

	EFFEditorBaseWindow * m_pTopWindow = new EFFEditorBaseWindow(this,ID_TOP_WINDOW);
	m_aryWindow.push_back(m_pTopWindow);


	EFFEditorBaseWindow * m_pFrontWindow = new EFFEditorBaseWindow(this,ID_FRONT_WINDOW);
	m_aryWindow.push_back(m_pFrontWindow);

	EFFEditorBaseWindow * m_pRightWindow = new EFFEditorBaseWindow(this,ID_RIGHT_WINDOW);
	m_aryWindow.push_back(m_pRightWindow);

	EFFEditorBaseWindow * m_pCameraWindow = new EFFEditorBaseWindow(this,ID_CAMERA_WINDOW);
	m_aryWindow.push_back(m_pCameraWindow);


	
	//m_pPropertyPanel = new EFFEditorPropertyPanel(this,this);

	//m_AuiManager.AddPane(wxTemp,wxLEFT,wxT(""));
	//m_AuiManager.AddPane(m_pPropertyPanel,wxRIGHT,wxT("ÊôÐÔ"));
	//m_AuiManager.GetPane(m_pPropertyPanel);

	/*m_AuiManager.AddPane(wxTemp6,wxBOTTOM);
	m_AuiManager.GetPane(wxTemp6).CaptionVisible(false).Layer(4).DockFixed().PaneBorder(false);

	m_AuiManager.AddPane(wxTemp5,wxLEFT);
	m_AuiManager.GetPane(wxTemp5).CaptionVisible(false).Layer(3).DockFixed().PaneBorder(false);*/

	/*m_AuiManager.AddPane(wxTemp1,wxLEFT);
	m_AuiManager.GetPane(wxTemp1);

	m_AuiManager.AddPane(wxTemp2,wxLEFT);
	m_AuiManager.GetPane(wxTemp2);

	m_AuiManager.AddPane(wxTemp3,wxLEFT);
	m_AuiManager.GetPane(wxTemp3);

	m_AuiManager.AddPane(wxTemp4,wxLEFT);
	m_AuiManager.GetPane(wxTemp4);*/

	//m_AuiManager.Update();

	wxBoxSizer * vBox = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer * hBox1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * hBox2 = new wxBoxSizer(wxHORIZONTAL);

	vBox->Add(hBox1,1,wxALIGN_TOP | wxGROW,0);
	vBox->Add(hBox2,1,wxALIGN_BOTTOM | wxGROW,0);

	hBox1->Add(m_pTopWindow,1,wxALIGN_LEFT | wxGROW,0);
	hBox1->Add(m_pFrontWindow,1,wxALIGN_RIGHT | wxGROW,0);

	hBox2->Add(m_pRightWindow,1,wxALIGN_LEFT | wxGROW,0);
	hBox2->Add(m_pCameraWindow,1,wxALIGN_RIGHT | wxGROW,0);

	SetSizer(vBox);
	//vBox->Fit(this);
	//vBox->SetSizeHints(this);

	HWND hWnd = GetHwnd();
	EFFELevelWindow * pWindow = new EFFELevelWindow;
	pWindow->Init(hWnd);

	m_bOneWindow = false;
}

EFFEditorFrame::~EFFEditorFrame()
{
	m_AuiManager.UnInit();
}

void EFFEditorFrame::OnQuit(wxCommandEvent& event)
{
	Close(true);
}

void EFFEditorFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(_T("EFFEditor"),_T("1.0"), wxOK | wxICON_INFORMATION);

}


wxAuiDockArt * EFFEditorFrame::GetDockArt()
{
	  return m_AuiManager.GetArtProvider();
}

void EFFEditorFrame::ToggleOneOrMoreWindow(wxWindowID winId)
{
	m_bOneWindow = !m_bOneWindow;

	if ( m_bOneWindow )
	{
		for ( size_t i = 0; i < m_aryWindow.size(); i++ )
		{
			if ( m_aryWindow[i]->GetId() != winId )
			{
				m_aryWindow[i]->Show(false);
			}
		}
	}
	else
	{
		for ( size_t i = 0; i < m_aryWindow.size(); i++ )
		{
			m_aryWindow[i]->Show(true);
		}
	}



}