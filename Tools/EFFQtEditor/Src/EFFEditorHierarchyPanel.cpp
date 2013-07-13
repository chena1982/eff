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



	m_pContent = new QWidget(this);
	m_pContent->setObjectName("hierarchy");

	
	m_pMainLayout = new QVBoxLayout();
	m_pMainLayout->setContentsMargins(1, 1, 1, 1);
	m_pMainLayout->setSpacing(0);



	createToolbar();
	m_pMainLayout->addWidget(m_pToolbar);


	m_pTreeWidget = new QTreeWidget(m_pContent);
	/*treeWidget->setColumnCount(1);
	QList<QTreeWidgetItem *> items;
	for (int i = 0; i < 10; ++i)
		items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
	treeWidget->insertTopLevelItems(0, items);*/
	m_pTreeWidget->header()->hide();
	m_pMainLayout->addWidget(m_pTreeWidget, 1);

	m_pContent->setLayout(m_pMainLayout);

	setWidget(m_pContent);



	//m_pTreeWidget->insertTopLevelItem(0, new QTreeWidgetItem(QStringList(QString()
}


EFFEditorHierarchyPanel::~EFFEditorHierarchyPanel()
{

}

void EFFEditorHierarchyPanel::titleBarAddOrDeleteTab(bool bAdd)
{
	m_pTitleBar->dockWidgetsTabified(this, bAdd);
}


void EFFEditorHierarchyPanel::createToolbar()
{
	m_pToolbar = new QToolBar(NULL);
	m_pToolbar->setObjectName("toolbar");
	m_pToolbar->setMinimumHeight(TOOLBAR_MIN_HEIGHT);

	m_pToolbar->addWidget(new QLabel());
	m_pToolbar->addSeparator();

	QToolButton * create = new QToolButton();
	create->setToolButtonStyle(Qt::ToolButtonTextOnly);
	create->setPopupMode(QToolButton::InstantPopup);
	create->setObjectName(tr("Create"));
	QMenu * createMenu = new QMenu(create);
	createMenu->addAction(new QAction(tr("Particle System"), createMenu));
	createMenu->addAction(new QAction(tr("Camera"), createMenu));
	createMenu->addAction(new QAction(tr("Directional Light"), createMenu));
	createMenu->addAction(new QAction(tr("Cube"), createMenu));

	connect(createMenu, SIGNAL(triggered(QAction *)), this, SLOT(createMenuPressed(QAction *)));
	create->setMenu(createMenu);
	create->setText(tr("Create"));
	m_pToolbar->addWidget(create);
	QMetaObject::invokeMethod(create, "triggered", Q_ARG(QAction *, *create->actions().begin()));

	m_pToolbar->addSeparator();

	//°Ñ¿Ø¼þ¼·µ½ÓÒ±ß
	QWidget * space = new QWidget();
	space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pToolbar->addWidget(space);

	QLineEdit * searchEdit = new QLineEdit();
	m_pToolbar->addWidget(searchEdit);
}

void EFFEditorHierarchyPanel::createMenuPressed(QAction * action)
{
	QToolButton * create = qobject_cast<QToolButton *>(action->parentWidget()->parentWidget());
	//create->setText(action->text());

	if ( action->text() == tr("Particle System") )
	{
		EFFObjectManager * objectManager = EFFGetObjectManager(EFF3DObject::GetThisClass());
		EFF3DObject * gameObject = (EFF3DObject *)objectManager->CreateObject(EFF3DObject::GetThisClass());

		gameObject->SetName(_effT("cccccccccccc"));

		AddObject(gameObject);
	}
}

void EFFEditorHierarchyPanel::AddObject(EFF3DObject * object)
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