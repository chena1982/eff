/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

/**
 * 	@file
 *
 *	This file provides the definition of the MemoryOStream class.
 *
 * 	@ingroup network
 */

#ifndef MEMORY_STREAM_HPP
#define MEMORY_STREAM_HPP

#include "cstdmf/binary_stream.hpp"

// TODO: should we abstract this class from network specific stuff (derek)
const int PACKET_MAX_SIZE = 2048;



// @todo: PM: This is here temporarily while we get the initial EventHistory working

/**
 *	This class is used to create a message that will later be placed on a
 *	bundle. It supports both the BinaryOStream and BinaryIStream
 *	interfaces, so it is possible to read and write data to and from
 *	this stream.
 *
 *	@ingroup network
 */
class MemoryOStream : public BinaryOStream, public BinaryIStream
{
public:
	MemoryOStream( int size = 64 );
	virtual ~MemoryOStream();
	int size() const;
	bool shouldDelete() const;
	void shouldDelete( bool value );
	void * data();
	virtual void * reserve( int nBytes );
	virtual void * retrieve( int nBytes );
	virtual char peek();

	void reset();
	void rewind();

	virtual int remainingLength() const;

protected:
	MemoryOStream( const MemoryOStream & );
	MemoryOStream & operator=( const MemoryOStream & );
	char * pBegin_;
	char * pCurr_;
	char * pEnd_;
	char * pRead_;
	bool shouldDelete_;
};

/**
 *	This class is used to stream data off a memory block.
 */
class MemoryIStream : public BinaryIStream
{
public:
	MemoryIStream( char * pData, int length ) :
		pCurr_( pData ),
		pEnd_( pCurr_ + length )
	{}

	MemoryIStream( void * pData, int length ) :
		pCurr_( reinterpret_cast< char * >( pData ) ),
		pEnd_( pCurr_ + length )
	{}

	virtual ~MemoryIStream();

	virtual void * retrieve( int nBytes );
	virtual int remainingLength() const { return (int)(pEnd_ - pCurr_); }
	virtual char peek();
	virtual void finish()	{ pCurr_ = pEnd_; }
	void * data() { return pCurr_; }

private:
	char * pCurr_;
	char * pEnd_;
};


/**
 * TODO: to be documented.
 */
class MessageStream : public MemoryOStream
{
public:
	MessageStream( int size = 64 );
	bool addToStream( BinaryOStream & stream, uint8 messageID );
	bool getMessage( BinaryIStream & stream,
		int & messageID, int & length );
};

#include "memory_stream.ipp"

#endif // MEMORY_STREAM_HPP
