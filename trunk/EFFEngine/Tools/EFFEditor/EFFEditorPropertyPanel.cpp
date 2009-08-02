/******************************************************************************
	created:	2009-3-15   0:16
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\EFFEditorPropertyPanel.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFEditorPropertyPanel.h"


BEGIN_EVENT_TABLE(EFFEditorPropertyPanel,wxPanel)
END_EVENT_TABLE()

EFFEditorPropertyPanel::EFFEditorPropertyPanel(wxWindow* parent,EFFEditorFrame * pFrame) : wxPanel(parent)
{
	m_pFrame = pFrame;


	SetSize(100,100);

	wxBoxSizer * vbox = new wxBoxSizer(wxVERTICAL);

	/*m_pPropertyGrid = new wxGrid(this,-1);
	m_pPropertyGrid->CreateGrid(1,1);

	vbox->Add(m_pPropertyGrid,0,wxEXPAND | wxALL,10);
	SetSizer(vbox);*/

	wxPropertyGridManager* pgMan = new wxPropertyGridManager(this, -1,
		wxDefaultPosition, wxDefaultSize,
		// These and other similar styles are automatically
		// passed to the embedded wxPropertyGrid.
		wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|
		// Include toolbar.
		wxPG_TOOLBAR |
		// Include description box.
		wxPG_DESCRIPTION |
		// Plus defaults.
		wxPGMAN_DEFAULT_STYLE
		);

	pgMan->SetExtraStyle(wxPG_EX_MODE_BUTTONS | wxPG_EX_UNFOCUS_ON_ENTER);

	pgMan->AddPage(wxT("First Page"));
	wxPropertyGridPage * page = pgMan->GetLastPage();

	page->Append( new wxPropertyCategory(wxT("Category A1")) );

	page->Append( new wxIntProperty(wxT("Number"),wxPG_LABEL,1) );

	page->Append( new wxColourProperty(wxT("Colour"),wxPG_LABEL,*wxWHITE) );

	pgMan->AddPage(wxT("Second Page"));
	page = pgMan->GetLastPage();

	//page->Append( wxT("Text"),wxPG_LABEL,wxT("(no text)") );

	page->Append( new wxFontProperty(wxT("Font"),wxPG_LABEL) );


	vbox->Add(pgMan,1,wxEXPAND,10);
	SetSizer(vbox);
}

EFFEditorPropertyPanel::~EFFEditorPropertyPanel()
{
}

