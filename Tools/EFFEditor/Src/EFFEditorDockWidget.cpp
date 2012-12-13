/********************************************************************
	created:	13:11:2010   20:23
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Src\EFFEditorUIDockWidget.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorDockWidget.h"
#include "EFFEditorDockWidgetTitleBar.h"
#include "EFFEditor.h"

extern EFFEditorMainWindow * g_pMainWindow;


char * QTabBarStyleSheet0 = "QTabBar { background-color : rgb(163, 163, 163); } \
						   \
						   QTabBar::tab { border-top-left-radius : 4px; \
						   border-top-right-radius : 4px; \
						   min-width : 100px; \
						   min-height : 17px; \
						   margin : 0px; } \
						   \
						   QTabBar::tab:!selected { background-color : rgb(163, 163, 163); \
						   border : 0px; \
						   border-bottom : 1px solid rgb(130, 130, 130); }	\
						   \
						   QTabBar::tab:selected { background : qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0, stop: 0 rgb(215, 215, 215), stop: 1.0 rgb(223, 223, 223)); \
						   border : 1px solid rgb(130, 130, 130); \
						   border-bottom-color : rgb(203, 203, 203); } \
						   \
						   QTabBar[currentFocusTabBar=\"true\"]::tab:selected { background : qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0, stop: 0 rgb(226, 226, 226), stop: 1.0 rgb(236, 236, 236)); }";




EFFEditorUIDockWidget::EFFEditorUIDockWidget(QWidget * pParent, const QString & strTitle) : QDockWidget(pParent)
{
	m_pTitleBar = new EFFEditorUIDockWidgetTitleBar(this, strTitle);


	//m_pTabBar = new QTabBar(m_pTitleBar);
	//m_pTabBar->addTab(strTitle);
	//m_pTabBar->move(0, 5);

	setWindowTitle(strTitle);
	setTitleBarWidget(m_pTitleBar);



	m_pStackedWidget = new QStackedWidget();
	QWidget * p = new QWidget(this);
	//m_pStackedWidget->addWidget(p);
	//setWidget(m_pStackedWidget);
	setWidget(p);



	connect(m_pTitleBar->getTabBar(), SIGNAL(currentChanged(int)), m_pStackedWidget, SLOT(setCurrentIndex(int)));

	connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(addOrRemoveTab(bool)));
}


EFFEditorUIDockWidget::~EFFEditorUIDockWidget()
{


}

void EFFEditorUIDockWidget::addWidget(QWidget * pWidget)
{
	if ( m_pStackedWidget->indexOf(pWidget) == - 1 )
	{
		m_pStackedWidget->addWidget(pWidget);
	}

}

void EFFEditorUIDockWidget::resetQss()
{
	//m_pTabBar->setStyleSheet(QString::Null());
	//m_pTabBar->setStyleSheet(QTabBarStyleSheet0);
	//m_pTabBar->resize(300, 24);
}


void EFFEditorUIDockWidget::addOrRemoveTab(bool bAdd)
{
	//m_pTitleBar->dockWidgetsTabified(this, bAdd);

	QList<QDockWidget *> tdws = g_pMainWindow->tabifiedDockWidgets(this);

	for ( int i = 0; i < tdws.size(); ++i )
	{
		QWidget * pWidget = tdws[i]->widget();
		tdws[i]->setWidget(NULL);
		addWidget(pWidget);
		m_pTitleBar->getTabBar()->addTab(tdws[i]->windowTitle());
		//tdws[i]->setFloating(true);
		//tdws[i]->hide();
		g_pMainWindow->removeDockWidget(tdws[i]);
		m_pTitleBar->resetQss();
	}
}