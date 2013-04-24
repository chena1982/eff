/********************************************************************
	created:	6:9:2010   0:17
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Src\EFFEditorUITransformPanel.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorTransformPanel.h"

EFFEditorTransformPanel::EFFEditorTransformPanel()
{
	setObjectName("transform");

	Create();
}


EFFEditorTransformPanel::~EFFEditorTransformPanel()
{

}

void EFFEditorTransformPanel::Create()
{

	QDoubleValidator * doubleValidator = new QDoubleValidator();

	QToolBar * pToolbar = new QToolBar();
	
	QVBoxLayout * pMainLayout = new QVBoxLayout();
	pMainLayout->addWidget(pToolbar);
	pMainLayout->setContentsMargins(1, 1, 1, 1);
	pMainLayout->setSpacing(2);

	//position
	QLabel * pPosLabel = new QLabel();
	pPosLabel->setText(tr("Position"));
	pMainLayout->addWidget(pPosLabel);

	QHBoxLayout * pPosLayout = new QHBoxLayout();
	pPosLayout->setSpacing(0);
	pPosLayout->addSpacing(10);

	QLabel * pPosXLabel = new QLabel(NULL);
	pPosXLabel->setText(tr("X"));
	pPosLayout->addWidget(pPosXLabel);
	m_pPosXLineEdit = new QLineEdit();
	m_pPosXLineEdit->setValidator(doubleValidator);
	pPosLayout->addWidget(m_pPosXLineEdit, 1);

	QLabel * pPosYLabel = new QLabel(NULL);
	pPosYLabel->setText(tr("Y"));
	pPosLayout->addWidget(pPosYLabel);
	m_pPosYLineEdit = new QLineEdit();
	m_pPosYLineEdit->setValidator(doubleValidator);
	pPosLayout->addWidget(m_pPosYLineEdit, 1);

	QLabel * pPosZLabel = new QLabel();
	pPosZLabel->setText(tr("Z"));
	pPosLayout->addWidget(pPosZLabel);
	m_pPosZLineEdit = new QLineEdit();
	m_pPosZLineEdit->setValidator(doubleValidator);
	pPosLayout->addWidget(m_pPosZLineEdit, 1);

	pMainLayout->addLayout(pPosLayout);



	//rotation
	QLabel * pRotLabel = new QLabel();
	pRotLabel->setText(tr("Rotation"));
	pMainLayout->addWidget(pRotLabel);

	QHBoxLayout * pRotLayout = new QHBoxLayout();
	pRotLayout->setSpacing(0);
	pRotLayout->addSpacing(10);

	QLabel * pRotXLabel = new QLabel();
	pRotXLabel->setText(tr("X"));
	pRotLayout->addWidget(pRotXLabel);
	m_pRotXLineEdit = new QLineEdit();
	m_pRotXLineEdit->setValidator(doubleValidator);
	pRotLayout->addWidget(m_pRotXLineEdit, 1);

	QLabel * pRotYLabel = new QLabel();
	pRotYLabel->setText(tr("Y"));
	pRotLayout->addWidget(pRotYLabel);
	m_pRotYLineEdit = new QLineEdit();
	m_pRotYLineEdit->setValidator(doubleValidator);
	pRotLayout->addWidget(m_pRotYLineEdit, 1);

	QLabel * pRotZLabel = new QLabel();
	pRotZLabel->setText(tr("Z"));
	pRotLayout->addWidget(pRotZLabel);
	m_pRotZLineEdit = new QLineEdit();
	m_pRotZLineEdit->setValidator(doubleValidator);
	pRotLayout->addWidget(m_pRotZLineEdit, 1);

	pMainLayout->addLayout(pRotLayout);


	//scale
	QLabel * pScaleLabel = new QLabel();
	pScaleLabel->setText(tr("Scale"));
	pMainLayout->addWidget(pScaleLabel);

	QHBoxLayout * pScaleLayout = new QHBoxLayout();
	pScaleLayout->setSpacing(0);
	pScaleLayout->addSpacing(10);

	QLabel * pScaleXLabel = new QLabel();
	pScaleXLabel->setText(tr("X"));
	pScaleLayout->addWidget(pScaleXLabel);
	m_pScaleXLineEdit = new QLineEdit();
	m_pScaleXLineEdit->setValidator(doubleValidator);
	pScaleLayout->addWidget(m_pScaleXLineEdit, 1);

	QLabel * pScaleYLabel = new QLabel();
	pScaleYLabel->setText(tr("Y"));
	pScaleLayout->addWidget(pScaleYLabel);
	m_pScaleYLineEdit = new QLineEdit();
	m_pScaleYLineEdit->setValidator(doubleValidator);
	pScaleLayout->addWidget(m_pScaleYLineEdit, 1);

	QLabel * pScaleZLabel = new QLabel();
	pScaleZLabel->setText(tr("Z"));
	pScaleLayout->addWidget(pScaleZLabel);
	m_pScaleZLineEdit = new QLineEdit();
	m_pScaleZLineEdit->setValidator(doubleValidator);
	pScaleLayout->addWidget(m_pScaleZLineEdit, 1);

	pMainLayout->addLayout(pScaleLayout);


	setLayout(pMainLayout);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

effVOID EFFEditorTransformPanel::BindObject(EFF3DObject * object)
{
	m_pBindedObject = object;

	Update();
}

void EFFEditorTransformPanel::Update()
{
	EFFVector3 temp;
	m_pBindedObject->GetPropertyValue(_effT("pos"), temp);

	m_pPosXLineEdit->setText(QString().setNum(temp.x));
	m_pPosYLineEdit->setText(QString().setNum(temp.x));
	m_pPosZLineEdit->setText(QString().setNum(temp.x));


	m_pBindedObject->GetPropertyValue(_effT("rot"), temp);

	m_pRotXLineEdit->setText(QString().setNum(temp.x));
	m_pRotYLineEdit->setText(QString().setNum(temp.x));
	m_pRotZLineEdit->setText(QString().setNum(temp.x));

	m_pBindedObject->GetPropertyValue(_effT("scale"), temp);

	m_pScaleXLineEdit->setText(QString().setNum(temp.x));
	m_pScaleYLineEdit->setText(QString().setNum(temp.x));
	m_pScaleZLineEdit->setText(QString().setNum(temp.x));
}