/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include "cstdmf/stdmf.hpp"

// Indicates whether or not to use a call to RDTSC (Read Time Stamp Counter)
// to calculate timestamp. The benefit of using this is that it is fast and
// accurate, returning actual clock ticks. The downside is that this does not
// work well with CPUs that use Speedstep technology to vary their clock speeds.
#ifdef unix
// #define BW_USE_RDTSC
#else
#define BW_USE_RDTSC
#endif

/**
 * This function returns the processor's (real-time) clock cycle counter.
 */
#ifdef unix

enum BWTimingMethod
{
	RDTSC_TIMING_METHOD,
	GET_TIME_OF_DAY_TIMING_METHOD,
	NO_TIMING_METHOD,
};

extern BWTimingMethod g_timingMethod;

inline uint64 timestamp_rdtsc()
{
	uint64	ret;
	__asm__ __volatile__ ( 
		// Read Time-Stamp Counter
		// loads current value of processor's timestamp counter into EDX:EAX
		"rdtsc":
		"=A"	(ret) 	// output EDX:EAX->ret
	);
	return ret;
}

// Alternate Linux implementation uses gettimeofday. In rough tests, this can
// be between 20 and 600 times slower than using RDTSC. Also, there is a problem
// under 2.4 kernels where two consecutive calls to gettimeofday may actually
// return a result that goes backwards.
#include <sys/time.h>

inline uint64 timestamp_gettimeofday()
{
	timeval tv;
	gettimeofday( &tv, NULL );

	return 1000000ULL * uint64( tv.tv_sec ) + uint64( tv.tv_usec );
}

inline uint64 timestamp()
{
#ifdef BW_USE_RDTSC
	return timestamp_rdtsc();
#else
	if (g_timingMethod == RDTSC_TIMING_METHOD)
		return timestamp_rdtsc();
	else // if (g_timingMethod == GET_TIME_OF_DAY_TIMING_METHOD)
		return timestamp_gettimeofday();
#endif
}

#elif defined(_WIN32)

#ifdef BW_USE_RDTSC
#pragma warning (push)
#pragma warning (disable: 4035)
inline uint64 timestamp()
{
	__asm rdtsc
	// MSVC complains about no return value here.
	// According to the help, this warning is 'harmless', and
	//  they even have example code which does it. Go figure.
}
#pragma warning (pop)
#else

#include <windows.h>

inline uint64 timestamp()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter( &counter );
	return counter.QuadPart;
}

#endif // BW_USE_RDTSC

#elif defined(_WIN32)
uint64 __fastcall timestamp();


/*
inline uint64 timestamp()
{
	return timeGetTime();
}
*/
#else
#error Unsupported platform!
#endif

uint64 stampsPerSecond();
double stampsPerSecondD();

uint64 stampsPerSecond_rdtsc();
double stampsPerSecondD_rdtsc();

uint64 stampsPerSecond_gettimeofday();
double stampsPerSecondD_gettimeofday();


#endif // TIMESTAMP_H
