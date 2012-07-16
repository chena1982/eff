/********************************************************************
	created:	20:11:2010   13:54
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Src\EFFEditorComponentPanel.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorComponentPanel.h"

#include "qtpropertybrowser.h"  
#include "qtpropertymanager.h"
#include <QtTreePropertyBrowser>

class QtReferencePropertyManager : public QtStringPropertyManager
{
	Q_OBJECT

public:
	QtReferencePropertyManager(QObject * parent = NULL) : QtStringPropertyManager(parent) {}
};


EFFEditorComponentPanel::EFFEditorComponentPanel()
{
	QToolBar * pToolbar = new QToolBar();
	
	setObjectName(tr("componentPanel"));

	m_pMainLayout = new QVBoxLayout();
	m_pMainLayout->addWidget(pToolbar);
	m_pMainLayout->setContentsMargins(1, 1, 1, 1);
	m_pMainLayout->setSpacing(0);

	setLayout(m_pMainLayout);
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

	//QtTreePropertyBrowser * propertyGrid = new QtTreePropertyBrowser(this);
	//propertyGrid->setObjectName(tr("propertyGrid"));
	//treeWidget->setColumnCount(2);
	//QtStringPropertyManager * stringManager = new QtStringPropertyManager();

	EFFClass * componentClass = component->GetRuntimeClass();
	std::vector<EFFProperty *> componentProperties = componentClass->GetProperties();
	for ( effINT i = 0; i < componentProperties.size(); i++ )
	{
		EFFProperty * componentProperty = componentProperties[i];



		/*QPalette palette;
		palette.setColor(QPalette::Background, QColor(255, 0, 0));
		propertyNameWidget->setPalette(palette);*/
		QWidget * lineWidget;
		if ( componentProperty->GetIsSTLContainer() )
		{
			lineWidget = AddPropertyName(componentProperty->GetName(), HasChild);
		}
		else
		{
			lineWidget = AddPropertyName(componentProperty->GetName(), Normal);
		}

		AddPropertyValue(component, componentProperty, lineWidget);

	}


}

QWidget * EFFEditorComponentPanel::AddPropertyName(const effString & propertyName, PropertyNameType propertyNameType)
{
	QString qPropertyName(EFFSTRING2ANSI(propertyName));
	//propertyGrid->addProperty(stringManager->addProperty(qname));

	QWidget * lineWidget = new QWidget(this);
	m_pMainLayout->addWidget(lineWidget);

	QHBoxLayout * propertyLineLayout = new QHBoxLayout();
	propertyLineLayout->setContentsMargins(0, 0, 0, 0);

	lineWidget->setLayout(propertyLineLayout);

	QHBoxLayout * propertyNameLayout = new QHBoxLayout();
	propertyLineLayout->addLayout(propertyNameLayout, 5);

	QLabel * propertyNameWidget = new QLabel(lineWidget);
	propertyNameWidget->setText(qPropertyName);
	propertyNameWidget->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	if ( propertyNameType == HasChild )
	{
		QCheckBox * expandWidget = new QCheckBox(lineWidget);
		expandWidget->setObjectName(tr("expand"));
		propertyNameLayout->addWidget(expandWidget);

		QObject::connect(expandWidget, SIGNAL(stateChanged(int)), this, SLOT(ToggleHasChildProperty(int)));
	}
	else if ( propertyNameType == Normal )
	{
		propertyNameLayout->insertSpacing(0, 20);
	}
	else if ( propertyNameType == Child )
	{
		propertyNameLayout->insertSpacing(0, 40);
	}

	propertyNameLayout->addWidget(propertyNameWidget);

	return lineWidget;
}

void test(const effString & element)
{
}

void EFFEditorComponentPanel::AddPropertyValue(EFFComponent * component, EFFProperty * addedProperty, QWidget * lineWidget)
{
	QHBoxLayout * lineLayout = (QHBoxLayout *)lineWidget->layout();



	EFFClass * componentClass = component->GetRuntimeClass();

	if ( addedProperty->GetClass()->IsPODType() )
	{
		if ( addedProperty->GetIsSTLContainer() )
		{

			effString propertyClassName = addedProperty->GetClass()->GetName();
			if ( propertyClassName == _effT("effString") )
			{
				componentClass->PropertyForEach<effString>(addedProperty->GetName(), component, 
									[=, this](const effString & propertyValue)
									{
										QWidget * elementLineWidget = AddPropertyName(_effT("Element"), Child);
										AddProperyValueString(propertyValue, elementLineWidget);
									});

			}

		}
		else
		{
			effString propertyClassName = addedProperty->GetClass()->GetName();
			if ( propertyClassName == _effT("effBOOL") )
			{
				effBOOL propertyValue;
				componentClass->GetProperty(addedProperty->GetName(), component, propertyValue);
				AddProperyValueBool(propertyValue, lineWidget);
			}
			else if ( propertyClassName == _effT("effString") )
			{
				effString propertyValue;
				componentClass->GetProperty(addedProperty->GetName(), component, propertyValue);
				AddProperyValueString(propertyValue, lineWidget);
			}

		}
	}
}

void EFFEditorComponentPanel::AddProperyValueString(const effString & propertyValue, QWidget * lineWidget)
{
	QHBoxLayout * lineLayout = (QHBoxLayout *)lineWidget->layout();

	QHBoxLayout * propertyValueLayout = new QHBoxLayout();
	lineLayout->addLayout(propertyValueLayout, 5);

	QString qPropertyValue = QString::fromUtf16((const ushort *)propertyValue.c_str());
	QLineEdit * propertyValueWidget = new QLineEdit(lineWidget);
	propertyValueWidget->setText(qPropertyValue);

	propertyValueLayout->addWidget(propertyValueWidget, 5);
}

void EFFEditorComponentPanel::AddProperyValueBool(effBOOL propertyValue, QWidget * lineWidget)
{

	QHBoxLayout * lineLayout = (QHBoxLayout *)lineWidget->layout();

	QHBoxLayout * propertyValueLayout = new QHBoxLayout();
	lineLayout->addLayout(propertyValueLayout, 5);

	QCheckBox * propertyValueWidget = new QCheckBox(lineWidget);
					
	propertyValueWidget->setCheckState(propertyValue ? Qt::Checked : Qt::Unchecked);

	propertyValueLayout->addWidget(propertyValueWidget, 5, Qt::AlignLeft);

}

void EFFEditorComponentPanel::ToggleHasChildProperty(int state)
{
	int z = 0;
}