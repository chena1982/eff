/******************************************************************************
	created:	2008-12-10   0:53
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"



EFFBASE_BEGIN


RTTI_IMPLEMENT_BASE_NO_SAVE(EFFReferenceCount, 0)
RTTI_IMPLEMENT(EFFObject, 0)



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



EFFObject::EFFObject()
{
	id = 0;
	parent = NULL;
}

EFFObject::~EFFObject()
{

}




EFFProperty * EFFObject::GetProperty(const effString & propertyName)
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

effVOID EFFObject::GetPropertyJason(const effString & propertyName, effString & jasonString)
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

effVOID	EFFObject::AddPropertyToJson(const effString & propertyName, Json::Value * jsonValue)
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

effVOID	EFFObject::DetachFromTree()
{
	if ( parent != NULL )
	{
		parent->RemoveChild(this);
	}

	parent = NULL;
}

EFFBASE_END