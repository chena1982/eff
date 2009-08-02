/******************************************************************************
	created:	2008-12-13   0:04
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DVertexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DVertexBuffer_2008_12_13__
#define __EFF3DVertexBuffer_2008_12_13__


EFF3D_BEGIN

class EFF3DVertexBuffer
{
public:
	EFF3DVertexBuffer() {}
	virtual ~EFF3DVertexBuffer() {}
public:
	virtual effHRESULT				GetDesc(EFF3DVERTEXBUFFER_DESC * pDesc) = 0;
	virtual effHRESULT				Lock(effUINT OffsetToLock,effUINT SizeToLock,VOID ** ppbData,effDWORD Flags) = 0;
	virtual effHRESULT				Unlock() = 0;
};

EFF3D_END

#endif