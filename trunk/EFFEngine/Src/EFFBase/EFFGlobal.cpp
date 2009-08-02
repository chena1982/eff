/******************************************************************************
	created:	2008-12-10   23:32
	file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFGlobal.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFGlobal.h"
#include "EFFObjectManager.h"

#define new EFFNEW

EFFBASE_BEGIN



EFFGlobal EFFGlobalUnExport::effGlobal;

EFFGlobal & GetEFFGlobal()
{
	return EFFGlobalUnExport::effGlobal;
}

EFFGlobal::EFFGlobal()
{
	m_pEFFObjectManager = new EFFObjectManager;
}

EFFGlobal::~EFFGlobal()
{
}

EFFBASE_END