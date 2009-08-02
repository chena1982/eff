/******************************************************************************
	created:	2008-12-15   0:48
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DRenderQueue.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DRenderQueue_2008_12_15__
#define __EFF3DRenderQueue_2008_12_15__

EFF3D_BEGIN

class EFF3DRenderQueueGroup;
class EFF3DAutoParamDataSource;
class EFF3DIRenderable;
class EFF3DDevice;

class EFF3DRenderQueue
{
public:
	EFF3DRenderQueue();
	~EFF3DRenderQueue();
public:
	effVOID											AddRenderable(EFF3DIRenderable * pRenderable);
	effVOID											RemoveRenderable(EFF3DIRenderable * pRenderable);
	effVOID											Render(	EFF3DDevice * pDevice,EFF3DAutoParamDataSource * pDataSource);
protected:
	EFF3DRenderQueueGroup *				GetRenderQueueGroup(EFF3DIRenderable * pRenderable,effUINT uiRGQIndex);
protected:
	std::map<effUINT,EFF3DRenderQueueGroup *>					m_mapRQG;
};

EFF3D_END

#endif