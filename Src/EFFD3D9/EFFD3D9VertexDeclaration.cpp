/******************************************************************************
	created:	2008-12-14   23:12
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9VertexDeclaration.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "EFFD3D9PCH.h"
#include "EFFD3D9VertexDeclaration.h"
#include "EFFD3D9Device.h"

//#define new EFFNEW


EFFD3D9VertexDeclaration::EFFD3D9VertexDeclaration()
{
    vertexDeclaration = NULL;
    needRebuild = effTRUE;
}

EFFD3D9VertexDeclaration::~EFFD3D9VertexDeclaration()
{
	SF_RELEASE(vertexDeclaration);
}

LPDIRECT3DVERTEXDECLARATION9 EFFD3D9VertexDeclaration::GetD3D9VertexDeclaration(EFFD3D9Device * device)
{
    assert(device != NULL);
    return NULL;
}

