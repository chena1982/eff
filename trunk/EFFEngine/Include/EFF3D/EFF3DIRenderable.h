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

	EFFIUNKNOWN_DECLARE

	BEGIN_INTERFACE_TABLE(EFF3DIRenderable)
		IMPLEMENTS_INTERFACE(EFFIUnknown)
	END_INTERFACE_TABLE()

public:
	/*----------------------------------------------------------------------------
	@return 需要放到几个RenderQueueGroup里
	 ---------------------------------------------------------------------------*/
	virtual effINT					GetRenderQueueGroupCount() = 0;	

	/*----------------------------------------------------------------------------
	 @param nRGQIndex RGQ在这个Renderable的所有RGQ中的索引
	 @return 得到nRGQIndex对应的RenderQueueGroup的id
	 ---------------------------------------------------------------------------*/
	virtual effINT					GetRenderQueueGroupId(effINT nRQGIndex) = 0;

	/*----------------------------------------------------------------------------
	 @return 得到nRQGIndex对应的RenderQueueGroup的类名字
	 ---------------------------------------------------------------------------*/
	virtual const effCHAR *		GetRenderQueueGroupClassName(effINT nRQGIndex) = 0;

	virtual EFF3DMaterial *		GetMaterial() = 0;
	virtual EFF3DIEntity *		GetParentEntity() = 0;
	virtual effVOID					Render(EFF3DDevice * pDevice) = 0;
protected:
	
};


class EFF3DRenderable : public EFF3DIRenderable
{
	//RTTI_DECLARE_PURE(EFF3DRenderable,EFF3DIRenderable)
public:
	EFF3DRenderable(EFF3DMaterial * pMaterial,EFF3DIEntity * pEntity) : m_pMaterial(pMaterial),m_pEntity(pEntity) {}
	~EFF3DRenderable() {}
public:

	virtual effINT					GetRenderQueueGroupCount() { return (effINT)m_aryRQG.size(); }
	virtual effINT					GetRenderQueueGroupId(effINT nRQGIndex) { return m_aryRQG[nRQGIndex]; }
	virtual const effCHAR *		GetRenderQueueGroupClassName(effINT nRQGIndex) { return NULL; }

	virtual EFF3DMaterial *		GetMaterial() { return m_pMaterial; }
	virtual EFF3DIEntity *		GetParentEntity() { return m_pEntity; }

protected:

	EFF3DMaterial *				m_pMaterial;
	EFF3DIEntity *					m_pEntity;
	std::vector<effINT>			m_aryRQG;
};

EFF3D_END

#endif