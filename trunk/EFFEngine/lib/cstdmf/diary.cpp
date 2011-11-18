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
#include "diary.hpp"

#ifndef CODE_INLINE
#include "diary.ipp"
#endif

#include "cstdmf/concurrency.hpp"
#include <algorithm>


static DiaryEntry s_dummyEntry( "dummy" );

// -----------------------------------------------------------------------------
// Section: Diary
// -----------------------------------------------------------------------------

/**
 *	Constructor.
 */
Diary::Diary() :
	pSaver_( NULL )
{
	SimpleMutexHolder smh( s_diariesMutex_ );
	s_diaries_.push_back( this );

	levelColours_[0] = 0;
}


/**
 *	Destructor.
 */
Diary::~Diary()
{
	SimpleMutexHolder smh( s_diariesMutex_ );
	std::vector<Diary*>::iterator it = std::find(
		s_diaries_.begin(), s_diaries_.end(), this );
	if (it != s_diaries_.end()) s_diaries_.erase( it );
}


/**
 *	Add an entry of the given desc
 */
DiaryEntry & Diary::add( const std::string & desc )
{
	if (!s_enabled_) return s_dummyEntry;

	uint64 ts = timestamp();
	DiaryEntry * pDE = new DiaryEntry( desc );

	pDE->start_ = ts;
	pDE->stop_ = 0;

	for (int i = int(levels_.size()) - 1; i >= 0; i--)
		if (levels_[i]->stop_ != 0) levels_.pop_back();

	pDE->level_ = levels_.size();
	pDE->colour_ = levelColours_[levels_.size()]++;

	levels_.push_back( pDE );
	levelColours_[levels_.size()] = 0;

	entriesMutex_.grab();
	entries_.push_back( pDE );
	if (entries_.size() > MAX_STORED_ENTRIES)
	{
		int nerase = (MAX_STORED_ENTRIES - MAX_READABLE_ENTRIES) / 2;
		if (pSaver_ != NULL)
			pSaver_->save( entries_.begin(), entries_.begin() + nerase );
		for (int i = 0; i < nerase; i++) delete entries_[i];
		entries_.erase( entries_.begin(), entries_.begin() + nerase );
	}
	entriesMutex_.give();

	return *pDE;
}


/**
 *	Static method to look at all the diaries at this point in time
 */
void Diary::look( std::vector<Diary*> & diaries )
{
	SimpleMutexHolder smh( s_diariesMutex_ );
	diaries = s_diaries_;
}

/**
 *	Method to look at all the entries in this diary
 */
void Diary::look( DiaryEntries & entries )
{
	SimpleMutexHolder smh( entriesMutex_ );
	int nentries = entries_.size();
	int nshown = std::min( int(MAX_READABLE_ENTRIES), nentries );
	entries.assign( entries_.end() - nshown, entries_.end() );
}



/// Our funky TLS global
#ifndef MF_SINGLE_THREADED
static THREADLOCAL(Diary*) s_pMyDiary(NULL);
#else
Diary * s_pMyDiary = NULL;
#endif

/**
 *	Static instance method
 */
Diary & Diary::instance()
{
	if (s_pMyDiary == NULL) s_pMyDiary = new Diary();
	return *s_pMyDiary;
}

/// Other statics
std::vector<Diary*>	Diary::s_diaries_;
SimpleMutex			Diary::s_diariesMutex_;
bool				Diary::s_enabled_ = false;

// diary.cpp
