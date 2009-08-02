/******************************************************************************
	created:	2008-12-15   1:21
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DRenderQueueGroup.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DRenderQueueGroup_2008_12_15__
#define __EFF3DRenderQueueGroup_2008_12_15__

EFF3D_BEGIN

class EFF3DIRenderable;
class EFF3DDevice;
class EFF3DAutoParamDataSource;


class EFF3DRenderQueueGroup
{
public:
	EFF3DRenderQueueGroup() {}
	virtual ~EFF3DRenderQueueGroup() {}
public:
	virtual effVOID						AddRenderable(EFF3DIRenderable * pRenderable) = 0;
	virtual effVOID						RemoveRenderable(EFF3DIRenderable * pRenderable) = 0;
	//virtual EFF3DIRenderable *	GetFirstRenderable() = 0;
	//virtual EFF3DIRenderable *	GetNextRenderable() = 0;
	virtual effVOID						Render(	EFF3DDevice * pDevice,EFF3DAutoParamDataSource * pDataSource) = 0;
};

EFF3D_END

#endif