/******************************************************************************
created:	2018-07-09   22:58
file path:	d:\EFF\Src\EFF3D\EFF3DIndexBuffer.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DIndexBuffer.h"

//#define new EFFNEW

EFF3D_BEGIN

//RTTI_IMPLEMENT_PURE(EFF3DVertexBuffer, 0)

effBOOL	EFF3DIndexBuffer::LoadDataFromFile(const effString & filePath)
{
    return effTRUE;
}


EFF3DIndexBufferManager::EFF3DIndexBufferManager()
{

}

EFF3DIndexBufferManager::~EFF3DIndexBufferManager()
{

}

effBOOL EFF3DIndexBufferManager::CreateFromFileImpl(const effString & filePath, EFF3DResource * resource, EFF3DResourceType resourceType)
{



    return effFALSE;
}

EFF3D_END