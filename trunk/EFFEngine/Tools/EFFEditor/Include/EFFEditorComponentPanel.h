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
public:
	EFFEditorComponentPanel();
	~EFFEditorComponentPanel();


public:
	template<class T>
	void addComponentPanel(T * pComponent) {};


	void BindComponent(EFFComponent * component);

	void createTransformPanel();
protected:
	EFFObject * object;
	QVBoxLayout * mainLayout;
};

#endif