/********************************************************************
	created:	6:9:2010   22:20
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Include\EFFEditorScenePanel.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorUIScenePanel_2010_9_6__
#define __EFFEditorUIScenePanel_2010_9_6__

class EFFEditorUIDockWidgetTitleBar;

#include "EFFEditorDockWidget.h"

class EFFEditorScenePanel : public QDockWidget
{
	Q_OBJECT
public:
	EFFEditorScenePanel(QWidget * pParent);
	~EFFEditorScenePanel();

public slots:
	void titleBarAddOrDeleteTab(bool bAdd);
	void drawModeMenuPressed(QAction *);
	void renderModeMenuPressed(QAction *);
	void test(int index);
protected:
	void createToolbar();
private:
	//EFFEditorUIDockWidgetTitleBar *			m_pTitleBar;
	//QStackedWidget *									m_pStackedWidget;

	QVBoxLayout *											m_pMainLayout;
	QWidget *												m_pContent;
	QToolBar *												m_pToolbar;
	QWidget *												m_pRealTimeContent;

};

#endif