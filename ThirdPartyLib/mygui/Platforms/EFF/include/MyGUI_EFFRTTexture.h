/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#ifndef __MYGUI_DIRECTX_RTTEXTURE_H__
#define __MYGUI_DIRECTX_RTTEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

namespace EFF3D
{
	class EFF3DDevice;
	class EFF3DTexture;
	class EFF3DSurface;
}

namespace MyGUI
{

	class MyGUI_EFFRTTexture :
		public IRenderTarget
	{
	public:
		MyGUI_EFFRTTexture(EFF3DDevice * _device, EFF3DTexture * _texture);
		virtual ~MyGUI_EFFRTTexture();

		virtual void begin();
		virtual void end();

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		virtual const RenderTargetInfo& getInfo()
		{
			return mRenderTargetInfo;
		}

	private:
		EFF3DDevice * mpEFF3DDevice;
		EFF3DTexture * mpTexture;
		EFF3DSurface * mpRenderSurface;
		EFF3DSurface * mpBackBuffer;
		RenderTargetInfo mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_RTTEXTURE_H__
