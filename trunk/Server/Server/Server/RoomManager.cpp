#include "RoomManager.h"

DEF_SINGLENESS(RoomManager)

RoomManager::RoomManager(void)
{
}

RoomManager::~RoomManager(void)
{
}

bool RoomManager::create( std::string name )
{
	stdext::hash_map<std::string, Room*>::iterator iter = roomMap_.find( name );
	
	if( iter != roomMap_.end() )
		return false;

	rooms_.push_back( new Room( name ) );
	return true;
}

Room* RoomManager::find( std::string name )
{
	stdext::hash_map<std::string, Room*>::iterator iter = roomMap_.find( name );
	
	if( iter == roomMap_.end() )
		return 0;

	return (*iter).second;
}