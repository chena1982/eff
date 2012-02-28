/********************************************************************
	created:	6:9:2010   22:10
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Src\EFFEditorUIProjectPanel.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorProjectPanel.h"
#include "EFFEditorDockWidgetTitleBar.h"


EFFEditorProjecPanel::EFFEditorProjecPanel(QWidget * parent) : QDockWidget(parent)
{
	/*m_pTitleBar = new EFFEditorUIDockWidgetTitleBar(this, tr("Project"));
	setWindowTitle(tr("Project"));
	setTitleBarWidget(m_pTitleBar);

	QObject::connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(titleBarAddOrDeleteTab(bool)));*/

	setWindowTitle(tr("Project"));
	//hide the title bar
	/*QWidget* lTitleBar = titleBarWidget();
	QWidget* lEmptyWidget = new QWidget();
	setTitleBarWidget(lEmptyWidget);
	delete lTitleBar;*/


	QWidget * p = new QWidget(this);
	setWidget(p);
}


EFFEditorProjecPanel::~EFFEditorProjecPanel()
{

}

void EFFEditorProjecPanel::titleBarAddOrDeleteTab(bool bAdd)
{
	m_pTitleBar->dockWidgetsTabified(this, bAdd);
}