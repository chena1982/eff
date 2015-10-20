/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_DIRECTX_PLATFORM_H__
#define __MYGUI_DIRECTX_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_EFFRenderManager.h"
#include "MyGUI_EFFDataManager.h"
#include "MyGUI_EFFTexture.h"
#include "MyGUI_EFFVertexBuffer.h"
#include "MyGUI_EFFDiagnostic.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class MyGUI_EFFPlatform
	{
	public:
		MyGUI_EFFPlatform() :
			mIsInitialise(false)
		{
			mLogManager = new LogManager();
			mRenderManager = new MyGUI_EFFRenderManager();
			mDataManager = new MyGUI_EFFDataManager();
		}

		~MyGUI_EFFPlatform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mDataManager;
			delete mLogManager;
		}

		void initialise(EFF3DDevice * _device, const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME)
		{
			assert(!mIsInitialise);
			mIsInitialise = true;

			if (!_logName.empty())
				LogManager::getInstance().createDefaultSource(_logName);

			mRenderManager->initialise(_device);
			mDataManager->initialise();
		}

		void shutdown()
		{
			assert(mIsInitialise);
			mIsInitialise = false;

			mRenderManager->shutdown();
			mDataManager->shutdown();
		}

		MyGUI_EFFRenderManager* getRenderManagerPtr()
		{
			assert(mIsInitialise);
			return mRenderManager;
		}

		MyGUI_EFFDataManager* getDataManagerPtr()
		{
			assert(mIsInitialise);
			return mDataManager;
		}

	private:
		bool mIsInitialise;
		MyGUI_EFFRenderManager* mRenderManager;
		MyGUI_EFFDataManager* mDataManager;
		LogManager* mLogManager;

	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_PLATFORM_H__
