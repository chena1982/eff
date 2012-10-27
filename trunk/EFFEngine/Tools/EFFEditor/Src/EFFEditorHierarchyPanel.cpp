/********************************************************************
	created:	6:9:2010   22:17
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Src\EFFEditorHierarchyPanel.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorHierarchyPanel.h"
#include "EFFEditorDockWidgetTitleBar.h"
#include "EFFEditorInspectorPanel.h"
#include "EFFEditor.h"

extern EFFEditorMainWindow * g_pMainWindow;



EFFEditorHierarchyPanel::EFFEditorHierarchyPanel(QWidget * parent) : QDockWidget(parent)
{
	/*m_pTitleBar = new EFFEditorUIDockWidgetTitleBar(this, tr("Hierarchy"));
	setWindowTitle(tr("Hierarchy"));
	setTitleBarWidget(m_pTitleBar);

	QObject::connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(titleBarAddOrDeleteTab(bool)));*/


	setWindowTitle(tr("Hierarchy"));
	setObjectName(tr("HierarchyPanel"));
	setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

	//hide the title bar
	/*QWidget* lTitleBar = titleBarWidget();
	QWidget* lEmptyWidget = new QWidget();
	setTitleBarWidget(lEmptyWidget);
	delete lTitleBar;*/

	//QWidget * p = new QWidget(this);
	
	m_pTreeWidget = new QTreeWidget(this);
	/*treeWidget->setColumnCount(1);
	QList<QTreeWidgetItem *> items;
	for (int i = 0; i < 10; ++i)
		items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
	treeWidget->insertTopLevelItems(0, items);*/
	m_pTreeWidget->header()->hide();

	setWidget(m_pTreeWidget);



	//m_pTreeWidget->insertTopLevelItem(0, new QTreeWidgetItem(QStringList(QString()
}


EFFEditorHierarchyPanel::~EFFEditorHierarchyPanel()
{

}

void EFFEditorHierarchyPanel::titleBarAddOrDeleteTab(bool bAdd)
{
	m_pTitleBar->dockWidgetsTabified(this, bAdd);
}


void EFFEditorHierarchyPanel::AddObject(EFFGameObject * object)
{
	QTreeWidgetItem * objectWI = new QTreeWidgetItem(QStringList(QString(EFFSTRING2ANSI(object->GetName()))));
	m_pTreeWidget->insertTopLevelItem(0, objectWI);

	/*auto objectProperties = object->GetRuntimeClass()->GetProperties();
	for ( effUINT i = 0; i < objectProperties.size(); i++ )
	{
		EFFProperty * objectProperty = objectProperties[i];
		QTreeWidgetItem * objectPropertyWI = new QTreeWidgetItem(QStringList(QString(EFFSTRING2ANSI(objectProperty->GetName()))));
		objectWI->addChild(objectPropertyWI);
	}*/

	g_pMainWindow->GetInspectorPanel()->BindObject(object);
}