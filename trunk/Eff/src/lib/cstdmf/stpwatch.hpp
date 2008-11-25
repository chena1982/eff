/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef STPWATCH_HPP
#define STPWATCH_HPP

#include "stdmf.hpp"
#include "intrusive_object.hpp"

#include <string>

/**
 *	This class is used to do timing.
 */
class StopWatch
{
public:
	/// @name Construction/Destruction
	//@{
	StopWatch();
	//@}

	/// @name Timer controls
	//@{
	void start();
	void stop();
	void reset();
	void restart();

	uint32 time() const;
	uint32 splitTime() const;

	uint32 memory() const;
	void setMemory();
	//@}

private:
	uint32 totalTime_;
	uint32 startTime_;
	uint32 memory_;

#ifdef _DEBUG
	bool timerStarted_;
#endif
};


/**
 *	This class is used to time events. Every instance of this class
 *	automatically adds itself to the static pWatches container when it is
 *	constructed and removed from the collection when it is destroyed.
 */
class DebugWatch :
	public StopWatch,
	public IntrusiveObject< DebugWatch >
{
public:
	/// @name Construction/Destruction
	//@{
	DebugWatch( const char * title );
	//@}

	const std::string & title() const;

	/// @name Static members
	//@{
	static Container * pWatches();
	static void resetAll();
	static void setMemoryOnAll();
	//@}

private:
	std::string title_;
	static Container * pWatches_;
};

class ScopeWatch
{
private:
	DebugWatch& dw_;
public:
	inline ScopeWatch( DebugWatch& dw )
	: dw_( dw )
	{
		dw_.start();
	}
	inline ~ScopeWatch()
	{
		dw_.stop();
	}
};



#ifdef CODE_INLINE
#include "stpwatch.ipp"
#endif


#endif
/*stpwatch.hpp*/
