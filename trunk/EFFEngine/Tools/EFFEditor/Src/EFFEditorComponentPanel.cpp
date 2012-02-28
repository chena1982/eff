/********************************************************************
	created:	20:11:2010   13:54
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Src\EFFEditorComponentPanel.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorComponentPanel.h"

EFFEditorComponentPanel::EFFEditorComponentPanel()
{
	QToolBar * pToolbar = new QToolBar();
	
	mainLayout = new QVBoxLayout();
	mainLayout->addWidget(pToolbar);
	mainLayout->setContentsMargins(1, 1, 1, 1);
	mainLayout->setSpacing(2);

	setLayout(mainLayout);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}


EFFEditorComponentPanel::~EFFEditorComponentPanel()
{

}

void EFFEditorComponentPanel::createTransformPanel()
{

}

void EFFEditorComponentPanel::BindComponent(EFFComponent * component)
{

	QTreeWidget * treeWidget = new QTreeWidget();
	treeWidget->setColumnCount(2);

	EFFClass * componentClass = component->GetRuntimeClass();
	std::vector<EFFProperty *> componentProperties = componentClass->GetProperties();
	for ( effINT i = 0; i < componentProperties.size(); i++ )
	{
		EFFProperty * componentProperty = componentProperties[i];
		
		if ( componentProperty->GetClass()->IsPODType() )
		{
			if ( componentProperty->GetIsSTLContainer() )
			{
			}
			else
			{
				QTreeWidgetItem * propertyItem = new QTreeWidgetItem();

				QString qname;
				qname.fromStdWString(componentProperty->GetName());
				propertyItem->setText(0, qname);

				if ( componentProperty->GetClass()->GetClassName() == _effT("effBOOL") )
				{
					effBOOL checked;
					componentClass->GetProperty(componentProperty->GetName(), component, checked);
					if ( checked )
					{
						propertyItem->setCheckState(1, Qt::Checked);
					}
					else
					{
						propertyItem->setCheckState(1, Qt::Unchecked);
					}
				}
				treeWidget->addTopLevelItem(propertyItem);
			}
		}
	}

	mainLayout->addWidget(treeWidget);
}