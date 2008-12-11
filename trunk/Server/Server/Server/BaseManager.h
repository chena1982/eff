#pragma once
#include "AuServerSock.h"
#include "Base.h"
#include <vector>

class BaseManager
{
private:
	std::vector<Base*> baseArray_;
public:
	BaseManager(void);
	~BaseManager(void);
	
	// init
	void Init();

	// create base
	void create( SOCKET client ); 
};
