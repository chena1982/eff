/******************************************************************************
created:	2018-03-15   0:16
file path:	d:\EFF\Include\EFF3D\EFF3DRenderCommand.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "../EFF3DPCH.h"
#include "EFF3DRenderCommand.h"
#include "EFF3DDevice.h"

EFF3D_BEGIN



const BackendDispatchFunction EFF3DDrawCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::Draw;
const BackendDispatchFunction EFF3DDrawIndexedCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::DrawIndexed;

const BackendDispatchFunction EFF3DCreateVertexBufferCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::CreateVertexBuffer;
const BackendDispatchFunction EFF3DCreateVertexDeclarationCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::CreateVertexDeclaration;
const BackendDispatchFunction EFF3DCreateIndexBufferCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::CreateIndexBuffer;

effVOID EFF3DBackendDispatch::CreateVertexDeclaration(const effVOID * data)
{
	EFF3DCreateVertexDeclarationCommand * realData = (EFF3DCreateVertexDeclarationCommand *)(data);
	//realData->vbDeclHandle = EFF3DGetDevice()->GetVertexDeclarationManager()->AddVertexDeclaration(realData->vbDecl);
}

effVOID EFF3DBackendDispatch::CreateVertexBuffer(const effVOID * data)
{
	EFF3DCreateVertexBufferCommand * realData = (EFF3DCreateVertexBufferCommand *)(data);
	EFF3DGetDevice()->CreateVertexBuffer(realData->data, realData->size, realData->flags, realData->vbDeclHandle, &realData->vbHandle);
}

effVOID EFF3DBackendDispatch::CreateIndexBuffer(const effVOID * data)
{
	EFF3DCreateIndexBufferCommand * realData = (EFF3DCreateIndexBufferCommand *)(data);
	EFF3DGetDevice()->CreateIndexBuffer(realData->data, realData->size, realData->flags, &realData->ibHandle);
}

effVOID EFF3DBackendDispatch::Draw(const effVOID * data)
{
	EFF3DDrawCommand * realData = (EFF3DDrawCommand *)(data);
	EFF3DGetDevice()->Draw(*realData);
}

effVOID EFF3DBackendDispatch::DrawIndexed(const effVOID * data)
{
	EFF3DDrawIndexedCommand* realData = (EFF3DDrawIndexedCommand *)(data);
	//backend::DrawIndexed(realData->indexCount, realData->startIndex, realData->baseVertex);
}

effVOID EFF3DBackendDispatch::CopyConstantBufferData(const effVOID * data)
{
	EFF3DCopyConstantBufferDataCommand * realData = (EFF3DCopyConstantBufferDataCommand *)(data);
	//backend::CopyConstantBufferData(realData->constantBuffer, realData->data, realData->size);
}

EFF3D_END