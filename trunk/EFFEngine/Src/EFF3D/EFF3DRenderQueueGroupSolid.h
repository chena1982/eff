/******************************************************************************
	created:	2008-12-15   1:56
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DRenderQueueGroupSolid.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DRenderQueueGroupSolid_2008_12_15__
#define __EFF3DRenderQueueGroupSolid_2008_12_15__

#include "EFF3DRenderQueueGroup.h"

EFF3D_BEGIN

class EFF3DMaterial;
class EFF3DDevice;
class EFF3DAutoParamDataSource;
//����Ĭ�ϵĲ�͸���������Ⱦ����
class EFF3DRenderQueueGroupSolid : public EFF3DRenderQueueGroup
{
public:
	EFF3DRenderQueueGroupSolid();
	~EFF3DRenderQueueGroupSolid();
public:
	virtual effVOID					AddRenderable(EFF3DIRenderable * pRenderable);
	virtual effVOID					RemoveRenderable(EFF3DIRenderable * pRenderable);
	virtual effVOID					Render(	EFF3DDevice * pDevice,EFF3DAutoParamDataSource * pDataSource);
protected:

	//Ĭ�ϰ��ղ�����������в�ͬ�����󣬿����Լ�ʵ��һ��RenderQueueGroup
	std::map<EFF3DMaterial *,std::vector<EFF3DIRenderable *>>		m_mapRenderable;
};

EFF3D_END

#endif