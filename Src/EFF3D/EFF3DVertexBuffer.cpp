/******************************************************************************
	created:	2014-01-01   1:51
	file path:	d:\EFF\Src\EFF3D\EFF3DVertexBuffer.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFF3DPCH.h"
#include "EFF3DVertexBuffer.h"

//#define new EFFNEW

EFF3D_BEGIN

RTTI_IMPLEMENT_PURE(EFF3DVertexBuffer, 0)


effBOOL	EFF3DVertexBuffer::LoadDataFromFile(const effString & filePath)
{
    return effTRUE;
}


EFF3DVertexBufferManager::EFF3DVertexBufferManager()
{

}

EFF3DVertexBufferManager::~EFF3DVertexBufferManager()
{

}

effBOOL EFF3DVertexBufferManager::CreateFromFileImpl(const effString & filePath, EFF3DResource * resource, EFF3DResourceType resourceType)
{



    return effFALSE;
}


EFF3D_END