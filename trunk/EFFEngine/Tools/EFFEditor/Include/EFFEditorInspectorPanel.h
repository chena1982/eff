/********************************************************************
	created:	5:9:2010   23:20
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Include\EFFEditorInspectorPanel.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorInspectorPanel_2010_9_5__
#define __EFFEditorInspectorPanel_2010_9_5__

class EFFEditorUIDockWidgetTitleBar;
class EFFEditorTransformPanel;


class EFFEditorInspectorPanel : public QDockWidget
{
	Q_OBJECT
public:
	EFFEditorInspectorPanel(QWidget * parent);
	~EFFEditorInspectorPanel();
public:
	QSize sizeHint() const { return QSize(300, -1); }
public:
	effVOID BindObject(EFFObject * object);
public slots:
	void titleBarAddOrDeleteTab(bool bAdd);

private:
	EFFEditorUIDockWidgetTitleBar *			m_pTitleBar;
	EFFEditorTransformPanel *				m_pTransformPanel;

	QVBoxLayout *							m_pMainLayout;
	QWidget *								m_pContent;
	std::map<EFFComponent *, QWidget *>		m_ComponentPanels;
};

#endif