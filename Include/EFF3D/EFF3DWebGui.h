/******************************************************************************
	created:	2013-7-21   00:58
	file path:	d:\EFF\Include\EFF3D\EFF3DWebGui.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DWebGui_2013_7_21__
#define __EFF3DWebGui_2013_7_21__

EFF3D_BEGIN




class EFF3DTexture;

class EFF3D_API EFF3DWebGUIWindow
{

public:
    EFF3DWebGUIWindow();
    virtual ~EFF3DWebGUIWindow();

public:
    effVOID Initialise();

    //const effString & GetTypeName() const;

    //virtual void UpdateRenderQueue(Ogre::RenderQueue* queue);

    effVOID Resize(effINT width, effINT height);

    effBOOL KeyPressed(effUINT key);
    effBOOL KeyReleased(effUINT key);

    effBOOL MouseMoved(EFFInputManager * inputManager);
    effBOOL MousePressed(EFFInputManager * inputManager);
    effBOOL MouseReleased(EFFInputManager * inputManager);

    //void loadURL(const char* url);
    //void loadFile(const char* filename);
    effBOOL LoadHTML(const effString & fileName);

    //void goBack();
    //void goForward();

    //void runJS(const char* script);

    //void setClientHandler(wkeClientHandler* handler);

    //webview transparent
    effVOID SetTransparent(effBOOL transparent);
    //void focus();
    //void unfocus();
    
    //bool isCursorOver(const OIS::MouseEvent &arg);
	effVOID Render();

	effVOID SendMessageToJS(const effString & buffer);
protected:
    //virtual void addBaseParameters();

    wke::IWebView *		webView;
    EFF3DTexture *		texture;

	jsValue				messageCenter;
	jsValue				processMsg;
	jsExecState			execState;
};

//javascript »Øµ÷ c++
jsValue JS_CALL js_SendMessageToCpp(jsExecState es);



EFF3D_END

#endif