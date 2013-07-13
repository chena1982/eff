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




EFFEditorComponentPanel::EFFEditorComponentPanel()
{
	setObjectName(tr("componentPanel"));

	//setMinimumSize(QSize(-1, 100));

	m_pMainLayout = new QVBoxLayout();

	m_pMainLayout->setContentsMargins(0, 0, 0, 0);
	m_pMainLayout->setSpacing(0);

	m_pContent = new QWidget(this);

	m_pContentLayout = new QVBoxLayout();
	m_pContentLayout->setContentsMargins(0, 0, 0, 0);
	m_pContentLayout->setSpacing(0);

	m_pContent->setLayout(m_pContentLayout);

	m_pMainLayout->addWidget(m_pContent);

	setLayout(m_pMainLayout);
	//setWidget(m_pContent);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}


EFFEditorComponentPanel::~EFFEditorComponentPanel()
{

}



void EFFEditorComponentPanel::CreateTitle()
{

	QToolBar * toolbar = new QToolBar(this);
	m_pMainLayout->insertWidget(0, toolbar);

	QCheckBox * branchWidget = new QCheckBox(toolbar);
	branchWidget->setObjectName(tr("branch"));
	toolbar->addWidget(branchWidget);

	QObject::connect(branchWidget, SIGNAL(stateChanged(int)), this, SLOT(ComponentBranchWidgetClicked(int)));

	QLabel * componentTypeIcon = new QLabel(toolbar);
	componentTypeIcon->setPixmap(tr("./EditorRes/Skin/Dark/mesh_icon.png"));
	toolbar->addWidget(componentTypeIcon);

	QCheckBox * componentEnabledWidget = new QCheckBox(toolbar);
	toolbar->addWidget(componentEnabledWidget);

	QLabel * componentTypeNameWidget = new QLabel(toolbar);
	QString qComponentTypeName = QString::fromUtf16((const ushort *)m_pComponent->GetRuntimeClass()->GetName().c_str());
	componentTypeNameWidget->setObjectName(tr("componentTypeName"));
	componentTypeNameWidget->setText(qComponentTypeName);
	toolbar->addWidget(componentTypeNameWidget);
}



void EFFEditorComponentPanel::BindComponent(EFFComponent * component)
{
	m_pComponent = component;

	CreateTitle();

	EFFClass * componentClass = component->GetRuntimeClass();
	std::vector<EFFProperty *> componentProperties = componentClass->GetProperties();
	for ( effINT i = 0; i < componentProperties.size(); i++ )
	{
		EFFProperty * componentProperty = componentProperties[i];

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

void EFFEditorComponentPanel::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void EFFEditorComponentPanel::PropertyBranchWidgetClicked(int state)
{
	QCheckBox * branchWidget = (QCheckBox *)QObject::sender();

	QWidget * lineWidget = branchWidget->parentWidget();

	auto it = m_BranchPropertyNameWidgetPairs.find(branchWidget);
	if ( it != m_BranchPropertyNameWidgetPairs.end() )
	{
		QLabel * propertyNameWidget = it->second;
		QString qPropertyName = propertyNameWidget->text();

		

		EFFProperty * componentProperty = m_pComponent->GetProperty(effString((effTCHAR *)qPropertyName.utf16()));

		if ( componentProperty == NULL )
		{
			return;
		}

		effINT elementCount = componentProperty->GetElementCount(m_pComponent);

		for ( effUINT i = 0; i < m_LineWidgets.size(); i++ )
		{
			if ( m_LineWidgets[i] == lineWidget )
			{
				for ( effUINT j = i + 1; j < i + 1 + elementCount; j++ )
				{
					if ( state == Qt::Checked )
					{
						m_LineWidgets[j]->hide();
					}
					else if ( state == Qt::Unchecked )
					{
						m_LineWidgets[j]->show();
					}
				}
				break;
			}
		}



	}
}

void EFFEditorComponentPanel::ComponentBranchWidgetClicked(int state)
{
	/*for ( effUINT i = 0; i < m_LineWidgets.size(); i++ )
	{
		if ( state == Qt::Checked )
		{
			m_LineWidgets[i]->hide();
		}
		else if ( state == Qt::Unchecked )
		{
			m_LineWidgets[i]->show();
		}
	}*/

	if ( state == Qt::Checked )
	{
		m_pContent->hide();
	}
	else if ( state == Qt::Unchecked )
	{
		m_pContent->show();
	}
}

QWidget * EFFEditorComponentPanel::AddPropertyName(const effString & propertyName, PropertyNameType propertyNameType)
{
	QString qPropertyName = QString::fromUtf16((const ushort *)propertyName.c_str());

	QWidget * lineWidget = new QWidget(m_pContent);




	m_pContentLayout->addWidget(lineWidget, 0, Qt::AlignBottom);

	m_LineWidgets.push_back(lineWidget);

	QHBoxLayout * lineLayout = new QHBoxLayout();
	lineLayout->setContentsMargins(0, 0, 0, 0);

	lineWidget->setLayout(lineLayout);

	QHBoxLayout * propertyNameLayout = new QHBoxLayout();
	propertyNameLayout->setContentsMargins(0, 0, 0, 0);
	propertyNameLayout->setSpacing(3);

	lineLayout->addLayout(propertyNameLayout, 5);

	QLabel * propertyNameWidget = new QLabel(lineWidget);
	propertyNameWidget->setText(qPropertyName);
	propertyNameWidget->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	if ( propertyNameType == HasChild )
	{
		QCheckBox * branchWidget = new QCheckBox(lineWidget);
		branchWidget->setObjectName(tr("branch"));
		propertyNameLayout->addWidget(branchWidget);

		QObject::connect(branchWidget, SIGNAL(stateChanged(int)), this, SLOT(PropertyBranchWidgetClicked(int)));

		m_BranchPropertyNameWidgetPairs[branchWidget] = propertyNameWidget;
	}
	else if ( propertyNameType == Normal )
	{
		propertyNameLayout->insertSpacing(0, 16);
	}
	else if ( propertyNameType == Child )
	{
		propertyNameLayout->insertSpacing(0, 32);
	}

	propertyNameLayout->addWidget(propertyNameWidget);

	return lineWidget;
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
				addedProperty->ForEach<effString>(component, [&](const effString & propertyValue)
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
				addedProperty->GetValue(component, propertyValue);
				AddProperyValueBool(propertyValue, lineWidget);
			}
			else if ( propertyClassName == _effT("effString") )
			{
				effString propertyValue;
				addedProperty->GetValue(component, propertyValue);
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

	propertyValueLayout->addWidget(propertyValueWidget, 5, Qt::AlignLeft | Qt::AlignVCenter);

}

