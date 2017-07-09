/******************************************************************************
	created:	2012-03-06   23:11
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFD3D9\EFFD3D9Shader.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9Shader_2012_03_06__
#define __EFFD3D9Shader_2012_03_06__


class EFFD3D9Shader : EFF3DShader
{
public:
	EFFD3D9Shader();
	virtual ~EFFD3D9Shader();
public:
public:
	virtual effBOOL			SetParameterValue(const effString & name, const effVOID * address, effUINT size);
	virtual effBOOL			SetParameterValue(const EFF3DShaderParameter * parameter, const effVOID * address);
	virtual effBOOL			SetBonesMatrix(const EFF3DShaderParameter * parameter, const EFFMatrix4 * bonesMatrix, effINT bonesCount);
	virtual effBOOL			SetTexture(const EFF3DShaderParameter * parameter, const EFF3DTexture * texture);

	virtual effBOOL			CreateFromFile(const effString & filePath, EFF3DDevice * device);

public:
	//inline CGprogram		GetVertexShader() { return vertexShader; }
	//inline CGprogram		GetPixelShader() { return pixelShader; }

protected:
	//ParameterSemantic		GetParameterSemantic(CGparameter cgParameter);
protected:
	//CGprogram				vertexShader;
	//CGprogram				pixelShader;
};

class EFFD3D9ShaderParameter : public EFF3DShaderParameter
{
public:
	//CGparameter				cgParameter;
	//CGtype					cgType;
};



#endif