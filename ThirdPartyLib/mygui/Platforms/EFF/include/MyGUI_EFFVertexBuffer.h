/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_DIRECTX_VERTEX_BUFFER_H__
#define __MYGUI_DIRECTX_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_EFFRenderManager.h"

namespace EFF3D
{
	class EFF3DDevice;
	class EFF3DVertexBuffer;
}

namespace MyGUI
{

	class MyGUI_EFFVertexBuffer : public IVertexBuffer
	{
	public:
		MyGUI_EFFVertexBuffer(EFF3DDevice* _device, MyGUI_EFFRenderManager* _pRenderManager);
		virtual ~MyGUI_EFFVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual Vertex* lock();
		virtual void unlock();

	/*internal:*/
		virtual bool setToStream(size_t stream);

	private:
		bool create();
		void destroy();
		void resize();

	private:
		EFF3DDevice * mpEFF3DDevice;
		EFF3DVertexBuffer * mpBuffer;
		MyGUI_EFFRenderManager* pRenderManager;

		size_t mVertexCount;
		size_t mNeedVertexCount;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_VERTEX_BUFFER_H__
