#pragma once



EFF3D_BEGIN



class EFF3DShaderCompileManager
{
public:
	EFF3DShaderCompileManager();
	~EFF3DShaderCompileManager();


public:

	effVOID CompileShader(const effString & shaderPath, const effCHAR * entryPoint);
private:
	effVOID Init();

private:
	SlangSession * session;

};

EFF3D_END