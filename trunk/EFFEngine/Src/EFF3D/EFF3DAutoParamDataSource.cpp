/******************************************************************************
	created:	2008-12-15   2:24
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DAutoParamDataSource.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DAutoParamDataSource.h"

#define new EFFNEW

EFF3D_BEGIN

EFF3DAutoParamDataSource::EFF3DAutoParamDataSource()
{
	m_pRenderable = NULL;
}

EFF3DAutoParamDataSource::~EFF3DAutoParamDataSource()
{
}


effVOID EFF3DAutoParamDataSource::SetRenderable(EFF3DIRenderable * pRenderable)
{
	m_pRenderable = pRenderable;
}

EFF3D_END