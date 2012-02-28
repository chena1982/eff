/********************************************************************
	created:	6:9:2010   22:22
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Src\EFFEditorScenePanel.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorScenePanel.h"
#include "EFFEditorDockWidgetTitleBar.h"

extern QMainWindow * g_pMainWindow;

#define TOOLBAR_MIN_HEIGHT 16

EFFEditorScenePanel::EFFEditorScenePanel(QWidget * pParent) : QDockWidget(pParent)
{
	/*m_pTitleBar = new EFFEditorUIDockWidgetTitleBar(this, tr("Scene"));
	setWindowTitle(tr("Scene"));
	setTitleBarWidget(m_pTitleBar);



	QObject::connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(titleBarAddOrDeleteTab(bool)));
	//QObject::connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(titleBarAddOrDeleteTab(bool)));



	m_pStackedWidget = new QStackedWidget();

	QWidget * p = new QWidget(this);
	m_pStackedWidget->addWidget(p);

	setWidget(m_pStackedWidget);

	//connect(m_pTitleBar->getTabBar(), SIGNAL(currentChanged(int)), m_pStackedWidget, SLOT(setCurrentIndex(int)));
	connect(m_pTitleBar->getTabBar(), SIGNAL(currentChanged(int)), this, SLOT(test(int)));*/

	setWindowTitle(tr("Scene"));

	setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

	m_pContent = new QWidget(this);
	m_pContent->setObjectName("scene");

	m_pMainLayout = new QVBoxLayout();
	m_pMainLayout->setContentsMargins(1, 1, 1, 1);
	//m_pMainLayout->setSpacing(0);

	createToolbar();
	m_pRealTimeContent = new QWidget();
	m_pRealTimeContent->setObjectName("realtime");

	m_pMainLayout->addWidget(m_pToolbar);
	m_pMainLayout->addWidget(m_pRealTimeContent, 1);



	m_pContent->setLayout(m_pMainLayout);
	setWidget(m_pContent);

	int left, right, top, bottom;
	//m_pToolbar->layout()->getContentsMargins(&left, &top, &right, &bottom);
	m_pToolbar->layout()->setContentsMargins(0, 0, 0, 0);
	//m_pMainLayout->getContentsMargins(&left, &top, &right, &bottom);

}


EFFEditorScenePanel::~EFFEditorScenePanel()
{

}

void EFFEditorScenePanel::test(int index)
{
	int z = 0;
}

class EFFEditorUIScenePanelDrawModeAction : public  QWidgetAction
{
public:
	EFFEditorUIScenePanelDrawModeAction(QObject * pParent) : QWidgetAction(pParent) {};
	~EFFEditorUIScenePanelDrawModeAction() {}
public:
	QWidget * createWidget (QWidget * pParent)
	{
		QToolButton * pToolButton = new QToolButton(pParent);
		pToolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
		pToolButton->setPopupMode(QToolButton::InstantPopup);
		pToolButton->setMinimumHeight(TOOLBAR_MIN_HEIGHT);
		pToolButton->setMinimumWidth(100);

		QMenu * pMenu = new QMenu(pToolButton);
		pMenu->addMenu(tr("Textured"));
		pMenu->addMenu(tr("Wireframe"));
		pMenu->addMenu(tr("Tex-Wir"));

		pToolButton->setMenu(pMenu);
		return pToolButton;
	}
};

class EFFEditorUIScenePanelRenderModeAction : public  QWidgetAction
{
public:
	EFFEditorUIScenePanelRenderModeAction(QObject * pParent) : QWidgetAction(pParent) {};
	~EFFEditorUIScenePanelRenderModeAction() {}
public:
	QWidget * createWidget (QWidget * pParent)
	{
		QToolButton * pToolButton = new QToolButton(pParent);
		pToolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
		pToolButton->setPopupMode(QToolButton::InstantPopup);
		pToolButton->setMinimumHeight(TOOLBAR_MIN_HEIGHT);
		pToolButton->setMinimumWidth(80);

		QMenu * pMenu = new QMenu(pToolButton);
		pMenu->addMenu(tr("RGB"));
		pMenu->addMenu(tr("Alpha"));
		pMenu->addMenu(tr("Overdraw"));
		pMenu->addMenu(tr("Mipmaps"));

		pToolButton->setMenu(pMenu);
		return pToolButton;
	}
};

void EFFEditorScenePanel::createToolbar()
{

	/*m_pToolbar = new QWidget();
	m_pToolbar->setObjectName("toolbar");
	m_pToolbar->setMaximumHeight(16);

	QHBoxLayout * toolbarLayout = new QHBoxLayout();
	toolbarLayout->setContentsMargins(0, 0, 0, 0);

	QPushButton * pRenderModeButton = new QPushButton();
	QMenu * pRenderModeMenu = new QMenu(pRenderModeButton);
	pRenderModeMenu->addMenu(tr("Textured"));
	pRenderModeMenu->addMenu(tr("Wireframe"));
	pRenderModeButton->setMenu(pRenderModeMenu);

	toolbarLayout->addWidget(pRenderModeButton, 0, Qt::AlignLeft);
	m_pToolbar->setLayout(toolbarLayout);*/


	m_pToolbar = new QToolBar(NULL);
	m_pToolbar->setMinimumHeight(TOOLBAR_MIN_HEIGHT);

	m_pToolbar->addWidget(new QLabel());
	m_pToolbar->addSeparator();

	QToolButton * drawMode = new QToolButton();
	drawMode->setToolButtonStyle(Qt::ToolButtonTextOnly);
	drawMode->setPopupMode(QToolButton::InstantPopup);
	drawMode->setObjectName(tr("DrawMode"));
	QMenu * drawModeMenu = new QMenu(drawMode);
	drawModeMenu->addAction(new QAction(tr("Textured"), drawModeMenu));
	drawModeMenu->addAction(new QAction(tr("Wireframe"), drawModeMenu));
	drawModeMenu->addAction(new QAction(tr("Tex-Wire"), drawModeMenu));
	connect(drawModeMenu, SIGNAL(triggered(QAction *)), this, SLOT(drawModeMenuPressed(QAction *)));
	drawMode->setMenu(drawModeMenu);
	m_pToolbar->addWidget(drawMode);
	QMetaObject::invokeMethod(drawModeMenu, "triggered", Q_ARG(QAction *, *drawModeMenu->actions().begin()));

	m_pToolbar->addSeparator();
	m_pToolbar->addWidget(new QLabel());
	m_pToolbar->addSeparator();

	QToolButton * renderMode = new QToolButton();
	renderMode->setToolButtonStyle(Qt::ToolButtonTextOnly);
	renderMode->setPopupMode(QToolButton::InstantPopup);
	renderMode->setObjectName(tr("RenderMode"));
	QMenu * renderModeMenu = new QMenu(renderMode);
	renderModeMenu->addAction(new QAction(tr("RGB"), renderModeMenu));
	renderModeMenu->addAction(new QAction(tr("Alpha"), renderModeMenu));
	renderModeMenu->addAction(new QAction(tr("OverDraw"), renderModeMenu));
	renderModeMenu->addAction(new QAction(tr("Mipmaps"), renderModeMenu));
	connect(renderModeMenu, SIGNAL(triggered(QAction *)), this, SLOT(renderModeMenuPressed(QAction *)));
	renderMode->setMenu(renderModeMenu);
	m_pToolbar->addWidget(renderMode);
	QMetaObject::invokeMethod(renderModeMenu, "triggered", Q_ARG(QAction *, *renderModeMenu->actions().begin()));

	m_pToolbar->addSeparator();
	m_pToolbar->addWidget(new QLabel());
	m_pToolbar->addSeparator();

	QPushButton * pLightingButton = new QPushButton();
	pLightingButton->setCheckable(true);
	//pLightingButton->setText(tr("Lighting"));
	pLightingButton->setIcon(QIcon("Aqua100.png"));
	m_pToolbar->addWidget(pLightingButton);
	m_pToolbar->addSeparator();

	QPushButton * pGameOverlayButton = new QPushButton();
	pGameOverlayButton->setCheckable(true);
	pGameOverlayButton->setIcon(QIcon("Aqua100.png"));
	//pGameOverlayButton->setMinimumHeight(TOOLBAR_MIN_HEIGHT);
	m_pToolbar->addWidget(pGameOverlayButton);
	m_pToolbar->addSeparator();

	QPushButton * pAuditionButton = new QPushButton();
	pAuditionButton->setCheckable(true);
	pAuditionButton->setIcon(QIcon("Aqua100.png"));
	//pAuditionButton->setMinimumHeight(TOOLBAR_MIN_HEIGHT);
	m_pToolbar->addWidget(pAuditionButton);
	m_pToolbar->addSeparator();

	//°Ñ¿Ø¼þ¼·µ½ÓÒ±ß
	QWidget * space = new QWidget();
	space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pToolbar->addWidget(space);

	m_pToolbar->addSeparator();
	QPushButton * gizmos = new QPushButton();
	gizmos->setText(tr("Gizmos"));
	m_pToolbar->addWidget(gizmos);
	m_pToolbar->addSeparator();

	QLineEdit * searchEdit = new QLineEdit();
	m_pToolbar->addWidget(searchEdit);



}

void EFFEditorScenePanel::titleBarAddOrDeleteTab(bool bAdd)
{
	//m_pTitleBar->dockWidgetsTabified(this, bAdd);

	/*QList<QDockWidget *> tdws = g_pMainWindow->tabifiedDockWidgets(this);

	for ( int i = 0; i < tdws.size(); ++i )
	{
		QWidget * pWidget = tdws[i]->widget();
		tdws[i]->setWidget(NULL);
		m_pStackedWidget->addWidget(pWidget);
		m_pTitleBar->getTabBar()->addTab(tdws[i]->windowTitle());
		//tdws[i]->setFloating(true);
		//tdws[i]->hide();
		g_pMainWindow->removeDockWidget(tdws[i]);
		m_pTitleBar->resetQss();
	}*/
}

void EFFEditorScenePanel::drawModeMenuPressed(QAction * action)
{
	QToolButton * drawMode = qobject_cast<QToolButton *>(action->parentWidget()->parentWidget());
	drawMode->setText(action->text());
}

void EFFEditorScenePanel::renderModeMenuPressed(QAction * action)
{
	QToolButton * renderMode = qobject_cast<QToolButton *>(action->parentWidget()->parentWidget());
	renderMode->setText(action->text());
}