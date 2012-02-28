/******************************************************************************
	created:	2008-12-14   21:55
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9IndexBuffer.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFD3D9IndexBuffer.h"

//#define new EFFNEW

EFFD3D9IndexBuffer::EFFD3D9IndexBuffer()
{
	m_pBuf = NULL;
}

EFFD3D9IndexBuffer::~EFFD3D9IndexBuffer()
{
	SF_RELEASE(m_pBuf);
}


effHRESULT EFFD3D9IndexBuffer::GetDesc(EFF3DINDEXBUFFER_DESC * pDesc)
{
	assert(m_pBuf != NULL);
	return m_pBuf->GetDesc((D3DINDEXBUFFER_DESC *)pDesc);
}

effHRESULT EFFD3D9IndexBuffer::Lock(effUINT OffsetToLock,effUINT SizeToLock,VOID ** ppbData,effUINT Flags)
{
	assert(m_pBuf != NULL);
	return m_pBuf->Lock(OffsetToLock,SizeToLock,ppbData,Flags);
}

effHRESULT EFFD3D9IndexBuffer::Unlock()
{
	assert(m_pBuf != NULL);
	return m_pBuf->Unlock();
}