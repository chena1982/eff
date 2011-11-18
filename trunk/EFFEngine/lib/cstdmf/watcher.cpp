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

#include "watcher.hpp"




WatcherPtr Watcher::pRootWatcher_ = NULL;


// -----------------------------------------------------------------------------
// Section: Watcher Methods
// -----------------------------------------------------------------------------

/**
 *	This method returns the root watcher.
 *
 *	@return	A reference to the root watcher.
 */
Watcher & Watcher::rootWatcher()
{
	if (pRootWatcher_ == NULL)
	{
		pRootWatcher_ = new DirectoryWatcher();
	}

	return *pRootWatcher_;
}

void Watcher::fini() 
{	
	pRootWatcher_ = NULL;
}


#if ENABLE_WATCHERS


#include <assert.h>

#define SORTED_WATCHERS

/*
#ifndef CODE_INLINE
#include "debug_value.ipp"
#endif
*/

#include "cstdmf/debug.hpp"

//DECLARE_DEBUG_COMPONENT2( "CStdMF", 0 )


// -----------------------------------------------------------------------------
// Section: DirectoryWatcher Methods
// -----------------------------------------------------------------------------

/**
 *	Constructor for DirectoryWatcher.
 */
DirectoryWatcher::DirectoryWatcher()
{
}

DirectoryWatcher::~DirectoryWatcher()
{
}


/*
 *	This method is an override from Watcher.
 */
bool DirectoryWatcher::getAsString( const void * base,
								const char * path,
								std::string & result,
								std::string & desc,
								Type & type ) const
{
	if (isEmptyPath(path))
	{
		result = "<DIR>";
		type = WT_DIRECTORY;
		desc = comment_;
		return true;
	}
	else
	{
		DirData * pChild = this->findChild( path );

		if (pChild != NULL)
		{
			const void * addedBase = (const void*)(
				((const uintptr)base) + ((const uintptr)pChild->base) );
			return pChild->watcher->getAsString( addedBase, this->tail( path ),
				result, desc, type );
		}
		else
		{
			return false;
		}
	}
	// never gets here
}



/*
 *	Override from Watcher.
 */
bool DirectoryWatcher::setFromString( void * base,
	const char * path, const char * value )
{
	DirData * pChild = this->findChild( path );

	if (pChild != NULL)
	{
		void * addedBase = (void*)( ((uintptr)base) + ((uintptr)pChild->base) );
		return pChild->watcher->setFromString( addedBase, this->tail( path ),
			value );
	}
	else
	{
		return false;
	}
}




/*
 *	Override from Watcher
 */
bool DirectoryWatcher::visitChildren( const void * base, const char * path,
	WatcherVisitor & visitor )
{
	bool handled = false;

	if (isEmptyPath(path))
	{
		Container::iterator iter = container_.begin();

		while (iter != container_.end())
		{
			const void * addedBase = (const void*)(
				((const uintptr)base) + ((const uintptr)(*iter).base) );

			std::string valueStr;
			std::string desc;
			Type resType;
			(*iter).watcher->getAsString( addedBase, NULL, valueStr, desc, resType );

			if (!visitor.visit( resType, (*iter).label, desc, valueStr ))
				break;

			iter++;
		}

		handled = true;
	}
	else
	{
		DirData * pChild = this->findChild( path );

		if (pChild != NULL)
		{
			const void * addedBase = (const void*)(
				((const uintptr)base) + ((const uintptr)pChild->base) );

			handled = pChild->watcher->visitChildren( addedBase,
				this->tail( path ), visitor );
		}
	}

	return handled;
}



/*
 * Override from Watcher.
 */
bool DirectoryWatcher::addChild( const char * path, WatcherPtr pChild,
	void * withBase )
{
	bool wasAdded = false;

	if (isEmptyPath( path ))						// Is it invalid?
	{
		ERROR_MSG( "DirectoryWatcher::addChild: "
			"tried to add unnamed child (no trailing slashes please)\n" );
	}
	else if (strchr(path,'/') == NULL)				// Is it for us?
	{
		// Make sure that we do not add it twice.
		if (this->findChild( path ) == NULL)
		{
			DirData		newDirData;
			newDirData.watcher = pChild;
			newDirData.base = withBase;
			newDirData.label = path;

#ifdef SORTED_WATCHERS
			Container::iterator iter = container_.begin();
			while ((iter != container_.end()) &&
					(iter->label < newDirData.label))
			{
				++iter;
			}
			container_.insert( iter, newDirData );
#else
			container_.push_back( newDirData );
#endif
			wasAdded = true;
		}
		else
		{
			ERROR_MSG( "DirectoryWatcher::addChild: "
				"tried to replace existing watcher %s\n", path );
		}
	}
	else											// Must be for a child
	{
		DirData * pFound = this->findChild( path );

		if (pFound == NULL)
		{
			char * pSeparator = strchr( (char*)path, WATCHER_SEPARATOR );

			int compareLength = (pSeparator == NULL) ?
				strlen( (char*)path ) : (pSeparator - path);

			Watcher * pChildWatcher = new DirectoryWatcher();

			DirData		newDirData;
			newDirData.watcher = pChildWatcher;
			newDirData.base = NULL;
			newDirData.label = std::string( path, compareLength );

#ifdef SORTED_WATCHERS
			Container::iterator iter = container_.begin();
			while ((iter != container_.end()) &&
					(iter->label < newDirData.label))
			{
				++iter;
			}
			pFound = &*container_.insert( iter, newDirData );
#else
			container_.push_back( newDirData );
			pFound = &container_.back();
#endif
		}

		if (pFound != NULL)
		{
			wasAdded = pFound->watcher->addChild( this->tail( path ),
				pChild, withBase );
		}
	}

	return wasAdded;
}


/**
 *	This method removes a child identifier in the path string.
 *
 *	@param path		This string must start with the identifier that you are
 *					searching for. For example, "myDir/myValue" would match the
 *					child with the label "myDir".
 *
 *	@return	true if remove, false if not found
 */
bool DirectoryWatcher::removeChild( const char * path )
{
	if (path == NULL)
		return false;

	char * pSeparator = strchr( (char*)path, WATCHER_SEPARATOR );

	unsigned int compareLength =
		(pSeparator == NULL) ? strlen( path ) : (pSeparator - path);

	if (compareLength != 0)
	{
		Container::iterator iter = container_.begin();

		while (iter != container_.end())
        {
			if (compareLength == (*iter).label.length() &&
				strncmp(path, (*iter).label.data(), compareLength) == 0)
			{
				if( pSeparator == NULL )
				{
					container_.erase( iter );
					return true;
				}
				else
				{
					DirData* pChild = const_cast<DirData*>(&(const DirData &)*iter);
					return pChild->watcher->removeChild( tail( path ) );
				}
			}
			iter++;
		}
	}

	return false;
}

/**
 *	This method finds the immediate child of this directory matching the first
 *	identifier in the path string.
 *
 *	@param path		This string must start with the identifier that you are
 *					searching for. For example, "myDir/myValue" would match the
 *					child with the label "myDir".
 *
 *	@return	The watcher matching the input path. NULL if one was not found.
 */
DirectoryWatcher::DirData * DirectoryWatcher::findChild( const char * path ) const
{
	if (path == NULL)
	{
		return NULL;
	}

	char * pSeparator = strchr( (char*)path, WATCHER_SEPARATOR );

	unsigned int compareLength =
		(pSeparator == NULL) ? strlen( path ) : (pSeparator - path);

    DirData * pChild = NULL;

	if (compareLength != 0)
	{
		Container::const_iterator iter = container_.begin();

		while (iter != container_.end() && pChild == NULL)
        {
			if (compareLength == (*iter).label.length() &&
				strncmp(path, (*iter).label.data(), compareLength) == 0)
			{
				pChild = const_cast<DirData*>(&(const DirData &)*iter);
			}
			iter++;
		}
	}

	return pChild;
}


/**
 *	This method is used to find the string representing the path without the
 *	initial identifier.
 *
 *	@param path		The path that you want to find the tail of.
 *
 *	@return	A string that represents the remainder of the path.
 */
const char * DirectoryWatcher::tail( const char * path )
{
	if (path == NULL)
		return NULL;

	char * pSeparator = strchr( (char*)path, WATCHER_SEPARATOR );

	if (pSeparator == NULL)
		return NULL;

	return pSeparator + 1;
}




// -----------------------------------------------------------------------------
// Section: Constructors/Destructor
// -----------------------------------------------------------------------------

/**
 *	Constructor for Watcher.
 */
Watcher::Watcher()
{
}


/**
 *	The destructor deletes all of its children.
 */
Watcher::~Watcher()
{
}


// -----------------------------------------------------------------------------
// Section: Static methods of Watcher
// -----------------------------------------------------------------------------

/**
 *	This is a simple helper method used to partition a path string into the name
 *	and directory strings. For example, "section1/section2/hello" would be split
 *	into "hello" and "section1/section2/".
 *
 *	@param path				The string to be partitioned.
 *	@param resultingName	A reference to a string that is to receive the base
 *							name.
 *	@param resultingDirectory	A reference to a string that is to receive the
 *							directory string.
 */
void Watcher::partitionPath( const std::string path,
		std::string & resultingName,
		std::string & resultingDirectory )
{
	int pos = path.find_last_of( WATCHER_SEPARATOR );

	if ( 0 <= pos && pos < (int)path.size( ) )
	{
		resultingDirectory	= path.substr( 0, pos + 1 );
		resultingName		= path.substr( pos + 1, path.length() - pos - 1 );
	}
	else
	{
		resultingName		= path;
		resultingDirectory	= "";
	}
}


/**
 *	Utility method to watch the count of instances of a type
 */
void watchTypeCount( const char * basePrefix, const char * typeName, int & var )
{
	std::string str = basePrefix;
	str += typeName;
	addWatcher( str.c_str(), var );
}


#endif // ENABLE_WATCHERS


// watcher.cpp
