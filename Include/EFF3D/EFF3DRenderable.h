/******************************************************************************
	created:	2008-12-15   0:57
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DIRenderable.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFIRenderable_2008_12_15__
#define __EFFIRenderable_2008_12_15__


EFF3D_BEGIN

class EFF3DMaterial;
class EFF3DObject;
class EFF3DDevice;

/*class EFF3DIRenderable
{
	friend class EFF3DRenderQueue;

	RTTI_DECLARE_BASE_PURE(EFF3DIRenderable)


public:
	virtual effINT					GetRenderQueueGroupCount() = 0;	
	virtual effINT					GetRenderQueueGroupId(effINT renderQueueGroupIndex) = 0;
	virtual const effCHAR *			GetRenderQueueGroupClassName(effINT renderQueueGroupIndex) = 0;

	virtual EFF3DMaterial *			GetMaterial() = 0;
	virtual EFF3DIEntity *			GetParentEntity() = 0;
protected:
	
};*/



class EFF3DRenderable : public EFFComponent
{
	RTTI_DECLARE_PURE(EFF3DRenderable, EFFComponent)
public:
	EFF3DRenderable();
	virtual ~EFF3DRenderable() {}
public:

	enum FullscreenLayer
	{
		None = 0,
		PostEffect,
		GUI,
	};

	enum TransparencyType
	{
		Opaque = 0,
		Normal,
		Additive,
		Subtractive,
	};

	//virtual effINT					GetRenderQueueGroupCount() { return (effINT)renderQueueGroups.size(); }
	//virtual effINT					GetRenderQueueGroupId(effINT renderQueueGroupIndex) { return renderQueueGroups[renderQueueGroupIndex]; }
	//virtual const effString			GetRenderQueueGroupClassName(effINT renderQueueGroupIndex) { return effString(_effT("")); }

	inline effBOOL					GetNeedRecalculateRenderOrder() { return  needRecalculateRenderOrder; }
	inline effVOID					SetNeedRecalculateRenderOrder(effBOOL needRecalculateRenderOrder) { this->needRecalculateRenderOrder = needRecalculateRenderOrder; }

	inline FullscreenLayer			GetFullscreenLayer() { return fullscreenLayer; }
	inline effVOID					SetFullscreenLayer(FullscreenLayer fullscreenLayer) { this->fullscreenLayer = fullscreenLayer; needRecalculateRenderOrder = effTRUE; }



	inline TransparencyType			GetTransparencyType() { return transparencyType; }
	inline effVOID					SetTransparencyType(TransparencyType transparencyType) { this->transparencyType = transparencyType; needRecalculateRenderOrder = effTRUE; }


	effVOID							SetMaterial(EFF3DMaterial * material) { this->material = material; needRecalculateRenderOrder = effTRUE; }
	EFF3DMaterial *					GetMaterial() { return material; }
	//virtual EFF3DObject *			GetParent() { return object; }

	virtual effVOID					Render(EFF3DDevice * device) = 0;
protected:

	EFF3DMaterial *					material;

	FullscreenLayer					fullscreenLayer;

	TransparencyType				transparencyType;

	effBOOL							needRecalculateRenderOrder;
};

EFF3D_END

#endif