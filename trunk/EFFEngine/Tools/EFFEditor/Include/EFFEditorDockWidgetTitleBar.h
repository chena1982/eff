/********************************************************************
	created:	5:9:2010   23:12
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Include\EFFEditorUIDockWidgetTitleBar.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorUIDockWindowTitleBar_2010_9_5__
#define __EFFEditorUIDockWindowTitleBar_2010_9_5__


class EFFEditorUITabBar;

class EFFEditorUIDockWidgetTitleBar : public QWidget
{
public:
	EFFEditorUIDockWidgetTitleBar(QWidget * parent, const QString & title);
	~EFFEditorUIDockWidgetTitleBar();

public:
	QTabBar * getTabBar() { return (QTabBar *)m_pTabBar; }
	void resetQss();
public:
	QSize sizeHint() const { return minimumSizeHint(); }
	QSize minimumSizeHint()  const { return QSize(260, 24); }

public:
	static EFFEditorUITabBar *				m_pCurrentFocusTabBar;
protected:
	void mousePressEvent(QMouseEvent * pressEvent);
	void mouseReleaseEvent(QMouseEvent * mouseReleaseEvent);
	void mouseDoubleClickEvent(QMouseEvent * doubleClickEvnet) { doubleClickEvnet->ignore(); }
	void mouseMoveEvent(QMouseEvent * mouseMoveEvent) { mouseMoveEvent->ignore(); }

public:
	void dockWidgetsTabified(QDockWidget * pDockWidget, bool bVisiable);

private:
	EFFEditorUITabBar *						m_pTabBar;
	QWidget *									m_pEmptyTitleBar;
};
#endif