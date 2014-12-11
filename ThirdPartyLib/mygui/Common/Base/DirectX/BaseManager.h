/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#ifndef __BASE_MANAGER_H__
#define __BASE_MANAGER_H__

#include <MyGUI.h>

#include "InputManager.h"
#include "PointerManager.h"
#include "MyGUI_Delegate.h"

namespace MyGUI
{
	class DirectXPlatform;
}

struct IDirect3D9;
struct IDirect3DDevice9;
struct HWND__;
typedef HWND__* HWND;
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;

namespace base
{
	typedef MyGUI::delegates::CMultiDelegate1<float> EventHandle_Update;

	class BaseManager :
		public input::InputManager,
		public input::PointerManager
	{
	public:
		BaseManager();
		virtual ~BaseManager();

		virtual void prepare(); // инициализация коммандной строки
		bool create(int _width = 1024, int _height = 768); // создаем начальную точки каркаса приложения
		void destroy(); // очищаем все параметры каркаса приложения
		void run();
		void quit();

		void setWindowCaption(const std::wstring& _text);
		void makeScreenShot() { }

		const std::string& getRootMedia();
		void setResourceFilename(const std::string& _flename);
		void addResourceLocation(const std::string& _name, bool _recursive = false);

		size_t getWindowHandle();

		MyGUI::MapString getStatistic() { return MyGUI::MapString(); }

	/*internal:*/
		virtual void windowResized();

		virtual void windowMove();

		EventHandle_Update eventUpdate;
	protected:
		virtual void createScene() { }
		virtual void destroyScene() { }

		virtual void setupResources();

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		virtual void injectKeyRelease(MyGUI::KeyCode _key);

		virtual void createGui();
		virtual void destroyGui();

	private:
		void windowAdjustSettings(HWND hWnd, int width, int height, bool fullScreen);

		void resizeRender(int _width, int _height);
		bool createRender(int _width, int _height, bool _windowed);
		void drawOneFrame();
		void destroyRender();

	private:
		MyGUI::Gui* mGUI;
		MyGUI::DirectXPlatform* mPlatform;

		HWND hWnd;
		IDirect3D9* mD3d;
		IDirect3DDevice9* mDevice;
		HINSTANCE hInstance;

		bool mExit;

		std::string mRootMedia;
		std::string mResourceFileName;
		bool mIsDeviceLost;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
