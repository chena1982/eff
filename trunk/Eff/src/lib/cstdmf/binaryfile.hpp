/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef BINARY_FILE_HPP
#define BINARY_FILE_HPP

#include <stdio.h>
#include <string>
#include <map>

#include "stdmf.hpp"
#include "debug.hpp"

/**
 *	This class provides a wrapper to FILE * which makes it easy
 *	to read and write to a binary format. It can also wrap (or copy)
 *	an existing buffer in memory, making it easy to read from or
 *	write to it.
 */
class BinaryFile
{
public:
	BinaryFile( FILE * bf ) :
		f_( bf ),
		data_( NULL ),
		ptr_( NULL ),
		bytesCached_(0)
	{
	}

	~BinaryFile()
	{
		if (f_ != NULL)
		{
			if (int32(f_) != -1) fclose( f_ );

			if (data_ != NULL)
			{
				free(data_);
				data_ = NULL;
			}
		}
	}

	FILE * file()						{ return f_; }
	const FILE * file() const			{ return f_; }

	/**
	 *	Read the whole file into memory.
	 *
	 *	@return the number of bytes loaded
	 */
	long cache()
	{
		if (data_ == NULL)
		{
			fseek( f_, 0, SEEK_END );
			bytesCached_ = ftell( f_ );

			if (!bytesCached_)
				return 0L;

			data_ = malloc( bytesCached_ );
			fseek( f_, 0, SEEK_SET );
			fread( data_, 1, bytesCached_, f_ );
		}
		ptr_ = (char*)data_;
		return bytesCached_;
	}

	/**
	 *	Wrap this existing data, optionally making a copy of it
	 */
	void wrap( void * data, int len, bool makeCopy )
	{
		if (makeCopy)
		{
			data_ = malloc( len );
			memcpy( data_, data, len );
			f_ = (FILE*)-1;	// we only dispose if f is non-null
		}
		else
		{
			data_ = data;
		}
		ptr_ = (char*)data_;
		bytesCached_ = len;
	}

	/**
	 *	Allocate and reserve the given amount of data, and remember
	 *	to dispose it on destruction. Return a pointer to the data.
	 *	This makes this class effectively a memory stream.
	 */
	void * reserve( int len )
	{
		data_ = malloc( len );
		f_ = (FILE*)-1;
		ptr_ = (char*)data_;
		bytesCached_ = len;

		return data_;
	}
	
	template <class MAP> const BinaryFile & readMap( MAP & map ) const
	{
		map.clear();

		MAP::size_type	sz;
		(*this) >> sz;

		for (MAP::size_type i=0; i < sz; i++)
		{
			MAP::value_type	vt;
			(*this) >> const_cast<MAP::key_type&>(vt.first) >> vt.second;

			map.insert( vt );
		}

		return *this;
	}

	template <class MAP> BinaryFile & writeMap( const MAP & map )
	{
		(*this) << map.size();

		for (MAP::const_iterator it = map.begin();
			it != map.end();
			it++)
		{
			(*this) << (*it).first << (*it).second;
		}
		return *this;
	}

	template <class VEC> const BinaryFile & readSequence( VEC & vec ) const
	{
		vec.clear();

		VEC::size_type	sz;
		(*this) >> sz;

		vec.reserve( sz );

		for (VEC::size_type i=0; i < sz; i++)
		{
			VEC::value_type	vt;
			(*this) >> vt;

			vec.push_back( vt );
		}

		return *this;
	}

	template <class VEC> BinaryFile & writeSequence( const VEC & vec )
	{
		(*this) << vec.size();

		for (VEC::const_iterator it = vec.begin(); it != vec.end(); it++)
		{
			(*this) << (*it);
		}

		return *this;
	}

	// reads nbytes into buffer, returns actual amount of bytes read.
	size_t read( void * buffer, size_t nbytes ) const
	{
		if (ptr_)
		{
			switch ( nbytes )
			{
			case 1:
				*(uint8*)buffer = *(uint8*)ptr_;
				break;
			case 4:
				*(uint32*)buffer = *(uint32*)ptr_;
				break;
			default:
				memcpy( buffer, ptr_, nbytes );
				break;
			}

			ptr_ += nbytes;
			return nbytes;
		}

		return fread( buffer, 1, nbytes, f_ );
	}

	// writes nbytes from buffer, returns actual amount of bytes written.
	size_t write( const void * buffer, size_t nbytes )
	{
		return fwrite( buffer, 1, nbytes, f_ );
	}

private:
	FILE * f_;
	void * data_;
	mutable char * ptr_;
	long bytesCached_;
};


	
template <class T>
inline BinaryFile & operator<<( BinaryFile & bf, const T & data )
{
	bf.write( &data, sizeof(T) );

	return bf;
}

inline BinaryFile & operator<<( BinaryFile & bf, const char * & s )
{
	int	len = strlen( s );
	bf.write( &len, sizeof(int) );
	bf.write( s, len );
	return bf;
}

inline BinaryFile & operator<<( BinaryFile & bf, const std::string & s )
{
	int	len = s.length();
	bf.write( &len, sizeof(int) );
	bf.write( s.data(), len );
	return bf;
}


template <class T>
inline const BinaryFile & operator>>( const BinaryFile & bf, T & data )
{
	// read bytes
	const size_t	size= sizeof(T);
	size_t			n	= bf.read( &data, size );
	
	// check length
	if ( n != size )
	{
		WARNING_MSG( "BinaryFile& operator>> expected to read %d bytes, got %d.\n",
					 size, n );
	}

	return bf;
}

inline const BinaryFile & operator>>( const BinaryFile & bf, char * & s )
{
	// read string length.
	const size_t	size= sizeof(int);
	size_t			len = 0;
	size_t			n	= bf.read( &len, size );

	if ( n == size )
	{
		// if length is ok and non-zero try to read string.
		if ( len > 0 )
		{
			n = bf.read( s, len );

			// check actual amount read.
			if ( n != len)
			{
				WARNING_MSG( "BinaryFile& operator>> expected string of"
					"length %d, got %d.\n", len, n );
			}
		}
	}
	else
	{
		WARNING_MSG( "BinaryFile& operator>> couldn't read string length.\n" );
	}

	return bf;
}

inline const BinaryFile & operator>>( const BinaryFile & bf, std::string & s )
{
	// read string length.
	const size_t	size= sizeof(int);
	size_t			len = 0;
	size_t			n	= bf.read( &len, size );

	if ( n == size )
	{
		// if length is ok and non-zero try to read string.
		if ( len > 0 )
		{
			// resize string to match incoming.
			s.resize( len );
			n = bf.read( const_cast<char*>( s.data() ), len );
			
			// check actual amount read - bf.read wouldn't have read more bytes,
			// only less or equal. Assert the "impossible" condition and handle
			// other condition - we couldn't read enough.
			MF_ASSERT( !( n > len ) && "Overwrote buffer while reading!" );
			if ( n < len)
			{
				// resize back.
				s.resize( n );
				WARNING_MSG( "BinaryFile& operator>> expected string of"
							 "length %d, got %d.\n", len, n );
			}
		}
	}
	else
	{
		WARNING_MSG( "BinaryFile& operator>> couldn't read string length.\n" );
	}

	return bf;
}


#endif // BINARY_FILE_HPP
