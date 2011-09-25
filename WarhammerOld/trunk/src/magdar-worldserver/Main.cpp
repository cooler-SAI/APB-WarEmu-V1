#include "StdAfx.h"
#include <signal.h>
#ifndef WIN32
#include <sys/resource.h>
#endif

#ifndef WIN32
#include <sched.h>
#endif

// Database impl
bool mrunning = true;

void RunWS(int argc, char** argv)
{
	new WorldServer;
	WorldServer::getSingleton( ).Run(argc, argv);
	delete WorldServer::getSingletonPtr();
}

int main(int argc, char** argv)
{
	SetThreadName( "Main Thread" );
	StartCrashHandler();

	// Run!
	THREAD_TRY_EXECUTION
	{
		RunWS(argc, argv);
	}
	THREAD_HANDLE_CRASH

	exit( 0 );
	return 0;
}
void OnCrash(bool Terminate)
{
	if(!mrunning)
		return;

	Log.Error("Main","Advanced Crash initialized !");
	//objmgr.RemoveAllPlayer();
}