/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\EFFEngine\Src\EFFD3D12\EFFD3D12VertexDeclaration.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D12VertexDeclaration_2019_03_23__
#define __EFFD3D12VertexDeclaration_2019_03_23__


class EFFD3D12Device;

class EFFD3D12VertexDeclaration : public EFF3DVertexDeclaration
{
public:
	EFFD3D12VertexDeclaration();
	virtual ~EFFD3D12VertexDeclaration();

public:

	//LPDIRECT3DVERTEXDECLARATION9				GetD3D9VertexDeclaration(EFFD3D12Device * pDevice);
protected:
	//LPDIRECT3DVERTEXDECLARATION9				vertexDeclaration;
	effBOOL										needRebuild;
};

#endif