/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#include "pch.hpp"

#include "dogwatch.hpp"

#include "debug.hpp"
#include "memory_counter.hpp"

//DECLARE_DEBUG_COMPONENT2( "CStdMF", 0 )


#ifdef _WIN32
#pragma warning (push)
#pragma warning (disable : 4355 )	// this use in initialiser list
#endif // _WIN32

DogWatchManager *	DogWatchManager::pInstance	= NULL;
uint64              DogWatch::s_nullSlice_      = 0;

memoryCounterDefine( dogWatch, Base );

// -----------------------------------------------------------------------------
// Section DogWatch
// -----------------------------------------------------------------------------

/**
 *	Constructor.
 *
 *	@param title	The title to be associated with this stopwatch.
 */
DogWatch::DogWatch( const char * title ) :
	started_(0),
	pSlice_( &s_nullSlice_ ),
	id_( DogWatchManager::instance().add( *this ) ),
	title_( title )
{
}

#ifdef _WIN32
#pragma warning (pop)
#endif // _WIN32


// -----------------------------------------------------------------------------
// Section DogWatchManager
// -----------------------------------------------------------------------------

/**
 *	Default constructor. The associated title is the empty string.
 */
DogWatchManager::DogWatchManager()
{
	// make the root of the manifestation tree
	root_.statid = 0xFF;
	root_.tableid = 0xFF;

	// put it on the stack
	depth_ = 0;
	stack_[0] = &root_;

	// make statid 0 for root
	newManifestation( root_, -1 );

	// clear the cache
	this->clearCache();

	// start the frame timer
	slice_ = 0;
	frameStart_ = timestamp();
}


/**
 *	This method clears the manifestation cache.
 */
void DogWatchManager::clearCache()
{
	for (uint i=0; i < sizeof(cache_)/sizeof(*cache_); i++)
	{
		cache_[i].stackNow = NULL;
	}
}


/**
 *	This method adds a DogWatch to our collection.
 *
 *	@param watch	The DogWatch to add to the collection.
 *
 *	@return The identifier associated with the stopwatch.
 */
uint8 DogWatchManager::add( DogWatch & watch )
{
	MF_ASSERT( watches_.size() < 128 );

	uint8	id = watches_.size();
	watches_.push_back( &watch );

	return id;
}


/**
 *	This method grabs a slice when it is not in the cache.
 */
uint64 & DogWatchManager::grabSliceMissedCache( int id )
{
	// get the table at the top of the stack
	TableElt & pte = *stack_[ depth_ ];
	TablePtr pTable = (pte.tableid != 0xFF) ?
		tables_[ pte.tableid ] : this->newTable( pte );

	// get the manifestation of this stat
	TableElt & cte = (*pTable)[id];
	stack_[++depth_] = &cte;

	uint64 * pSlice = &(*( (cte.statid != 0xFF) ?
		stats_[ cte.statid ] : this->newManifestation( cte, id ) ))
			[ slice_ ];

	// Write the cache entry
	Cache & cache = cache_[id];
	cache.stackNow = &pte;
	cache.stackNew = &cte;
	cache.pSlice = pSlice;

	return *pSlice;
}


/**
 *	This method creates a new manifestation.
 */
DogWatchManager::StatPtr DogWatchManager::newManifestation( TableElt & te,
	int watchid )
{
	te.statid = stats_.size();

	StatPtr pStat = new Stat();

	for (int i=0; i < Stat::COUNT; i++)
	{
		(*pStat)[i] = 0;
	}

	pStat->watchid = watchid;
	pStat->flags = 0;

	{ memoryCounterSub( dogWatch ); memoryClaim( stats_ ); }

	stats_.push_back( pStat );

	memoryCounterAdd( dogWatch );
	memoryClaim( pStat );
	memoryClaim( stats_ );

	return pStat;
}


/**
 *	This method creates a new table.
 */
DogWatchManager::TablePtr DogWatchManager::newTable( TableElt & te )
{
	MF_ASSERT_DEBUG( tables_.size() < 255 );

	te.tableid = tables_.size();

	TablePtr pTable = new Table();

	for (int i=0; i < Table::COUNT; i++)
	{
		(*pTable)[i].statid = 0xFF;
		(*pTable)[i].tableid = 0xFF;
	}

	{ memoryCounterSub( dogWatch ); memoryClaim( tables_ ); }

	tables_.push_back( pTable );

	memoryCounterAdd( dogWatch );
	memoryClaim( pTable );
	memoryClaim( tables_ );

	return pTable;
}


/**
 *	This method records the end of the frame.
 */
void DogWatchManager::tick()
{
	// change the frame timer
	uint64	now = timestamp();
	(*stats_[ root_.statid ])[ slice_ ] = now - frameStart_;
	frameStart_ = now;

	// figure out what slice to look at
	slice_++;
	if (slice_ >= Stat::COUNT)
	{
		slice_ = 0;
	}

	// clear the new slice
	for (uint i=0; i < stats_.size(); i++)
	{
		(*stats_[i])[ slice_ ] = 0;
	}

	// restart all running watches, so this part of their timing
	//  gets allocated to the correct slice.
	DogWatch * restartList[32];
	int restartCount = 0;
	while( depth_ > 0 )
	{
		DogWatch * pWatch =
			watches_[ stats_[ stack_[ depth_ ]->statid ]->watchid ];
		pWatch->stop();
		restartList[ restartCount++ ] = pWatch;
	}

	// clear the cache
	this->clearCache();

	// restart the watches
	for (int i = restartCount - 1; i >= 0; i--)
	{
		restartList[i]->start();
	}
}


/**
 *	This method returns the singleton instance of the class.
 *
 *	@return The singleton instance.
 */
DogWatchManager & DogWatchManager::instance()
{
	if (pInstance == NULL)
	{
		pInstance = new DogWatchManager();
	}
	return *pInstance;
}


/**
 *	This method cleans the resources used by DogWatchManager.
 */
void DogWatchManager::fini()
{
	delete pInstance;
	pInstance = NULL;
}


/**
 *	This method returns the iterator at the beginning of the sequence. This
 *	allows this object to be iterated over in an STL fashion.
 */
DogWatchManager::iterator DogWatchManager::begin() const
{
	return DogWatchManager::iterator( NULL, -1, &root_ );
}


/**
 *	This method returns the iterator at the end of the sequence. This allows
 *	this object to be iterated over in an STL fashion.
 */
DogWatchManager::iterator DogWatchManager::end() const
{
	return iterator( NULL );
}


// -----------------------------------------------------------------------------
// Section: DogWatchManager::iterator
// -----------------------------------------------------------------------------

/**
 *	Constructor used to create a normal iterator.
 */
DogWatchManager::iterator::iterator( const Table * pTable, int id,
									const TableElt * pTE ) :
	pTable_( pTable ),
	id_( id ),
	te_( pTE != NULL ? *pTE : (*pTable)[id] )
{
}


/**
 *	Constructor used to create an end iterator.
 */
DogWatchManager::iterator::iterator( const Table * pTable ) :
	pTable_( pTable ),
	id_( -1024 ),
	te_( (*pTable)[0] )
{
}


/**
 *	This method implements the assignment operator.
 */
DogWatchManager::iterator & DogWatchManager::iterator::operator=(
	const DogWatchManager::iterator & iter )
{
	// slightly illegal ... reference-wise
	memcpy( this, &iter, sizeof( *this ) );
	return *this;
}


/**
 *	This method implements the equals operator.
 *
 *	@retval	true	If both iterators refer to the same element.
 *	@retval false	Otherwise.
 */
bool DogWatchManager::iterator::operator==( const iterator & iter )
{
	return (iter.pTable_ == pTable_ && iter.id_ == id_);
}


/**
 *	This method moves the iterator to the next element in the sequence.
 */
DogWatchManager::iterator & DogWatchManager::iterator::operator++()
{
	// can't move along an 'end' iterator
	if ( id_ == -1024 )
	{
		return *this;
	}


	// find the next statistic (but assume we won't)
	bool atEnd = true;

	if (pTable_ != NULL)
	{
		for (id_++; id_ < Table::COUNT; id_++)
		{
			if ((*pTable_)[id_].statid != 0xFF)
			{
				atEnd = false;
				break;
			}
		}
	}

	// we didn't find one
	if (atEnd)
	{
		id_ = -1024;
	}
	else
	{
		// only way to overwrite that 'te' reference... :)
		*this = iterator( pTable_, id_ );
	}

	return *this;
}


/**
 *	This method returns an iterator referring to the first DogWatch of this
 *	object. This method can be used to iterate over all DogWatch objects of this
 *	object in an STL fashion.
 */
DogWatchManager::iterator DogWatchManager::iterator::begin() const
{
	// make sure we have a sub table
	if (te_.tableid != 0xFF)
	{
		// find the first manifestation in it
		Table * pTable = &*pInstance->tables_[ te_.tableid ];
		for (int i=0; i < Table::COUNT; i++)
		{
			if( (*pTable)[i].statid != 0xFF )
			{
				return iterator( pTable, i );
			}
		}

		// hmmm... doesn't have any. strange.
		return iterator( pTable );
	}

	// ok, we don't have any children, so return an end
	// iterator for some unique fake table
	return iterator( (Table*)(1 + (char*)pTable_) );
}


/**
 *	This method returns an iterator referring to the element one past the last
 *	DogWatch of this object. This method can be used to iterate over all
 *	DogWatch objects of this object in an STL fashion.
 */
DogWatchManager::iterator DogWatchManager::iterator::end() const
{
	if (te_.tableid != 0xFF)
	{
		Table * pTable = &*pInstance->tables_[ te_.tableid ];
		return iterator( pTable );
	}

	return iterator( (Table*)(1 + (char*)pTable_) );
}


/**
 *	This method returns the name of the DogWatch referred to by this iterator.
 */
const std::string & DogWatchManager::iterator::name() const
{
	if (id_ >= 0)
	{
		return pInstance->watches_[ id_ ]->title();
	}
	else
	{
		static std::string rootName( "Frame" );
		return rootName;
	}
}


/**
 *	This method returns the flags of this manifestation.
 */
int DogWatchManager::iterator::flags() const
{
	return pInstance->stats_[ te_.statid ]->flags;
}


/**
 *	This method sets the flags of this manifestation.
 */
void DogWatchManager::iterator::flags( int flags )
{
	pInstance->stats_[ te_.statid ]->flags = flags;
}




/**
 *	This method gets the value of this iterator at 'frameAgo' frames ago.
 *
 *	@param frameAgo	Can be zero, but this will likely get incomplete data - only
 *					those DogWatches that have already been started and stopped
 *					this frame.
 */
uint64 DogWatchManager::iterator::value( int frameAgo ) const
{
	MF_ASSERT( te_.statid != 0xFF );

	StatPtr pStat = pInstance->stats_[ te_.statid ];
	int slice = int(pInstance->slice_) - frameAgo;
	if (slice < 0)
	{
		slice += Stat::COUNT;
	}

	return (*pStat)[ slice ];
}


/**
 *	This method returns the average value of this iterator over the given
 *	(inclusive) range.
 *
 *	@param firstAgo	Indicates the start of the range.
 *	@param lastAgo	Indicates the end of the range.
 *
 *	@return The average value over the input range.
 */
uint64 DogWatchManager::iterator::average( int firstAgo, int lastAgo ) const
{
	MF_ASSERT( firstAgo >= lastAgo );

	uint64	sum = 0;
	for (int i=firstAgo; i >= lastAgo; i--)
	{
		sum += this->value( i );
	}

	return sum / uint64( firstAgo + 1 - lastAgo );
}

// dogwatch.cpp
