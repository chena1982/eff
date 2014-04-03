/******************************************************************************
	created:	2008-12-10   23:32
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFGlobal.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFGlobal.h"
#include "EFFObjectManager.h"
#include "EFFObject.h"


EFFBASE_BEGIN





EFFContext::EFFContext()
{
	effObjectManager = EFFNEW EFFObjectManager;
}

EFFContext::~EFFContext()
{
	SF_DELETE(effObjectManager);
}

effVOID EFFContext::AddProperty()
{

}

EFFContext * context = NULL;

effVOID InitEFFContext()
{
	context = EFFNEW EFFContext();
	EFFBaseInitProperty();
}

effVOID ReleaseEFFContext()
{
	SF_DELETE(context);
}


EFFContext * GetEFFContext()
{
	return context;
}

EFFBASE_END