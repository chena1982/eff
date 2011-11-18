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
 *	@file stpwatch.ipp
 */

#include "timestamp.hpp"
#include "stdmf.hpp"
#include <vector>

#include "debug.hpp"

#ifdef CODE_INLINE
#define INLINE inline
#else
/// INLINE macro
#define INLINE
#endif


// -----------------------------------------------------------------------------
// Section: Construction/Destruction
// -----------------------------------------------------------------------------

/**
 *	Constructor.
 */
INLINE
StopWatch::StopWatch() :
	totalTime_(0),
	startTime_(0),
	memory_(0)
#ifdef _DEBUG
	, timerStarted_(false)
#endif
{
}


// -----------------------------------------------------------------------------
// Section: Timer controls
// -----------------------------------------------------------------------------

/**
 *	This method starts the stopwatch. There should be a corresponding stop
 *	called before the method is called again.
 *
 *	@see stop
 */
INLINE
void StopWatch::start()
{
#ifdef _DEBUG
#ifndef EDITOR_ENABLED
	MF_ASSERT(!timerStarted_);
	timerStarted_ = true;
#endif
#endif

	startTime_ = ((uint32)timestamp());
}


/**
 *	This method stops the stopwatch. A corresponding start should have been
 *	called. The elapsed time between the call to start and the call to stop is
 *	added to the total time.
 *
 *	@see start
 */
INLINE
void StopWatch::stop()
{
#ifdef _DEBUG
#ifndef EDITOR_ENABLED
	MF_ASSERT(timerStarted_);
	timerStarted_ = false;
#endif
#endif

	this->totalTime_ += (((uint32)timestamp()) - startTime_);
}


/**
 *	This method resets the total time to 0.
 */
INLINE
void StopWatch::reset()
{
#ifdef _DEBUG
#ifndef EDITOR_ENABLED
	MF_ASSERT(!timerStarted_);
#endif
#endif
	this->totalTime_ = 0;
}


/**
 *	This method resets the stopwatch and then starts it.
 */
INLINE
void StopWatch::restart()
{
	this->reset();
	this->start();
}


/**
 *	This method returns the total time currently associated with this stopwatch.
 *	The total time is the sum of the elapsed time between all start/stop pairs
 *	since the last reset call.
 *
 *	@return	The total time in clock cycles.
 */
INLINE
uint32 StopWatch::time() const
{
#ifdef _DEBUG
#ifndef EDITOR_ENABLED
	MF_ASSERT(!timerStarted_);
#endif
#endif
	return this->totalTime_;
}


/**
 *	This method returns the time elapsed from the last start call to the current
 *	time.
 *
 *	@return	The current split time in clock cycles.
 */
INLINE
uint32 StopWatch::splitTime() const
{
#ifdef _DEBUG
#ifndef EDITOR_ENABLED
	MF_ASSERT(timerStarted_);
#endif
#endif
	return this->totalTime_ + (((uint32)timestamp()) - this->startTime_);
}


/**
 *	This method returns the value stored in the memory of this object.
 *
 *	@see setMemory
 */
INLINE
uint32 StopWatch::memory() const
{
	return memory_;
}


/**
 *	This method sets the memory value of this object to the value stored in
 *	total time.
 */
INLINE
void StopWatch::setMemory()
{
	memory_ = totalTime_;
}


// -----------------------------------------------------------------------------
// Section: DebugWatch
// -----------------------------------------------------------------------------

/**
 *	This method returns the title of this stopwatch.
 */
INLINE
const std::string & DebugWatch::title() const
{
	return title_;
}


/**
 *	This method returns the container that stores all of the created DebugWatch
 *	objects. Whenever a DebugWatch is created, it inserts itself into this
 *	container.
 */
INLINE
DebugWatch::Container * DebugWatch::pWatches()
{
	return pWatches_;
}

/*stpwatch.ipp*/
