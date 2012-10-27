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

class EFFEditorRealTimeWidget;

class EFFEditorScenePanel : public QDockWidget
{
	Q_OBJECT
public:
	EFFEditorScenePanel(QWidget * pParent);
	~EFFEditorScenePanel();
public:
	void resizeEvent(QResizeEvent * resizeEvent);
public slots:
	void drawModeMenuPressed(QAction *);
	void renderModeMenuPressed(QAction *);
protected:
	void createToolbar();
private:
	//EFFEditorUIDockWidgetTitleBar *			m_pTitleBar;
	//QStackedWidget *									m_pStackedWidget;

	QVBoxLayout *											m_pMainLayout;
	QWidget *												m_pContent;
	QToolBar *												m_pToolbar;
	//EFFEditorRealTimeWidget *								m_pRealTimeContent;
	QWidget *												m_pRealTimeContent;

};

#endif