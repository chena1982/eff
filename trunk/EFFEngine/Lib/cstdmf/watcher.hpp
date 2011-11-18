/******************************************************************************
BigWorld Technology
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

/**
 *	@file watcher.hpp
 *
 *	This file declares all Watcher classes.
 *
 *	@author Paul Murphy
 *	@author John Dalgliesh
 */

#ifndef WATCHER_HPP
#define WATCHER_HPP

#include "cstdmf/config.hpp"
#include "cstdmf/smartpointer.hpp"


class Watcher;
class DirectoryWatcher;
typedef SmartPointer<Watcher> WatcherPtr;
typedef SmartPointer<DirectoryWatcher> DirectoryWatcherPtr;

#if ENABLE_WATCHERS


#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "cstdmf/stdmf.hpp"

// #include "intrusive_object.hpp"

/**
 *	The character that separates values in a watcher path.
 *
 *	@ingroup WatcherModule
 */
const char WATCHER_SEPARATOR = '/';

class WatcherVisitor;

template <class VALUE_TYPE>
bool watcherStringToValue( const char * valueStr, VALUE_TYPE &value )
{
	std::stringstream stream;
	stream.write( valueStr, std::streamsize(strlen( valueStr )) );
	stream.seekg( 0, std::ios::beg );
	stream >> value;

	return !stream.bad();
}

inline
bool watcherStringToValue( const char * valueStr, const char *& value )
{
	return false;
}

inline
bool watcherStringToValue( const char * valueStr, std::string & value )
{
	value = valueStr;

	return true;
}

inline
bool watcherStringToValue( const char * valueStr, bool & value )
{
	if (bw_stricmp( valueStr, "true" ) == 0)
	{
		value = true;
	}
	else if (bw_stricmp( valueStr, "false" ) == 0)
	{
		value = false;
	}
	else
	{
		std::stringstream stream;
		stream.write( valueStr, std::streamsize( strlen( valueStr )));
		stream.seekg( 0, std::ios::beg );
		stream >> value;

		return !stream.bad();
	}

	return true;
}

template <class VALUE_TYPE>
std::string watcherValueToString( const VALUE_TYPE & value )
{
	std::stringstream stream;
	stream << value << std::ends;

	return stream.str();
}

inline
std::string watcherValueToString( const std::string & value )
{
	return value;
}

inline
std::string watcherValueToString( bool value )
{
	return value ? "true" : "false";
}


/**
 *	This class is the base class for all debug value watchers. It is part of the
 *	@ref WatcherModule.
 *
 *	@ingroup WatcherModule
 */
class Watcher : public ReferenceCount
{
public:
	/**
	 *	This enumeration is used to specify the type of the watcher.
	 */
	enum Type
	{
		WT_INVALID,			///< Indicates an error.
		WT_READ_ONLY,		///< Indicates the watched value cannot be changed.
		WT_READ_WRITE,		///< Indicates the watched value can be changed.
		WT_DIRECTORY		///< Indicates that the watcher has children.
	};

	/// @name Construction/Destruction
	//@{
	/// Constructor.
	Watcher();
	virtual ~Watcher();
	//@}

	/// @name Methods to be overridden
	//@{
	/**
	 *	This method is used to get a string representation of the value
	 *	associated with the path. The path is relative to this watcher.
	 *
	 *	@param base		This is a pointer used by a number of Watchers. It is
	 *					used as an offset into a struct or container.
	 *	@param path		The path string used to find the desired watcher. If
	 *					the path is the empty string, the value associated with
	 *					this watcher is used.
	 *	@param result	A reference to a string where the resulting string will
	 *					be placed.
	 *	@param desc		A reference to a string that contains description of
	 *					this watcher.
	 *	@param type		A pointer to a watcher type. If this is not NULL, it is
	 *					so to the type of the returned value.
	 *
	 *	@return	True if successful, otherwise false.
	 */
	virtual bool getAsString( const void * base,
		const char * path,
		std::string & result,
		std::string & desc,
		Type & type ) const = 0;

	/**
	 *	This method is used to get a string representation of the value
	 *	associated with the path. The path is relative to this watcher.
	 *
	 *	@param base		This is a pointer used by a number of Watchers. It is
	 *					used as an offset into a struct or container.
	 *	@param path		The path string used to find the desired watcher. If
	 *					the path is the empty string, the value associated with
	 *					this watcher is used.
	 *	@param result	A reference to a string where the resulting string will
	 *					be placed.
	 *	@param type		A pointer to a watcher type. If this is not NULL, it is
	 *					so to the type of the returned value.
	 *
	 *	@return	True if successful, otherwise false.
	 */

	virtual bool getAsString( const void * base, const char * path,
		std::string & result, Type & type ) const
	{
		std::string desc;
		return this->getAsString( base, path, result, desc, type );
	}

	/**
	 *	This method sets the value of the watcher associated with the input
	 *	path from a string. The path is relative to this watcher.
	 *
	 *	@param base		This is a pointer used by a number of Watchers. It is
	 *					used as an offset into a struct or container.
	 *	@param path		The path string used to find the desired watcher. If
	 *					the path is the empty string, the value associated with
	 *					this watcher is set.
	 *	@param valueStr	A string representing the value which will be used to
	 *					set to watcher's value.
	 *
	 *	@return	True if the new value was set, otherwise false.
	 */
	virtual bool setFromString( void * base,
		const char * path,
		const char * valueStr )	= 0;

	/**
	 *	This method <i>visits</i> each child of a directory watcher.
	 *
	 *	@param base		This is a pointer used by a number of Watchers. It is
	 *					used as an offset into a struct or container.
	 *	@param path		The path string used to find the desired watcher. If
	 *					the path is the empty string, the children of this
	 *					object are visited.
	 *	@param visitor	An object derived from WatcherVisitor whose visit method
	 *					will be called for each child.
	 *
	 *	@return True if the children were visited, false if specified watcher
	 *		could not be found or is not a directory watcher.
	 */
	virtual bool visitChildren( const void * base,
		const char * path,
		WatcherVisitor & visitor )
		{ return false; }


	/**
	 *	This method adds a watcher as a child to another watcher.
	 *
	 *	@param path		The path of the watcher to add the child to.
	 *	@param pChild	The watcher to add as a child.
	 *	@param withBase	This is a pointer used by a number of Watchers. It is
	 *					used as an offset into a struct or container.
	 *
	 *	@return True if successful, otherwise false. This method may fail if the
	 *			watcher could not be found or the watcher cannot have children.
	 */
	virtual bool addChild( const char * path, WatcherPtr pChild,
		void * withBase = NULL )
		{ return false; }

	virtual bool removeChild( const char * path )
		{	return false;	}

	void setComment( const std::string & comment = std::string( "" ) )
   				{ comment_ = comment; }

	const std::string & getComment() { return comment_; }
	//@}


	/// @name Static methods
	//@{
	static Watcher & rootWatcher();

	static void partitionPath( const std::string path,
			std::string & resultingName,
			std::string & resultingDirectory );

	//@}


	static void fini();

protected:
	/**
	 *	This method is a helper that returns whether or not the input path is
	 *	an empty path. An empty path is an empty string or a NULL string.
	 *
	 *	@param path	The path to test.
	 *
	 *	@return	True if the path is <i>empty</i>, otherwise false.
	 */
	static bool isEmptyPath( const char * path )
		{ return (path == NULL) || (*path == '\0'); }

	std::string comment_;

private:
	static WatcherPtr pRootWatcher_;
};


/**
 *	This interface is used to implement a visitor. A visitor is an object that
 *	is used to <i>visit</i> each child of a directory watcher. It is part of the
 *	@ref WatcherModule.
 *
 * 	@see Watcher::visitChildren
 *
 * 	@ingroup WatcherModule
 */
class WatcherVisitor
{
public:
	/// Destructor.
	virtual ~WatcherVisitor() {};


	/**
	 *	This method is called once for each child of a directory watcher when
	 *	the visitChild member is called. This function can return false
	 * 	to stop any further visits.
	 *
	 *	@see Watcher::visitChildren
	 */
	virtual bool visit( Watcher::Type type,
		const std::string & label,
		const std::string & desc,
		const std::string & valueStr ) = 0;

	virtual bool visit( Watcher::Type type,
		const std::string & label,
		const std::string & valueStr )
	{
		std::string desc;
		return this->visit( type, label, desc, valueStr );
	}
};


/**
 *	This class implements a Watcher that can contain other Watchers. It is used
 *	by the watcher module to implement the tree of watchers. To find a watcher
 *	associated with a given path, you traverse this tree in a way similar to
 *	traversing a directory structure.
 *
 *	@see WatcherModule
 *
 *	@ingroup WatcherModule
 */
class DirectoryWatcher : public Watcher
{
public:
	/// @name Construction/Destruction
	//@{
	DirectoryWatcher();
	virtual ~DirectoryWatcher();
	//@}

	/// @name Overrides from Watcher
	//@{
	virtual bool getAsString( const void * base, const char * path,
		std::string & result, std::string & desc, Type & type ) const;

	virtual bool setFromString( void * base, const char * path,
		const char * valueStr );

	virtual bool visitChildren( const void * base, const char * path,
		WatcherVisitor & visitor );

	virtual bool addChild( const char * path, WatcherPtr pChild,
		void * withBase = NULL );

	virtual bool removeChild( const char * path );
	//@}

	/// @name Helper methods
	//@{
	static const char * tail( const char * path );
	//@}

private:
	struct DirData
	{
		WatcherPtr		watcher;
		void *			base;
		std::string		label;
	};

	DirData * findChild( const char * path ) const;

	typedef std::vector<DirData> Container;
	Container container_;
};



/**
 *	This class is used for watching the contents of vectors and other sequences.
 *
 *	@see WatcherModule
 *	@ingroup WatcherModule
 */
template <class SEQ> class SequenceWatcher : public Watcher
{
public:
	/// The type in the sequence.
	typedef typename SEQ::value_type SEQ_value;
	/// The type of a reference to an element in the sequence
#if defined(__GNUC__) && __GNUC__ >= 3 && __GNUC_MINOR__ >= 4
	typedef typename SEQ::iterator::reference SEQ_reference;
#else	// gcc 3.4's set's reference != set's iterator's reference (!!)
	typedef typename SEQ::reference SEQ_reference;
#endif
	/// The type of the iterator to use.
	typedef typename SEQ::iterator SEQ_iterator;
	/// The type of the const iterator to use.
	typedef typename SEQ::const_iterator SEQ_constiterator;
public:
	/// @name Construction/Destruction
	//@{
	/// Constructor.
	SequenceWatcher( SEQ & toWatch = *(SEQ*)NULL,
			const char ** labels = NULL ) :
		toWatch_( toWatch ),
		labels_( labels ),
		labelsub_( NULL ),
		child_( NULL ),
		subBase_( 0 )
	{
	}
	//@}

	/// @name Overrides from Watcher.
	//@{
	// Override from Watcher.
	virtual bool getAsString( const void * base, const char * path,
		std::string & result, std::string & desc, Type & type ) const
	{
		if (isEmptyPath(path))
		{
			result = "<DIR>";
			type = WT_DIRECTORY;
			desc = comment_;
			return true;
		}

		try
		{
			SEQ_reference rChild = this->findChild( base, path );

			return child_->getAsString( (void*)(subBase_ + (uintptr)&rChild),
				this->tail( path ), result, desc, type );
		}
		catch (NoSuchChild &)
		{
			return false;
		}
	}

	// Override from Watcher
	virtual bool setFromString( void * base, const char * path,
		const char * valueStr )
	{
		try
		{
			SEQ_reference rChild = this->findChild( base, path );

			return child_->setFromString( (void*)(subBase_ + (uintptr)&rChild),
				this->tail( path ), valueStr );
		}
		catch (NoSuchChild &)
		{
			return false;
		}
	}

	// Override from Watcher
	virtual bool visitChildren( const void * base, const char * path,
		WatcherVisitor & visitor )
	{
		if (isEmptyPath(path))
		{
			SEQ	& useVector = *(SEQ*)(
				((uintptr)&toWatch_) + ((uintptr)base) );

			const char ** ppLabel = labels_;

			int count = 0;
			for( SEQ_iterator iter = useVector.begin();
				iter != useVector.end();
				iter++, count++ )
			{
				std::string	callLabel;
				std::string desc;

				SEQ_reference rChild = *iter;

				if ((ppLabel != NULL) && (*ppLabel != NULL))
				{
					callLabel.assign( *ppLabel );
					ppLabel++;
				}
				else if (labelsub_)
				{
					Type unused;
					child_->getAsString( (void*)(subBase_ + (uintptr)&rChild),
						labelsub_, callLabel, desc, unused );
				}

				if (callLabel.empty())
				{
					char temp[32];
					sprintf( temp, "%u", count );
					callLabel.assign( temp );
				}

				std::string callValue;

				Type	resType;
				child_->getAsString( (void*)(subBase_ + (uintptr)&rChild),
					this->tail( path ), callValue, desc, resType );

				if (!visitor.visit( resType, callLabel, desc, callValue ))
					break;
			}

			return true;
		}
		else
		{
			try
			{
				SEQ_reference rChild = this->findChild( base, path );

				return child_->visitChildren(
					(void*)(subBase_ + (uintptr)&rChild),
					this->tail( path ), visitor );
			}
			catch (NoSuchChild &)
			{
				return false;
			}
		}
	}

	// Override from Watcher
	virtual bool addChild( const char * path, WatcherPtr pChild,
		void * withBase = NULL )
	{
		if (isEmptyPath( path ))
		{
			return false;
		}
		else if (strchr(path,'/') == NULL)
		{
			// we're a one-child family here
			if (child_ != NULL) return false;

			// ok, add away
			child_ = pChild;
			subBase_ = (uintptr)withBase;
			return true;
		}
		else
		{
			// we don't/can't check that our component of 'path'
			// exists here either.
			return child_->addChild( this->tail ( path ),
				pChild, withBase );
		}
	}
	//@}


	/// @name Helper methods
	//@{
	/**
	 *	This method sets the labels that are associated with the elements of
	 *	sequence that is being watched.
	 */
	void setLabels( const char ** labels )
	{
		labels_ = labels;
	}

	/**
	 *	This method sets the label sub-path associated with this object.
	 *
	 *	@todo More comments
	 */
	void setLabelSubPath( const char * subpath )
	{
		labelsub_ = subpath;
	}
	//@}

	/**
	 *	@see DirectoryWatcher::tail
	 */
	static const char * tail( const char * path )
	{
		return DirectoryWatcher::tail( path );
	}

private:

	class NoSuchChild
	{
	public:
		NoSuchChild( const char * path ): path_( path ) {}

		const char * path_;
	};

	SEQ_reference findChild( const void * base, const char * path ) const
	{
		SEQ	& useVector = *(SEQ*)(
			((const uintptr)&toWatch_) + ((const uintptr)base) );

		char * pSep = strchr( (char*)path, WATCHER_SEPARATOR );
		std::string lookingFor = pSep ?
			std::string( path, pSep - path ) : std::string ( path );

		// see if it matches a string
		uint	cLabels = 0;
		while (labels_ && labels_[cLabels] != NULL) cLabels++;

		uint	lIter = 0;
		uint	ende = std::min( cLabels, useVector.size() );
		SEQ_iterator	sIter = useVector.begin();
		while (lIter < ende)
		{
			if (!strcmp( labels_[lIter], lookingFor.c_str() ))
				return *sIter;

			lIter++;
			sIter++;
		}

		// try the label sub path if we have one
		if (labelsub_ != NULL)
		{
			for (sIter = useVector.begin(); sIter != useVector.end(); sIter++)
			{
				std::string	tri;
				std::string	desc;

				Type unused;
				SEQ_reference ref = *sIter;
				child_->getAsString( (void*)(subBase_ + (uintptr)&ref),
					labelsub_, tri, desc, unused );

				if (tri == lookingFor) return *sIter;
			}
		}

		// now try it as a number
		lIter = atoi( lookingFor.c_str() );
		for (sIter = useVector.begin(); sIter != useVector.end() && lIter > 0;
			sIter++) lIter--; // scan

		if (sIter != useVector.end())
			return *sIter;

		// don't have it, give up then
		throw NoSuchChild( path );
	}

	SEQ &			toWatch_;
	const char **	labels_;
	const char *	labelsub_;
	WatcherPtr		child_;
	uintptr			subBase_;
};


/**
 *	This class is used for watching the contents of maps.
 *
 *	@see WatcherModule
 *	@ingroup WatcherModule
 */

template <class MAP> class MapWatcher : public Watcher
{
private:
	typedef typename MapTypes<MAP>::_Tref MAP_reference;
	typedef typename MAP::key_type MAP_key;
	typedef typename MAP::iterator MAP_iterator;
	typedef typename MAP::const_iterator MAP_constiterator;
public:
	/// @name Construction/Destruction
	//@{
	/**
	 *	Constructor.
	 *
	 *	@param toWatch	Specifies the map to watch.
	 */
	MapWatcher( MAP & toWatch = *(MAP*)NULL ) :
		toWatch_( toWatch ),
		child_( NULL ),
		subBase_( 0 )
	{
	}
	//@}

	/// @name Overrides from Watcher.
	//@{
	virtual bool getAsString( const void * base, const char * path,
		std::string & result, std::string & desc, Type & type ) const
	{
		if (isEmptyPath(path))
		{
			result = "<DIR>";
			type = WT_DIRECTORY;
			desc = comment_;
			return true;
		}

		try
		{
			MAP_reference rChild = this->findChild( base, path );

			return child_->getAsString( (void*)(subBase_ + (uintptr)&rChild),
				this->tail( path ), result, desc, type );
		}
		catch (NoSuchChild &)
		{
			return false;
		}
	}

	virtual bool setFromString( void * base, const char * path,
		const char * valueStr )
	{
		try
		{
			MAP_reference rChild = this->findChild( base, path );

			return child_->setFromString( (void*)(subBase_ + (uintptr)&rChild),
			this->tail( path ), valueStr );
		}
		catch (NoSuchChild &)
		{
			return false;
		}
	}

	virtual bool visitChildren( const void * base, const char * path,
		WatcherVisitor & visitor )
	{
		if (isEmptyPath(path))
		{
			MAP	& useMap = *(MAP*)( ((uintptr)&toWatch_) + ((uintptr)base) );

			int count = 0;
			MAP_iterator iter = useMap.begin();
			while(iter != useMap.end())
			{
				std::string callLabel( watcherValueToString( (*iter).first ) );

				std::string callValue;
				std::string desc;

				Type	resType;
				child_->getAsString(
					(void*)(subBase_ + (uintptr)&(*iter).second),
					this->tail( path ), callValue, desc, resType );

				if (!visitor.visit( resType, callLabel, desc, callValue ))
					break;

				iter++;
				count++;
			}

			return true;
		}
		else
		{
			try
			{
				MAP_reference rChild = this->findChild( base, path );

				return child_->visitChildren(
					(void*)(subBase_ + (uintptr)&rChild),
					this->tail( path ), visitor );
			}
			catch (NoSuchChild &)
			{
				return false;
			}
		}
	}


	virtual bool addChild( const char * path, WatcherPtr pChild,
		void * withBase = NULL )
	{
		if (isEmptyPath( path ))
		{
			return false;
		}
		else if (strchr(path,'/') == NULL)
		{
			// we're a one-child family here
			if (child_ != NULL) return false;

			// ok, add away
			child_ = pChild;
			subBase_ = (uintptr)withBase;
			return true;
		}
		else
		{
			// we don't/can't check that our component of 'path'
			// exists here either.
			return child_->addChild( this->tail ( path ),
				pChild, withBase );
		}
	}
	//@}

	/**
	 *	This function is a helper. It strips the path from the input string and
	 *	returns a string containing the tail value.
	 *
	 *	@see DirectoryWatcher::tail
	 */
	static const char * tail( const char * path )
	{
		return DirectoryWatcher::tail( path );
	}

private:

	class NoSuchChild
	{
	public:
		NoSuchChild( const char * path ): path_( path ) {}

		const char * path_;
	};

	MAP_reference findChild( const void * base, const char * path ) const
	{
		MAP	& useMap = *(MAP*)(
			((const uintptr)&toWatch_) + ((const uintptr)base) );

		// find out what we're looking for
		char * pSep = strchr( (char*)path, WATCHER_SEPARATOR );
		std::string lookingFor = pSep ?
			std::string( path, pSep - path ) : std::string ( path );

		/*
		// turn the string into the key type
		std::strstream stream;		stream << lookingFor;
		MAP_key key;				stream >> key;

		// see if that's in there anywhere
		MAP_iterator iter = useMap.find( key );
		if (iter != useMap.end())
		{
			return (*iter).second;
		}
		*/

		// Unfortunately, we can't actually use some map's finds,
		//  because it's impossible to turn a string back into a key
		//  (only works for simple types like strings and numbers...
		//   not, say, complex objects or pointers to these).
		// So we have to do a big slow search... sigh.

		MAP_iterator iter = useMap.begin();
		for (;iter != useMap.end();iter++)
		{
			if (lookingFor == watcherValueToString( (*iter).first ))
			{
				return (*iter).second;
			}
		}


		// now try it as a number
		uint	countTo = atoi( lookingFor.c_str() );
		for (iter = useMap.begin(); iter != useMap.end() && countTo > 0;
			iter++) countTo--; // scan

		if (iter != useMap.end())
			return (*iter).second;

		// don't have it, give up then
		throw NoSuchChild( path );
	}

	MAP &		toWatch_;
	WatcherPtr	child_;
	uintptr		subBase_;
};


/**
 *	This class is a helper watcher that wraps another Watcher. It consumes no
 *	path and is only responsible for dereferencing base pointer and passing it
 *	through to the wrapped Watcher (except for the one in addChild, of course).
 *
 *	@see WatcherModule
 *	@ingroup WatcherModule
 */
class BaseDereferenceWatcher : public Watcher
{
public:
	/// @name Construction/Destruction
	//@{
	/**
	 *	Constructor.
	 */
	BaseDereferenceWatcher( WatcherPtr watcher, void * withBase = NULL ) :
		watcher_( watcher ),
		sb_( (uintptr)withBase )
	{ }
	//@}

	/// @name Overrides from Watcher.
	//@{
	// Override from Watcher
	virtual bool getAsString( const void * base, const char * path,
		std::string & result, std::string & desc, Type & type ) const
	{ return (base == NULL || *(char**)base == NULL) ? false :
		watcher_->getAsString(
			(void*)(sb_ + *(uintptr*)base), path, result, desc, type ); }

	// Override from Watcher
	virtual bool setFromString( void * base, const char * path,
		const char * valueStr )
	{ return (base == NULL || *(char**)base == NULL) ? false :
		watcher_->setFromString(
			(void*)(sb_ + *(uintptr*)base), path, valueStr ); }

	// Override from Watcher
	virtual bool visitChildren( const void * base, const char * path,
		WatcherVisitor & visitor )
	{ return (base == NULL || *(char**)base == NULL) ? false :
		watcher_->visitChildren(
			(void*)(sb_ + *(uintptr*)base), path, visitor ); }

	// Override from Watcher
	virtual bool addChild( const char * path, WatcherPtr pChild,
		void * withBase = NULL )
	{ return watcher_->addChild( path, pChild, withBase ); }
	// this base is passed through unchanged, because it is interpreted
	// deeper in the hierarchy than we are. All the bases we dereference are
	// passed to us by an owning Watcher (e.g. VectorWatcher), or the user.
	//@}
private:
	WatcherPtr	watcher_;
	uintptr		sb_;
};



/**
 *	This templatised class is used to store debug values that are a member of a
 *	class.
 *
 *	@see WatcherModule
 *	@ingroup WatcherModule
 */
template <class RETURN_TYPE,
	class OBJECT_TYPE,
	class CONSTRUCTION_TYPE = RETURN_TYPE>
class MemberWatcher : public Watcher
{
	public:
		/// @name Construction/Destruction
		//@{
		/**
		 *	Constructor.
		 *
		 *	This constructor should only be called from the Debug::addValue
		 *	function.
		 */
		MemberWatcher( OBJECT_TYPE & rObject,
				RETURN_TYPE (OBJECT_TYPE::*getMethod)() const,
				void (OBJECT_TYPE::*setMethod)( RETURN_TYPE ) = NULL ) :
			rObject_( rObject ),
			getMethod_( getMethod ),
			setMethod_( setMethod )
		{}
		//@}

	protected:
		/// @name Overrides from Watcher.
		//@{
		virtual bool getAsString( const void * base, const char * path,
			std::string & result, std::string & desc, Type & type ) const
		{
			if (!isEmptyPath( path )) return false;

			if (getMethod_ == (GetMethodType)NULL)
			{
				result = "";
				type = WT_READ_ONLY;
				desc = comment_;
				return true;
			}

			const OBJECT_TYPE & useObject = *(OBJECT_TYPE*)(
				((const uintptr)&rObject_) + ((const uintptr)base) );

			RETURN_TYPE value = (useObject.*getMethod_)();

			result = watcherValueToString( value );
			desc = comment_;

			type = (setMethod_ != (SetMethodType)NULL) ?
				WT_READ_WRITE : WT_READ_ONLY;
			return true;
		};

		virtual bool setFromString( void * base, const char *path,
			const char * valueStr )
		{
			if (!isEmptyPath( path ) || (setMethod_ == (SetMethodType)NULL))
				return false;

			bool wasSet = false;

			CONSTRUCTION_TYPE value = CONSTRUCTION_TYPE();
			if (watcherStringToValue( valueStr, value ))
			{
				OBJECT_TYPE & useObject =
					*(OBJECT_TYPE*)( ((uintptr)&rObject_) + ((uintptr)base) );
				(useObject.*setMethod_)( value );
				wasSet = true;
			}

			return wasSet;
		}
		//@}

	private:

		typedef RETURN_TYPE (OBJECT_TYPE::*GetMethodType)() const;
		typedef void (OBJECT_TYPE::*SetMethodType)( RETURN_TYPE);

		OBJECT_TYPE & rObject_;
		RETURN_TYPE (OBJECT_TYPE::*getMethod_)() const;
		void (OBJECT_TYPE::*setMethod_)( RETURN_TYPE );
};


/**
 *	This function is used to create a read-only watcher using a get method.
 *
 *	@ingroup WatcherModule
 */
template <class RETURN_TYPE, class OBJECT_TYPE>
WatcherPtr makeWatcher( OBJECT_TYPE & rObject,
			const RETURN_TYPE & (OBJECT_TYPE::*getMethod)() const )
{
	return new MemberWatcher<const RETURN_TYPE &, OBJECT_TYPE, RETURN_TYPE>(
			rObject, getMethod, NULL );
}


/**
 *	This function is used to create a read/write watcher using a get and set
 *	methods.
 *
 *	@ingroup WatcherModule
 */
// The DUMMY_TYPE template argument is just used to get around a VC++ problem.
template <class RETURN_TYPE, class OBJECT_TYPE, class DUMMY_TYPE>
WatcherPtr makeWatcher( OBJECT_TYPE & rObject,
						const RETURN_TYPE & (OBJECT_TYPE::*getMethod)() const,
						void (OBJECT_TYPE::*setMethod)(DUMMY_TYPE) )
{
	return new MemberWatcher<const RETURN_TYPE &, OBJECT_TYPE, RETURN_TYPE>(
			rObject, getMethod, setMethod );
}


/**
 *	This function is used to create a read-only watcher using a get method.
 *
 *	@ingroup WatcherModule
 */
template <class RETURN_TYPE, class OBJECT_TYPE>
WatcherPtr makeWatcher(	OBJECT_TYPE & rObject,
						RETURN_TYPE (OBJECT_TYPE::*getMethod)() const )
{
	return new MemberWatcher<RETURN_TYPE, OBJECT_TYPE, RETURN_TYPE>(
				rObject,
				getMethod,
				static_cast< void (OBJECT_TYPE::*)( RETURN_TYPE ) >( NULL ) );
}


/**
 *	This function is used to create a read/write watcher using a get and set
 *	methods.
 *
 *	@ingroup WatcherModule
 */
// The DUMMY_TYPE template argument is just used to get around a VC++ problem.
template <class RETURN_TYPE, class OBJECT_TYPE, class DUMMY_TYPE>
WatcherPtr makeWatcher(	const char * path,
						OBJECT_TYPE & rObject,
						RETURN_TYPE (OBJECT_TYPE::*getMethod)() const,
						void (OBJECT_TYPE::*setMethod)(DUMMY_TYPE),
	   					const char * comment = NULL	)
{
	return new MemberWatcher<RETURN_TYPE, OBJECT_TYPE>(
			rObject, getMethod, setMethod );
}


/**
 *	This templatised class is used to watch a given bit a data.
 *
 *	@see WatcherModule
 *	@ingroup WatcherModule
 */
template <class TYPE>
class DataWatcher : public Watcher
{
	public:
		/// @name Construction/Destruction
		//@{
		/**
		 *	Constructor. Leave 'path' as NULL to prevent the watcher adding
		 *	itself to the root watcher. In the normal case, the MF_WATCH style
		 *	macros should be used, as they do error checking and this
		 *	constructor cannot (it's certainly not going to delete itself
		 *	anyway).
		 */
		explicit DataWatcher( TYPE & rValue,
				Watcher::Type access = WT_READ_ONLY,
				const char * path = NULL ) :
			rValue_( rValue ),
			access_( access )
		{
			if (access_ != WT_READ_WRITE && access_ != WT_READ_ONLY)
				access_ = WT_READ_ONLY;

			if (path != NULL)
				Watcher::rootWatcher().addChild( path, this );
		}
		//@}

	protected:
		/// @name Overrides from Watcher.
		//@{
		// Override from Watcher.
		virtual bool getAsString( const void * base, const char * path,
			std::string & result, std::string & desc, Type & type ) const
		{
			if (isEmptyPath( path ))
			{
				const TYPE & useValue = *(const TYPE*)(
					((const uintptr)&rValue_) + ((const uintptr)base) );

				result = watcherValueToString( useValue );
				desc = comment_;
				type = access_;
				return true;
			}
			else
			{
				return false;
			}
		};

		// Override from Watcher.
		virtual bool setFromString( void * base, const char * path,
			const char * valueStr )
		{
			if (isEmptyPath( path ) && (access_ == WT_READ_WRITE))
			{
				TYPE & useValue =
					*(TYPE*)( ((uintptr)&rValue_) + ((uintptr)base) );

				return watcherStringToValue( valueStr, useValue );
			}
			else
			{
				return false;
			}
		}
		//@}


	private:
		TYPE & rValue_;
		Watcher::Type	access_;
};


/**
 *	This template function helps to create new DataWatchers.
 */
template <class TYPE>
WatcherPtr makeWatcher( TYPE & rValue,
				Watcher::Type access = Watcher::WT_READ_ONLY )
{
	return new DataWatcher< TYPE >( rValue, access );
}


/**
 *	This templatised class is used to watch the result of a given function.
 *
 *	@see WatcherModule
 *	@ingroup WatcherModule
 */
template <class RETURN_TYPE, class CONSTRUCTION_TYPE = RETURN_TYPE>
class FunctionWatcher : public Watcher
{
	public:
		/// @name Construction/Destruction
		//@{
		/**
		 *	Constructor. Leave 'path' as NULL to prevent the watcher adding
		 *	itself to the root watcher. In the normal case, the MF_WATCH style
		 *	macros should be used, as they do error checking and this
		 *	constructor cannot (it's certainly not going to delete itself
		 *	anyway).
		 */
		explicit FunctionWatcher( RETURN_TYPE (*getFunction)(),
				void (*setFunction)( RETURN_TYPE ) = NULL,
				const char * path = NULL ) :
			getFunction_( getFunction ),
			setFunction_( setFunction )
		{
			if (path != NULL)
				Watcher::rootWatcher().addChild( path, this );
		}
		//@}

	protected:
		/// @name Overrides from Watcher.
		//@{
		// Override from Watcher.
		virtual bool getAsString( const void * base, const char * path,
			std::string & result, std::string & desc, Type & type ) const
		{
			if (isEmptyPath( path ))
			{
				result = watcherValueToString( (*getFunction_)() );
				type = (setFunction_ != NULL) ? WT_READ_WRITE : WT_READ_ONLY;
				desc = comment_;
				return true;
			}
			else
			{
				return false;
			}
		};

		// Override from Watcher.
		virtual bool setFromString( void * base, const char * path,
			const char * valueStr )
		{
			bool result = false;

			if (isEmptyPath( path ) && (setFunction_ != NULL))
			{
				CONSTRUCTION_TYPE value = CONSTRUCTION_TYPE();
				result = watcherStringToValue( valueStr, value );

				if (result)
				{
					(*setFunction_)( value );
				}
			}

			return result;
		}
		//@}


	private:
		RETURN_TYPE (*getFunction_)();
		void (*setFunction_)( RETURN_TYPE );
};


/**
 *	This function is used to add a read-only watcher using a get-accessor
 *	method.
 *
 *	@ingroup WatcherModule
 */
template <class RETURN_TYPE, class OBJECT_TYPE>
WatcherPtr addReferenceWatcher( const char * path,
			OBJECT_TYPE & rObject,
			const RETURN_TYPE & (OBJECT_TYPE::*getMethod)() const,
	   		const char * comment = NULL	)
{
	WatcherPtr pNewWatcher = makeWatcher( rObject, getMethod, NULL );

	if (!Watcher::rootWatcher().addChild( path, pNewWatcher ))
	{
		pNewWatcher = NULL;
	}
	else if (comment)
	{
		std::string s( comment );
		pNewWatcher->setComment( s );
	}

	return pNewWatcher;
}


/**
 *	This function is used to add a read/write watcher using a get and set
 *	methods.
 *
 *	@ingroup WatcherModule
 */
// The DUMMY_TYPE template argument is just used to get around a VC++ problem.
template <class RETURN_TYPE, class OBJECT_TYPE, class DUMMY_TYPE>
WatcherPtr addReferenceWatcher( const char * path,
						OBJECT_TYPE & rObject,
						const RETURN_TYPE & (OBJECT_TYPE::*getMethod)() const,
						void (OBJECT_TYPE::*setMethod)(DUMMY_TYPE),
						const char * comment = NULL	)
{
	WatcherPtr pNewWatcher = makeWatcher( rObject, getMethod, setMethod );

	if (!Watcher::rootWatcher().addChild( path, pNewWatcher ))
	{
		pNewWatcher = NULL;
	}
	else if (comment)
	{
		std::string s( comment );
		pNewWatcher->setComment( s );
	}

	return pNewWatcher;
}


/**
 *	This function is used to add a read-only watcher using a get-accessor
 *	method.
 *
 *	@ingroup WatcherModule
 */
template <class RETURN_TYPE, class OBJECT_TYPE>
WatcherPtr addWatcher(	const char * path,
						OBJECT_TYPE & rObject,
						RETURN_TYPE (OBJECT_TYPE::*getMethod)() const,
	   					const char * comment = NULL	)
{
	WatcherPtr pNewWatcher =
		new MemberWatcher<RETURN_TYPE, OBJECT_TYPE, RETURN_TYPE>(
				rObject,
				getMethod,
				static_cast< void (OBJECT_TYPE::*)( RETURN_TYPE ) >( NULL ) );

	if (!Watcher::rootWatcher().addChild( path, pNewWatcher ))
	{
		pNewWatcher = NULL;
	}
	else if (comment)
	{
		std::string s( comment );
		pNewWatcher->setComment( s );
	}

	return pNewWatcher;
}


/**
 *	This function is used to add a read/write watcher using a get and set
 *	accessor methods.
 *
 *	@ingroup WatcherModule
 */
// HACK: The DUMMY_TYPE template argument is just used to get around a VC++
// problem.
template <class RETURN_TYPE, class OBJECT_TYPE, class DUMMY_TYPE>
WatcherPtr addWatcher(	const char * path,
						OBJECT_TYPE & rObject,
						RETURN_TYPE (OBJECT_TYPE::*getMethod)() const,
						void (OBJECT_TYPE::*setMethod)(DUMMY_TYPE),
	   					const char * comment = NULL	)
{
	// WatcherPtr pNewWatcher = makeWatcher( rObject, getMethod, setMethod );
	WatcherPtr pNewWatcher = new MemberWatcher<RETURN_TYPE, OBJECT_TYPE>(
			rObject, getMethod, setMethod );

	if (!Watcher::rootWatcher().addChild( path, pNewWatcher ))
	{
		pNewWatcher = NULL;
	}
	else if (comment)
	{
		std::string s( comment );
		pNewWatcher->setComment( s );
	}

	return pNewWatcher;
}


/**
 *	This function is used to add a watcher of a function value.
 *
 *	@ingroup WatcherModule
 */
template <class RETURN_TYPE>
WatcherPtr addWatcher(	const char * path,
						RETURN_TYPE (*getFunction)(),
						void (*setFunction)( RETURN_TYPE ) = NULL,
	   					const char * comment = NULL	)
{
	WatcherPtr pNewWatcher =
		new FunctionWatcher<RETURN_TYPE>( getFunction, setFunction );

	if (!Watcher::rootWatcher().addChild( path, pNewWatcher ))
	{
		pNewWatcher = NULL;
	}
	else if (comment)
	{
		std::string s( comment );
		pNewWatcher->setComment( s );
	}

	return pNewWatcher;
}


/**
 *	This function is used to add a watcher of a simple value.
 *
 *	@ingroup WatcherModule
 */
template <class TYPE>
WatcherPtr addWatcher(	const char * path,
						TYPE & rValue,
						Watcher::Type access = Watcher::WT_READ_WRITE,
   						const char * comment = NULL	)
{
	WatcherPtr pNewWatcher = makeWatcher( rValue, access );

	if (!Watcher::rootWatcher().addChild( path, pNewWatcher ))
	{
		pNewWatcher = NULL;
	}
	else if (comment)
	{
		std::string s( comment );
		pNewWatcher->setComment( s );
	}

	return pNewWatcher;
}

/**
 *	This macro is used to watch a value at run-time for debugging purposes. This
 *	value can then be inspected and changed at run-time through a variety of
 *	methods. These include using the Web interface or using the in-built menu
 *	system in an appropriate build of the client. (This is displayed by pressing
 *	\<F7\>.)
 *
 *	The simplest usage of this macro is when you want to watch a fixed variable
 *	such as a global or a variable that is static to a file, class, or method.
 *	The macro should be called once during initialisation for each value that
 *	is to be watched.
 *
 *	For example, to watch a value that is static to a file, do the following:
 *
 *	@code
 *	static int myValue = 72;
 *	...
 *	MF_WATCH( "myValue", myValue );
 *	@endcode
 *
 *	This allows you to inspect and change the value of @a myValue. The first
 *	argument to the macro is a string specifying the path associated with this
 *	value. This is used in displaying and setting this value via the different
 *	%Watcher interfaces.
 *
 *	If you want to not allow the value to be changed using the Watch interfaces,
 *	include the extra argument Watcher::WT_READ_ONLY.
 *
 *	@code
 *	MF_WATCH( "myValue", myValue, WT_READ_ONLY );
 *	@endcode
 *
 *	Any type of value can be watched as long as it has streaming operators to
 *	ostream and istream.
 *
 *	You can also look at a value associated with an object using accessor
 *	methods on that object.
 *
 *	For example, if you had the following class:
 *	@code
 *	class ExampleClass
 *	{
 *		public:
 *			int getValue() const;
 *			void setValue( int setValue ) const;
 *	};
 *
 *	ExampleClass exampleObject_;
 *	@endcode
 *
 *	You can watch this value as follows:
 *	@code
 *	MF_WATCH( "some value", exampleObject_,
 *			ExampleClass::getValue,
 *			ExampleClass::setValue );
 *	@endcode
 *
 *	If you have overloaded your accessor methods, use the @ref MF_ACCESSORS
 *	macro to help with casting.
 *
 *	If you want the value to be read-only, do not add the set accessor to the
 *	macro call.
 *	@code
 *	MF_WATCH( "some value", exampleObject_,
 *			ExampleClass::getValue );
 *	@endcode
 *
 *	If you want to watch a value and the accessors take and return a reference
 *	to the object, instead of a copy of the object.
 *
 *	@ingroup WatcherModule
 *
 *	@todo More details in comment including the run-time interfaces for Watcher.
 */
#define MF_WATCH				::addWatcher

/**
 *	This macro is used to watch a value at run-time for debugging purposes. This
 *	macro should be used instead of MF_WATCH if the accessors take and return a
 *	reference to the object being watched.
 *
 *	For example,
 *
 *	@code
 *	class ExampleClass
 *	{
 *		const Vector3 & getPosition() const;
 *		void setPosition( const Vector3 & newPosition );
 *	};
 *	ExampleClass exampleObject_;
 *
 *	...
 *
 *	MF_WATCH_REF( "someValue", exampleObject_,
 *			ExampleClass::getPosition,
 *			ExampleClass::setPosition );
 *	@endcode
 *
 *	For more details, see MF_WATCH.
 *
 *	@ingroup WatcherModule
 */
#define MF_WATCH_REF			::addReferenceWatcher

/**
 *	This is a simple macro that helps us do casting.
 *	This allows us to do
 *
 *	@code
 *	MF_WATCH(  "Comms/Desired in",
 *		g_server,
 *		MF_ACCESSORS( uint32, ServerConnection, bandwidthFromServer ) );
 *	@endcode
 *
 * instead of
 *
 *	@code
 *	MF_WATCH( "Comms/Desired in",
 *		g_server,
 *		(uint32 (MyClass::*)() const)(MyClass::bandwidthFromServer),
 *		(void   (MyClass::*)(uint32))(MyClass::bandwidthFromServer) );
 *	@endcode
 *
 *	@ingroup WatcherModule
 */
#define MF_ACCESSORS( TYPE, CLASS, METHOD )							\
	static_cast< TYPE (CLASS::*)() const >(&CLASS::METHOD),			\
	static_cast< void (CLASS::*)(TYPE)   >(&CLASS::METHOD)

/**
 *	This macro is like MF_ACCESSORS except that the write accessor is NULL.
 *
 *	@see MF_ACCESSORS
 *
 *	@ingroup WatcherModule
 */
#define MF_WRITE_ACCESSOR( TYPE, CLASS, METHOD )					\
	static_cast< TYPE (CLASS::*)() const >( NULL ),					\
	static_cast< void (CLASS::*)(TYPE)   >( &CLASS::METHOD )


/*
#ifdef CODE_INLINE
#include "watcher.ipp"
#endif
*/



#else // ENABLE_WATCHERS


class Watcher;
class WatcherVisitor;
class DirectoryWatcher;
template <typename> class SequenceWatcher;
template <typename> class DataWatcher;

class Watcher : public ReferenceCount
{
public:
	enum Type
	{
		WT_INVALID,			///< Indicates an error.
		WT_READ_ONLY,		///< Indicates the watched value cannot be changed.
		WT_READ_WRITE,		///< Indicates the watched value can be changed.
		WT_DIRECTORY		///< Indicates that the watcher has children.
	};

	bool getAsString(	const void *, const char *,	std::string &, std::string &, Type & ) const {return false;};
	bool getAsString(	const void *, const char *,	std::string &, Type & ) const {return false;};
	bool setFromString(	void *,	const char *, const char * ) {return false;};
	bool visitChildren(	const void *, const char *, WatcherVisitor & ) {return false;};
	bool addChild(	const char * path, WatcherPtr watcher, void * = NULL  ) {return false;};
	bool removeChild( const char * path ) {	return false; };
	void setLabels( const char ** labels ) {};
	void setLabelSubPath( const char * subpath ) {};
	void setComment( const std::string &) {};
	const std::string & getComment() { static std::string str("Watchers disabled"); return str; };
	static Watcher& rootWatcher();
	static void partitionPath(	const std::string, std::string &, std::string & ) {};
	static void fini();

private:
	static WatcherPtr pRootWatcher_;
};


class DirectoryWatcher : public Watcher
{
public:
	static const char* tail( const char* ) {return NULL;}
};


template <class SEQ> class SequenceWatcher : public Watcher
{
public:
	SequenceWatcher(	SEQ & toWatch = *(SEQ*)NULL,
						const char ** labels = NULL )
	{
	}
};

class BaseDereferenceWatcher : public Watcher
{
public:
	BaseDereferenceWatcher( WatcherPtr watcher, void * withBase = NULL )
	{
	}
};


template <class TYPE>
class DataWatcher : public Watcher
{
public:
	explicit DataWatcher(	TYPE & rValue,
							Watcher::Type access = WT_READ_ONLY,
							const char * path = NULL )
	{
	}
};


template <class RETURN_TYPE,
	class OBJECT_TYPE,
	class CONSTRUCTION_TYPE = RETURN_TYPE>
class MemberWatcher : public Watcher
{
public:
	MemberWatcher( OBJECT_TYPE & rObject,
			RETURN_TYPE (OBJECT_TYPE::*getMethod)() const,
			void (OBJECT_TYPE::*setMethod)( RETURN_TYPE ) = NULL ) {};
};


class WatcherVisitor
{
};




#define MF_WATCH				::addWatcher
#define MF_WATCH_REF			::addReferenceWatcher
#define MF_ACCESSORS( TYPE, CLASS, METHOD )		static_cast< TYPE (CLASS::*)() const >(&CLASS::METHOD),		\
												static_cast< void (CLASS::*)(TYPE)   >(&CLASS::METHOD)




template <class RETURN_TYPE, class OBJECT_TYPE>
WatcherPtr addWatcher(	const char * path,
						OBJECT_TYPE & rObject,
						RETURN_TYPE (OBJECT_TYPE::*getMethod)() const,
	   					const char * comment = NULL	)
{
	return NULL;
}

template <class RETURN_TYPE, class OBJECT_TYPE, class DUMMY_TYPE>
WatcherPtr addWatcher(	const char * path,
						OBJECT_TYPE & rObject,
						RETURN_TYPE (OBJECT_TYPE::*getMethod)() const,
						void (OBJECT_TYPE::*setMethod)(DUMMY_TYPE),
	   					const char * comment = NULL	)
{
	return NULL;
}

template <class RETURN_TYPE>
WatcherPtr addWatcher(	const char * path,
						RETURN_TYPE (*getFunction)(),
						void (*setFunction)( RETURN_TYPE ) = NULL,
	   					const char * comment = NULL	)
{
	return NULL;
}

template <class TYPE>
WatcherPtr addWatcher(	const char * path,
						TYPE & rValue,
						Watcher::Type access = Watcher::WT_READ_WRITE,
	   					const char * comment = NULL	)
{
	return NULL;
}




template <class RETURN_TYPE, class OBJECT_TYPE>
WatcherPtr addReferenceWatcher( const char * path,
			OBJECT_TYPE & rObject,
			const RETURN_TYPE & (OBJECT_TYPE::*getMethod)() const,
	   		const char * comment = NULL	)
{
	return NULL;
}

template <class RETURN_TYPE, class OBJECT_TYPE, class DUMMY_TYPE>
WatcherPtr addReferenceWatcher( const char * path,
			OBJECT_TYPE & rObject,
			const RETURN_TYPE & (OBJECT_TYPE::*getMethod)() const,
			void (OBJECT_TYPE::*setMethod)(DUMMY_TYPE),
	   		const char * comment = NULL	)
{
	return NULL;
}


#endif // ENABLE_WATCHERS


#endif // WATCHER_HPP
