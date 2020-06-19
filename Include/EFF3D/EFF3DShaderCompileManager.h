#pragma once


EFF3D_BEGIN


class EFF3D_API EFF3DShaderCompileManager
{
	friend class EFF3DDevice;
public:
	~EFF3DShaderCompileManager();
private:
	EFF3DShaderCompileManager();
public:
	effVOID CompileShader(const effString & shaderPath, const effCHAR * entryPoint);
private:
	effVOID Init();

private:
	SlangSession * session;

};

EFF3D_END