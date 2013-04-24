/******************************************************************************
	created:	2012-02-15   22:32
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DObject.h"

//#define new EFFNEW

EFF3D_BEGIN


//EFFIUNKNOWN_IMPLEMENT(EFF3DIResource)

RTTI_IMPLEMENT(EFF3DObject, 0)


effVOID EFF3DObjectInitProperty()
{
	EFF3DObject::GetThisClass()->AddProperty(&EFF3DObject::name, _effT("name"));
	EFF3DObject::GetThisClass()->AddProperty(&EFF3DObject::position, _effT("pos"));
	EFF3DObject::GetThisClass()->AddProperty(&EFF3DObject::scale, _effT("scale"));
	EFF3DObject::GetThisClass()->AddProperty(&EFF3DObject::rotation, _effT("rot"));
}

EFF3DObject::EFF3DObject() : EFFObject()
{
	firstUnusedLayerSlot = 0;
	for ( effINT i = 0; i < OBJECT_MAX_RENDER_LAYER; i++ )
	{
		layers[i] = -1;
	}
}

EFF3DObject::~EFF3DObject()
{
}

effVOID EFF3DObject::AddRenderLayer(effINT renderLayer)
{
	layers[firstUnusedLayerSlot] = renderLayer;
	firstUnusedLayerSlot++;
	if ( firstUnusedLayerSlot >= OBJECT_MAX_RENDER_LAYER )
	{
		firstUnusedLayerSlot = 0;
	}
}

effVOID	EFF3DObject::RemoveRenderLayer(effINT renderLayer)
{
	for ( effINT i = 0; i < OBJECT_MAX_RENDER_LAYER; i++ )
	{
		if ( layers[i] == renderLayer )
		{
			layers[i] = -1;
			firstUnusedLayerSlot = i;
			return;
		}
	}
}

//这里暂时有个问题，如果一个Object包含了两个动态组件，如果这两个动态组件都支持同一个接口，那么
//无法判断返回哪个接口
EFFComponent * EFF3DObject::GetComponent(const ClassID & classID)
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

EFFComponent * EFF3DObject::GetComponent(effINT index)
{
	if ( index < 0 || index >= (effINT)components.size() )
	{
		return NULL;
	}

	return components[index];
}




EFFComponent * EFF3DObject::AddComponent(const ClassID & classID)
{
	EFFComponent * component = static_cast<EFFComponent *>(EFFCreateObject(classID));
	component->SetObject(this);
	components.push_back(component);
	return component;
}

effVOID	EFF3DObject::SaveComponentsToFile(EFFFile * file, effBOOL isBinary, YAML::Node & node)
{
	for ( effUINT i = 0; i < components.size(); i++ )
	{
		components[i]->SaveToFile(file, isBinary, node);
	}
}


EFF3D_END