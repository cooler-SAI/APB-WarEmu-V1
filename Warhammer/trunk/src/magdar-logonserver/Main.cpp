#include "LogonStdAfx.h"
#include <signal.h>
#ifndef WIN32
#include <sys/resource.h>
#endif

#ifndef WIN32
#include <sched.h>
#endif

// Database impl
bool mrunning = true;

void RunLS(int argc, char** argv)
{
	new LogonServer;
	LogonServer::getSingleton( ).Run(argc, argv);
	delete LogonServer::getSingletonPtr();
}

int main(int argc, char** argv)
{
	// Run!
	THREAD_TRY_EXECUTION
	{
		RunLS(argc, argv);
	}
	THREAD_HANDLE_CRASH
}
void OnCrash(bool Terminate)
{
	Log.Error("Main","Advanced Crash initialized !");
}