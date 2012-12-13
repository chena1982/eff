/******************************************************************************
	created:	2008-12-16   23:17
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DShader.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DShader.h"
#include "EFF3DAutoParamDataSource.h"
//#define new EFFNEW

EFF3D_BEGIN

effVOID EFF3DShader::UpdateAutoParametersPerShader(EFF3DAutoParamDataSource * dataSource)
{
	for ( effUINT i = 0; i < perShaderAutoParameters.size(); i++ )
	{
		EFF3DShaderParameter * parameter = perShaderAutoParameters[i];
		switch( parameter->parameterSemantic )
		{
		case CAMERA_POSITION:
			SetParameterValue(parameter, &dataSource->GetCameraPosition());
			break;
		case REFLECTION_WORLDVIEWPROJ_MATRIX:
			SetParameterValue(parameter, &dataSource->GetReflectionWorldViewProjMatrix());
			break;
		case SUN_DIRECTION:
			break;
		}
	}
}


effVOID EFF3DShader::UpdateAutoParametersPerObject(EFF3DAutoParamDataSource * dataSource)
{
	for ( effUINT i = 0; i < perObjectAutoParameters.size(); i++ )
	{
		EFF3DShaderParameter * parameter = perShaderAutoParameters[i];
		switch( parameter->parameterSemantic )
		{
		case WORLD_MATRIX:
			SetParameterValue(parameter, &dataSource->GetWorldMatrix());
			break;
		case WORLDVIEWPROJ_MATRIX:
			SetParameterValue(parameter, &dataSource->GetWorldViewProjMatrix());
			break;
		case BONE_MATRIX_ARRAY:
			SetBonesMatrix(parameter, dataSource->GetBonesMatrix(), dataSource->GetBonesMatrixCount());
			break;
		}
	}
}

effVOID	EFF3DShader::UpdateAutoParametersPerRenderable(EFF3DAutoParamDataSource * dataSource)
{
	for ( effUINT i = 0; i < perObjectAutoParameters.size(); i++ )
	{
		EFF3DShaderParameter * parameter = perShaderAutoParameters[i];
	}
}

EFF3DShader::AutoParameterType EFF3DShader::GetAutoParameterType(ParameterSemantic parameterSemantic)
{
	switch( parameterSemantic )
	{
		case CAMERA_POSITION:
		case REFLECTION_WORLDVIEWPROJ_MATRIX:
		case SUN_DIRECTION:
			return AutoParameterType_PerShader;
		case WORLD_MATRIX:
		case WORLDVIEWPROJ_MATRIX:
		case BONE_MATRIX_ARRAY:
			return AutoParameterType_PerObject;
		default:
			return AutoParameterType_None;
	}
}

EFF3D_END