/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef MF_DEBUG_HPP
#define MF_DEBUG_HPP

/**
 *	@file debug.hpp
 *
 *	This file contains macros and functions related to debugging.
 *
 *	A number of macros are defined to display debug information. They should be
 *	used as you would use the printf function. To use these macros,
 *	DECLARE_DEBUG_COMPONENT needs to be used in the cpp file. Its argument is
 *	the initial component priority.
 *
 *	With each message, there is an associated message priority. A message is
 *	only displayed if its priority is not less than
 *	DebugFilter::filterThreshold() plus the component's priority.
 *
 *	@param TRACE_MSG	Used to display trace information. That is, when you enter a
 *					method.
 *	@param DEBUG_MSG	Used to display debug information such as what a variable is
 *					equal to.
 *	@param INFO_MSG	Used to display general information such as when a
 *					particular process is started.
 *	@param NOTICE_MSG	Used to display information that is more important than an
 *					INFO_MSG message but is not a possible error.
 *	@param WARNING_MSG	Used to display warning messages. These are messages
 *					that could be errors and should be looked into.
 *	@param ERROR_MSG	Used to display error messages. These are messages that are
 *					errors and need to be fixed. The software should hopefully
 *					be able to survive these situations.
 *	@param CRITICAL_MSG Used to display critical error messages. These are message
 *					that are critical and cause the program not to continue.
 *	@param HACK_MSG	Used to display temporary messages. This is the highest
 *					priority message that can be used to temporarily print a
 *					message. No code should be commited with this in it.
 *	@param SCRIPT_MSG	Used to display messages printed from Python script.
 *
 *	@param MF_ASSERT 	is a macro that should be used instead of assert or ASSERT.
 */

#include <assert.h>

#include "config.hpp"
#include "stdmf.hpp"
#include "dprintf.hpp"
//#include "watcher.hpp"

/**
 *	This enumeration is used to indicate the priority of a message. The higher
 *	the enumeration's value, the higher the priority.
 */
enum DebugMessagePriority
{
	MESSAGE_PRIORITY_TRACE,
	MESSAGE_PRIORITY_DEBUG,
	MESSAGE_PRIORITY_INFO,
	MESSAGE_PRIORITY_NOTICE,
	MESSAGE_PRIORITY_WARNING,
	MESSAGE_PRIORITY_ERROR,
	MESSAGE_PRIORITY_CRITICAL,
	MESSAGE_PRIORITY_HACK,
	MESSAGE_PRIORITY_SCRIPT,
	MESSAGE_PRIORITY_ASSET,
	NUM_MESSAGE_PRIORITY
};

inline const char * messagePrefix( DebugMessagePriority p )
{
	static char * prefixes[] =
	{
		"TRACE",
		"DEBUG",
		"INFO",
		"NOTICE",
		"WARNING",
		"ERROR",
		"CRITICAL",
		"HACK",
		"SCRIPT",
		"ASSET"
	};

	return (p >= 0 && (size_t)p < ARRAY_SIZE(prefixes)) ? prefixes[(int)p] : "";
}

class SimpleMutex;

class DebugMsgHelper
{
public:
	DebugMsgHelper( int componentPriority, int messagePriority ) :
		componentPriority_( componentPriority ),
		messagePriority_( messagePriority )
	{
	}
	DebugMsgHelper() :
		componentPriority_( 0 ),
		messagePriority_( MESSAGE_PRIORITY_CRITICAL )
	{
	}

	void message( const char * format, ... );
	void criticalMessage( const char * format, ... );
	void devCriticalMessage( const char * format, ... );

	static void shouldWriteToSyslog( bool state = true );

	static void showErrorDialogs( bool show );
	static bool showErrorDialogs();

private:
	void criticalMessageHelper( bool isDevAssertion, const char * format,
			va_list argPtr );

	int componentPriority_;
	int messagePriority_;

	static bool showErrorDialogs_;
	static SimpleMutex* mutex_;
};


/**
 *	This macro is a helper used by the *_MSG macros.
 */
// NOTE: Using comma operator to avoid scoping problem.
#ifndef _RELEASE
	#define DEBUG_MSG_WITH_PRIORITY( PRIORITY )								\
	DebugMsgHelper( ::s_componentPriority, PRIORITY ).message

	namespace
	{
		// This is the default s_componentPriority for files that does not
		// have //DECLARE_DEBUG_COMPONENT2(). Useful for hpp and ipp files that
		// uses debug macros. s_componentPriority declared by
		//	//DECLARE_DEBUG_COMPONENT2() will have precedence over this one.
		const int s_componentPriority = 0;
	}
#else
#define DEBUG_MSG_WITH_PRIORITY( PRIORITY )	debugMsgNULL
#endif

// The following macros are used to display debug information. See comment at
// the top of this file.

/// This macro prints a debug message with TRACE priority.
#define TRACE_MSG		DEBUG_MSG_WITH_PRIORITY( MESSAGE_PRIORITY_TRACE )

/// This macro prints a debug message with DEBUG priority.
#define DEBUG_MSG		DEBUG_MSG_WITH_PRIORITY( MESSAGE_PRIORITY_DEBUG )

/// This macro prints a debug message with INFO priority.
#define INFO_MSG		DEBUG_MSG_WITH_PRIORITY( MESSAGE_PRIORITY_INFO )

/// This macro prints a debug message with NOTICE priority.
#define NOTICE_MSG		DEBUG_MSG_WITH_PRIORITY( MESSAGE_PRIORITY_NOTICE )

/// This macro prints a debug message with WARNING priority.
#define WARNING_MSG															\
		DEBUG_MSG_WITH_PRIORITY( MESSAGE_PRIORITY_WARNING )

/// This macro prints a debug message with ERROR priority.
#define ERROR_MSG															\
		DEBUG_MSG_WITH_PRIORITY( MESSAGE_PRIORITY_ERROR )

/// This macro prints a debug message with CRITICAL priority.
#ifndef _RELEASE
#define CRITICAL_MSG														\
		DebugMsgHelper( ::s_componentPriority, MESSAGE_PRIORITY_CRITICAL ).	\
			criticalMessage
#else
#define CRITICAL_MSG	DebugMsgHelper().criticalMessage
#endif

/// This macro prints a debug message with HACK priority.
#define HACK_MSG		DEBUG_MSG_WITH_PRIORITY( MESSAGE_PRIORITY_HACK )

/// This macro prints a debug message with SCRIPT priority.
#define SCRIPT_MSG		DEBUG_MSG_WITH_PRIORITY( MESSAGE_PRIORITY_SCRIPT )

/**
 *	This macro used to display trace information. Can be used later to add in
 *	our own callstack if necessary.
 */
#define ENTER( className, methodName )									\
	TRACE_MSG( className "::" methodName "\n" )


/**
 *	This function eats the arguments of *_MSG macros when in Release mode
 */
inline void debugMsgNULL( const char * /*format*/, ... )
{
}



const char * mf_debugMunge( const char * path, const char * module = NULL );


#ifndef _RELEASE
#if ENABLE_WATCHERS
	/**
	*	This macro needs to be placed in a cpp file before any of the *_MSG macros
	*	can be used.
	*
	*	@param module	The name (or path) of the watcher module that the component
	*					priority should be displayed in.
	*	@param priority	The initial component priority of the messages in the file.
	*/
		#define DECLARE_DEBUG_COMPONENT2( module, priority )					\
			static int s_componentPriority = priority;							\
			static DataWatcher<int> * neverUseThisDataWatcherPointer =			\
				new DataWatcher<int>(	::s_componentPriority,					\
										Watcher::WT_READ_WRITE,					\
										mf_debugMunge( __FILE__, module ) );
#else
		#define DECLARE_DEBUG_COMPONENT2( module, priority )					\
			static int s_componentPriority = priority;
#endif
#else
#define DECLARE_DEBUG_COMPONENT2( module, priority )
#endif

/**
 *	This macro needs to be placed in a cpp file before any of the *_MSG macros
 *	can be used.
 *
 *	@param priority	The initial component priority of the messages in the file.
 */
#define DECLARE_DEBUG_COMPONENT( priority )									\
	//DECLARE_DEBUG_COMPONENT2( NULL, priority )




#include <assert.h>

#ifdef __ASSERT_FUNCTION
#	define MF_FUNCNAME __ASSERT_FUNCTION
#else
#		define MF_FUNCNAME ""
#endif

#ifdef MF_USE_ASSERTS
#	define MF_REAL_ASSERT assert(0);
#else
#	define MF_REAL_ASSERT
#endif

// The MF_ASSERT macro should used in place of the assert and ASSERT macros.
#if !defined( _RELEASE )
/**
 *	This macro should be used instead of assert.
 *
 *	@see MF_ASSERT_DEBUG
 */
#	define MF_ASSERT( exp )													\
		if (!(exp))															\
		{																	\
			DebugMsgHelper().criticalMessage(								\
				"ASSERTION FAILED: " #exp "\n"								\
					__FILE__ "(%d)%s%s\n", (int)__LINE__,					\
					*MF_FUNCNAME ? " in " : "",								\
					MF_FUNCNAME );											\
																			\
			MF_REAL_ASSERT													\
		}
#else	// _RELEASE

#	define MF_ASSERT( exp )
#endif // !_RELEASE


// The MF_ASSERT_DEBUG is like MF_ASSERT except it is only evaluated
// in debug builds.
#ifdef _DEBUG
#	define MF_ASSERT_DEBUG		MF_ASSERT
#else
/**
 *	This macro should be used instead of assert. It is enabled only
 *	in debug builds, unlike MF_ASSERT which is enabled in both
 *	debug and hybrid builds.
 *
 *	@see MF_ASSERT
 */
#	define MF_ASSERT_DEBUG( exp )
#endif


/**
 *	An assertion which is only lethal when not in a production environment.
 *
 *	@see MF_ASSERT
 */
#define MF_ASSERT_DEV( exp )												\
		if (!(exp))															\
		{																	\
			DebugMsgHelper().devCriticalMessage(							\
					"MF_ASSERT_DEV FAILED: " #exp "\n"						\
						__FILE__ "(%d)%s%s\n",								\
					(int)__LINE__,											\
					*MF_FUNCNAME ? " in " : "", MF_FUNCNAME );				\
		}

/**
 *	An assertion which is only lethal when not in a production environment.
 *	In a production environment, the block of code following the macro will
 *	be executed if the assertion fails.
 *
 *	@see MF_ASSERT_DEV
 */
#define IF_NOT_MF_ASSERT_DEV( exp )											\
		if ((!( exp )) && (													\
			DebugMsgHelper().devCriticalMessage(							\
				"MF_ASSERT_DEV FAILED: " #exp "\n"							\
					__FILE__ "(%d)%s%s\n", (int)__LINE__,					\
					*MF_FUNCNAME ? " in " : "", MF_FUNCNAME ),				\
			true))		// leave trailing block after message


/**
 *	This macro is used to assert a pre-condition.
 *
 *	@see MF_ASSERT
 *	@see POST
 */
#define PRE( exp )	MF_ASSERT( exp )

/**
 *	This macro is used to assert a post-condition.
 *
 *	@see MF_ASSERT
 *	@see PRE
 */
#define POST( exp )	MF_ASSERT( exp )

/**
 *	This macro is used to verify an expression. In non-release it
 *	asserts on failure, and in release the expression is still
 *	evaluated.
 *
 *	@see MF_ASSERT
 */
#ifdef _RELEASE
#define MF_VERIFY( exp ) (exp)
#define MF_VERIFY_DEV( exp ) (exp)
#else
#define MF_VERIFY MF_ASSERT
#define MF_VERIFY_DEV MF_ASSERT_DEV
#endif


/**
 *	This class is used to query if the current thread is the main thread.
 */
class MainThreadTracker
{
public:
	MainThreadTracker();

	static bool isCurrentThreadMain();
};


class CriticalErrorHandler
{
	static CriticalErrorHandler* handler_;
public:
	enum Result
	{
		ENTERDEBUGGER = 0,
		EXITDIRECTLY
	};
	static CriticalErrorHandler* get();
	static CriticalErrorHandler* set( CriticalErrorHandler* );
	virtual ~CriticalErrorHandler(){}
	virtual Result ask( const char* msg ) = 0;
	virtual void recordInfo( bool willExit ) = 0;
};

#endif // MF_DEBUG_HPP
