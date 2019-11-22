/******************************************************************************
	created:	2019-03-23   00:41
	file path:	d:\EFF\EFFEngine\Src\EFFD3D12\EFFD3D12IndexBuffer.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFD3D12IndexBuffer_2019_03_23__
#define __EFFD3D12IndexBuffer_2019_03_23__



class EFFD3D12IndexBuffer : public EFF3DIndexBuffer
{
	friend class EFFD3D12Device;
	friend class EFFD3D12DeviceCommandList;
public:
	EFFD3D12IndexBuffer();
	virtual ~EFFD3D12IndexBuffer();
public:


    virtual effVOID             CreateFromMemory(effUINT size, effVOID * data, effUINT flag);
    virtual effVOID             Update(effUINT offset, effUINT size, effVOID * data, effBOOL discard = effFALSE);

protected:
	virtual effVOID				CreateViews(effUINT numElements, effUINT elementSize);
protected:
	D3D12_INDEX_BUFFER_VIEW		indexBufferView;
	ComPtr<ID3D12Resource>		d3d12Resource;
};

#endif