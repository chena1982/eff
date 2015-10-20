/******************************************************************************
	created:	2008-12-13   0:04
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DVertexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DVertexBuffer_2008_12_13__
#define __EFF3DVertexBuffer_2008_12_13__

#include "EFF3DResource.h"

EFF3D_BEGIN

class EFF3D_API EFF3DVertexBuffer : public EFF3DResource
{
	RTTI_DECLARE_PURE(EFF3DVertexBuffer, EFF3DResource)
public:
	EFF3DVertexBuffer() {}
	virtual ~EFF3DVertexBuffer() {}
public:
	virtual effBOOL					LoadDataFromFile(const effString & filePath) { return effFALSE; }

	virtual effHRESULT				GetDesc(EFF3DVERTEXBUFFER_DESC * pDesc) = 0;
	virtual effHRESULT				LockBuffer(effUINT OffsetToLock, effUINT SizeToLock, effVOID ** ppbData, effUINT Flags) = 0;
	virtual effHRESULT				UnlockBuffer() = 0;
};

EFF3D_END

#endif