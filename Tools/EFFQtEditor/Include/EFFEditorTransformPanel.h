/********************************************************************
	created:	6:9:2010   0:15
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Include\EFFEditorUITransformPanel.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorUITransformPanel_2010_9_6__
#define __EFFEditorUITransformPanel_2010_9_6__

class EFFEditorTransformPanel : public QWidget
{
public:
	EFFEditorTransformPanel();
	~EFFEditorTransformPanel();

public:
	//QSize sizeHint() const { return minimumSizeHint(); }
	//QSize minimumSizeHint()  const { return QSize(260, -1); }
	effVOID BindObject(EFF3DObject * object);
protected:
	void Create();
	void Update();
protected:

	QLineEdit * m_pPosXLineEdit;
	QLineEdit * m_pPosYLineEdit;
	QLineEdit * m_pPosZLineEdit;

	QLineEdit * m_pRotXLineEdit;
	QLineEdit * m_pRotYLineEdit;
	QLineEdit * m_pRotZLineEdit;

	QLineEdit * m_pScaleXLineEdit;
	QLineEdit * m_pScaleYLineEdit;
	QLineEdit * m_pScaleZLineEdit;

	EFF3DObject * m_pBindedObject;

};

#endif