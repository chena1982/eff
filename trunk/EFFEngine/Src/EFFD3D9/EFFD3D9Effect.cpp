/******************************************************************************
	created:	2008-12-16   23:10
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9Effect.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFD3D9Effect.h"

#define new EFFNEW


EFFD3D9Effect::EFFD3D9Effect()
{
	m_pEffect = NULL;
}

EFFD3D9Effect::~EFFD3D9Effect()
{
	SF_RELEASE(m_pEffect);
}

effHRESULT EFFD3D9Effect::Begin(effUINT * pPasses,effUINT Flags)
{
	assert(m_pEffect != NULL);
	return m_pEffect->Begin(pPasses,Flags);
}

effHRESULT EFFD3D9Effect::BeginPass(effUINT Pass)
{
	assert(m_pEffect != NULL);
	return m_pEffect->BeginPass(Pass);
}

effHRESULT EFFD3D9Effect::CommitChanges()
{
	assert(m_pEffect != NULL);
	return m_pEffect->CommitChanges();
}

effHRESULT EFFD3D9Effect::EndPass()
{
	assert(m_pEffect != NULL);
	return m_pEffect->EndPass();
}

effHRESULT EFFD3D9Effect::End()
{
	assert(m_pEffect != NULL);
	return m_pEffect->End();
}