/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_DIRECTX_RENDER_MANAGER_H__
#define __MYGUI_DIRECTX_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

namespace EFF3D
{
	class EFF3DDevice;
}

namespace MyGUI
{

	class MyGUI_EFFRenderManager :
		public RenderManager,
		public IRenderTarget
	{
	public:
		MyGUI_EFFRenderManager();

		void initialise(EFF3D::EFF3DDevice* _device);
		void shutdown();

		static MyGUI_EFFRenderManager& getInstance()
		{
			return *getInstancePtr();
		}
		static MyGUI_EFFRenderManager* getInstancePtr()
		{
			return static_cast<MyGUI_EFFRenderManager*>(RenderManager::getInstancePtr());
		}

		/** @see RenderManager::getViewSize */
		virtual const IntSize& getViewSize() const
		{
			return mViewSize;
		}

		/** @see RenderManager::getVertexFormat */
		virtual VertexColourType getVertexFormat()
		{
			return mVertexFormat;
		}

		/** @see RenderManager::createVertexBuffer */
		virtual IVertexBuffer* createVertexBuffer();
		/** @see RenderManager::destroyVertexBuffer */
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		/** @see RenderManager::createTexture */
		virtual ITexture* createTexture(const std::string& _name);
		/** @see RenderManager::destroyTexture */
		virtual void destroyTexture(ITexture* _texture);
		/** @see RenderManager::getTexture */
		virtual ITexture* getTexture(const std::string& _name);

		/** @see RenderManager::isFormatSupported */
		virtual bool isFormatSupported(PixelFormat _format, TextureUsage _usage);

		/** @see IRenderTarget::begin */
		virtual void begin();
		/** @see IRenderTarget::end */
		virtual void end();

		/** @see IRenderTarget::doRender */
		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		/** @see IRenderTarget::getInfo */
		virtual const RenderTargetInfo& getInfo()
		{
			return mInfo;
		}

		/*internal:*/
		void drawOneFrame();
		void setViewSize(int _width, int _height);

		void deviceLost();
		void deviceRestore();

	private:
		void destroyAllResources();

	private:
		EFF3D::EFF3DDevice * mpEFF3DDevice;
		IntSize mViewSize;
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;
		bool mUpdate;

		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_RENDER_MANAGER_H__
