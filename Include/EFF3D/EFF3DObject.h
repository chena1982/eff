/******************************************************************************
	created:	2008-12-15   3:01
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DIEntity.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DIEntity_2008_12_15__
#define __EFF3DIEntity_2008_12_15__

EFF3D_BEGIN



class EFF3D_API EFF3DObject : public EFFObject
{
	RTTI_DECLARE(EFF3DObject, EFFObject)
public:
	EFF3DObject();
	virtual ~EFF3DObject();
public:
	effVOID							AddRenderLayer(effINT renderLayer);
	effVOID							RemoveRenderLayer(effINT renderLayer);
public:
	inline effUINT					GetRenderLayer(effINT renderLayerSlot) { return layers[renderLayerSlot]; }

protected:
	effINT							layers[OBJECT_MAX_RENDER_LAYER];//unity每个object最多只能有一个render layer，正常情况应该是这样，不过考虑到特殊情况renderable最多支持4个render layer
	effINT							firstUnusedLayerSlot;
};

EFF3D_END

#endif