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


#include "debug.hpp"

//DECLARE_DEBUG_COMPONENT2( "StpWatch", 0 )
//DECLARE_DEBUG_COMPONENT(0);	// debugLevel for this file

#include "stpwatch.hpp"

#ifndef CODE_INLINE
#include "stpwatch.ipp"
#endif



std::vector< DebugWatch * > * DebugWatch::pWatches_ = NULL;


/**
 *	Constructor for DebugWatch.
 *
 *	@param title	The title associated with this stop watch.
 */
DebugWatch::DebugWatch( const char * title ) :
	IntrusiveObject< DebugWatch >( pWatches_ ),
	title_( title )
{

}


/**
 *	This static method restarts all debug watches.
 */
void DebugWatch::resetAll()
{
	if (pWatches_ != NULL)
	{
		Container::iterator iter = pWatches_->begin();

		while (iter != pWatches_->end())
		{
			(*iter)->reset();

			iter++;
		}
	}
}


/**
 *	This static method sets the memory on all debug stop watches.
 */
void DebugWatch::setMemoryOnAll()
{
	if (pWatches_ != NULL)
	{
		Container::iterator iter = pWatches_->begin();

		while (iter != pWatches_->end())
		{
			(*iter)->setMemory();

			iter++;
		}
	}
}

// stpwatch.cpp
