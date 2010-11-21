/******************************************************************************
	created:	2008-12-16   23:07
	file path:	d:\EFF\EFFEngine\Include\EFFD3D9\EFFD3D9Effect.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9Effect_2008_12_16__
#define __EFFD3D9Effect_2008_12_16__



class EFFD3D9Effect : public EFF3DEffect
{
public:
	EFFD3D9Effect();
	virtual ~EFFD3D9Effect();
public:
	virtual effHRESULT			Begin(effUINT * pPasses,effUINT Flags);
	virtual effHRESULT			BeginPass(effUINT Pass);
	virtual effHRESULT			CommitChanges();
	virtual effHRESULT			EndPass();
	virtual effHRESULT			End();
protected:
	ID3DXEffect *					m_pEffect;
};

#endif