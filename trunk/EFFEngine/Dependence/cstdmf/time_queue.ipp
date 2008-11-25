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

#include "time_queue.hpp"
#include "cstdmf/debug.hpp"

// -----------------------------------------------------------------------------
// Section: TimeQueueT
// -----------------------------------------------------------------------------

/**
 *	This is the constructor.
 */
template< class TIME_STAMP >
TimeQueueT< TIME_STAMP >::TimeQueueT() :
	pProcessingNode_( NULL ),
	lastProcessTime_( 0 )
{
}

/**
 * 	This is the destructor. It walks the queue and cancels events,
 *	then deletes them. If the cancellation of events
 */
template <class TIME_STAMP>
TimeQueueT< TIME_STAMP >::~TimeQueueT()
{
	this->clear();
}


/**
 *	This method cancels all events in this queue.
 */
template <class TIME_STAMP>
void TimeQueueT< TIME_STAMP >::clear()
{
	typedef Node * const * NodeIter;

	// cancel everything
	int count = 0;
	while (!timeQueue_.empty())
	{
		uint oldSize = timeQueue_.size();
		NodeIter begin = &timeQueue_.top();
		NodeIter end = begin + oldSize;
		for (NodeIter it = begin; it != end; it++)
			(*it)->cancel( TimeQueueId( *it ) );

		if (oldSize == timeQueue_.size()) break;

		if (++count >= 16)
		{
			dprintf( "TimeQueue::~TimeQueue: "
				"Unable to cancel whole queue after 16 rounds!\n" );
			break;
		}
	}

	// and delete them
	if (!timeQueue_.empty())
	{
		NodeIter begin = &timeQueue_.top();
		NodeIter end = begin + timeQueue_.size();
		for (NodeIter it = begin; it != end; it++)
		{
			delete *it;
		}
	}

	// Clear the queue
	timeQueue_ = QueueType();
}


/**
 *	This method adds an event to the time queue. If interval is zero,
 *	the event will happen once and will then be deleted. Otherwise,
 *	the event will be fired repeatedly.
 *
 *	@param startTime	Time of the initial event, in game ticks
 *	@param interval		Number of game ticks between subsequent events
 *	@param pHandler 	Object that is to receive the event
 *	@param pUser		User data to be passed with the event.
 *	@return				Id of the new event.
 */
template <class TIME_STAMP>
TimeQueueId TimeQueueT< TIME_STAMP >::add( TimeStamp startTime, TimeStamp interval,
						TimeQueueHandler* pHandler, void * pUser )
{
	// We have to check this timing stuff here because we can't ever let things
	// be in the queue from two different system times, as then the 'use head as
	// base time' policy in checkTimeSanity() wouldn't work, as it assumes all
	// the times are at least correct relative to one other.
	this->checkTimeSanity( startTime );

	Node * pNode = new Node( startTime, interval, pHandler, pUser );
	timeQueue_.push( pNode );
	return (TimeQueueId)pNode;
}

/**
 *	This method cancels an existing event. It is safe to call it
 *	from within a time queue callback.
 *
 *	@param id		Id of the timer event
 */
template <class TIME_STAMP>
void TimeQueueT< TIME_STAMP >::cancel( TimeQueueId id )
{
	Node * pNode = (Node*)id;
	pNode->cancel( id );
}


/**
 *	This method processes the time queue and dispatches events.
 *	All events with a timestamp earlier than the given one are
 *	processed.
 *
 *	@param now		Process events earlier than or exactly on this.
 */
template <class TIME_STAMP>
void TimeQueueT< TIME_STAMP >::process( TimeStamp now )
{
	this->checkTimeSanity( now );

	while ((!timeQueue_.empty()) && (
		timeQueue_.top()->time <= now ||
		timeQueue_.top()->state == STATE_CANCELLED))
	{
		Node * pNode = pProcessingNode_ = timeQueue_.top();
		timeQueue_.pop();

		if (pNode->state != STATE_CANCELLED)
		{
			pNode->state = STATE_EXECUTING;
			pNode->pHandler->handleTimeout( (TimeQueueId)pNode, pNode->pUser );

			if (pNode->interval == 0)
				this->cancel( (TimeQueueId)pNode );
		}

		// This event could have been cancelled within the callback.
		// If so, delete it now.

		if (pNode->state == STATE_CANCELLED)
		{
			delete pNode;
		}
		else
		{
			// Otherwise put it back on the queue.  We don't simply increment
			// the time by the interval because that doesn't cope well with
			// changes to system time, and also because we don't think it's a
			// good thing to be running this callback again before at least the
			// interval has passed.
			pNode->time = now + pNode->interval;
			pNode->state = STATE_PENDING;
			timeQueue_.push( pNode );
		}
	}

	pProcessingNode_ = NULL;
	lastProcessTime_ = now;
}


/**
 *	This method determines whether or not the given id is legal.
 */
template <class TIME_STAMP>
bool TimeQueueT< TIME_STAMP >::legal( TimeQueueId id ) const
{
	typedef Node * const * NodeIter;

	Node * pNode = (Node*)id;

	if (pNode == NULL) return false;
	if (pNode == pProcessingNode_) return true;

	NodeIter begin = &timeQueue_.top();
	NodeIter end = begin + timeQueue_.size();
	for (NodeIter it = begin; it != end; it++)
		if (*it == pNode) return true;

	return false;
}

template <class TIME_STAMP>
TIME_STAMP TimeQueueT< TIME_STAMP >::nextExp( TimeStamp now ) const
{
	if (timeQueue_.size() == 0)
		return 0;
	else
		return timeQueue_.top()->time - now;
}


/**
 *	This method returns information associated with the timer with the input id.
 */
template <class TIME_STAMP>
bool TimeQueueT< TIME_STAMP >::getTimerInfo( TimeQueueId id,
					TimeStamp &			time,
					TimeStamp &			interval,
					TimeQueueHandler *&	pHandler,
					void * &			pUser ) const
{
	Node * pNode = (Node*)id;

	if ((pNode->state == STATE_PENDING) ||
		(pNode->state == STATE_EXECUTING))
	{
		time = pNode->time;
		interval = pNode->interval;
		pHandler = pNode->pHandler;
		pUser = pNode->pUser;

		return true;
	}

	return false;
}


/**
 *  This method drains the priority queue of nodes and rebuilds it using the
 *  former head of the queue as the basis for all time offsets if it is
 *  detected that time has gone backwards somehow.
 */
template <class TIME_STAMP>
void TimeQueueT< TIME_STAMP >::checkTimeSanity( TimeStamp now )
{
	if (lastProcessTime_ && now < lastProcessTime_ && !timeQueue_.empty())
	{
		// Drain existing queue
		std::vector< Node* > nodes;
		nodes.reserve( timeQueue_.size() );
		while (!timeQueue_.empty())
		{
			nodes.push_back( timeQueue_.top() );
			timeQueue_.pop();
		}

		// Use former head of queue as time basis and re-queue all nodes
		TimeStamp baseTime = nodes[0]->time;
		for (uint i=0; i < nodes.size(); i++)
		{
			nodes[i]->time = now + nodes[i]->time - baseTime;
			timeQueue_.push( nodes[i] );
		}
	}
}


// -----------------------------------------------------------------------------
// Section: TimeQueueT::Node
// -----------------------------------------------------------------------------

/**
 *	Constructor
 */
template <class TIME_STAMP>
TimeQueueT< TIME_STAMP >::Node::Node( TimeStamp _startTime, TimeStamp _interval,
		TimeQueueHandler * _pHandler, void * _pUser ) :
	time( _startTime ),
	interval( _interval ),
	state( STATE_PENDING ),
	pHandler( _pHandler ),
	pUser( _pUser )
{
}

/**
 *	Destructor
 */
template <class TIME_STAMP>
TimeQueueT< TIME_STAMP >::Node::~Node()
{
}


/**
 *	This method cancels the time queue node.
 */
template <class TIME_STAMP>
void TimeQueueT< TIME_STAMP >::Node::cancel( TimeQueueId id )
{
	state = STATE_CANCELLED;

	if (pHandler)
	{
		pHandler->onRelease( id, pUser );
		pHandler = NULL;
	}
}

// time_queue.ipp
