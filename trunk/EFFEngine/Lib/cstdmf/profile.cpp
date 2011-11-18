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

#include "cstdmf/config.hpp"

#if ENABLE_WATCHERS


#include "cstdmf/profile.hpp"
#include "cstdmf/timestamp.hpp"


/**
 *	This method returns the Watcher associated with this class.
 */
Watcher & ProfileVal::watcher()
{
	static DataWatcher<ProfileVal> *watchMe = NULL;

	if (watchMe == NULL)
	{
		ProfileVal * pNull = NULL;
		watchMe = new DataWatcher<ProfileVal>( *pNull, Watcher::WT_READ_WRITE );
	}

	return *watchMe;
}

Watcher & ProfileVal::detailWatcher()
{
	static DirectoryWatcher	*watchMe = NULL;

	if (watchMe == NULL)
	{
		watchMe = new DirectoryWatcher();
		ProfileVal	* pNull = NULL;

		watchMe->addChild( "count", new DataWatcher<long>(
							   (long&)pNull->count, Watcher::WT_READ_ONLY ) );
		watchMe->addChild( "inProgress", new DataWatcher<bool>(
							   (bool&)pNull->inProgress, Watcher::WT_READ_ONLY ) );
		watchMe->addChild( "sumQuantity", new DataWatcher<long>(
							   (long&)pNull->sumQuantity, Watcher::WT_READ_ONLY ) );
		watchMe->addChild( "lastQuantity", new DataWatcher<long>(
							   (long&)pNull->lastQuantity, Watcher::WT_READ_ONLY ) );
		watchMe->addChild( "sumTime", new DataWatcher<int64>(
							   (int64&)pNull->sumTime, Watcher::WT_READ_ONLY ) );
		watchMe->addChild( "lastTime", new DataWatcher<int64>(
							   (int64&)pNull->lastTime, Watcher::WT_READ_ONLY ) );
	}

	return *watchMe;
}

// Comment in header file.
std::ostream& operator<<( std::ostream &s, const ProfileVal &v )
{
	ProfileVal	pv = v;

	if (pv.inProgress)
	{
		pv.lastTime = timestamp() - pv.lastTime;
	}

	s << "Count: " << pv.count;
	if (pv.inProgress) s << " (inProgress)";
	s << std::endl;

	if (pv.sumQuantity != 0)
	{
		s << "Quantity: " << pv.sumQuantity <<
			" (last " << pv.lastQuantity <<
			", avg " << ((double)pv.sumQuantity)/pv.count << ')' << std::endl;
	}

	s << "Time: " << NiceTime(pv.sumTime) <<
	" (last " << NiceTime(pv.lastTime) << ')' << std::endl;

	if (pv.sumTime != 0)
	{
		s << "Time/Count: " << NiceTime(pv.sumTime/pv.count) << std::endl;

		if (pv.sumQuantity)
		{
			s << "Time/Qty: " <<
				NiceTime(pv.sumTime/pv.sumQuantity) << std::endl;
		}
	}

	return s;
}


/**
 *	This function is the input stream operator for ProfileVal.
 *
 *	@relates ProfileVal
 */
std::istream& operator>>( std::istream &s, ProfileVal &v )
{
	v = ProfileVal();
	ProfileGroupResetter::instance().resetIfDesired( v );
	return s;
}


/*
void ProfileGroup::Accumulate(C_ProfileVal &acc, const C_ProfileVal &add)
{
	acc.count			+= add.count;
	acc.inProgress		+= add.inProgress;	// intentional
	acc.sumQuantity 	+= add.sumQuantity;
	acc.lastQuantity	+= add.lastQuantity;
	acc.sumTime			+= add.sumTime;
	acc.lastTime		+= add.lastTime;
}
*/


/**
 *	Constructor.
 *
 *	@param num	Indicates the size of the group.
 */
ProfileGroup::ProfileGroup(unsigned int num) :
	std::vector<ProfileVal>(num)
{
	if (num > 0) (*this)[0].start();
}


/**
 *	This method resets all of the ProfileVal's in this ProfileGroup.
 */
void ProfileGroup::reset()
{
	for (uint i=0;i<this->size();i++)
	{
		(*this)[i] = ProfileVal();
	}

	if (this->size() > 0) (*this)[0].start();
}



/// Constructor
ProfileGroupResetter::ProfileGroupResetter() :
	nominee_( 0 ),
	groups_( 0 ),
	doingReset_( false )
{
}

/// Destructor
ProfileGroupResetter::~ProfileGroupResetter()
{
}

/**
 * Records the ProfileVal whose reset should trigger a global reset.
 * Note: If this is in a ProfileGroup, you shouldn't add to the
 * group after you call this function as it could move the memory
 * containing the vector elements around and stuff everything up.
 */
void ProfileGroupResetter::nominateProfileVal( ProfileVal * pVal )
{
	nominee_ = pVal;
}

/**
 * Records a ProfileGroup which should be reset when the nominated
 * ProfileVal is. Don't call this when profiles are being reset.
 */
void ProfileGroupResetter::addProfileGroup( ProfileGroup * pGroup )
{
	if (doingReset_) return;

	groups_.push_back( pGroup );
}

/**
 * Static function to get the singleton instance.
 */
ProfileGroupResetter & ProfileGroupResetter::instance()
{
	static ProfileGroupResetter staticInstance;
	return staticInstance;
}

/**
 * Function called by a ProfileVal when it is reset to see if
 * ProfileGroupResetter should do its stuff.
 */
void ProfileGroupResetter::resetIfDesired( ProfileVal & val )
{
	if (doingReset_) return;
	if (nominee_ != &val) return;

	doingReset_ = true;

	std::vector<ProfileGroup *>::iterator iter;

	for (iter = groups_.begin(); iter != groups_.end(); iter++)
	{
		(*iter)->reset();
	}
	doingReset_ = false;
}



#endif // ENABLE_WATCHERS



// Commented in header file.
std::ostream& operator<<(std::ostream &o, const NiceTime &nt)
{
	// Overflows after 12 hours if we don't do this with doubles.
	static double microsecondsPerStamp = 1000000.0 / stampsPerSecondD();

	double microDouble = ((double)(int64)nt.t_) * microsecondsPerStamp;
	uint64 micros = (uint64)microDouble;

	double truncatedStampDouble = (double)(int64)
		( nt.t_ - ((uint64)(((double)(int64)micros)/microsecondsPerStamp)));
	uint32 picos = (uint32)( truncatedStampDouble * 1000000.0 *
		microsecondsPerStamp );

	// phew! Logic of lines above is: turn micros value we have back into
	// stamps, subtract it from the total number of stamps, and turn the
	// result into seconds just like before (except in picoseconds this time).
	// Simpler schemes seem to suffer from floating-point underflow.

	// Now see if micros was rounded and if so square it (hehe :)
	// It bothers me that I have to check for this...
//	uint32 origPicos = picos;
	if (picos > 1000000)
	{
		uint32 negPicos = 1 + ~picos;
		if (negPicos > 1000000)
		{
			picos = 0;	// sanity check
		}
		else
		{
			picos = negPicos;
			micros--;
		}
	}
	// Well, it all doesn't work anyway. At high values (over 10s say) we
	// still get wild imprecision in the calculation of picos. We shouldn't
	// I'll just leave it here for the moment 'tho. TODO: Fix this.

	if (micros == 0 && picos == 0)
	{
		o << "0us";
		return o;
	}

	if (micros>=1000000)
	{
		uint32 s = uint32(micros/1000000);

		if (s>=60)
		{
			o << (s/60) << "min ";
		}
		o << (s%60) << "sec ";
	}

	//o.width( 3 );		o.fill( '0' );		o << std::ios::right;

	//o.form( "%03d", (micros/1000)%1000 ) << " ";
	//o.form( "%03d", micros%1000 ) << ".";
	//o.form( "%03d", (picos/1000)%1000 ) << "us";

	// OKok, I give up on streams!

	char	lastBit[32];
	sprintf( lastBit, "%03d %03d",
		(int)(uint32)((micros/1000)%1000),
		(int)(uint32)(micros%1000) );
	o << lastBit;

	//if (micros < 60000000)
	{		// Only display this precision if <1min. Just silly otherwise.
		sprintf(lastBit, ".%03d",
			(int)((picos/1000)%1000) );
		o << lastBit;
	}

	o << "us";

	return o;
}




// profile.cpp
