

#include "../EFF3DPCH.h"

#include "EFF3DShaderCompileManager.h"


EFF3D_BEGIN

EFF3DShaderCompileManager::EFF3DShaderCompileManager()
{
	Init();
}

EFF3DShaderCompileManager::~EFF3DShaderCompileManager()
{
	spDestroySession(session);
}

effVOID EFF3DShaderCompileManager::Init()
{
	session = spCreateSession(NULL);
}

effVOID EFF3DShaderCompileManager::CompileShader(const effString & shaderPath, const effCHAR * entryPoint)
{
	SlangCompileRequest * request = spCreateCompileRequest(session);
	spSetCodeGenTarget(request, SLANG_HLSL);

	spAddSearchPath(request, "Shader");

	effINT translationUnitIndex = spAddTranslationUnit(request, SLANG_SOURCE_LANGUAGE_SLANG, "");

	spAddTranslationUnitSourceFile(request, translationUnitIndex, EFFSTRING2ANSI(shaderPath));

	SlangProfileID profileID = spFindProfile(session, "ps_5_0");

	effINT entryPointIndex = spAddEntryPoint(
		request,
		translationUnitIndex,
		entryPoint,
		profileID);

	effINT anyErrors = spCompile(request);

	const effCHAR * diagnostics = spGetDiagnosticOutput(request);

	const effCHAR * code = spGetEntryPointSource(request, entryPointIndex);

	effString resultPath = shaderPath.substr(0, shaderPath.size() - 5);
	resultPath += _effT("hlsl");


	EFFFile * file = EFFNEW EFFSTLFile();
	file->Open(resultPath, _effT("wt+"));
	file->Write(&code, strlen(code));
	file->Close();
	SF_DELETE(file);

}

EFF3D_END