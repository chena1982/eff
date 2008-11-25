#pragma once


#ifndef EFFMATH_EXPORTS
#ifndef _EFFMATHLIB
#define EFFMATH_API		__declspec(dllimport)
#else
#define EFFMATH_API
#endif
#else
#define EFFMATH_API		__declspec(dllexport)
#endif