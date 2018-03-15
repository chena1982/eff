/******************************************************************************
created:	2018-03-15   0:16
file path:	d:\EFF\Include\EFF3D\EFF3DRenderCommand.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFF3DRenderCommand_2018_03_15__
#define __EFF3DRenderCommand_2018_03_15__

//https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/

EFF3D_BEGIN


typedef effVOID(*BackendDispatchFunction)(const effVOID *);


struct DrawCommand
{
    static const BackendDispatchFunction DISPATCH_FUNCTION;

    effUINT vertexCount;
    effUINT startVertex;

    EFF3DVertexDeclHandle vertexDecl;
    EFF3DVertexBufferHandle vertexBuffer;
    EFF3DIndexBufferHandle indexBuffer;
};

struct DrawIndexedCommand
{
    static const BackendDispatchFunction DISPATCH_FUNCTION;

    effUINT indexCount;
    effUINT startIndex;
    effUINT baseVertex;

    EFF3DVertexDeclHandle vertexDecl;
    EFF3DVertexBufferHandle vertexBuffer;
    EFF3DIndexBufferHandle indexBuffer;
};

struct CopyConstantBufferDataCommand
{
    static const BackendDispatchFunction DISPATCH_FUNCTION;

    EFF3DConstantBufferHandle constantBuffer;
    effVOID * data;
    effUINT size;
};



class EFF3DBackendDispatch
{
public:
    static effVOID Draw(const effVOID * data)
    {
        const DrawCommand * realData = (DrawCommand *)(data);
        //backend::Draw(realData->vertexCount, realData->startVertex);
    }

    static effVOID DrawIndexed(const effVOID * data)
    {
        const DrawIndexedCommand* realData = (DrawIndexedCommand *)(data);
        //backend::DrawIndexed(realData->indexCount, realData->startIndex, realData->baseVertex);
    }

    static effVOID CopyConstantBufferData(const effVOID * data)
    {
        const CopyConstantBufferDataCommand * realData = (CopyConstantBufferDataCommand *)(data);
        //backend::CopyConstantBufferData(realData->constantBuffer, realData->data, realData->size);
    }
};



typedef effVOID * EFF3DCommandPacket;

class EFF3DCommandPacketUtilities
{
    static const effUINT64 OFFSET_NEXT_COMMAND_PACKET = 0u;
    static const effUINT64 OFFSET_BACKEND_DISPATCH_FUNCTION = OFFSET_NEXT_COMMAND_PACKET + sizeof(EFF3DCommandPacket);
    static const effUINT64 OFFSET_COMMAND = OFFSET_BACKEND_DISPATCH_FUNCTION + sizeof(BackendDispatchFunction);

    template <typename T>
    static EFF3DCommandPacket Create(effUINT64 auxMemorySize)
    {
        return ::operator new(GetSize<T>(auxMemorySize));
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

template <typename T>
class CommandBucket
{
public:
    typedef T Key;


    CommandBucket()
    {
        keys = NULL;
        datas = NULL;

        offsetTlsIndex = TlsAlloc();
        remainingTlsIndex = TlsAlloc();
    }

    ~CommandBucket()
    {
        TlsFree(offsetTlsIndex);
        TlsFree(remainingTlsIndex);
    }

    template <typename U>
    U * AddCommand(Key key, effUINT64 auxMemorySize)
    {
        EFF3DCommandPacket packet = EFF3DCommandPacketUtilities::Create<U>(auxMemorySize);

        // store key and pointer to the data
        {
            effINT id = tlsThreadId;

            // fetch number of remaining packets in this layer for the thread with this id
            //effINT remaining = m_tlsRemaining[id];
            //effINT offset = m_tlsOffset[id];
            effINT remaining = (effINT)TlsGetValue(remainingTlsIndex);
            effINT offset = (effINT)TlsGetValue(offsetTlsIndex);

            if (remaining == 0)
            {
                // no more storage in this block remaining, get new one
                //offset = core::atomic::Add(&current, 32);
                offset = eastl::Internal::atomic_compare_and_swap(&current, current + 32, current);
                remaining = 32;

                // write back
                //m_tlsOffset[id] = offset;
                TlsSetValue(offsetTlsIndex, &offset);
            }

            effINT index = offset + (32 - remaining);
            keys[index] = key;
            packets[index] = packet;
            --remaining;

            // write back
            //m_tlsRemaining[id] = remaining;
            TlsSetValue(remainingTlsIndex, &offset);
        }

        EFF3DCommandPacketUtilities::StoreNextCommandPacket(packet, nullptr);
        EFF3DCommandPacketUtilities::StoreBackendDispatchFunction(packet, U::DISPATCH_FUNCTION);

        return EFF3DCommandPacketUtilities::GetCommand<U>(packet);
    }

    template <typename U, typename V>
    U * AppendCommand(V * command, effUINT64 auxMemorySize)
    {
        EFF3DCommandPacket packet = EFF3DCommandPacketUtilities::Create<U>(auxMemorySize);

        // append this command to the given one
        EFF3DCommandPacketUtilities::StoreNextCommandPacket<V>(command, packet);

        EFF3DCommandPacketUtilities::StoreNextCommandPacket(packet, nullptr);
        EFF3DCommandPacketUtilities::StoreBackendDispatchFunction(packet, U::DISPATCH_FUNCTION);

        return EFF3DCommandPacketUtilities::GetCommand<U>(packet);
    }
private:
    Key * keys;
    effVOID ** datas;
    effUINT offsetTlsIndex;
    effUINT remainingTlsIndex;
};


EFF3D_END



#endif