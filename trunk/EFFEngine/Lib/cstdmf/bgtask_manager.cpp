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

#include "memory_counter.hpp"
#include "bgtask_manager.hpp"
#include "chunk/chunk_manager.hpp"
#include "chunk/chunk_loader.hpp"

DECLARE_DEBUG_COMPONENT2( "CStdMF", 0 )

/**
 *	BackgroundTask constructor
 */
BackgroundTask::BackgroundTask() :
	taskCompleted_( false ),
	id_( 0 )
{
	func_ = NULL;
	arg_ = NULL;
	callback_ = NULL;
	cbarg_ = NULL;
}

BackgroundTask::BackgroundTask( void (*func)(void *),
			void * arg, void (*callback)(void *), void * cbarg ) :
	taskCompleted_( false ),
	id_( 0 )
{
	func_ = func;
	callback_ = callback;
	arg_ = arg;
	cbarg_ = cbarg;
}

/**
 *	BackgroundTask destructor
 */
BackgroundTask::~BackgroundTask()
{
}

void BackgroundTask::notifyTaskCaller()
{
	if (callback_)
	{
		(*callback_)( cbarg_ );
	}
}

void BackgroundTask::setTaskCompletion( void * pTask )
{
	BackgroundTask * pl = (BackgroundTask *) pTask;
	pl->taskCompleted_ = true;
}

//=========================================================================
/**
 * BackgroundTaskThread
 */
BackgroundTaskThread::BackgroundTaskThread( int id ) :
	thread_( NULL ),
	id_( id )
{
}

BackgroundTaskThread::~BackgroundTaskThread()
{
	this->stop();
}

void BackgroundTaskThread::start()
{ 
	thread_ = new SimpleThread( BackgroundTaskThread::s_start, this );
}

void BackgroundTaskThread::stop()
{
	if (thread_)
	{
		BackgroundTask pTask;
		this->addPayload( &pTask );
		delete thread_;
		thread_ = NULL;
	}
}

void BackgroundTaskThread::s_start( void * arg )
{
	TRACE_MSG( "BackgroundTaskThread: started\n" );
	BackgroundTaskThread * pTaskThread = (BackgroundTaskThread*)arg;
	pTaskThread->run();
}

void BackgroundTaskThread::addPayload( BackgroundTask * payload )
{
	// DEBUG_MSG( "BackgroundTaskThread: adding payload task %d func %x\n",
	//	payload->id(), payload->func_ );
	mutex_.grab();
	pTaskList_.push_back( payload );
	mutex_.give();

	semaphore_.push();
}
void BackgroundTaskThread::run()
{
	while (true)
	{
		semaphore_.pull();
		mutex_.grab();
		BackgroundTask * pl = pTaskList_.front();
		pTaskList_.pop_front();
		mutex_.give();

		if (pl->func_ == NULL)
			break;
		//finally we execute the function
		(*pl->func_)( pl->arg_ );
		pl->taskCompleted_ = true;
	}
}

//==================================================================
/**
 * BgTaskManager
 */
BgTaskManager * BgTaskManager::bgtMgr_ = NULL;

BgTaskManager * BgTaskManager::instance()
{
	if (bgtMgr_ == NULL)
	{
		bgtMgr_ = new BgTaskManager();
	}
	return bgtMgr_;
}

BgTaskManager::BgTaskManager() :
	threadPoolSize_( 5 ),
	autoClean_( true ),
	leastLoadedThread_( 1 ),
	leastThreadLoad_( 1 ), // bootstrap new thread
	reducePoolSize_( 0 ),
	useChunkLoadingThread_( false )
{
}

int BgTaskManager::topTaskID_ = 0;

BgTaskManager::~BgTaskManager()
{
}

void BgTaskManager::init( int threadPoolSize, bool autoClean )
{
	this->useLoadingThread( threadPoolSize == USE_LOADING_THREAD );
	this->autoClean( autoClean );
	if ( !useChunkLoadingThread_ )
	{
		// set thread pool size if useChunkLoadingThread_ is false
		this->threadPoolSize( threadPoolSize );
	}
}

void BgTaskManager::fini()
{
	stopAll();
	bgTaskList_.clear();
}

void BgTaskManager::stopAll()
{
	while (!bgThreadList_.empty())
	{
		BackgroundTaskThread * pTaskThread
			= bgThreadList_.back();
		pTaskThread->stop();
		bgThreadList_.pop_back();
		delete pTaskThread;
	}
}

bool BgTaskManager::addTask( BackgroundTask & backgroundTask )
{
	BgTaskList::iterator found = bgTaskList_.find( &backgroundTask );
	if (found == bgTaskList_.end())
	{
		updateTaskID( backgroundTask );

		if (useChunkLoadingThread_)
		{
			// use existing chunk loading thread
			ChunkLoader::LoadOrder lo;
			lo.func_ = backgroundTask.func_;
			lo.arg_ = backgroundTask.arg_;
			ChunkManager::instance().chunkLoader()->load( lo );
			lo.func_ = &BackgroundTask::setTaskCompletion;
			lo.arg_ = &backgroundTask;
			ChunkManager::instance().chunkLoader()->load( lo );
			bgTaskList_.insert( std::make_pair( &backgroundTask, (BackgroundTaskThread *)NULL ) );
		}
		else
		{
			int curThreadListSize = bgThreadList_.size();

			// check there is no idle thread and our thread pool is
			// not full yet.
			BackgroundTaskThread * pTaskThread = NULL;
			if (leastThreadLoad_ != 0 && 
				curThreadListSize < threadPoolSize_ )
			{
				pTaskThread = 
					new BackgroundTaskThread( ++curThreadListSize );

				if (pTaskThread == NULL)
				{
					ERROR_MSG( "BgTaskManager::addTask: Unable to add "
								"new thread for background task %d\n",
								backgroundTask.id() );
					return false;
				}
				bgThreadList_.push_back( pTaskThread );
				if ( leastThreadLoad_ > 1 )
				{
					leastThreadLoad_ = 1;
					leastLoadedThread_ = curThreadListSize;
				}
				pTaskThread->start();
			}
			else if (reducePoolSize_ > 0)
			{
				// we need to select a thread that does not have
				// least load a silly algorithm [check]
				int pickThread = (leastThreadLoad_ + reducePoolSize_ - 1)
							% threadPoolSize_;
				if (pickThread == 0)
					pickThread = threadPoolSize_ - 1;

				pTaskThread = bgThreadList_[pickThread];
			}
			else // select the least loaded thread
			{
				pTaskThread = bgThreadList_[leastLoadedThread_ - 1];
				leastThreadLoad_++;
			}
			DEBUG_MSG( "Adding new task %d to thread %d\n",
				backgroundTask.id(), pTaskThread->id() );

			bgTaskList_.insert( std::make_pair( &backgroundTask, pTaskThread ) );

			// add our task to the thread
			pTaskThread->addPayload( &backgroundTask );
		}
	}
	else
	{
		INFO_MSG( "BgTaskManager::addTask: Found existing thread "
					"for background task %d\n",
					backgroundTask.id() );
	}
	return true;
}

void BgTaskManager::threadPoolSize( int poolSize )
{
	if (useChunkLoadingThread_)
	{
		ERROR_MSG( "BgTaskManager::threadPoolSize: using default "
			"loading thread. Cannot change thread pool size.\n" );
		return;
	}

	if (poolSize < 1 )
	{
		ERROR_MSG( "BgTaskManager::threadPoolSize: thread "
			"pool size must be greater than one.\n" );
		return;
	}
	if (poolSize < threadPoolSize_)
	{
		// check against current pool size since we may
		// not have all threads spawned.
		int curThreadPoolSize = bgThreadList_.size();

		if (poolSize < curThreadPoolSize)
		{
			// we need to cleanup some threads when they
			// are idle.
			reducePoolSize_ = curThreadPoolSize - poolSize;
		}
	}
	threadPoolSize_ = poolSize;
}

void BgTaskManager::tick()
{
	BgTaskList::iterator tit = bgTaskList_.begin();

	while (tit != bgTaskList_.end())
	{
		BackgroundTask * pTask = tit->first;
		if (pTask->isTaskCompleted())
		{

			//DEBUG_MSG( "Removing task %d from thread %d\n",
			//	pTask->id(), pThread->id() );

			pTask->notifyTaskCaller();

			if (!useChunkLoadingThread_)
			{
				BackgroundTaskThread * pThread = tit->second;
				//check thread load
				int curThreadLoad = pThread->currentLoad();
				if (leastThreadLoad_ > curThreadLoad )
				{
					leastThreadLoad_ = curThreadLoad;
					leastLoadedThread_ = pThread->id();
				}
				if ((autoClean_ || reducePoolSize_) &&
						curThreadLoad == 0)
				{
					// make sure if we are the least loaded
					// thread and we are being deleted, we will 
					// start adding new thread if new task
					// comes in.
					//DEBUG_MSG( "Stopping thread %d.\n", pThread->id() );

					pThread->stop();
					if (leastLoadedThread_ == pThread->id())
					{
						leastThreadLoad_ = 1; 
					}
					BgThreadList::iterator it = bgThreadList_.begin();
					it += (pThread->id() - 1);
					bgThreadList_.erase( it );
					delete pThread;

					// if we need to reduce the thread pool, we will
					// need to refind the least loaded thread.

					if (reducePoolSize_ > 0)
					{
						reducePoolSize_--;
						findLeastLoadedThread();
					}
				}
			}
			// dodge way of remove the task from the list
			BgTaskList::iterator curit = tit++;

			if (tit == bgTaskList_.end())
			{
				bgTaskList_.erase( curit );
				break;
			}
			else
			{
				BackgroundTask * nextTask = tit->first;
				bgTaskList_.erase( curit );
				tit = bgTaskList_.find( nextTask );
			}
		}
		else
		{
			tit++;
		}
	}
}

void BgTaskManager::findLeastLoadedThread()
{
	for (BgThreadList::const_iterator it = bgThreadList_.begin();
		 it != bgThreadList_.end(); it++)
	{
		int curThreadLoad = (*it)->currentLoad();
		if (leastThreadLoad_ > curThreadLoad)
		{
			leastThreadLoad_ = curThreadLoad;
			leastLoadedThread_ = (*it)->id();
		}
	}
}
