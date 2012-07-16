/******************************************************************************
	created:	2008-12-10   0:53
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFObject.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFObject.h"



EFFBASE_BEGIN

RTTI_IMPLEMENT(EFFObject, 0)

EFFObject::EFFObject()
{
	id = 0;
}

EFFObject::~EFFObject()
{
	components.clear();
}

//������ʱ�и����⣬���һ��Object������������̬����������������̬�����֧��ͬһ���ӿڣ���ô
//�޷��жϷ����ĸ��ӿ�
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




EFFComponent * EFFObject::AddComponent(const ClassID & classID)
{
	EFFComponent * component = static_cast<EFFComponent *>(EFFCreateObject(classID));
	component->SetObject(this);
	components.push_back(component);
	return component;
}

EFFBASE_END