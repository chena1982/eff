/******************************************************************************
	created:	2008-12-14   21:59
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9VertexBuffer.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFFD3D9VertexBuffer.h"

//#define new EFFNEW


EFFD3D9VertexBuffer::EFFD3D9VertexBuffer()
{
	m_pBuf = NULL;
}

EFFD3D9VertexBuffer::~EFFD3D9VertexBuffer()
{
	SF_RELEASE(m_pBuf);
}

effHRESULT EFFD3D9VertexBuffer::GetDesc(EFF3DVERTEXBUFFER_DESC * pDesc)
{
	assert(m_pBuf != NULL);
	return m_pBuf->GetDesc((D3DVERTEXBUFFER_DESC *)pDesc);
}

effHRESULT EFFD3D9VertexBuffer::Lock(effUINT OffsetToLock,effUINT SizeToLock,VOID ** ppbData,effUINT Flags)
{
	assert(m_pBuf != NULL);
	return m_pBuf->Lock(OffsetToLock,SizeToLock,ppbData,Flags);
}

effHRESULT EFFD3D9VertexBuffer::Unlock()
{
	assert(m_pBuf != NULL);
	return m_pBuf->Unlock();
}