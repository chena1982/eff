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

public:
	void AddObject(EFF3DObject * object);

public slots:
	void titleBarAddOrDeleteTab(bool bAdd);
	void createMenuPressed(QAction *);
protected:
	void createToolbar();
private:
	EFFEditorUIDockWidgetTitleBar *			m_pTitleBar;

	QVBoxLayout *							m_pMainLayout;
	QWidget *								m_pContent;
	QToolBar *								m_pToolbar;
	QTreeWidget *							m_pTreeWidget;
};

#endif