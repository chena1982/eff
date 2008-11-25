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

#ifndef PROFILE_H
#define PROFILE_H

#include "cstdmf/config.hpp"
#include "cstdmf/stdmf.hpp"

#if ENABLE_WATCHERS


#include <iostream>
#include <vector>

#include "cstdmf/watcher.hpp"
#include "cstdmf/timestamp.hpp"

// #define CAREFUL_TIMERS
#ifdef CAREFUL_TIMERS
#include "cstdmf/debug.hpp"
#endif

/**
 *	This class is used to profile the performance of parts of the code.
 */
class ProfileVal
{
public:
	/**
	 *	Constructor.
	 */
	ProfileVal() :
		lastTime( 0 ),
		sumTime( 0 ),
		lastQuantity( 0 ),
		sumQuantity( 0 ),
		count( 0 ),
		inProgress( 0 )
	{
	}

	/**
	 *	This method starts this profile object.
	 */
	void start()
	{
#ifdef CAREFUL_TIMERS
		MF_ASSERT(!inProgress);
#endif
		if (inProgress == 0)
		{
			lastTime = timestamp();
		}

		++inProgress;
	}

	/**
	 *	This method stops this profile object.
	 */
	void stop(uint32 qty = 0)
	{
#ifdef CAREFUL_TIMERS
		MF_ASSERT(inProgress);
#endif
		uint64 now = timestamp();
		--inProgress;

		if (inProgress == 0)
		{
			lastTime = now - lastTime;
			sumTime += lastTime;

		}

		lastQuantity = qty;
		sumQuantity += qty;

		count++;
	}

	/**
	 *	This method returns whether or not this profile is currently running.
	 *	That is, whether start has been called without a corresponding call to
	 *	stop.
	 */
	bool running() const
	{
		return (inProgress != 0);
	}

	// data is nicely 32 bytes
	uint64		lastTime;		///< The time the profile was started.
	uint64		sumTime;		///< The total time between all start/stops.
	uint32		lastQuantity;	///< The last value passed into stop.
	uint32		sumQuantity;	///< The total of all values passed into stop.
	uint32		count;			///< The number of times stop has been called.
	int			inProgress;		///< Whether the profile is currently timing.

	static Watcher & watcher();
	static Watcher & detailWatcher();
};

/**
 *	This function is the output stream operator for a ProfileVal.
 *
 *	@relates ProfileVal
 */
std::ostream& operator<<( std::ostream &s, const ProfileVal &v );

/**
 *	This function is the input stream operator for ProfileVal.
 *
 *	@relates ProfileVal
 */
std::istream& operator>>( std::istream &s, ProfileVal &v );

/**
 * A class to wrap up a group of profiles. The first one is
 * started immediately, to keep track of the group's running time.
 */
class ProfileGroup : public std::vector<ProfileVal>
{
public:
	ProfileGroup( unsigned int num = 1 );

	void reset();
};

/**
 *	This structure wraps up a uint64 timestamp delta and has an operator defined
 *	on it to print it out nicely.
 */
struct NiceTime
{
	/// Constructor
	explicit NiceTime(uint64 t) : t_(t) {}

	uint64 t_;	///< Associated timestamp.
};

/**
 *	This function is the output stream operator for a NiceTime.
 *
 *	@relates NiceTime
 */
std::ostream& operator<<( std::ostream &o, const NiceTime &nt );


/**
 *	This singleton class resets all registered ProfileGroups when a nominated
 *	ProfileVal is reset.
 */
class ProfileGroupResetter
{
public:
	ProfileGroupResetter();
	~ProfileGroupResetter();

	void nominateProfileVal( ProfileVal * pVal = NULL );
	void addProfileGroup( ProfileGroup * pGroup );

	static ProfileGroupResetter & instance();

private:
	ProfileVal * nominee_;

	std::vector< ProfileGroup * >	groups_;

	bool doingReset_;

	// called by every ProfileVal when it is reset
	void resetIfDesired( ProfileVal & val );

	friend std::istream& operator>>( std::istream &s, ProfileVal &v );
};

class MethodProfiler
{
public:
	MethodProfiler(ProfileVal& profileVal) :
		profileVal_(profileVal)
	{
		profileVal_.start();
	}

	~MethodProfiler()
	{
		profileVal_.stop();
	}

private:
	ProfileVal& profileVal_;
};

class ProfileWatcher : public DirectoryWatcher
{
public:
	explicit ProfileWatcher( ProfileGroup &pg, const char **labels )
	{
		// summary profiles
		Watcher *sum = new SequenceWatcher<ProfileGroup>( pg, labels );
		sum->addChild( "*", &ProfileVal::watcher() );

		// detail profiles
		Watcher *det = new SequenceWatcher<ProfileGroup>( pg, labels );
		det->addChild( "*", &ProfileVal::detailWatcher() );

		// Add subdirs
		this->addChild( "summary", sum );
		this->addChild( "details", det );
	}
};


#else

struct NiceTime
{
	explicit NiceTime(uint64 t) : t_(t) {}

	uint64 t_;
};

std::ostream& operator<<( std::ostream &o, const NiceTime &nt );



#endif //ENABLE_WATCHERS


#endif	// PROFILE_H
