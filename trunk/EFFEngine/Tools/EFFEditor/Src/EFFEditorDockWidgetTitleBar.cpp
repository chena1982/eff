/********************************************************************
	created:	5:9:2010   23:13
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\EFFEditorUIDockWidgetTitleBar.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorDockWidgetTitleBar.h"


extern QMainWindow * g_pMainWindow;

char * QTabBarStyleSheet = "QTabBar { background-color : rgb(163, 163, 163); } \
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



EFFEditorUITabBar * EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar = NULL;

/*class EFFEditorUITabBarEventFilter : public QObject
{
	bool eventFilter(QObject * object, QEvent * event)
	{
		if ( event->type() == QEvent::MouseButtonPress )
		{
			QTabBar * pTabBar = qobject_cast<QTabBar *>(object);
			if ( pTabBar != NULL && pTabBar != EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar )
			{
				if ( EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar != NULL )
				{
					EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setProperty("currentFocusTabBar", false);
					EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setStyleSheet(QString::Null());
					EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setStyleSheet(QTabBarStyleSheet);
				}
				EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar = pTabBar;
				EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setProperty("currentFocusTabBar", true);
				EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setStyleSheet(QString::Null());
				EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setStyleSheet(QTabBarStyleSheet);
			}




		}
		return false;
	}
};*/


class EFFEditorUITabBar : public QTabBar
{
public:
	EFFEditorUITabBar(QWidget * pParent) : QTabBar(pParent) { }
	~EFFEditorUITabBar() { }
public:
	void mousePressEvent(QMouseEvent * pressEvent)
	{
		if ( this != EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar )
		{
			if ( EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar != NULL )
			{
				EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setProperty("currentFocusTabBar", false);
				EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setStyleSheet(QString::Null());
				EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setStyleSheet(QTabBarStyleSheet);
			}
			EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar = this;
			EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setProperty("currentFocusTabBar", true);
			EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setStyleSheet(QString::Null());
			EFFEditorUIDockWidgetTitleBar::m_pCurrentFocusTabBar->setStyleSheet(QTabBarStyleSheet);
		}

		pressEvent->ignore();
	}

	void mouseDoubleClickEvent(QMouseEvent * doubleClickEvnet) { doubleClickEvnet->ignore(); }
	void mouseMoveEvent(QMouseEvent * mouseMoveEvent) { mouseMoveEvent->ignore(); }
	void mouseReleaseEvent(QMouseEvent * mouseReleaseEvent) { mouseReleaseEvent->ignore(); }
};


EFFEditorUIDockWidgetTitleBar::EFFEditorUIDockWidgetTitleBar(QWidget * parent, const QString & title) : QWidget(parent)
{
	setObjectName("dockWidgetTitleBar");

	/*QWidget * p = new QWidget(this);
	p->setObjectName("dockWidgetTitleBarLabelWrap");
	p->resize(120, 16);
	p->move(0, 8);

	QLabel * pL = new QLabel("Inspector", p);
	pL->resize(120, 16);*/

	m_pTabBar = new EFFEditorUITabBar(this);
	m_pTabBar->addTab(title);
	m_pTabBar->move(0, 5);
	//m_pTabBar->setStyleSheet(QTabBarStyleSheet);


	m_pEmptyTitleBar = new QWidget();

}


EFFEditorUIDockWidgetTitleBar::~EFFEditorUIDockWidgetTitleBar()
{

}

void EFFEditorUIDockWidgetTitleBar::resetQss()
{
	m_pTabBar->setStyleSheet(QString::Null());
	m_pTabBar->setStyleSheet(QTabBarStyleSheet);
	m_pTabBar->resize(300, 24);
}


void EFFEditorUIDockWidgetTitleBar::mousePressEvent(QMouseEvent * pressEvent)
{
	//m_pTabBar->mousePressEvent(pressEvent);
	if ( m_pTabBar->geometry().contains(pressEvent->pos()) )
	{
		pressEvent->ignore();
	}
}

void EFFEditorUIDockWidgetTitleBar::mouseReleaseEvent(QMouseEvent * mouseReleaseEvent)
{
	//m_pTabBar->mouseReleaseEvent(mouseReleaseEvent);
	if ( m_pTabBar->geometry().contains(mouseReleaseEvent->pos()) )
	{
		mouseReleaseEvent->ignore();
	}
}

void EFFEditorUIDockWidgetTitleBar::dockWidgetsTabified(QDockWidget * pDockWidget, bool bVisiable)
{
	return;
	QString s = pDockWidget->windowTitle();

	QList<QDockWidget *> tdws = g_pMainWindow->tabifiedDockWidgets(pDockWidget);

	if ( tdws.size() > 0 )
	{
		pDockWidget->setTitleBarWidget(m_pEmptyTitleBar);
	}
	else
	{
		if ( pDockWidget->titleBarWidget() != this )
		{
			pDockWidget->setTitleBarWidget(this);
		}
	}

	/*QList<QTabBar *> allTabBars = g_pMainWindow->findChildren<QTabBar *>();
	for ( int i = 0; i < allTabBars.size(); ++i )
	{
		allTabBars[i]->setTabsClosable(true);
	}*/

	return;

	if ( tdws.size() == 0 )
	{
		for ( int i = 1; i < m_pTabBar->count(); i++ )
		{
			m_pTabBar->removeTab(i);
		}
	}
	else
	{
		for ( int i = 0; i < tdws.size(); ++i )
		{
			if ( tdws[i] != pDockWidget )
			{
				bool bFind = false;
				for ( int j = 0; j < m_pTabBar->count(); j++ )
				{
					if ( m_pTabBar->tabText(j) == tdws[i]->windowTitle() )
					{
						bFind = true;
						break;
					}
				}

				if ( !bFind )
				{
					m_pTabBar->addTab(tdws[i]->windowTitle());
					m_pTabBar->setStyleSheet(QString::Null());
					m_pTabBar->setStyleSheet(QTabBarStyleSheet);
					m_pTabBar->resize(300, 24);
				}

			}
		}
	}

}