/******************************************************************************
	created:	2009-3-22   23:08
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\EFFEditorBaseWindow.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFEditorBaseWindow.h"
#include "EFFEditorFrame.h"

static const long ID_TOGGLE_ONE_OR_MORE_WINDOW = wxNewId();

BEGIN_EVENT_TABLE(EFFEditorBaseWindow, wxPanel)
	EVT_SIZE(OnSize)
	EVT_BUTTON(ID_TOGGLE_ONE_OR_MORE_WINDOW,OnToggleOneOrMoreWindow)
END_EVENT_TABLE()

EFFEditorBaseWindow::EFFEditorBaseWindow(EFFEditorFrame * pFrame,wxWindowID id /* = wxID_ANY */) : wxPanel(pFrame,id,wxDefaultPosition,wxDefaultSize,wxBORDER_SIMPLE)
{
	m_pFrame = pFrame;
	//SetWindowStyleFlag(wxBORDER_SIMPLE);
	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	m_pToolBar = new wxToolBar(this,-1,wxDefaultPosition,wxSize(-1,24),wxBORDER_SIMPLE | wxTB_NODIVIDER | wxTB_HORIZONTAL | wxTB_FLAT);


	sizer->Add(m_pToolBar,0,wxALIGN_LEFT,0);

	wxButton * pSelectWindowButton = new wxButton(m_pToolBar,-1,wxT("Top"));
	m_pToolBar->AddControl(pSelectWindowButton);

	wxSize size = m_pToolBar->GetSize();
	m_pToogleOneOrMoreWindowButton = new wxButton(m_pToolBar,ID_TOGGLE_ONE_OR_MORE_WINDOW,wxEmptyString,wxDefaultPosition,wxSize(28,-1),wxNO_BORDER);
	m_pToolBar->AddControl(m_pToogleOneOrMoreWindowButton);


	SetSizer(sizer);
	sizer->SetSizeHints(this);

	
}

EFFEditorBaseWindow::~EFFEditorBaseWindow()
{

}

void EFFEditorBaseWindow::OnSize(wxSizeEvent& event)
{
	wxPanel::OnSize(event);

	wxSize size = m_pToolBar->GetSize();
	m_pToogleOneOrMoreWindowButton->SetPosition(wxPoint(size.GetWidth()-30,0));
}

void EFFEditorBaseWindow::OnToggleOneOrMoreWindow(wxCommandEvent& event)
{
	m_pFrame->ToggleOneOrMoreWindow(GetId());
}