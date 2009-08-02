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
class EFF3DIEntity;
class EFF3DDevice;

class EFF3DIRenderable : public EFFIUnknown
{
	friend class EFF3DRenderQueue;

	RTTI_DECLARE_PURE(EFF3DIRenderable,EFFIUnknown)

public:
	/*----------------------------------------------------------------------------
	@return ��Ҫ�ŵ�����RenderQueueGroup��
	 ---------------------------------------------------------------------------*/
	virtual effUINT					GetRenderQueueGroupCount() = 0;	

	/*----------------------------------------------------------------------------
	 @param uiRGQIndex RGQ�����Renderable������RGQ�е�����
	 @return �õ�uiRGQIndex��Ӧ��RenderQueueGroup��id
	 ---------------------------------------------------------------------------*/
	virtual effUINT					GetRenderQueueGroupId(effUINT uiRQGIndex) = 0;

	/*----------------------------------------------------------------------------
	 @return �õ�uiRQGIndex��Ӧ��RenderQueueGroup��������
	 ---------------------------------------------------------------------------*/
	virtual const char *			GetRenderQueueGroupClassName(effUINT uiRQGIndex) = 0;

	virtual EFF3DMaterial *		GetMaterial() = 0;
	virtual EFF3DIEntity *		GetParentEntity() = 0;
	virtual effVOID					Render(EFF3DDevice * pDevice) = 0;
protected:
	
};

EFF3D_END

#endif