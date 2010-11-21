/********************************************************************
	created:	29:1:2010   20:46
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DForm.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFF3DForm_2010_1_29__
#define __EFF3DForm_2010_1_29__


namespace Awesomium
{
	class WebView;
};

EFF3D_BEGIN

class EFF3DDevice;




class EFF3D_API EFF3DForm
{
	friend class EFF3DFormWebViewListener;
public:
	EFF3DForm(EFF3DDevice * pDevice,effINT nWidth,effINT nHeight);
	~EFF3DForm();
public:
	effBOOL										LoadFromFile(EFF3DDevice * pDevice,const std::string & strFilePath);
	effVOID										Render(EFF3DDevice * pDevice);
protected:
	effVOID										CopyFromMemoryToTexture();
private:
	effINT											m_nWidth;
	effINT											m_nHeight;
	effINT											m_nPosX;
	effINT											m_nPosY;
	Awesomium::WebView *				m_pWebView;
	effBYTE *										m_pBuffer;

	EFF3DTexture *								m_pTexture;
	EFF3DFormWebViewListener *		m_pListener;
};

// Templated helper utility to write binary to stream
template<class T>
void writeVal(std::ofstream& destination, T value)
{
	destination.write(reinterpret_cast<char*>(&value), sizeof(T));
}

void saveImageTGA(const std::string& filename, unsigned char* buffer, int width, int height);

class EFF3DFormWebViewListener : public Awesomium::WebViewListener
{
public:
	EFF3DFormWebViewListener(EFF3DForm * pForm)
	{
		m_pForm = pForm;
	}

	void onBeginNavigation(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName)
	{
		std::cout << "Navigating to URL: " << url << std::endl;
	}

	void onBeginLoading(Awesomium::WebView* caller, const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType)
	{
		std::cout << "Beginning to load URL: " << url;
		std::cout << "\n\twith status code: " << statusCode;
		std::wcout << L"\n\twith mime-type: " << mimeType << std::endl;
	}

	void onFinishLoading(Awesomium::WebView* caller)
	{
		if ( caller->isDirty() )
		{
			caller->render(m_pForm->m_pBuffer,m_pForm->m_nWidth * 4,4);
			m_pForm->CopyFromMemoryToTexture();
			saveImageTGA("1.tga",m_pForm->m_pBuffer,m_pForm->m_nWidth,m_pForm->m_nHeight);
		}
	}

	void onCallback(Awesomium::WebView* caller, const std::wstring& objectName, const std::wstring& callbackName, const Awesomium::JSArguments& args)
	{
	}

	void onReceiveTitle(Awesomium::WebView* caller, const std::wstring& title, const std::wstring& frameName)
	{
	}

	void onChangeTooltip(Awesomium::WebView* caller, const std::wstring& tooltip)
	{
	}

#if defined(_WIN32)
	void onChangeCursor(Awesomium::WebView* caller, const HCURSOR& cursor)
	{
	}
#endif

	void onChangeKeyboardFocus(Awesomium::WebView* caller, bool isFocused)
	{
	}

	void onChangeTargetURL(Awesomium::WebView* caller, const std::string& url)
	{
	}

	void onOpenExternalLink(Awesomium::WebView* caller, const std::string& url, const std::wstring& source)
	{
	}
private:
	EFF3DForm *			m_pForm;
};

EFF3D_END

#endif