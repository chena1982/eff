/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFD3D12\EFFD3D12Shader.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D12Shader_2019_03_23__
#define __EFFD3D12Shader_2019_03_23__


class EFFD3D12Shader : EFF3DShader
{
public:
	EFFD3D12Shader();
	virtual ~EFFD3D12Shader();
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

class EFFD3D12ShaderParameter : public EFF3DShaderParameter
{
public:
	//CGparameter				cgParameter;
	//CGtype					cgType;
};



#endif