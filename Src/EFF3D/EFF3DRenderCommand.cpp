/******************************************************************************
created:	2018-03-15   0:16
file path:	d:\EFF\Include\EFF3D\EFF3DRenderCommand.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DRenderCommand.h"


EFF3D_BEGIN



const BackendDispatchFunction EFF3DDrawCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::Draw;
const BackendDispatchFunction EFF3DDrawIndexedCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::DrawIndexed;

const BackendDispatchFunction EFF3DCreateVertexBufferCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::CreateVertexBuffer;
const BackendDispatchFunction EFF3DCreateVertexDeclarationCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::CreateVertexDeclaration;
const BackendDispatchFunction EFF3DCreateIndexBufferCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::CreateIndexBuffer;

EFF3D_END