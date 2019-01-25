/******************************************************************************
created:	2018-03-15   0:16
file path:	d:\EFF\Include\EFF3D\EFF3DRenderCommand.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DRenderCommand_2018_03_15__
#define __EFF3DRenderCommand_2018_03_15__

#include "EFF3DVertexDeclaration.h"

//https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/

EFF3D_BEGIN


struct EFF3DBinding
{
    EFFId handle;

    union
    {
        struct
        {
            effUINT flags;
        } draw;

        struct
        {
            effBYTE  format;
            effBYTE  access;
            effBYTE  mip;
        } compute;

    } data;
};

struct EFF3DStream
{
    EFF3DStream()
    {
        startVertex = 0;
        vertexDeclHandle = 0;
    }

    effUINT startVertex;
    EFF3DVertexBufferHandle vertexBufferHandle;
    EFF3DVertexDeclarationHandle vertexDeclHandle;
};



typedef effVOID(*BackendDispatchFunction)(const effVOID *);



struct EFF3D_API EFF3DTextureBind
{
    EFF3DTextureBind()
    {
        for (effUINT i = 0; i < EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS; ++i)
        {
            EFF3DBinding & bind = binds[i];
            bind.data.draw.flags = 0;
        }
    };

    EFF3DBinding binds[EFF3D_CONFIG_MAX_TEXTURE_SAMPLERS];
};

struct EFF3D_API EFF3DUniformBufferBindCommand
{
    EFF3DUniformBufferBindCommand()
    {
        for (effUINT i = 0; i < EFF3D_CONFIG_MAX_UNIFORM_BUFFERS; ++i)
        {
            EFF3DBinding & bind = binds[i];
            bind.data.draw.flags = 0;
        }
    };

    EFF3DBinding binds[EFF3D_CONFIG_MAX_UNIFORM_BUFFERS];
};

struct EFF3D_API EFF3DDrawCommand
{
    static const BackendDispatchFunction DISPATCH_FUNCTION;




    EFF3DDrawCommand()
    {
        uniformBegin = 0;
        uniformEnd = 0;
        stateFlags = EFF3D_STATE_DEFAULT;
        stencil = PackStencil(EFF3D_STENCIL_DEFAULT, EFF3D_STENCIL_DEFAULT);
        rgba = 0;
        startMatrix = 0;
        startIndex = 0;
        numIndices = UINT32_MAX;
        numVertices = UINT32_MAX;
        instanceDataOffset = 0;
        instanceDataStride = 0;
        numInstances = 1;
        startIndirect = 0;
        numIndirect = UINT16_MAX;
        numMatrices = 1;
        submitFlags = EFF3D_SUBMIT_EYE_FIRST;
        scissor = UINT16_MAX;
        streamMask = 0;
        uniformIdx = UINT8_MAX;
    }

    effBOOL SetStreamBit(effBYTE streamIndex, EFF3DVertexBufferHandle handle)
    {
        const effBYTE bit = 1 << streamIndex;
        const effBYTE mask = streamMask & ~bit;
        const effBYTE tmp = handle.IsValid() ? bit : 0;
        streamMask = mask | tmp;
        return 0 != tmp;
    }

    effUINT64 key;
    EFF3DStream stream[EFF3D_CONFIG_MAX_VERTEX_STREAMS];
    EFF3DTextureBind textureBind;
    effUINT64 stateFlags;
    effUINT64 stencil;
    effUINT rgba;
    effUINT uniformBegin;
    effUINT uniformEnd;
    effUINT startMatrix;
    effUINT startVertex;
    effUINT startIndex;
    effUINT numIndices;
    effUINT numVertices;
    effUINT instanceDataOffset;
    effUINT numInstances;
    effUINT16 instanceDataStride;
    effUINT16 startIndirect;
    effUINT16 numIndirect;
    effUINT16 numMatrices;
    effUINT16 scissor;
    effBYTE  submitFlags;
    effBYTE  streamMask;
    effBYTE  uniformIdx;


  

    EFF3DIndexBufferHandle indexBufferHandle;
    EFF3DVertexBufferHandle instanceDataBufferHandle;
    EFF3DIndirectBufferHandle indirectBufferHandle;
    EFF3DOcclusionQueryHandle occlusionQueryHandle;
};

struct EFF3DDrawIndexedCommand
{
    static const BackendDispatchFunction DISPATCH_FUNCTION;

    effUINT indexCount;
    effUINT startIndex;
    effUINT baseVertex;

    EFF3DVertexDeclarationHandle vertexDecl;
    EFF3DVertexBufferHandle vertexBuffer;
    EFF3DIndexBufferHandle indexBuffer;
};

struct EFF3DCopyConstantBufferDataCommand
{
    static const BackendDispatchFunction DISPATCH_FUNCTION;

    EFF3DConstantBufferHandle constantBuffer;
    effVOID * data;
    effUINT size;
};

struct EFF3DCreateVertexBufferCommand
{
    static const BackendDispatchFunction DISPATCH_FUNCTION;

    EFF3DVertexBufferHandle vbHandle;
    EFF3DVertexDeclarationHandle vbDeclHandle;
    effVOID * data;
    effUINT size;
};


struct EFF3DCreateVertexDeclarationCommand
{
    static const BackendDispatchFunction DISPATCH_FUNCTION;

    EFF3DVertexDeclarationHandle vbDeclHandle;
    EFF3DVertexDeclaration vbDecl;
};

struct EFF3DCreateIndexBufferCommand
{
    static const BackendDispatchFunction DISPATCH_FUNCTION;

    EFF3DIndexBufferHandle ibHandle;
    effVOID * data;
    effUINT size;
    effUINT flags;
};

class EFF3DBackendDispatch
{
public:

	static effVOID CreateVertexDeclaration(const effVOID * data)
	{
		const EFF3DCreateVertexDeclarationCommand * realData = (EFF3DCreateVertexDeclarationCommand *)(data);
		//backend::Draw(realData->vertexCount, realData->startVertex);
	}

	static effVOID CreateVertexBuffer(const effVOID * data)
	{
		const EFF3DCreateVertexBufferCommand * realData = (EFF3DCreateVertexBufferCommand *)(data);
		//backend::Draw(realData->vertexCount, realData->startVertex);
	}

	static effVOID CreateIndexBuffer(const effVOID * data)
	{
		const EFF3DCreateIndexBufferCommand * realData = (EFF3DCreateIndexBufferCommand *)(data);
		//backend::Draw(realData->vertexCount, realData->startVertex);
	}

    static effVOID Draw(const effVOID * data)
    {
        const EFF3DDrawCommand * realData = (EFF3DDrawCommand *)(data);
        //backend::Draw(realData->vertexCount, realData->startVertex);
    }

    static effVOID DrawIndexed(const effVOID * data)
    {
        const EFF3DDrawIndexedCommand* realData = (EFF3DDrawIndexedCommand *)(data);
        //backend::DrawIndexed(realData->indexCount, realData->startIndex, realData->baseVertex);
    }

    static effVOID CopyConstantBufferData(const effVOID * data)
    {
        const EFF3DCopyConstantBufferDataCommand * realData = (EFF3DCopyConstantBufferDataCommand *)(data);
        //backend::CopyConstantBufferData(realData->constantBuffer, realData->data, realData->size);
    }
};



typedef effVOID * EFF3DCommandPacket;

class EFF3DCommandPacketUtilities
{
public:
    static const effUINT64 OFFSET_NEXT_COMMAND_PACKET = 0u;
    static const effUINT64 OFFSET_BACKEND_DISPATCH_FUNCTION = OFFSET_NEXT_COMMAND_PACKET + sizeof(EFF3DCommandPacket);
    static const effUINT64 OFFSET_COMMAND = OFFSET_BACKEND_DISPATCH_FUNCTION + sizeof(BackendDispatchFunction);

    template <typename T>
    static EFF3DCommandPacket Create(effUINT64 auxMemorySize, EFFLinearAllocator & allocator)
    {
        //return ::operator new(GetSize<T>(auxMemorySize));

        return allocator.Allocate(GetSize<T>(auxMemorySize), 8, 0);
    }

    template <typename T>
    static effUINT64 GetSize(effUINT64 auxMemorySize)
    {
        return OFFSET_COMMAND + sizeof(T) + auxMemorySize;
    };

    static EFF3DCommandPacket * GetNextCommandPacket(EFF3DCommandPacket packet)
    {
        return (EFF3DCommandPacket *)(reinterpret_cast<effBYTE *>(packet) + OFFSET_NEXT_COMMAND_PACKET);
    }

    template <typename T>
    static EFF3DCommandPacket * GetNextCommandPacket(T* command)
    {
        return (EFF3DCommandPacket *)(reinterpret_cast<effBYTE *>(command) - OFFSET_COMMAND + OFFSET_NEXT_COMMAND_PACKET);
    }

    static BackendDispatchFunction* GetBackendDispatchFunction(EFF3DCommandPacket packet)
    {
        return (BackendDispatchFunction *)(reinterpret_cast<effBYTE *>(packet) + OFFSET_BACKEND_DISPATCH_FUNCTION);
    }

    template <typename T>
    static T * GetCommand(EFF3DCommandPacket packet)
    {
        return (T *)(reinterpret_cast<effBYTE *>(packet) + OFFSET_COMMAND);
    }

    template <typename T>
    static effBYTE * GetAuxiliaryMemory(T * command)
    {
        return reinterpret_cast<effBYTE *>(command) + sizeof(T);
    }

    static effVOID StoreNextCommandPacket(EFF3DCommandPacket packet, EFF3DCommandPacket nextPacket)
    {
        *GetNextCommandPacket(packet) = nextPacket;
    }

    template <typename T>
    static effVOID StoreNextCommandPacket(T * command, EFF3DCommandPacket nextPacket)
    {
        *GetNextCommandPacket<T>(command) = nextPacket;
    }

    static effVOID StoreBackendDispatchFunction(EFF3DCommandPacket packet, BackendDispatchFunction dispatchFunction)
    {
        *GetBackendDispatchFunction(packet) = dispatchFunction;
    }

    static const EFF3DCommandPacket LoadNextCommandPacket(const EFF3DCommandPacket packet)
    {
        return *GetNextCommandPacket(packet);
    }

    static const BackendDispatchFunction LoadBackendDispatchFunction(const EFF3DCommandPacket packet)
    {
        return *GetBackendDispatchFunction(packet);
    }

    static const effVOID * LoadCommand(const EFF3DCommandPacket packet)
    {
        return reinterpret_cast<effBYTE *>(packet) + OFFSET_COMMAND;
    }
};


//TODO: tls封装成跨平台的

#define COMMAND_BUCKET_CHUNK_COUNT 32

template <typename T>
class EFF3DCommandBucket
{
public:
    typedef T Key;


    EFF3DCommandBucket(effUINT commandCount)
    {
        keys = EFFNEW Key[commandCount];
        packets = EFFNEW effVOID * [commandCount];
        memset(packets, 0, sizeof(effVOID *) * commandCount);

        offsetTlsIndex = TlsAlloc();
        remainingTlsIndex = TlsAlloc();

        current = 0;

		effINT64 value = 0;
        TlsSetValue(offsetTlsIndex, (LPVOID)value);
        TlsSetValue(remainingTlsIndex, (LPVOID)value);
    }

    ~EFF3DCommandBucket()
    {
        TlsFree(offsetTlsIndex);
        TlsFree(remainingTlsIndex);
    }

    template <typename U>
    U * AddCommand(Key key, effUINT64 auxMemorySize, EFFLinearAllocator & allocator)
    {
        EFF3DCommandPacket packet = EFF3DCommandPacketUtilities::Create<U>(auxMemorySize, allocator);

		if (packet == NULL)
		{
			return NULL;
		}

        // store key and pointer to the data
        {
            // fetch number of remaining packets in this layer for the thread with this id
			effINT64 remaining = (effINT64)TlsGetValue(remainingTlsIndex);
			effINT64 offset = (effINT64)TlsGetValue(offsetTlsIndex);

            if (remaining == 0)
            {
                // no more storage in this block remaining, get new one
                offset = InterlockedExchangeAdd64(&current, COMMAND_BUCKET_CHUNK_COUNT);
                remaining = 32;

                // write back
                TlsSetValue(offsetTlsIndex, (LPVOID)offset);
            }

			effINT64 index = offset + (COMMAND_BUCKET_CHUNK_COUNT - remaining);
            keys[index] = key;
            packets[index] = packet;
            remaining--;

            // write back
            TlsSetValue(remainingTlsIndex, (LPVOID)remaining);
        }

        EFF3DCommandPacketUtilities::StoreNextCommandPacket(packet, NULL);
        EFF3DCommandPacketUtilities::StoreBackendDispatchFunction(packet, U::DISPATCH_FUNCTION);

        return EFF3DCommandPacketUtilities::GetCommand<U>(packet);
    }

    template <typename U, typename V>
    U * AppendCommand(V * command, effUINT64 auxMemorySize)
    {
        EFF3DCommandPacket packet = EFF3DCommandPacketUtilities::Create<U>(auxMemorySize);

        // append this command to the given one
        EFF3DCommandPacketUtilities::StoreNextCommandPacket<V>(command, packet);

        EFF3DCommandPacketUtilities::StoreNextCommandPacket(packet, NULL);
        EFF3DCommandPacketUtilities::StoreBackendDispatchFunction(packet, U::DISPATCH_FUNCTION);

        return EFF3DCommandPacketUtilities::GetCommand<U>(packet);
    }

	//在渲染线程提交
    effVOID Submit()
    {
        for (effUINT i = 0; i < current; i++)
        {
            EFF3DCommandPacket packet = packets[i];
            while (packet != NULL)
            {
                SubmitPacket(packet);
                packet = EFF3DCommandPacketUtilities::LoadNextCommandPacket(packet);
            }
        }
    }

    effVOID SubmitPacket(const EFF3DCommandPacket packet)
    {
        const BackendDispatchFunction function = EFF3DCommandPacketUtilities::LoadBackendDispatchFunction(packet);
        const effVOID * command = EFF3DCommandPacketUtilities::LoadCommand(packet);
        function(command);
    }

	effVOID Reset()
	{
		current = 0;

		//todo 需要每个线程调用一次
		effINT64 value = 0;
		TlsSetValue(offsetTlsIndex, (LPVOID)value);
		TlsSetValue(remainingTlsIndex, (LPVOID)value);
	}
private:
    Key * keys;
    effVOID ** packets;
    effUINT offsetTlsIndex;
    effUINT remainingTlsIndex;

	effINT64 current;
};


EFF3D_END



#endif