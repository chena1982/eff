/******************************************************************************
	created:	2008-12-15   2:00
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DMaterial.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DMaterial_2008_12_15__
#define __EFF3DMaterial_2008_12_15__


EFF3D_BEGIN

class EFF3DShader;

class EFF3D_API EFF3DMaterial
{
public:
	EFF3DMaterial();
	virtual ~EFF3DMaterial();

public:
	virtual effVOID				BeforeMaterialRender() {}
	virtual effVOID				AfterMaterialRender() {}
public:
	effBOOL						CreateFromFile(const effString & filePath);
public:
	inline EFF3DShader *		GetShader() { return shader; }
protected:
	EFF3DShader *				shader;
	//PyObject *					m_pScriptModule;
};

EFF3D_END


#endif