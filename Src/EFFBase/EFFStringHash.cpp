#include "EFFBasePCH.h"
#include "EFFStringHash.h"

EFFBASE_BEGIN


effUINT CalculateFNV(const effTCHAR * str)
{
#if defined UNICODE || defined _UNICODE
	const effUINT length = wcslen(str) + 1;
#else
	const effUINT length = strlen(str) + 1;
#endif

	effUINT hash = 2166136261u;
	for ( effUINT i = 0; i < length; ++i )
	{

		effUINT ch = *str++;

		hash ^= ch;
		hash *= 16777619u;
	}
 
	return hash;
}

EFFBASE_END