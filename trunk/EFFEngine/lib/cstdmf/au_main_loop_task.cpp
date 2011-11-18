#include "pch.hpp"
#include "au_main_loop_task.h"

bool MainLoopTasks::init()
{
	for( int i = 0; i < (int)tasks.size(); ++i )
	{
		if( tasks[i] )
		{
			tasks[i]->init();
		}
	}
	return true;
}

void MainLoopTasks::fini()
{
	for( int i = 0; i < (int)tasks.size(); ++i )
	{
		if( tasks[i] )
		{
			tasks[i]->fini();
		}
	}
}

void MainLoopTasks::tick( float dTime )
{
	for( int i = 0; i < (int)tasks.size(); ++i )
	{
		if( tasks[i] )
		{
			tasks[i]->tick( dTime );
		}
	}
}

void MainLoopTasks::draw()
{
	for( int i = 0; i < (int)tasks.size(); ++i )
	{
		if( tasks[i] )
		{
			tasks[i]->draw();
		}
	}
}

void MainLoopTasks::unDraw()
{
	for( int i = 0; i < (int)tasks.size(); ++i )
	{
		if( tasks[i] )
		{
			tasks[i]->unDraw();
		}
	}
}

void MainLoopTasks::add( float position, MainLoopTask* pTask )
{
	if( pTask )
	{
		pTask->position = position;
		tasks.push_back( pTask );
	}
}

void MainLoopTasks::del( MainLoopTask* pTask )
{	
	std::vector<MainLoopTask*>::iterator iter = tasks.begin();
	for( int i = 0; i < (int)tasks.size(); ++i, ++iter )
	{
		if( tasks[i] == pTask && *iter == pTask )
		{
			tasks.erase( iter );
			break;
		}
	}
}
bool lt( const MainLoopTask* a, const MainLoopTask* b )
{
	return a->position < b->position;
}

void MainLoopTasks::sort()
{
	std::sort( tasks.begin(), tasks.end(), lt );
}