/******************************************************************************
	created:	2008-12-15   3:06
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DShader.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DShader_2008_12_15__
#define __EFF3DShader_2008_12_15__

#include "EFF3DResource.h"

EFF3D_BEGIN


class EFF3DDevice;
class EFF3DAutoParamDataSource;
class EFF3DTexture;
class EFF3DShaderParameter;


class EFF3D_API EFF3DShader : public EFF3DResource
{
public:


#define ALL_PARAMETER_SEMANTIC(x, y) \
	ENUM_ITEM(x, y, NONE_SEMANTIC)\
	ENUM_ITEM(x, y, WORLD_MATRIX)\
	ENUM_ITEM(x, y, BONE_MATRIX_ARRAY)\
	ENUM_ITEM(x, y, VIEW_MATRIX)\
	ENUM_ITEM(x, y, PROJECTION_MATRIX)\
	ENUM_ITEM(x, y, VIEWPROJ_MATRIX)\
	ENUM_ITEM(x, y, WORLDVIEW_MATRIX)\
	ENUM_ITEM(x, y, WORLDVIEWPROJ_MATRIX)\
	ENUM_ITEM(x, y, REFLECTION_WORLDVIEWPROJ_MATRIX)\
	ENUM_ITEM(x, y, TEXTURE_VIEWPROJ_MATRIX)\
	ENUM_ITEM(x, y, VIEWPORT_WIDTH)\
	ENUM_ITEM(x, y, VIEWPORT_HEIGHT)\
	ENUM_ITEM(x, y, LIGHT_DIFFUSE_COLOR)\
	ENUM_ITEM(x, y, LIGHT_SPECULAR_COLOR)\
	ENUM_ITEM(x, y, LIGHT_AMBIENT_COLOR)\
	ENUM_ITEM(x, y, LIGHT_ATTENUATION)\
	ENUM_ITEM(x, y, LIGHT_POSITION)\
	ENUM_ITEM(x, y, LIGHT_DIRECTION)\
	ENUM_ITEM(x, y, LIGHT_POSITION_OBJECT_SPACE)\
	ENUM_ITEM(x, y, LIGHT_DIRECTION_OBJECT_SPACE)\
	ENUM_ITEM(x, y, SUN_POSITION)\
	ENUM_ITEM(x, y, SUN_DIRECTION)\
	ENUM_ITEM(x, y, LIGHT_DISTANCE_OBJECT_SPACE)\
	ENUM_ITEM(x, y, CAMERA_POSITION_OBJECT_SPACE)\
	ENUM_ITEM(x, y, CAMERA_POSITION)\
	ENUM_ITEM(x, y, FPS)\
	ENUM_ITEM(x, y, INVERSE_VIEWPORT_WIDTH)\
	ENUM_ITEM(x, y, INVERSE_VIEWPORT_HEIGHT)	

	DEF_ENUM_1(EFF3D::EFF3DShader, ParameterSemantic, ALL_PARAMETER_SEMANTIC)


	enum AutoParameterType
	{
		AutoParameterType_None,
		AutoParameterType_PerShader,
		AutoParameterType_PerObject,
		AutoParameterType_PerRenderable,
	};

public:
	virtual effBOOL			SetParameterValue(const effString & name, const effVOID * address, effUINT size) = 0;
	virtual effBOOL			SetParameterValue(const EFF3DShaderParameter * parameter, const effVOID * address) = 0;
	virtual effBOOL			SetBonesMatrix(const EFF3DShaderParameter * parameter, const EFFMatrix4 * bonesMatrix, effINT bonesCount) = 0;
	virtual effBOOL			SetTexture(const EFF3DShaderParameter * parameter, const EFF3DTexture * texture) = 0;

	virtual effBOOL			CreateFromFile(const effString & filePath, EFF3DDevice * device) = 0;
public:

	effVOID					UpdateAutoParametersPerShader(EFF3DAutoParamDataSource * dataSource);
	effVOID					UpdateAutoParametersPerObject(EFF3DAutoParamDataSource * dataSource);
	effVOID					UpdateAutoParametersPerRenderable(EFF3DAutoParamDataSource * dataSource);

protected:
	AutoParameterType		GetAutoParameterType(ParameterSemantic parameterSemantic);
public:
	EFFEvent				OnBeforeRender;
	EFFEvent				OnEndRender;
	effString				errorCode;
protected:
	std::vector<EFF3DShaderParameter *>		perShaderAutoParameters;
	std::vector<EFF3DShaderParameter *>		perObjectAutoParameters;
	std::vector<EFF3DShaderParameter *>		perRenderableAutoParameters;


};

class EFF3D_API EFF3DShaderParameter
{
	friend class EFF3DShader;
public:
	EFF3DShaderParameter() : parameterSemantic(EFF3DShader::NONE_SEMANTIC), parameterSize(0) {}
	~EFF3DShaderParameter() {}

public:
	inline EFF3DShader::ParameterSemantic	GetParameterSemantic() { return parameterSemantic; }
	inline effVOID							SetParameterSemantic(EFF3DShader::ParameterSemantic parameterSemantic) { this->parameterSemantic = parameterSemantic; }

	inline effINT							GetParameterSize() { return parameterSize; }
	inline effVOID							SetParameterSize(effINT parameterSize) { this->parameterSize = parameterSize; }
protected:
	EFF3DShader::ParameterSemantic			parameterSemantic;
	effINT									parameterSize;
};

EFF3D_END

DEF_ENUM_2(EFF3D::EFF3DShader, ParameterSemantic, ALL_PARAMETER_SEMANTIC)


#endif