/******************************************************************************
	created:	2008-12-15   1:21
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DRenderQueue.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DRenderQueue_2008_12_15__
#define __EFF3DRenderQueue_2008_12_15__

EFF3D_BEGIN

class EFF3DRenderable;
class EFF3DDevice;



class EFF3DRenderQueue
{
public:
	EFF3DRenderQueue() {}
	virtual ~EFF3DRenderQueue() {}
public:
	virtual effVOID						AddRenderable(EFF3DRenderable * renderable) {}
	virtual effVOID						RemoveRenderable(EFF3DRenderable * renderable) {}
	//virtual EFF3DIRenderable *	GetFirstRenderable() = 0;
	//virtual EFF3DIRenderable *	GetNextRenderable() = 0;
	virtual effVOID						Render(EFF3DDevice * device) {}
protected:
	std::vector<EFF3DRenderable *>		renderables;
};

EFF3D_END

#endif