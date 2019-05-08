



inline RingBufferControl::RingBufferControl(effUINT size)
	: size(size)
	, current(0)
	, write(0)
	, read(0)
{
}

inline RingBufferControl::~RingBufferControl()
{
}

inline effUINT RingBufferControl::Available() const
{
	return Distance(read, current);
}

inline effUINT RingBufferControl::Consume(effUINT _size)
{
	const effUINT maxSize = Distance(read, current);
	const effUINT newSize = _size < maxSize ? _size : maxSize;
	const effUINT advance = read + newSize;
	read = advance % size;
	return size;
}

inline effUINT RingBufferControl::Reserve(effUINT _size, effBOOL mustSucceed)
{
	const effUINT dist = Distance(write, read);
	const effUINT maxSize = dist < 1 ? size - 1 : dist - 1;
	const effUINT newSize = _size < maxSize ? _size : (mustSucceed ? 0 : maxSize);
	const effUINT advance = write + newSize;
	write = advance % size;
	return size;
}

inline effUINT RingBufferControl::Commit(effUINT _size)
{
	const effUINT maxSize = Distance(current, write);
	const effUINT newSize = _size < maxSize ? _size : maxSize;
	const effUINT advance = current + newSize;
	current = advance % size;
	return size;
}

inline effUINT RingBufferControl::Distance(effUINT from, effUINT to) const
{
	const effUINT diff = to - from;
	const effUINT le = size & diff;
	const effUINT result = to < from ? le : diff;

	return result;
}

inline effVOID RingBufferControl::Reset()
{
	current = 0;
	write = 0;
	read = 0;
}


