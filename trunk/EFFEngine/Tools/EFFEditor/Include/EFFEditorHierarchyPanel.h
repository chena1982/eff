/********************************************************************
	created:	6:9:2010   22:15
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Include\EFFEditorHierarchyPanel.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorUIHierarchyPanel_2010_9_6__
#define __EFFEditorUIHierarchyPanel_2010_9_6__

class EFFEditorUIDockWidgetTitleBar;

class EFFEditorHierarchyPanel : public QDockWidget
{
	Q_OBJECT
public:
	EFFEditorHierarchyPanel(QWidget * parent);
	~EFFEditorHierarchyPanel();

public slots:
		void titleBarAddOrDeleteTab(bool bAdd);

private:
	EFFEditorUIDockWidgetTitleBar *			m_pTitleBar;
};

#endif