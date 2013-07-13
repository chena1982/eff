/********************************************************************
	created:	6:9:2010   22:08
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Include\EFFEditorUIProjectPanel.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorUIProjectPanel_2010_9_6__
#define __EFFEditorUIProjectPanel_2010_9_6__

class EFFEditorUIDockWidgetTitleBar;

class EFFEditorProjecPanel : public QDockWidget
{
	Q_OBJECT
public:
	EFFEditorProjecPanel(QWidget * parent);
	~EFFEditorProjecPanel();

public slots:
		void titleBarAddOrDeleteTab(bool bAdd);

private:
	EFFEditorUIDockWidgetTitleBar *			m_pTitleBar;
};

#endif