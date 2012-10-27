/********************************************************************
	created:	13:11:2010   20:31
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Include\EFFEditorUIDockWidget.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorUIDockWidget_2010_11_13__
#define __EFFEditorUIDockWidget_2010_11_13__


class EFFEditorUIDockWidgetTitleBar;



class EFFEditorUIDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	EFFEditorUIDockWidget(QWidget * pParent, const QString & strTitle);
	virtual ~EFFEditorUIDockWidget();

public:

public slots:
		void addOrRemoveTab(bool bAdd);

protected:
	void addWidget(QWidget * pWidget);
	void resetQss();

private:
	//EFFEditorUIDockWidgetTitleBar1 *									m_pTitleBar;
	//QTabBar *										m_pTabBar;
	EFFEditorUIDockWidgetTitleBar *	m_pTitleBar;
	QStackedWidget *						m_pStackedWidget;
};


#endif