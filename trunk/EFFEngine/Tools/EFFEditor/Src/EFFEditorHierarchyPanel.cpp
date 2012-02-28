/********************************************************************
	created:	6:9:2010   22:17
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Src\EFFEditorHierarchyPanel.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorHierarchyPanel.h"
#include "EFFEditorDockWidgetTitleBar.h"


EFFEditorHierarchyPanel::EFFEditorHierarchyPanel(QWidget * parent) : QDockWidget(parent)
{
	/*m_pTitleBar = new EFFEditorUIDockWidgetTitleBar(this, tr("Hierarchy"));
	setWindowTitle(tr("Hierarchy"));
	setTitleBarWidget(m_pTitleBar);

	QObject::connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(titleBarAddOrDeleteTab(bool)));*/


	setWindowTitle(tr("Hierarchy"));
	//hide the title bar
	/*QWidget* lTitleBar = titleBarWidget();
	QWidget* lEmptyWidget = new QWidget();
	setTitleBarWidget(lEmptyWidget);
	delete lTitleBar;*/

	QWidget * p = new QWidget(this);
	setWidget(p);
}


EFFEditorHierarchyPanel::~EFFEditorHierarchyPanel()
{

}

void EFFEditorHierarchyPanel::titleBarAddOrDeleteTab(bool bAdd)
{
	m_pTitleBar->dockWidgetsTabified(this, bAdd);
}