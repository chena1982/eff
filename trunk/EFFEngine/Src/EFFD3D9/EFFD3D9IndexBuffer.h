/******************************************************************************
	created:	2008-12-14   21:55
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9IndexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9IndexBuffer_2008_12_14__
#define __EFFD3D9IndexBuffer_2008_12_14__


class EFFD3D9IndexBuffer : public EFF3DIndexBuffer
{
	friend class EFFD3D9Device;
public:
	EFFD3D9IndexBuffer();
	virtual ~EFFD3D9IndexBuffer();
public:
	virtual effHRESULT				GetDesc(EFF3DINDEXBUFFER_DESC * pDesc);
	virtual effHRESULT				Lock(effUINT OffsetToLock,effUINT SizeToLock,VOID ** ppbData,effUINT Flags);
	virtual effHRESULT				Unlock();
protected:
	LPDIRECT3DINDEXBUFFER9		m_pBuf;
};

#endif