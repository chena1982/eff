#pragma once

#include "cstdmf/pch.hpp"
#include "Room.h"
#include <vector>
#include <string>
#include <hash_map>

class RoomManager:public Singleness<RoomManager>
{
private:
	std::vector<Room*> rooms_;
	stdext::hash_map<std::string, Room*> roomMap_;
public:
	RoomManager(void);
	~RoomManager(void);

	bool create( std::string name );
	Room* find( std::string name );
};
