/******************************************************************************
	created:	2012-03-06   23:11
	file path:	d:\EFF\trunk\EFFEngine\Include\EFFD3D9\EFFD3D9Shader.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/
#include "stdafx.h"
#include "EFFD3D9Shader.h"
#include "EFFD3D9Device.h"
#include "EFFD3D9Texture.h"

EFFD3D9Shader::EFFD3D9Shader()
{
	vertexShader = NULL;
	pixelShader = NULL;
}

EFFD3D9Shader::~EFFD3D9Shader()
{
	cgDestroyProgram(vertexShader);
	cgDestroyProgram(pixelShader);
}

effBOOL EFFD3D9Shader::SetParameterValue(const effString & name, const effVOID * address, effUINT size)
{
	return effTRUE;
}


effBOOL EFFD3D9Shader::SetParameterValue(const EFF3DShaderParameter * parameter, const effVOID * address)
{
	const EFFD3D9ShaderParameter * d3d9ShaderParameter = (const EFFD3D9ShaderParameter *)parameter;

	switch ( d3d9ShaderParameter->cgType )
	{
	case CG_HALF1:
	case CG_FLOAT1:
		cgSetParameter4fv(d3d9ShaderParameter->cgParameter, (const effFLOAT *)address);
		break;
	case CG_HALF2:
	case CG_FLOAT2:
		cgSetParameter4fv(d3d9ShaderParameter->cgParameter, (const effFLOAT *)address);
		break;
	case CG_HALF3:
	case CG_FLOAT3:
		cgSetParameter4fv(d3d9ShaderParameter->cgParameter, (const effFLOAT *)address);
		break;
	case CG_HALF4:
	case CG_FLOAT4:
		cgSetParameter4fv(d3d9ShaderParameter->cgParameter, (const effFLOAT *)address);
		break;
	}
	return effTRUE;
}

effBOOL	EFFD3D9Shader::SetBonesMatrix(const EFF3DShaderParameter * parameter, const EFFMatrix4 * bonesMatrix, effINT bonesCount)
{
	return effTRUE;
}

effBOOL EFFD3D9Shader::SetTexture(const EFF3DShaderParameter * parameter, const EFF3DTexture * texture)
{
	const EFFD3D9ShaderParameter * d3d9ShaderParameter = (const EFFD3D9ShaderParameter *)parameter;
	const EFFD3D9Texture * d3d9Texture = (const EFFD3D9Texture *)texture;
	return SUCCEEDED(cgD3D9SetTexture(d3d9ShaderParameter->cgParameter, d3d9Texture->texture));
}

effBOOL EFFD3D9Shader::CreateFromFile(const effString & filePath, EFF3DDevice * device)
{
	EFFSTLFile file;
	if ( !file.Open(filePath, _effT("r")) )
	{
		file.Close();
		return effFALSE;
	}


	effCHAR * source = EFFNEW effCHAR[file.Length()];
	file.Read(source, file.Length());
	file.Close();

	EFFD3D9Device * effD3D9Device = (EFFD3D9Device *)device;
	vertexShader = cgCreateProgram(effD3D9Device->GetCGContex(), CG_SOURCE, source, CG_PROFILE_VS_3_0, "main_vs", NULL);
	cgD3D9LoadProgram(vertexShader, CG_FALSE, 0);

	if ( CG_NO_ERROR != cgGetError() )
	{
		const effCHAR * errorString = cgGetErrorString(cgGetError());
		SF_DELETE(source);
		return effFALSE;
	}

	pixelShader = cgCreateProgram(effD3D9Device->GetCGContex(), CG_SOURCE, source, CG_PROFILE_PS_3_0, "main_ps", NULL);
	cgD3D9LoadProgram(pixelShader, CG_FALSE, 0);

	if ( CG_NO_ERROR != cgGetError() )
	{
		const effCHAR * errorString = cgGetErrorString(cgGetError());
		SF_DELETE(source);
		cgDestroyProgram(vertexShader);
		return effFALSE;
	}

	CGparameter cgParameter = cgGetFirstParameter(vertexShader, CG_PROGRAM);

	while ( cgParameter != NULL )
	{
		EFFD3D9ShaderParameter * parameter = EFFNEW EFFD3D9ShaderParameter();
		parameter->cgParameter = cgParameter;
		parameter->cgType = cgGetParameterType(cgParameter);
		parameter->SetParameterSemantic(GetParameterSemantic(cgParameter));

		switch( GetAutoParameterType(parameter->GetParameterSemantic()) )
		{
		case EFF3DShader::AutoParameterType_PerShader:
			perShaderAutoParameters.push_back(parameter);
			break;
		case EFF3DShader::AutoParameterType_PerObject:
			perObjectAutoParameters.push_back(parameter);
			break;
		case EFF3DShader::AutoParameterType_PerRenderable:
			perRenderableAutoParameters.push_back(parameter);
			break;
		}
		//cgParameters.push_back(param);


		cgParameter = cgGetNextParameter(cgParameter);
	}

	SF_DELETE(source);
	return effTRUE;
}

EFF3DShader::ParameterSemantic EFFD3D9Shader::GetParameterSemantic(CGparameter cgParameter)
{
	const effCHAR * semantic = cgGetParameterSemantic(cgParameter);
	if ( semantic == NULL )
	{
		return EFF3DShader::NONE_SEMANTIC;
	}

	try
	{
		return EnumHelper<EFF3D::EFF3DShader::ParameterSemantic>::parse(semantic, false);
	}
	catch( std::runtime_error & e)
	{
		fprintf(stderr, "exception: %s\n", e.what() );
	}

	return EFF3DShader::NONE_SEMANTIC;
}