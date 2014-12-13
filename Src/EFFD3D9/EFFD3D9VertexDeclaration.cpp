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
	m_pVertexDeclaration = NULL;
}

EFFD3D9VertexDeclaration::~EFFD3D9VertexDeclaration()
{
	SF_RELEASE(m_pVertexDeclaration);
}

LPDIRECT3DVERTEXDECLARATION9 EFFD3D9VertexDeclaration::GetD3D9VertexDeclaration(EFFD3D9Device * pDevice)
{
	assert(pDevice != NULL);

	if ( m_bNeedRebuild )
	{
		SF_RELEASE(m_pVertexDeclaration);
		// Create D3D elements
		D3DVERTEXELEMENT9* d3delems = new D3DVERTEXELEMENT9[m_aryElement.size() + 1];

		effUINT i;
		for (  i = 0; i < m_aryElement.size(); i++ )
		{
			EFF3DVertexElement & element = m_aryElement[i];
			d3delems[i].Method = element.GetMethod();
			d3delems[i].Offset = element.GetOffset();
			d3delems[i].Stream = element.GetStream();
			d3delems[i].Type = element.GetType();
			d3delems[i].Usage = element.GetSemantic();
			d3delems[i].UsageIndex = element.GetSemanticIndex();
		}
		// Add terminator
		d3delems[i].Stream = 0xff;
		d3delems[i].Offset = 0;
		d3delems[i].Type = VET_UNUSED;
		d3delems[i].Method = 0;
		d3delems[i].Usage = 0;
		d3delems[i].UsageIndex = 0;

		HRESULT hr = pDevice->GetD3D9Device()->CreateVertexDeclaration(d3delems, &m_pVertexDeclaration);

		if ( FAILED(hr) )
		{
			//log(_effT("cannot create D3D9 vertex declaration: %s\n."),pDevice->GetErrorDescription(hr));
		}

		delete [] d3delems;

		m_bNeedRebuild = effFALSE;

	}

	return m_pVertexDeclaration;
}

