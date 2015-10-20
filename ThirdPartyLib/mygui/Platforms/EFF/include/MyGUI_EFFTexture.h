/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_DIRECTX_TEXTURE_H__
#define __MYGUI_DIRECTX_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_Types.h"

namespace EFF3D
{
	class EFF3DDevice;
	class EFF3DTexture;
}

namespace MyGUI
{

	class MyGUI_EFFTexture : public ITexture
	{
	public:
		MyGUI_EFFTexture(const std::string& _name, EFF3D::EFF3DDevice * _device);
		virtual ~MyGUI_EFFTexture();

		virtual const std::string& getName() const;

		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename) { }

		virtual void destroy();

		virtual void* lock(TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked();

		virtual int getWidth();
		virtual int getHeight();

		virtual PixelFormat getFormat();
		virtual TextureUsage getUsage();
		virtual size_t getNumElemBytes();

		virtual IRenderTarget* getRenderTarget();

		/*internal:*/
		EFF3DTexture * getEFFTexture()
		{
			return mpTexture;
		}
		void deviceLost();
		void deviceRestore();

	private:
		EFF3D::EFF3DDevice* mpEFF3DDevice;
		EFF3D::EFF3DTexture * mpTexture;
		IntSize mSize;
		TextureUsage mTextureUsage;
		PixelFormat mPixelFormat;
		size_t mNumElemBytes;
		bool mLock;
		std::string mName;
		IRenderTarget* mRenderTarget;
		EFF3DPOOL mInternalPool;
		EFF3DFORMAT mInternalFormat;
		unsigned long mInternalUsage;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_TEXTURE_H__
