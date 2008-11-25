/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef TIME_QUEUE_HEADER
#define TIME_QUEUE_HEADER

#include <map>
#include <queue>
#include "stdmf.hpp"

/**
 *	This type is a handle to a timer event.
 */
typedef uintptr TimeQueueId;


/**
 *	This is an interface which must be derived from in order to
 *	receive time queue events.
 */
class TimeQueueHandler
{
public:
	virtual ~TimeQueueHandler() {};

	/**
	 * 	This method is called when a timeout expires.
	 *
	 * 	@param	id		The id returned when the event was added.
	 * 	@param	pUser	The user data passed in when the event was added.
	 */
	virtual void handleTimeout( TimeQueueId id, void * pUser ) = 0;
	virtual void onRelease( TimeQueueId id, void * pUser ) = 0;
};


/**
 * 	This class implements a time queue, measured in game ticks. The logic is
 * 	basically stolen from Mercury, but it is intended to be used as a low
 * 	resolution timer.  Also, timestamps should be synchronised between servers.
 */
template< class TIME_STAMP >
class TimeQueueT
{
public:
	TimeQueueT();
	~TimeQueueT();

	void clear();

	/// This is the unit of time used by the time queue
	typedef TIME_STAMP TimeStamp;

	/// Schedule an event
	TimeQueueId	add( TimeStamp startTime, TimeStamp interval,
						TimeQueueHandler* pHandler, void * pUser );

	/// Cancel the event with the given id
	void		cancel( TimeQueueId id );

	/// Process all events older than or equal to now
	void		process( TimeStamp now );

	/// Determine whether or not the given id is legal (slow)
	bool		legal( TimeQueueId id ) const;

	/// Return the number of timestamps until the first node expires.  This will
	/// return 0 if size() == 0, so you must check this first
	TIME_STAMP nextExp( TimeStamp now ) const;

	/// Returns the number of timers in the queue
	inline uint32 size() const { return timeQueue_.size(); }

	bool		getTimerInfo( TimeQueueId id,
					TimeStamp &			time,
					TimeStamp &			interval,
					TimeQueueHandler *&	pHandler,
					void * &			pUser ) const;

	/// This enumeration is used to describe the current state of an element on
	/// the queue.
	enum State
	{
		STATE_PENDING,
		STATE_EXECUTING,
		STATE_CANCELLED
	};

private:

	/// This structure represents one event in the time queue.
	class Node
	{
	public:
		Node( TimeStamp startTime, TimeStamp interval,
			TimeQueueHandler * pHandler, void * pUser );
		~Node();

		void cancel( TimeQueueId id );

		TimeStamp			time;
		TimeStamp			interval;
		State				state;
		TimeQueueHandler *	pHandler;
		void *				pUser;

	private:
		Node( const Node & );
		Node & operator=( const Node & );
	};

	/// Comparison object for the priority queue.
	class Comparator
	{
	public:
		bool operator()(const Node* a, const Node* b)
		{
			return a->time > b->time;
		}
	};

	typedef std::priority_queue< Node*, std::vector<Node*>, Comparator >
		QueueType;
	QueueType	timeQueue_;
	Node * 		pProcessingNode_;
	TimeStamp 	lastProcessTime_;

	TimeQueueT(const TimeQueueT&);
	TimeQueueT& operator=(const TimeQueueT&);

	void checkTimeSanity( TimeStamp now );
};

typedef TimeQueueT< uint32 > TimeQueue;
typedef TimeQueueT< uint64 > TimeQueue64;

#include "time_queue.ipp"

#endif // TIME_QUEUE_HEADER
