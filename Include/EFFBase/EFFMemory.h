
/******************************************************************************
	created:	2008-12-1   22:34
	file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFMemory.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFMemory_2008_12_1__
#define __EFFMemory_2008_12_1__





#ifndef _NOEFFNEW

	#ifdef _DEBUG

		/*struct AllocInfo
		{
		public:
			const char * file;
			unsigned int line;
			unsigned int id;
			AllocInfo * next;
			AllocInfo * previous;
		};

		struct MemoryInfo
		{
			AllocInfo * header;
			MemoryInfo();
			~MemoryInfo();
		};

		EFFBASE_API void * EFFMemoryNew(size_t size,const char * pszFileName,unsigned int dwLine);
		EFFBASE_API void EFFMemoryDelete(void * memory,const char * pszFileName,unsigned int dwLine);

		inline void * _cdecl operator new (size_t size,const char * pszFileName,unsigned int dwLine)
		{	
			return EFFMemoryNew(size,pszFileName,dwLine); 
		}


		inline void _cdecl operator delete (void * pMem,const char * pszFileName,unsigned int dwLine)
		{
			EFFMemoryDelete(pMem,pszFileName,dwLine);
		}

		inline void * _cdecl operator new [] (size_t size,const char * pszFileName,unsigned int dwLine)
		{
			return EFFMemoryNew(size,pszFileName,dwLine); 
		}

		inline void _cdecl operator delete [] (void * pMem,const char * pszFileName,unsigned int dwLine)
		{
			EFFMemoryDelete(pMem,pszFileName,dwLine);
		}

		inline void * _cdecl operator new (size_t size)
		{
			return EFFMemoryNew(size,0,0);
		}

		inline void _cdecl operator delete (void * pMem)
		{
			EFFMemoryDelete(pMem,0,0);
		}	

		inline void * _cdecl operator new [] (size_t size)
		{
			return EFFMemoryNew(size,0,0);
		}

		inline void _cdecl operator delete [] (void * pMem)
		{
			EFFMemoryDelete(pMem,0,0);
		}*/

		// EASTL需要定义这两个函数

		inline void* _cdecl operator new[](size_t size, const char* pName, int flags, unsigned int debugFlags, const char* file, int line)
		{
			return malloc(size);
		}

		inline void* _cdecl operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned int debugFlags, const char* file, int line)
		{
			return malloc(size);
		}

	#else

		EFFBASE_API void * EFFMemoryNew(size_t size);
		EFFBASE_API void EFFMemoryDelete(void * memory);

		void * _cdecl operator new (size_t size)
		{
			return EFFMemoryNew(size);
		}

		void _cdecl operator delete (void * pMem)
		{
			EFFMemoryDelete(pMem);
		}	

		void * _cdecl operator new [] (size_t size)
		{
			return EFFMemoryNew(size);
		}

		void _cdecl operator delete [] (void * pMem)
		{
			EFFMemoryDelete(pMem);
		}	

	#endif //end of _DEBUG



	#ifdef _DEBUG
		#define EFFNEW	new
	#else
		#define EFFNEW	new
	#endif
#else
	#define EFFNEW new
#endif //end of _NOEFFNEW


EFFBASE_BEGIN

class EFFBASE_API EFFLinearAllocator
{
public:
    explicit EFFLinearAllocator(effSIZE size);
    //EFFLinearAllocator(effVOID * start, effVOID * end);

    effVOID * Allocate(effSIZE size, effSIZE alignment, effSIZE offset);

    inline effVOID Free(effVOID * ptr) {}

    inline effVOID Reset(effVOID) { current = start; }

private:
    effBYTE * start;
    effBYTE * end;
    effBYTE * current;
};

EFFBASE_END


#endif