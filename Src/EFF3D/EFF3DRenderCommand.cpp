/******************************************************************************
created:	2018-03-15   0:16
file path:	d:\EFF\Include\EFF3D\EFF3DRenderCommand.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DRenderCommand.h"


EFF3D_BEGIN



const BackendDispatchFunction DrawCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::Draw;
const BackendDispatchFunction DrawIndexedCommand::DISPATCH_FUNCTION = &EFF3DBackendDispatch::DrawIndexed;




EFF3D_END