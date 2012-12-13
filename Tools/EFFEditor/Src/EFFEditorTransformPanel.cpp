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

	create();
}


EFFEditorTransformPanel::~EFFEditorTransformPanel()
{

}

void EFFEditorTransformPanel::create()
{
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
	QLineEdit * pPosXLineEdit = new QLineEdit();
	//pPosXLineEdit->setText(tr("1111"));
	pPosLayout->addWidget(pPosXLineEdit, 1);

	QLabel * pPosYLabel = new QLabel(NULL);
	pPosYLabel->setText(tr("Y"));
	pPosLayout->addWidget(pPosYLabel);
	QLineEdit * pPosYLineEdit = new QLineEdit();
	//pPosYLineEdit->setText(tr("1111"));
	pPosLayout->addWidget(pPosYLineEdit, 1);

	QLabel * pPosZLabel = new QLabel();
	pPosZLabel->setText(tr("Z"));
	pPosLayout->addWidget(pPosZLabel);
	QLineEdit * pPosZLineEdit = new QLineEdit();
	//pPosZLineEdit->setText(tr("1111"));
	pPosLayout->addWidget(pPosZLineEdit, 1);

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
	QLineEdit * pRotXLineEdit = new QLineEdit();
	//pRotXLineEdit->setText(tr("1111"));
	pRotLayout->addWidget(pRotXLineEdit, 1);

	QLabel * pRotYLabel = new QLabel();
	pRotYLabel->setText(tr("Y"));
	pRotLayout->addWidget(pRotYLabel);
	QLineEdit * pRotYLineEdit = new QLineEdit();
	//pRotYLineEdit->setText(tr("1111"));
	pRotLayout->addWidget(pRotYLineEdit, 1);

	QLabel * pRotZLabel = new QLabel();
	pRotZLabel->setText(tr("Z"));
	pRotLayout->addWidget(pRotZLabel);
	QLineEdit * pRotZLineEdit = new QLineEdit();
	//pRotZLineEdit->setText(tr("1111"));
	pRotLayout->addWidget(pRotZLineEdit, 1);

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
	QLineEdit * pScaleXLineEdit = new QLineEdit();
	//pScaleXLineEdit->setText(tr("1111"));
	pScaleLayout->addWidget(pScaleXLineEdit, 1);

	QLabel * pScaleYLabel = new QLabel();
	pScaleYLabel->setText(tr("Y"));
	pScaleLayout->addWidget(pScaleYLabel);
	QLineEdit * pScaleYLineEdit = new QLineEdit();
	//pScaleYLineEdit->setText(tr("1111"));
	pScaleLayout->addWidget(pScaleYLineEdit, 1);

	QLabel * pScaleZLabel = new QLabel();
	pScaleZLabel->setText(tr("Z"));
	pScaleLayout->addWidget(pScaleZLabel);
	QLineEdit * pScaleZLineEdit = new QLineEdit();
	//pScaleZLineEdit->setText(tr("1111"));
	pScaleLayout->addWidget(pScaleZLineEdit, 1);

	pMainLayout->addLayout(pScaleLayout);


	setLayout(pMainLayout);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}