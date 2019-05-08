#ifndef __EFFRingBuffer_2019_04_03__
#define __EFFRingBuffer_2019_04_03__



EFFBASE_BEGIN

class RingBufferControl
{
public:
	RingBufferControl(effUINT size);

	
	~RingBufferControl();


	inline effUINT Available() const;

	inline effUINT Consume(effUINT size); // consumer only

	inline effUINT Reserve(effUINT size, effBOOL mustSucceed = effFALSE); // producer only

	inline effUINT Commit(effUINT size); // producer only

	effUINT Distance(effUINT from, effUINT to) const; // both

	effVOID Reset();

	const effUINT size;
	effUINT current;
	effUINT write;
	effUINT read;
};


#include "EFFRingBuffer.inl"

EFFBASE_END




#endif
