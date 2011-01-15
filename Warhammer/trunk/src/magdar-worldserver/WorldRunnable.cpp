#include "StdAfx.h"
#define WORLD_UPDATE_DELAY 50

WorldRunnable::WorldRunnable() : ThreadContext()
{
eventholder = new EventableObjectHolder(-1);
}

bool WorldRunnable::run()
{
	SetThreadName("Global Runnable(no map)");
	uint32 LastWorldUpdate=getMSTime();
	uint32 LastSessionsUpdate=getMSTime();

	THREAD_TRY_EXECUTION2

	while(m_threadRunning)
	{
		uint32 diff;
		//calce time passed
		uint32 now,execution_start;
		now=getMSTime();
		execution_start=now;

		if( now < LastWorldUpdate)//overrun
			diff=WORLD_UPDATE_DELAY;
		else
			diff=now-LastWorldUpdate;
		
		LastWorldUpdate=now;

		sWorldSocket.Update();
		eventholder->Update( (uint32)diff );
		
		now=getMSTime();
		
		if( now < LastSessionsUpdate)//overrun
			diff=WORLD_UPDATE_DELAY;
		else
			diff=now-LastSessionsUpdate;
		
		LastSessionsUpdate=now;
		
		now=getMSTime();
		//we have to wait now 
		
		if(execution_start > now)//overrun
			diff=WORLD_UPDATE_DELAY-now;
		else
			diff=now-execution_start;//time used for updating 


		if(diff<WORLD_UPDATE_DELAY)
		Sleep(WORLD_UPDATE_DELAY);
	}

	THREAD_HANDLE_CRASH2
	return true;
}
