/******************************************************************************
	created:	2008-12-2   0:19
	file path:	d:\EFF\EFFEngine\Src\EFFD3D9.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D9Device_2008_12_2__
#define __EFFD3D9Device_2008_12_2__


class EFFD3D9Device : public EFF3DDevice
{
public:
	EFFD3D9Device();
	~EFFD3D9Device();
public:
	virtual effVOID Release() {};

public:
	LPDIRECT3D9 &						GetD3D() { return m_pD3D; }
	effVOID									SetD3D(LPDIRECT3D9 pD3D) { m_pD3D = pD3D; } 
	LPDIRECT3DDEVICE9 &			GetD3DDevice() { return m_pD3DDevice; }
protected:
	LPDIRECT3D9							m_pD3D;       
	LPDIRECT3DDEVICE9				m_pD3DDevice;
};

effVOID InitFullScreen(effINT nWidth,effINT nHeight,D3DPRESENT_PARAMETERS *d3dpp,HWND hWnd);
effVOID InitWindow(effINT with,effINT height,effBOOL bFSA,D3DPRESENT_PARAMETERS *d3dpp,effDWORD dwDSFormat);

effBOOL EFFD3D9_API effCreate3DDevice(EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA = effFALSE,effBOOL bMultiThread = effFALSE,effDWORD dwDSFormat = EFF3DFMT_D16);
typedef effBOOL (*effCREATE3DDEVICE)(EFF3DDevice** lpp3DDevice ,effBOOL bWindow,HWND hWnd,effINT nWidth,effINT nHeight,effBOOL bFSA,effBOOL bMultiThread,effDWORD dwDSFormat);


#endif