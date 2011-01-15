#include "LogonStdAfx.h"
#include "ShellApi.h" 

initialiseSingleton(LogonServer);

// Handler for the console signals
BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
    switch (ctrl_type)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        mrunning = false;
        return TRUE;
    default:
        return FALSE;
    }
}

bool startdb()
{
	string lhostname, lusername, lpassword, ldatabase;
	int lport = 0;
	int ltype = 1;
	// Configure Main Database

	bool result;

	// Configure Logon Database...
	result = Config.MainConfig.GetString("LogonDatabase", "Username", &lusername);
	result = !result ? result : Config.MainConfig.GetString("LogonDatabase", "Password", &lpassword);
	result = !result ? result : Config.MainConfig.GetString("LogonDatabase", "Hostname", &lhostname);
	result = !result ? result : Config.MainConfig.GetString("LogonDatabase", "Name", &ldatabase);
	result = !result ? result : Config.MainConfig.GetInt("LogonDatabase", "Port", &lport);
	result = !result ? result : Config.MainConfig.GetInt("LogonDatabase", "Type", &ltype);

	if(result == false)
	{
		sLog.outString("Sql: Invalid database info.");
		return false;
	}

	sLogonSQL = Database::Create();

	// Initialize it
	if(!sLogonSQL->Initialize(lhostname.c_str(), (unsigned int)lport, lusername.c_str(),
		lpassword.c_str(), ldatabase.c_str(), Config.MainConfig.GetIntDefault("LogonDatabase", "ConnectionCount", 5),
		16384))
	{
		sLog.outError("sql: Logon database initialization failed. Exiting.");
		return false;
	}   

	return true;
}
void LogonServer::Run(int argc, char ** argv)
{
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	sLog.outString("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
	sLog.outString("INITIALIZING MAGDAR EMULATOR (C)");
	sLog.outString("     http://waremu.fr");
	sLog.outString("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");

	char * config_file = "magdar-logonserver.conf";

	sLog.m_fileLogLevel = -1;
	sLog.m_screenLogLevel = 3;
	Log.Init(3);
	Log.Notice("Config", "Loading conf file [%s]...",config_file);
	if(!Config.MainConfig.SetSource(config_file))
	{
		printf("Can't open conf file.\n");
		Sleep(2000);
		return;
	}

	Log.Notice("ThreadMgr", "Starting...");
	ThreadPool.Startup(2);
	new DatabaseMgr;
	if(!startdb())
		return;

	Log.Notice("AccountMgr", "Starting...");
	new AccountMgr;
	Config.MainConfig.GetBool("LogonServer","CleanPass",&sAccountMgr.m_cleanpass);

	sAccountMgr.ReloadAccounts(true);
	Log.Notice("AccountMgr", "%u accounts loaded.", sAccountMgr.GetCount());
	Log.Line();

	int atime = 60;
	Config.MainConfig.GetInt("LogonServer", "ReloadTime", &atime);
	atime *= 1000;

	PeriodicFunctionCaller<AccountMgr> * pfc = new PeriodicFunctionCaller<AccountMgr>(AccountMgr::getSingletonPtr(),&AccountMgr::ReloadAccountsCallback, atime);
	ThreadPool.ExecuteTask(pfc);
	
	string host,clientport,realmport;
	Config.MainConfig.GetString("Listen","Host",&host);
	Config.MainConfig.GetString("Listen","ClientPort",&clientport);
	Config.MainConfig.GetString("Listen","RealmPort",&realmport);

	new InformationCore;
	Config.MainConfig.GetString("LogonServer","Password",&sInfoCore.m_password);

	Log.Debug("LogonServer","Starting Client acceptor");
	Network n;
	try
    {
		n.createConnectionAcceptor<AuthClient>(host,clientport,2);
	}
    catch (boost::exception &e)
    {
		Log.LargeErrorMessage(1,"LogonServer","Exception while creating client listeners: %s",e.diagnostic_information());
        return;
    }
	try
    {
		n.createConnectionAcceptor<AuthWorld>(host,realmport,1);
	}
    catch (boost::exception &e)
    {
		Log.LargeErrorMessage(1,"LogonServer","Exception while creating world listeners: %s",e.diagnostic_information());
        return;
    }


   //ShellExecute(NULL,"open","magdar-world.exe",0,0,SW_NORMAL); 

        SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

		uint32 loop_counter = 0;
        ///- Run and wait for exit
        while( mrunning )
		{
			if(!(++loop_counter%10000))	// 2mins
			{
				ThreadPool.IntegrityCheck(2); //Logonserver don't need as many threads as world-server, 2 will do
			}

			if(!(loop_counter%100))  //100 loop ~ 1seconds
			{
				UNIXTIME = time(NULL);
				g_localTime = *localtime(&UNIXTIME);
			}
			Sleep(10);
		}
		Log.Notice("LogonServer","Closing.");

        n.stop();

	// kill db
	sLog.outString("Closing DB : EndThreads.");
	sLogonSQL->EndThreads();
	sLog.outString("Closing DB : Shutdown.");
	sLogonSQL->Shutdown();
	sLog.outString("Closing DB : Delete.");
	delete sLogonSQL;

	sLog.outString("AccountMgr Delete");
	delete AccountMgr::getSingletonPtr();
	sLog.outString("InformationCore Delete");
	delete InformationCore::getSingletonPtr();
	Log.Notice("LogonServer","Shutdown complete.\n");
}
