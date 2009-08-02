/******************************************************************************
	created:	2008-12-15   2:00
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DMaterial.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DMaterial_2008_12_15__
#define __EFF3DMaterial_2008_12_15__


EFF3D_BEGIN

class EFF3DEffect;

class EFF3D_API EFF3DMaterial
{
public:
	EFF3DMaterial();
	~EFF3DMaterial();
public:
	EFF3DEffect *				GetEffect() { return m_pEffect; }
	effBOOL						CreateFromFile(const effCHAR * szFilePath);
public:
	EFFEvent						OnMaterialBeforeRender;
	EFFEvent						OnMaterialEndRender;

	EFFEvent						OnRenderableBeforeRender;
	EFFEvent						OnRenderableEndRender;

protected:
	EFF3DEffect *				m_pEffect;
	PyObject *					m_pScriptModule;
};

EFF3D_END


#endif