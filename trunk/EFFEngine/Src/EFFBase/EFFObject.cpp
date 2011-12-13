/******************************************************************************
	created:	2008-12-10   0:53
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"

#define new EFFNEW



EFFBASE_BEGIN

RTTI_IMPLEMENT_BASE(EFFObject, 0)

EFFObject::EFFObject()
{
	refCount = 0;
	objectId = 0;
}

EFFObject::~EFFObject()
{
	componentArray.clear();
}

//������ʱ�и����⣬���һ��Object������������̬����������������̬�����֧��ͬһ���ӿڣ���ô
//�޷��жϷ����ĸ��ӿ�
EFFComponent * EFFObject::GetComponent(ClassID & classID)
{	
	std::vector<EFFComponent *>::iterator it = componentArray.begin();

	for ( ; it != componentArray.end(); it++ )
	{
		if ( (*it)->GetRuntimeClass()->IsKindOf(classID) )
		{
			return *it;
		}
	}
	return NULL;
}

effULONG EFFObject::AddRef()
{
	return ++refCount;
}

effULONG EFFObject::Release()
{
	effULONG ulRefCount = refCount--;
	if ( ulRefCount == 0 )
	{
		delete this;
	}
	return ulRefCount;
}


EFFComponent * EFFObject::AddComponent(ClassID & classID)
{
	EFFComponent * pComponent = static_cast<EFFComponent *>(EFFCreateObject(classID));

	componentArray.push_back(pComponent);
	return pComponent;
}

EFFBASE_END