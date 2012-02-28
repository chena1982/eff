#include "stdafx.h"
#include "effeditor.h"

#include "EFFEditorInspectorPanel.h"
#include "EFFEditorProjectPanel.h"
#include "EFFEditorHierarchyPanel.h"
#include "EFFEditorScenePanel.h"

EFFEditorMainWindow::EFFEditorMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//ui.setupUi(this);

	//setDocumentMode(true);


	//setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);

	m_pInspectorPanel = new EFFEditorInspectorPanel(this);
	addDockWidget(Qt::RightDockWidgetArea, m_pInspectorPanel);

	m_pProjectPanel = new EFFEditorProjecPanel(this);
	addDockWidget(Qt::AllDockWidgetAreas, m_pProjectPanel);

	m_pHierarchyPanel = new EFFEditorHierarchyPanel(this);
	addDockWidget(Qt::AllDockWidgetAreas, m_pHierarchyPanel);

	m_pScenePanel = new EFFEditorScenePanel(this);
	addDockWidget(Qt::LeftDockWidgetArea, m_pScenePanel);


	//tabifyDockWidget(m_pInspectorPanel, m_pProjectPanel);
	/*setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
	setTabShape(QTabWidget::Triangular);*/
}

EFFEditorMainWindow::~EFFEditorMainWindow()
{

}
