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
#include "dprintf.hpp"
#include "concurrency.hpp"
#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <syslog.h>
#endif

//DECLARE_DEBUG_COMPONENT2( "CStdMF", 0 );

bool g_shouldWriteToSyslog = false;
std::string g_syslogAppName;


//-------------------------------------------------------
//	Section: MainThreadTracker
//-------------------------------------------------------

/**
 *	This static thread-local variable is initialised to false, and set to true
 *	in the constructor of the static s_mainThreadTracker object below
 */
static THREADLOCAL( bool ) s_isCurrentMainThread_( false );


/**
 *	Constructor
 */
MainThreadTracker::MainThreadTracker()
{
	s_isCurrentMainThread_ = true;
}


/**
 *	Static method that returns true if the current thread is the main thread,
 *  false otherwise.
 *
 *	@returns      true if the current thread is the main thread, false if not
 */
/* static */ bool MainThreadTracker::isCurrentThreadMain()
{
	return s_isCurrentMainThread_;
}


// Instantiate it, so it initialises the flag to the main thread
static MainThreadTracker s_mainThreadTracker;


//-------------------------------------------------------
//	Section: debug funtions
//-------------------------------------------------------

/**
 *	This function is used to strip the path and return just the basename from a
 *	path string.
 */
const char * mf_debugMunge( const char * path, const char * module )
{
	static char	staticSpace[128];

	const char * pResult = path;

	const char * pSeparator;

	pSeparator = strrchr( pResult, '\\' );
	if (pSeparator != NULL)
	{
		pResult = pSeparator + 1;
	}

	pSeparator = strrchr( pResult, '/' );
	if (pSeparator != NULL)
	{
		pResult = pSeparator + 1;
	}

	strcpy(staticSpace,"debug/");

	if (module != NULL)
	{
		strcat( staticSpace, module );
		strcat( staticSpace, "/" );
	}

	strcat(staticSpace,pResult);
	return staticSpace;
}


/**
 *	This is a helper function used by the CRITICAL_MSG macro.
 */
void DebugMsgHelper::criticalMessage( const char * format, ... )
{
	va_list argPtr;
	va_start( argPtr, format );
	this->criticalMessageHelper( false, format, argPtr );
	va_end( argPtr );
}


/**
 *	This is a helper function used by the CRITICAL_MSG macro. If
 *	DebugFilter::hasDevelopmentAssertions() is true, this will cause a proper
 *	critical message otherwise, it'll behaviour similar to a normal error
 *	message.
 */
void DebugMsgHelper::devCriticalMessage( const char * format, ... )
{
	va_list argPtr;
	va_start( argPtr, format );
	this->criticalMessageHelper( true, format, argPtr );
	va_end( argPtr );
}


/**
 *	This is a helper function used by the CRITICAL_MSG macro.
 */
void DebugMsgHelper::criticalMessageHelper( bool isDevAssertion,
		const char * format, va_list argPtr )
{
	char buffer[ BUFSIZ * 2 ];

	vsprintf( buffer, format, argPtr );

#ifndef _WIN32
	// send to syslog if it's been initialised
	if (g_shouldWriteToSyslog)
	{
		syslog( LOG_CRIT, buffer );
	}
#endif

	// output it as a normal message
	this->message( "%s", buffer );
/*
	if (isDevAssertion && ! DebugFilter::instance().hasDevelopmentAssertions())
	{
		// dev assert and we don't have dev asserts enabled
		return;
	}

	// now do special critical message stuff
	if (DebugFilter::instance().getCriticalCallbacks().size() != 0)
	{
		DebugFilter::CriticalCallbacks::const_iterator it =
			DebugFilter::instance().getCriticalCallbacks().begin();
		DebugFilter::CriticalCallbacks::const_iterator end =
			DebugFilter::instance().getCriticalCallbacks().end();

		for (; it!=end; ++it)
		{
			(*it)->handleCritical( buffer );
		}
	}
*/
#if defined(_WIN32)
	strcat( buffer, "\nDo you want to enter debugger?\nSelect no will exit the program.\n" );

	if( CriticalErrorHandler::get() )
	{
		switch( CriticalErrorHandler::get()->ask( buffer ) )
		{
		case CriticalErrorHandler::ENTERDEBUGGER:
			CriticalErrorHandler::get()->recordInfo( false );
			ENTER_DEBUGGER();
			break;
		case CriticalErrorHandler::EXITDIRECTLY:
			CriticalErrorHandler::get()->recordInfo( true );
			exit( 1 );
			break;
		}
	}
	else
		exit( 1 );

#else

	char	filename[512],	hostname[256];
	if (gethostname( hostname, sizeof(hostname) ) != 0)
		hostname[0] = 0;

	char exeName[512];
	char * pExeName = "unknown";

	int len = readlink( "/proc/self/exe", exeName, sizeof(exeName) - 1 );
	if (len > 0)
	{
		exeName[ len ] = '\0';

		char * pTemp = strrchr( exeName, '/' );
		if (pTemp != NULL)
		{
			pExeName = pTemp + 1;
		}
	}

	sprintf( filename, "assert.%s.%s.%d.log", pExeName, hostname, getpid() );

	FILE * assertFile = fopen( filename, "a" );
	fprintf( assertFile, "%s", buffer );
	fclose( assertFile );

	*(int*)NULL = 0;
	typedef void(*BogusFunc)();
	((BogusFunc)NULL)();

#endif
}

namespace
{
	const char * const prefixes[] =
	{
		"TRACE: ",
		"DEBUG: ",
		"INFO: ",
		"NOTICE: ",
		"WARNING: ",
		"ERROR: ",
		"CRITICAL: ",
		"HACK: ",
		NULL	// Script
	};
}

/*static*/ bool DebugMsgHelper::showErrorDialogs_ = true;
/*static*/ SimpleMutex* DebugMsgHelper::mutex_ = NULL;

/*static*/ void DebugMsgHelper::shouldWriteToSyslog( bool state )
{
	g_shouldWriteToSyslog = state;
}

/**
 *	This method allow tools to have a common method to set whether to show error dialogs or not
 *  Do this in a thread-safe way.
 */
/*static*/ void DebugMsgHelper::showErrorDialogs( bool show )
{
	if (!mutex_) mutex_ = new SimpleMutex;
	mutex_->grab();
	showErrorDialogs_ = show;
	mutex_->give();
}

/**
 *	This method allow tools to have a common method to determine whether to show error dialogs
 */
/*static*/ bool DebugMsgHelper::showErrorDialogs()
{
	if (!mutex_) mutex_ = new SimpleMutex;
	mutex_->grab();
	bool showErrorDialogs = showErrorDialogs_;
	mutex_->give();
	return showErrorDialogs;
}

/**
 *	This function is a helper to the *_MSG macros.
 */
void DebugMsgHelper::message( const char * format, ... )
{
	bool handled = false;
/*
	// Break early if this message should be filtered out.
	if (!DebugFilter::shouldAccept( componentPriority_, messagePriority_ ))
	{
		return;
	}
*/
	va_list argPtr;
	va_start( argPtr, format );

	char buffer[ BUFSIZ * 2 ];
	vsprintf( buffer, format, argPtr );
    OutputDebugString( buffer );
#ifndef _WIN32
	// send to syslog if it's been initialised
	if ((g_shouldWriteToSyslog) &&
		( (messagePriority_ == MESSAGE_PRIORITY_ERROR) ||
	      (messagePriority_ == MESSAGE_PRIORITY_CRITICAL) ))
	{
		char buffer[ BUFSIZ * 2 ];
		vsprintf( buffer, format, argPtr );

		syslog( LOG_CRIT, buffer );
	}
#endif
/*
	DebugFilter::DebugCallbacks::const_iterator it =
		DebugFilter::instance().getMessageCallbacks().begin();
	DebugFilter::DebugCallbacks::const_iterator end =
		DebugFilter::instance().getMessageCallbacks().end();

	for (; it!=end; ++it)
	{
		if (!handled)
		{
			handled = (*it)->handleMessage(
				componentPriority_, messagePriority_, format, argPtr );
		}
	}

	if (!handled)
	{
		if (0 <= messagePriority_ &&
			messagePriority_ < int(sizeof(prefixes) / sizeof(prefixes[0])) &&
			prefixes[messagePriority_] != NULL)
		{
			vdprintf( componentPriority_, messagePriority_,
					format, argPtr,
					prefixes[messagePriority_] );
		}
		else
		{
			vdprintf( componentPriority_, messagePriority_,
				format, argPtr );
		}
	}
*/
	va_end( argPtr );
}


// -----------------------------------------------------------------------------
// Section: Back end hijacking
// -----------------------------------------------------------------------------

// TODO: This is not be the best file for this code. It should really be in a
// stdmf.cpp.??

int g_backEndPID = 0;
int g_backEndUID = 0;
bool g_isBackEndProcess = false;

/*
 *	This function is used to set alternative UserID and ProcessID for this
 *	process.
 */
void initBackEndProcess( int uid, int pid )
{
	g_isBackEndProcess = true;
	g_backEndPID = pid;
	g_backEndUID = uid;
}


char __scratch[] = "DebugLibTestString Tue Nov 29 11:54:35 EST 2005";

#if defined(_WIN32) && !defined(_XBOX)

class DefaultCriticalErrorHandler : public CriticalErrorHandler
{
	virtual Result ask( const char* msg )
	{
		if( ::MessageBox( GetForegroundWindow(), msg, "Critical Error", MB_YESNO ) == IDYES )
			return ENTERDEBUGGER;

		return EXITDIRECTLY;
	}
	virtual void recordInfo( bool willExit )
	{}
}
DefaultCriticalErrorHandler;

CriticalErrorHandler* CriticalErrorHandler::handler_ = &DefaultCriticalErrorHandler;

#else//defined(_WIN32)

CriticalErrorHandler* CriticalErrorHandler::handler_;

#endif//defined(_WIN32)

CriticalErrorHandler* CriticalErrorHandler::get()
{
	return handler_;
}

CriticalErrorHandler* CriticalErrorHandler::set( CriticalErrorHandler* handler )
{
	CriticalErrorHandler* old = handler_;
	handler_ = handler;
	return old;
}

// debug.cpp
