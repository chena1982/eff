/******************************************************************************
	created:	2008-12-10   0:53
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"
#include "EFFComponent.h"


EFFBASE_BEGIN


RTTI_IMPLEMENT_BASE_NO_SAVE(EFFReferenceCount, 0)
RTTI_IMPLEMENT_CUSTOM_SAVE(EFFObjectBase, 0)
RTTI_IMPLEMENT_CUSTOM_SAVE(EFFObject, 0)



effUINT EFFReferenceCount::AddRef()
{
	return ++refCount;
}

effVOID EFFReferenceCount::Release()
{
	refCount--;
	if ( refCount == 0 )
	{
		delete this;
	}
}



EFFObjectBase::EFFObjectBase()
{
	id = 0;
}

EFFObjectBase::~EFFObjectBase()
{

}


effVOID EFFObjectBase::SaveToFile(EFFFile * file, effBOOL isBinary)
{
	std::vector<EFFClass *> classes;
	EFFClass * Class = GetRuntimeClass();
	while ( Class != NULL )
	{
		classes.push_back(Class);
		Class = Class->GetBaseClass();
	}

	for ( effINT i = (effINT)(classes.size() - 1); i >= 0; i-- )
	{
		std::vector<EFFProperty *> & properties = classes[i]->GetProperties();
		for ( effUINT j = 0; j < properties.size(); j++ )
		{
			EFFProperty * curProperty = properties[j];
			curProperty->SaveToFile(file, this, isBinary);
		}
	}
}

effVOID EFFObjectBase::SaveToFile(const effString & filePath, effBOOL isBinary)
{
}


EFFProperty * EFFObjectBase::GetProperty(const effString & propertyName)
{
	EFFClass * objectClass = GetRuntimeClass();
	auto properties = objectClass->GetProperties();
	for ( effUINT i = 0; i < properties.size(); i++ )
	{
		if ( properties[i]->GetNameHash() == EFFStringHash(propertyName) )
		{
			return properties[i];
		}
	}

	return NULL;
}




effVOID EFFObjectBase::GetPropertyJason(const effString & propertyName, effString & jasonString)
{
	Json::Value root;
	root["encoding"] = "UTF-8";
	root["object_type"] = EFFSTRING2ANSI(GetRuntimeClass()->GetName());
	root["object_id"] = GetID();
	
	Json::Value objectProperty;
	AddPropertyToJson(propertyName, &objectProperty);
	root["object_property"] = objectProperty;

	jasonString = ANSI2EFFSTRING(root.toStyledString().c_str());
}

effVOID	EFFObjectBase::AddPropertyToJson(const effString & propertyName, Json::Value * jsonValue)
{

	EFFProperty * addedProperty = GetProperty(propertyName);

	if ( addedProperty == NULL )
	{
		return;
	}

	if ( addedProperty->GetClass()->IsPODType() )
	{
		if ( addedProperty->GetIsSTLContainer() )
		{

			effString propertyClassName = addedProperty->GetClass()->GetName();
			if ( propertyClassName == _effT("effString") )
			{
				//Json::Value elements;
				addedProperty->ForEach<effString>(this, [&](const effString & propertyValue) {
														(*jsonValue)[EFFSTRING2ANSI(propertyName)].append(EFFSTRING2ANSI(propertyValue)); });

				//(*root)[EFFSTRING2ANSI(propertyName)] = elements;
			}

		}
		else
		{
			effString propertyClassName = addedProperty->GetClass()->GetName();
			if ( propertyClassName == _effT("effBOOL") )
			{
				effBOOL propertyValue;
				addedProperty->GetValue(this, propertyValue);
				(*jsonValue)[EFFSTRING2ANSI(propertyName)] = propertyValue;
			}
			else if ( propertyClassName == _effT("effString") )
			{
				effString propertyValue;
				addedProperty->GetValue(this, propertyValue);
				(*jsonValue)[EFFSTRING2ANSI(propertyName)] = EFFSTRING2ANSI(propertyValue);
			}

		}
	}
}



EFFObject::EFFObject()
{
	parent = NULL;
}

EFFObject::~EFFObject()
{
}


effVOID EFFObject::SaveToFile(EFFFile * file, effBOOL isBinary)
{

	EFFObjectBase::SaveToFile(file, isBinary);

	SaveComponentsToFile(file, isBinary);
}

effVOID EFFObject::SaveToFile(const effString & filePath, effBOOL isBinary)
{
	if ( isBinary )
	{
		EFFSTLFile file;
		if ( !file.Open(filePath, _effT("wb")) )
		{
			return;
		}
		SaveToFile(&file, isBinary);
	}
	else
	{
		if ( StartSaveToYAMLFile(filePath) )
		{
			std::vector<EFFObject *> objects;
			std::vector<EFFComponent *> properties;
			TraverseObjectTree(objects, properties);

			effINT implicit = 0;

			for ( effUINT i = 0; i < objects.size(); i++ )
			{
				EFFObject * object = objects[i];

				BeginSaveObjectBaseToYAMLFile(object, implicit);

				//只有第一个object需要显示的version directive和tag directive
				if ( implicit == 0 )
				{
					implicit = 1;
				}

				object->SaveToFile(NULL, isBinary);
				EndSaveObjectBaseToYAMLFile(object);

				for ( effUINT i = 0; i < object->components.size(); i++ )
				{
					EFFComponent * component = object->components[i];
					BeginSaveObjectBaseToYAMLFile(component, implicit);
					component->SaveToFile(NULL, isBinary);
					EndSaveObjectBaseToYAMLFile(component);
				}
			}

			for ( effUINT i = 0; i < properties.size(); i++ )
			{
				EFFComponent * component = properties[i];
				BeginSaveObjectBaseToYAMLFile(component, implicit);
				component->SaveToFile(NULL, isBinary);
				EndSaveObjectBaseToYAMLFile(component);
			}

			EndSaveToYAMLFile();
		}
	}
}

effVOID	EFFObject::SaveComponentsToFile(EFFFile * file, effBOOL isBinary)
{
	if ( isBinary )
	{
	}
	else
	{
		BeginSaveSequenceToYAMLFile(_effT("component"));

		for ( effUINT i = 0; i < components.size(); i++ )
		{
			EFFComponent * component = components[i];
			SaveComponentToYAMLFile(component);
		}

		EndSaveSequenceToYAMLFile();
	}
}

effBOOL	EFFObject::AddChild(EFFObject * object)
{
	if ( object == NULL )
	{
		return effFALSE;
	}

	for ( effUINT i = 0; i < children.size(); i++ )
	{
		if ( children[i] == object )
		{
			return effFALSE;
		}
	}

	if ( object->parent != NULL )
	{
		object->DetachFromTree();
	}

	children.push_back(object);
	return effTRUE;
}

effBOOL	EFFObject::RemoveChild(EFFObject * object)
{
	auto it = children.begin();
	for ( ; it != children.end(); it++ )
	{
		if ( *it == object )
		{
			children.erase(it);
			return effTRUE;
		}
	}

	return effFALSE;
}

EFFObject *	EFFObject::GetChild(effUINT index)
{
	if ( index >= children.size() )
	{
		return NULL;
	}

	return children[index];
}

EFFComponent * EFFObject::AddComponent(EFFComponent * component)
{
	if ( component == NULL )
	{
		return NULL;
	}

#ifdef _DEBUG
	for ( effUINT i = 0; i < components.size(); i++ )
	{
		if ( component == components[i] )
		{
			return component;
		}
	}
#endif

	components.push_back(component);
	return component;
}

EFFComponent * EFFObject::AddComponent(const ClassID & classID)
{
	EFFComponent * component = static_cast<EFFComponent *>(EFFCreateObject(classID));
	component->SetObject(this);
	components.push_back(component);
	return component;
}

//这里暂时有个问题，如果一个Object包含了两个动态组件，如果这两个动态组件都支持同一个接口，那么
//无法判断返回哪个接口
EFFComponent * EFFObject::GetComponent(const ClassID & classID)
{	
	std::vector<EFFComponent *>::iterator it = components.begin();

	for ( ; it != components.end(); it++ )
	{
		if ( (*it)->GetRuntimeClass()->IsKindOf(classID) )
		{
			return *it;
		}
	}
	return NULL;
}

EFFComponent * EFFObject::GetComponent(effINT index)
{
	if ( index < 0 || index >= (effINT)components.size() )
	{
		return NULL;
	}

	return components[index];
}




effVOID	EFFObject::DetachFromTree()
{
	if ( parent != NULL )
	{
		parent->RemoveChild(this);
	}

	parent = NULL;
}

effVOID	EFFObject::TraverseObjectTree(std::vector<EFFObject *> & objects, std::vector<EFFComponent *> & properties)
{
	objects.push_back(this);

	effUINT index = 0;
	while ( index < objects.size() )
	{
		EFFObject * currentObject = objects[index];
		EFFClass * objectClass = currentObject->GetRuntimeClass();
		auto objectProperties = objectClass->GetProperties();
		for ( effUINT j = 0; j < objectProperties.size(); j++ )
		{
			EFFProperty * objectProperty = objectProperties[j];

			if ( !objectProperty->GetClass()->IsPODType() )
			{
				if (objectProperty->GetIsPointer() )
				{
					EFFComponent * propertyComponent = *((EFFComponent **)((effBYTE *)((effVOID *)currentObject) + objectProperty->GetOffset()));
					properties.push_back(propertyComponent);
				}
				else
				{
					EFFComponent * propertyComponent = (EFFComponent *)((effBYTE *)((effVOID *)currentObject) + objectProperty->GetOffset());
					properties.push_back(propertyComponent);
				}
			}
		}
		
		
		objects.insert(objects.end(), objects[index]->children.begin(), objects[index]->children.end());

		index++;
	}
}


effVOID EFFBaseInitProperty()
{
	EFFObjectBase::GetThisClass()->AddProperty(&EFFObjectBase::id, _effT("id"));

	EFFObject::GetThisClass()->AddProperty(&EFFObject::parent, _effT("parent"));
	//EFFObject::GetThisClass()->AddProperty(&EFFObject::children, _effT("children"));
}

EFFBASE_END