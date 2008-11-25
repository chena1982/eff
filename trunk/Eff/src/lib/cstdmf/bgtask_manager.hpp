/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef BGTASK_MANAGER_HPP
#define BGTASK_MANAGER_HPP

#include <vector>
#include <queue>
#include <map>

#include "cstdmf/concurrency.hpp"
#include "cstdmf/debug.hpp"

/**
 *	This class encapsulate a task that can be submitted to 
 *	the background task manager for processing. The task 
 *	function and callback function must be static methods.
 */
class BackgroundTask
{
public:
	BackgroundTask();
	BackgroundTask( void (*func)(void *), void * arg,
		void (*callback)(void *) = NULL, void * cbarg = NULL );

	~BackgroundTask();

	bool isTaskCompleted() { return taskCompleted_; }
	int id() { return id_; }
	void notifyTaskCaller();

private:
	void (*func_)( void * );
	void * arg_;
	void (*callback_)( void * );
	void * cbarg_;
	void id( int newID ) { id_ = newID; }

	int	id_;
	bool taskCompleted_;
	static void setTaskCompletion( void * pTask );

	friend class BackgroundTaskThread;
	friend class BgTaskManager;
};

/**
 *	This class encapsulates a working thread execute given
 *	tasks.
 */
class BackgroundTaskThread
{
public:
	typedef std::deque<BackgroundTask *> BgThreadTaskList;

	BackgroundTaskThread( int id );
	int currentLoad() { return pTaskList_.size(); }

	void BackgroundTaskThread::addPayload( BackgroundTask * payload );
	int id() { return id_; }

	void start();
	void stop();
	~BackgroundTaskThread();

private:
	static void s_start( void * arg );
	void run();

	int id_;
	BgThreadTaskList taskList_;
	SimpleThread * thread_;
	SimpleSemaphore	semaphore_;
	SimpleMutex	mutex_;

	BgThreadTaskList pTaskList_;
};

/**
 *	This class defines a background task manager that manages a pool
 *	of working threads. It allocates task submitted by caller to the
 *	least loaded thread and (could) notify the caller when the task
 *	is done. The threads can be either automatically remove when the
 *	allocated task is done or preserved till the task manager quits.
 *	The size of the thread pool can be dynamically modified. (Oh,
 *	better increase the size and not descrease the thread).
 */
class BgTaskManager
{
	typedef std::map<BackgroundTask *, BackgroundTaskThread *> BgTaskList;
	typedef std::vector<BackgroundTaskThread *> BgThreadList;

public:
	~BgTaskManager();

	static const int USE_LOADING_THREAD = -1;

	void tick();

	/**
	 *	Init method.
	 *
	 *	@param threadPoolSize USE_LOADING_THREAD to use the default chunk
	 *						  loading thread, or a positive value to create
	 *						  multiple working threads.
	 *	@param autoClean	  set to true so threads delete themselves when 
	 *						  when done. Ignored if USE_LOADING_THREAD is used
	 */
	void init(
		int threadPoolSize = 5,
		bool autoClean = true );

	void fini();
	void stopAll();

	bool addTask( BackgroundTask & backgroundTask );
	void threadPoolSize( int poolSize );
	int threadPoolSize() { threadPoolSize_; }
	void autoClean( bool autoClean ) { autoClean_ = autoClean; }

	static BgTaskManager * instance();

protected:
	BgTaskManager();
	void useLoadingThread( bool useLoadingThread )
		{ useChunkLoadingThread_ = useLoadingThread; }

private:
	bool useChunkLoadingThread_; // set this flag to use existing chunk loading thread
	bool autoClean_;
	int threadPoolSize_;
	int reducePoolSize_;
	int leastLoadedThread_;
	int leastThreadLoad_;

	static BgTaskManager * bgtMgr_;

	void updateTaskID( BackgroundTask & backgroundTask )
		{ backgroundTask.id( ++topTaskID_ ); }

	void findLeastLoadedThread();

	BgTaskList	bgTaskList_;
	BgThreadList bgThreadList_;
	static int topTaskID_;
};

#endif // BGTASK_MANAGER_HPP
