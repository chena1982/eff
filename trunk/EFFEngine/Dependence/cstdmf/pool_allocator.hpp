/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef POOLED_OBJECT_HPP
#define POOLED_OBJECT_HPP

#include <vector>
#include "concurrency.hpp"


/**
 *  A PoolAllocator is an object that never truly frees instances once
 *  allocated.  Therefore it should be used for small, frequently
 *  constructed/destroyed objects that will maintain a roughly constant active
 *  population over time.
 *
 *  Mercury::Packet is a good example of an object that fits this description.
 */
template <class MUTEX = DummyMutex>
class PoolAllocator
{
public:
	/**
	 *  Initialise a new empty Pool.
	 */
	PoolAllocator() : pHead_( NULL ), totalInstances_( 0 ) {}

	/**
	 *  This method frees all memory used in this pool.  This is just for
	 *  completeness, as typically this should only be called on exit.
	 */
	~PoolAllocator()
	{
		while (pHead_)
		{
			void * pNext = *pHead_;
			delete [] (char*)pHead_;
			pHead_ = (void**)pNext;
		}
	}

	/**
	 *  This method returns the total number of instances spawned.
	 */
	int totalInstances() const { return totalInstances_; }


	/**
	 *  This method returns a pointer to an available PooledObject instance,
	 *  allocating memory for a new one if necessary.
	 */
	void * allocate( size_t size )
	{
		void * ret;

		mutex_.grab();
		{
			// Grab an instance from the pool if there's one available.
			if (pHead_)
			{
				ret = (void*)pHead_;
				pHead_ = (void**)*pHead_;
			}

			// Otherwise just allocate new memory and return that instead.
			else
			{
				ret = (void*)new char[ size ];
				++totalInstances_;
			}
		}
		mutex_.give();

		return ret;
	}

	/**
	 *  This method returns a deleted instance of PooledObject to the pool.
	 */
	void deallocate( void * pInstance )
	{
		mutex_.grab();
		{
			void ** pNewHead = (void**)pInstance;
			*pNewHead = pHead_;
			pHead_ = pNewHead;
		}
		mutex_.give();
	}

private:
	/// The linked-list of memory chunks in the pool.
	void ** pHead_;

	/// The total number of instances ever spawned.
	int	totalInstances_;

	/// A lock to guarantee thread-safety.
	MUTEX mutex_;
};


#endif // POOLED_OBJECT_HPP
