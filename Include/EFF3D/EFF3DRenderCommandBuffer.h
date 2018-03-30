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

    effVOID Write(const effVOID * _data, effUINT _size)
    {
        //BX_CHECK(m_size == EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE, "Called write outside start/finish?");
        //BX_CHECK(m_pos < m_size, "CommandBuffer::write error (pos: %d, size: %d).", m_pos, m_size);
        memcpy(&buffer[pos], _data, _size);
        pos += _size;
    }

    template<typename Type>
    effVOID Write(const Type& _in)
    {
        Align(EFF_ALIGNOF(Type));
        Write(reinterpret_cast<const effBYTE *>(&_in), sizeof(Type));
    }

    effVOID Read(effVOID * _data, effUINT _size)
    {
        //BX_CHECK(m_pos < m_size, "CommandBuffer::read error (pos: %d, size: %d).", m_pos, m_size);
        memcpy(_data, &buffer[pos], _size);
        pos += _size;
    }

    template<typename Type>
    effVOID Read(Type& _in)
    {
        Align(EFF_ALIGNOF(Type));
        Read(reinterpret_cast<effBYTE *>(&_in), sizeof(Type));
    }

    const effBYTE * Skip(effUINT _size)
    {
        //BX_CHECK(m_pos < m_size, "CommandBuffer::skip error (pos: %d, size: %d).", m_pos, m_size);
        const effBYTE * result = &buffer[pos];
        pos += _size;
        return result;
    }

    template<typename Type>
    effVOID Skip()
    {
        Align(EFF_ALIGNOF(Type));
        Skip(sizeof(Type));
    }

    effVOID Align(effUINT _alignment)
    {
        const effUINT mask = _alignment - 1;
        const effUINT newPos = (pos + mask) & (~mask);
        pos = newPos;
    }

    effVOID Reset()
    {
        pos = 0;
    }

    effVOID Start()
    {
        pos = 0;
        size = EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE;
    }

    effVOID Finish()
    {
        effBYTE cmd = CommandEnd;
        Write(cmd);
        size = pos;
        pos = 0;
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
    effUINT pos;
    effUINT size;
    effBYTE buffer[EFF3D_CONFIG_MAX_COMMAND_BUFFER_SIZE];
    //EFF3DCommandBucket

    EFF3DDevice * device;
};






EFF3D_END

#endif