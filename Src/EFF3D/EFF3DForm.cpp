/********************************************************************
	created:	29:1:2010   20:55
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DForm.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "EFF3DPCH.h"

#include "EFF3DDevice.h"
#include "EFF3DTexture.h"
#include "EFF3DForm.h"

#define new EFFNEW

EFF3D_BEGIN

EFF3DForm::EFF3DForm(EFF3DDevice * pDevice,effINT nWidth,effINT nHeight)
{
	Awesomium::WebCore * pWebCore = pDevice->GetWebCore();

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_pWebView = pWebCore->createWebView(m_nWidth, m_nHeight);
	
	m_pBuffer = new effBYTE[m_nWidth*m_nHeight*4];
	memset(m_pBuffer,0,m_nWidth*m_nHeight*4);

	m_pTexture = NULL;

	m_nPosX = 0;
	m_nPosY = 0;


	m_pListener = NULL;

}

EFF3DForm::~EFF3DForm()
{
	m_pWebView->destroy();
	SF_DELETE(m_pListener);
	SFT_DELETE(m_pBuffer);
}

effBOOL EFF3DForm::LoadFromFile(EFF3DDevice * pDevice,const std::string & strFilePath)
{
	effHRESULT hr;
	if ( m_pTexture == NULL )
	{
		if ( FAILED(hr = pDevice->CreateTexture(m_nWidth,m_nHeight,1,EFF3DUSAGE_DYNAMIC,EFF3DFMT_A8R8G8B8,EFF3DPOOL_DEFAULT,&m_pTexture)) )
		{
			return effFALSE;
		}
	}

	if ( m_pListener == NULL )
	{
		m_pListener = new EFF3DFormWebViewListener(this);

		m_pWebView->setListener(m_pListener);
	}
	m_pWebView->loadFile(strFilePath);
	//m_pWebView->loadURL("http://www.baidu.com");
	return effTRUE;
}



// Utility function that saves a TGA Image from a BGRA buffer
void saveImageTGA(const std::string& filename, unsigned char* buffer, int width, int height)
{
	std::ofstream out(filename.c_str(), std::ios_base::binary);

	if(out.bad())
	{
		std::cerr << "Could not save image." << std::endl;
		out.close();
		return;
	}

	// Write TGA Header
	writeVal<char>(out, 0);
	writeVal<char>(out, 0);
	writeVal<char>(out, 2);         // compressed RGBA
	writeVal<short>(out, 0);
	writeVal<short>(out, 0);
	writeVal<char>(out, 0);
	writeVal<short>(out, 0);        // x origin
	writeVal<short>(out, 0);        // y origin
	writeVal<short>(out, width);    // width
	writeVal<short>(out, height);   // height
	writeVal<char>(out, 32);        // 32 BPP
	writeVal<char>(out, 0);

	int rowSpan = width * 4;

	// Write Image Data
	for(int row = height - 1; row >= 0; row--)
		out.write(reinterpret_cast<char*>(buffer + row * rowSpan), rowSpan);

	out.close();
}


effVOID EFF3DForm::Render(EFF3DDevice * pDevice)
{



	/*if ( m_pWebView->isDirty() )
	{
		m_pWebView->render(m_pBuffer,m_nWidth * 4,4);
		CopyFromMemoryToTexture();
	}*/




	pDevice->DrawQuad(m_pTexture);

}

effVOID EFF3DForm::CopyFromMemoryToTexture()
{
	effHRESULT hr;
	EFF3DLOCKED_RECT rect;
	if ( !FAILED(hr = m_pTexture->LockRect(0,&rect,NULL,EFF3DLOCK_DISCARD)) )
	{
		effBYTE * pDest = (effBYTE *)rect.pBits;
		effBYTE * pSrc = m_pBuffer;

		if ( m_nWidth * 4 == rect.Pitch )
		{
			memcpy(pDest,pSrc,m_nWidth*m_nHeight*4);
		}
		else
		{
			for ( effINT i = 0 ; i < m_nHeight; i++ )
			{
				memcpy(pDest,pSrc,m_nWidth * 4);
				pDest += rect.Pitch;
				pSrc += m_nWidth * 4;
			}
		}

		hr = m_pTexture->UnlockRect(0);
	}
}

EFF3D_END