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
	class MyGUI_EFFPlatform;
}

namespace EFF3D
{
	class EFF3DDevice;
}

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
		bool create(HWND hWnd); // создаем начальную точки каркаса приложения
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

		void drawOneFrame(float elapsedTime);

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
		bool createRender();

		void destroyRender();

	private:
		MyGUI::Gui* mGUI;
		MyGUI::MyGUI_EFFPlatform* mPlatform;

		HWND hWnd;
		EFF3D::EFF3DDevice * mDevice;
		HINSTANCE hInstance;

		bool mExit;

		std::string mRootMedia;
		std::string mResourceFileName;
		bool mIsDeviceLost;

		EFFApplication mApp;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
