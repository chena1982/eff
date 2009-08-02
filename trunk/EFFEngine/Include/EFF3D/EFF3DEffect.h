/******************************************************************************
	created:	2008-12-15   3:06
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DEffect.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DEffect_2008_12_15__
#define __EFF3DEffect_2008_12_15__

EFF3D_BEGIN

class EFF3DDevice;
class EFF3DAutoParamDataSource;

class EFF3DEffect
{
public:
	virtual effHRESULT			Begin(effUINT * pPasses,effDWORD Flags) = 0;
	virtual effHRESULT			BeginPass(effUINT Pass) = 0;
	virtual effHRESULT			CommitChanges() = 0;
	virtual effHRESULT			EndPass() = 0;
	virtual effHRESULT			End() = 0;
public:

	effVOID							UpdateAutoParametersPerEffect(EFF3DDevice * pDevice,EFF3DAutoParamDataSource * pDataSource);
	effVOID							UpdateAutoParametersPerEntity(EFF3DDevice * pDevice,EFF3DAutoParamDataSource * pDataSource);
public:
	EFFEvent							OnEffectBeforeRender;
	EFFEvent							OnEffectEndRender;
};


EFF3D_END

#endif