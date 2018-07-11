/******************************************************************************
	created:	2008-12-14   22:04
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9\EFFD3D9VertexDeclaration.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9VertexDeclaration_2008_12_14__
#define __EFFD3D9VertexDeclaration_2008_12_14__


class EFFD3D9Device;

class EFFD3D9VertexDeclaration : public EFF3DVertexDeclaration
{
public:
	EFFD3D9VertexDeclaration();
	virtual ~EFFD3D9VertexDeclaration();

public:

	LPDIRECT3DVERTEXDECLARATION9				GetD3D9VertexDeclaration(EFFD3D9Device * pDevice);
protected:
	LPDIRECT3DVERTEXDECLARATION9				vertexDeclaration;
	effBOOL										needRebuild;
};

#endif