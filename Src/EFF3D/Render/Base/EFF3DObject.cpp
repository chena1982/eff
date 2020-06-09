/******************************************************************************
	created:	2012-02-15   22:32
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "../../EFF3DPCH.h"
#include "EFF3DObject.h"

//#define new EFFNEW

EFF3D_BEGIN


//EFFIUNKNOWN_IMPLEMENT(EFF3DIResource)

RTTI_IMPLEMENT(EFF3DObject, 0)


effVOID EFF3DObjectInitProperty()
{
	EFF3DObject::GetThisClass()->AddProperty(&EFF3DObject::name, _effT("name"));
	//EFF3DObject::GetThisClass()->AddProperty(&EFF3DObject::position, _effT("pos"));
	//EFF3DObject::GetThisClass()->AddProperty(&EFF3DObject::scale, _effT("scale"));
	//EFF3DObject::GetThisClass()->AddProperty(&EFF3DObject::rotation, _effT("rot"));
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




EFF3D_END