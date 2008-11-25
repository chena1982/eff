
#pragma once


#undef new
#undef delete

#ifdef _DEBUG

	struct AllocInfo
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
	}

#else

	EFFBASE_API void * EFFMemoryNew(size_t size);
	EFFBASE_API void EFFMemoryDelete(void * memory);

	inline void * _cdecl operator new (size_t size)
	{
		return EFFMemoryNew(size);
	}

	inline void _cdecl operator delete (void * pMem)
	{
		EFFMemoryDelete(pMem);
	}	

	inline void * _cdecl operator new [] (size_t size)
	{
		return EFFMemoryNew(size);
	}

	inline void _cdecl operator delete [] (void * pMem)
	{
		EFFMemoryDelete(pMem);
	}	

#endif

#undef EFFNEW

#ifdef _DEBUG
	#define EFFNEW	new(__FILE__,__LINE__)
#else
	#define EFFNEW	new
#endif 
