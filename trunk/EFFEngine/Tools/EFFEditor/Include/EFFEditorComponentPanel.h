/********************************************************************
	created:	20:11:2010   13:52
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\Include\EFFEditorComponentPanel.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorComponentPanel_2010_11_20__
#define __EFFEditorComponentPanel_2010_11_20__



class EFFEditorComponentPanel : public QWidget
{
	Q_OBJECT
public:
	enum PropertyNameType
	{
		Normal,
		HasChild,
		Child,
	};
public:
	EFFEditorComponentPanel();
	~EFFEditorComponentPanel();


public:
	void BindComponent(EFFComponent * component);

	void CreateTitle();

public:
	void paintEvent(QPaintEvent *);

public slots:
	void PropertyBranchWidgetClicked(int state);
	void ComponentBranchWidgetClicked(int state);
protected:
	QWidget * AddPropertyName(const effString & propertyName, PropertyNameType propertyNameType);
	void AddPropertyValue(EFFComponent * component, EFFProperty * addedProperty, QWidget * lineWidget);

	void AddProperyValueString(const effString & propertyValue, QWidget * lineWidget);
	void AddProperyValueBool(effBOOL propertyValue, QWidget * lineWidget);

protected:
	EFFComponent *	m_pComponent;
	QVBoxLayout *	m_pMainLayout;
	QWidget *		m_pContent;
	QVBoxLayout *	m_pContentLayout;

	std::map<QCheckBox *, QLabel *>	m_BranchPropertyNameWidgetPairs;
	std::vector<QWidget *>			m_LineWidgets;
};

#endif