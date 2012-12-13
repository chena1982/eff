/******************************************************************************
	created:	2008-12-13   0:07
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DIndexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DIndexBuffer_2008_12_13__
#define __EFF3DIndexBuffer_2008_12_13__

EFF3D_BEGIN

class EFF3DIndexBuffer
{
public:
	EFF3DIndexBuffer() {}
	virtual ~EFF3DIndexBuffer() {}
public:
	virtual effHRESULT				GetDesc(EFF3DINDEXBUFFER_DESC * pDesc) = 0;
	virtual effHRESULT				Lock(effUINT OffsetToLock, effUINT SizeToLock, effVOID ** ppbData, effUINT Flags) = 0;
	virtual effHRESULT				Unlock() = 0;
};


EFF3D_END

#endif