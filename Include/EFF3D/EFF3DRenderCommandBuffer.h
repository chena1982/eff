/******************************************************************************
created:	2018-03-06   23:33
file path:	d:\EFF\Include\EFF3D\EFF3DRenderCommandBuffer.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DRenderCommandBuffer_2018_03_06__
#define __EFF3DRenderCommandBuffer_2018_03_06__


EFF3D_BEGIN

class EFF3DVertexElement;
class EFF3DVertexDeclare;
class EFF3DDevice;

class EFF3DRenderCommandBuffer
{
public:
    EFF3DRenderCommandBuffer();
    ~EFF3DRenderCommandBuffer();

public:
    enum CommandType
    {
        CommandRendererInit,
        CommandRendererShutdownBegin,
        CommandCreateVertexDecl,
        CommandCreateIndexBuffer,
        CommandCreateVertexBuffer,
        CommandCreateDynamicIndexBuffer,
        CommandUpdateDynamicIndexBuffer,
        CommandCreateDynamicVertexBuffer,
        CommandUpdateDynamicVertexBuffer,
        CommandCreateShader,
        CommandCreateProgram,
        CommandCreateTexture,
        CommandUpdateTexture,
        CommandResizeTexture,
        CommandCreateFrameBuffer,
        CommandCreateUniform,
        CommandUpdateViewName,
        CommandInvalidateOcclusionQuery,
        CommandSetName,
        CommandEnd,
        CommandRendererShutdownEnd,
        CommandDestroyVertexDecl,
        CommandDestroyIndexBuffer,
        CommandDestroyVertexBuffer,
        CommandDestroyDynamicIndexBuffer,
        CommandDestroyDynamicVertexBuffer,
        CommandDestroyShader,
        CommandDestroyProgram,
        CommandDestroyTexture,
        CommandDestroyFrameBuffer,
        CommandDestroyUniform,
        CommandReadTexture,
        CommandRequestScreenShot,
    };

    effVOID write(const effVOID * _data, effUINT _size)
    {
        //BX_CHECK(m_size == EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE, "Called write outside start/finish?");
        //BX_CHECK(m_pos < m_size, "CommandBuffer::write error (pos: %d, size: %d).", m_pos, m_size);
        memcpy(&m_buffer[m_pos], _data, _size);
        m_pos += _size;
    }

    template<typename Type>
    effVOID write(const Type& _in)
    {
        align(EFF_ALIGNOF(Type));
        write(reinterpret_cast<const uint8_t*>(&_in), sizeof(Type));
    }

    effVOID read(effVOID * _data, effUINT _size)
    {
        //BX_CHECK(m_pos < m_size, "CommandBuffer::read error (pos: %d, size: %d).", m_pos, m_size);
        memcpy(_data, &m_buffer[m_pos], _size);
        m_pos += _size;
    }

    template<typename Type>
    effVOID read(Type& _in)
    {
        align(EFF_ALIGNOF(Type));
        read(reinterpret_cast<effBYTE *>(&_in), sizeof(Type));
    }

    const effBYTE * skip(effUINT _size)
    {
        //BX_CHECK(m_pos < m_size, "CommandBuffer::skip error (pos: %d, size: %d).", m_pos, m_size);
        const uint8_t* result = &m_buffer[m_pos];
        m_pos += _size;
        return result;
    }

    template<typename Type>
    effVOID skip()
    {
        align(EFF_ALIGNOF(Type));
        skip(sizeof(Type));
    }

    effVOID align(effUINT _alignment)
    {
        const effUINT mask = _alignment - 1;
        const effUINT pos = (m_pos + mask) & (~mask);
        m_pos = pos;
    }

    effVOID reset()
    {
        m_pos = 0;
    }

    effVOID start()
    {
        m_pos = 0;
        m_size = EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE;
    }

    effVOID finish()
    {
        effBYTE cmd = CommandEnd;
        write(cmd);
        m_size = m_pos;
        m_pos = 0;
    }

public:


    EFFId                       CreateResourceFromMemory(effVOID * data, effUINT size, EFF3DResourceType resourceType);

    EFF3DTextureHandle			CreateTexture(effUINT width, effUINT height, effUINT levels, effUINT flag, 
                                            EFF3DTextureFormat format, EFF3DResourceType resourceType);

    EFF3DIndexBufferHandle      CreateIndexBuffer(effVOID * data, effUINT size, effBOOL flag);
    effVOID                     UpdateIndexBuffer(effUINT offset, effVOID * data, effUINT size);

    EFF3DVertexBufferHandle     CreateVertexBuffer(effVOID * data, effUINT size, effUINT flag, EFF3DVertexBufferHandle * vbHandle);
    effVOID                     UpdateVertexBuffer(effUINT offset, effVOID * data, effUINT size);

    EFF3DVertexDeclHandle       CreateVertexDeclaration(const EFF3DVertexElement * vertexElements);
protected:
    effUINT m_pos;
    effUINT m_size;
    effBYTE m_buffer[EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE];

    EFF3DDevice * device;
};


EFF3D_END

#endif