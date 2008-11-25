#pragma once

class MainLoopTask
{
public:
	MainLoopTask(){	}

	virtual bool init() { return true; }
	virtual void fini()	{ }

	virtual void tick( float dTime ) { }
	virtual void draw() { }
	virtual void unDraw() {}

	float position;
};


class MainLoopTasks:public MainLoopTask
{
public:
	MainLoopTasks() {}
	~MainLoopTasks()
	{
		for( int i = 0; i < (int)tasks.size(); ++i )
		{
			if( tasks[i] )
			{
				SAFE_DELETE( tasks[i] );
			}
		}
		tasks.clear();
	}

	virtual bool init();
	virtual void fini();

	virtual void tick( float dTime );
	virtual void draw();
	virtual void unDraw();
	
	virtual void add( float position, MainLoopTask* pTask );
	virtual void del( MainLoopTask* pTask );

	void sort();
private:
	std::vector<MainLoopTask*> tasks;
};