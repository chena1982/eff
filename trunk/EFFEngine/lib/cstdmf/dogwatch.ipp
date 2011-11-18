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
 *	@file
 */

#include "timestamp.hpp"
#include "debug.hpp"

// -----------------------------------------------------------------------------
// Section: DogWatch
// -----------------------------------------------------------------------------

/**
 *	This method starts this stopwatch.
 */
inline void DogWatch::start()
{
#ifndef NO_DOG_WATCHES
	pSlice_ = &DogWatchManager::pInstance->grabSlice( id_ );
	started_ = timestamp();
#endif
}


/**
 *	This method stops this stopwatch.
 */
inline void DogWatch::stop()
{
#ifndef NO_DOG_WATCHES
	
#ifdef _DEBUG
	if ( pSlice_ == &s_nullSlice_ )
	{
		WARNING_MSG("Stopping DogWatch '%s', which hasn't been started yet.\n", 
					title_.c_str() );
		return;
	}
#endif

	(*pSlice_) += timestamp() - started_;
	DogWatchManager::pInstance->giveSlice();
	
#endif
}


/**
 *	This method returns the current slice value accumulated in this watcher.
 */
inline uint64 DogWatch::slice() const
{
	if ( pSlice_ )
		return *pSlice_;
		
	return 0;
}


/**
 *	This method returns the title assoicated with this stopwatch.
 */
inline const std::string & DogWatch::title() const
{
	return title_;
}


// -----------------------------------------------------------------------------
// Section DogWatchManager
// -----------------------------------------------------------------------------

/**
 *	@todo Comment
 */
inline uint64 & DogWatchManager::grabSlice( int id )
{
	// set up some variables
	Cache & cache = cache_[id];
	TableElt ** backStack = &stack_[ depth_ ];

	// try the cache
	if (cache.stackNow == *backStack)
	{
		backStack[1] = cache.stackNew;
		depth_++;

		return *cache.pSlice;
	}

	// ok, fall back to a bit more code
	return this->grabSliceMissedCache( id );
}


/**
 *	@todo Comment
 */
inline void DogWatchManager::giveSlice()
{
	depth_--;
}

// dogwatch.ipp
