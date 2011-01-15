#include "StdAfx.h"

#include <signal.h>

initialiseSingleton(WorldServer);

// Database defines.
SERVER_DECL Database* Database_Character;
SERVER_DECL Database* Database_World;

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

bool startWdb()
{
	string lhostname, lusername, lpassword, ldatabase;
	int lport = 0;
	int ltype = 1;
	// Configure Main Database

	bool result;

	// Configure Logon Database...
	result = Config.MainConfig.GetString("WorldDatabase", "Username", &lusername);
	result = !result ? result : Config.MainConfig.GetString("WorldDatabase", "Password", &lpassword);
	result = !result ? result : Config.MainConfig.GetString("WorldDatabase", "Hostname", &lhostname);
	result = !result ? result : Config.MainConfig.GetString("WorldDatabase", "Name", &ldatabase);
	result = !result ? result : Config.MainConfig.GetInt("WorldDatabase", "Port", &lport);

	if(result == false)
	{
		sLog.outString("sql: Parametre database introuvable.");
		return false;
	}

	Database_World = Database::Create();

	// Initialize it
	if(!WorldDatabase.Initialize(lhostname.c_str(), (unsigned int)lport, lusername.c_str(),
		lpassword.c_str(), ldatabase.c_str(), Config.MainConfig.GetIntDefault("WorldDatabase", "ConnectionCount", 5),
		16384))
	{
		sLog.outError("sql: World database initialization failed. Exiting.");
		return false;
	}   

	return true;
}
bool startCdb()
{
	string lhostname, lusername, lpassword, ldatabase;
	int lport = 0;
	int ltype = 1;
	// Configure Main Database

	bool result;

	// Configure Logon Database...
	result = Config.MainConfig.GetString("CharactersDatabase", "Username", &lusername);
	result = !result ? result : Config.MainConfig.GetString("CharactersDatabase", "Password", &lpassword);
	result = !result ? result : Config.MainConfig.GetString("CharactersDatabase", "Hostname", &lhostname);
	result = !result ? result : Config.MainConfig.GetString("CharactersDatabase", "Name", &ldatabase);
	result = !result ? result : Config.MainConfig.GetInt("CharactersDatabase", "Port", &lport);

	if(result == false)
	{
		sLog.outString("sql: Parametre database introuvable.");
		return false;
	}

	Database_Character = Database::Create();

	// Initialize it
	if(!CharacterDatabase.Initialize(lhostname.c_str(), (unsigned int)lport, lusername.c_str(),
		lpassword.c_str(), ldatabase.c_str(), Config.MainConfig.GetIntDefault("WorldDatabase", "ConnectionCount", 5),
		16384))
	{
		sLog.outError("sql: Characters database initialization failed. Exiting.");
		return false;
	}   

	return true;
}
bool WorldServer::Run(int argc, char ** argv)
{
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);
	time_t curTime;

	m_Shutdown=false;
	m_ShutdownTimer=0;
	

	sLog.outString("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
	sLog.outString("INITIALIZING MAGDAR EMULATOR (C)");
	sLog.outString("     http://waremu.fr");
	sLog.outString("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
	Log.Notice("Config", "Loading Conf File...");

	char * world_file = "magdar-worldserver.conf";

	if(!Config.MainConfig.SetSource(world_file))
	{
		printf("Invalid Conf File !.\n");
		Sleep(2000);
		return false;
	}
	
	int screen=3;
	Config.MainConfig.GetInt("LogLevel","Screen",&screen);
	sLog.Init(-1,screen);
	Log.Init(screen);
	GetMACaddress();
	InitRandomNumberGenerators();
	Log.Success( "Rnd", "Initialized Random Number Generators." );

	Log.Notice("ThreadMgr", "Starting...");
	ThreadPool.Startup(4);

	if(!startWdb())
		return false;

	if(!startCdb())
		return false;

	Log.Notice("Config", "Loading ScriptMgr...");
	new ScriptMgr;
	Log.Notice("Config", "Loading StatsMgr...");
	new StatsMgr;
	Log.Notice("Config","Loading HookEvent...");
	new EventHook;
	Log.Notice("Config", "Loading WorldSocket...");
	new WorldSocket;
	Log.Notice("Config", "Loading EventMgr...");
	new EventMgr;
	Log.Notice("Config", "Loading MapMgr...");
	new MapMgr;
	Log.Notice("Config", "Loading HeightFieldMgr...");
	new HeightFieldMgr;
	Log.Notice("Config", "Loading ObjMgr...");
	new ObjectMgr;
	Log.Notice("Config", "Loading ChatHandler...");
	new ChatHandler;
	
	Log.Notice("Config", "Loading WorldRunnable...");
	WorldRunnable * wr = new WorldRunnable();
	ThreadPool.ExecuteTask(wr);

	string host,logonport,password;
	Config.MainConfig.GetString("LogonInfo","Adresse",&host);
	Config.MainConfig.GetString("LogonInfo","Port",&logonport);

	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

	HANDLE hThread = GetCurrentThread();

	uint32 realCurrTime, realPrevTime;
	realCurrTime = realPrevTime = getMSTime();
	// Socket loop!
	uint32 start;
	uint32 diff;
	uint32 last_time = now();
	uint32 etime;
	uint32 loop_counter = 0;
	uint32 next_printout = getMSTime(), next_send = getMSTime();

	 Network n;
	ConnectionRequestor<AuthLogon>* cr = n.createConnectionRequestor<AuthLogon>(host,logonport);

        ///- Run and wait for exit
	while( mrunning )
	{
		start = now();
		diff = start - last_time;
		if(! ((++loop_counter) % 10000) )		// 5mins
		{
			ThreadPool.ShowStats();
			ThreadPool.IntegrityCheck();//Checks if THREAD_RESERVE is met
		}

		/* since time() is an expensive system call, we only update it once per server loop */
		curTime = time(NULL);
		if( UNIXTIME != curTime )
		{
			UNIXTIME = time(NULL);
			g_localTime = *localtime(&curTime);
		}

		/* UPDATE */
		last_time = now();
		etime = last_time - start;
		if( 50 > etime )
		{
			WaitForSingleObject( hThread, 50 - etime );
		}
		if(cr->m_connected==false || cr->getConnection()->b_connected==false)
		{
			if(mrunning)
			{
				cr = n.createConnectionRequestor<AuthLogon>(host,logonport);
				if(mrunning)
					continue;
				else break;
			}
		}
		if(objmgr.registered && !objmgr.opened && objmgr.loaded)
		{
			n.createConnectionAcceptor<ClientCom>("0.0.0.0", objmgr.m_realm->Port,2);
			objmgr.opened=true;
		}

		if( m_Shutdown )
		{
			if( getMSTime() >= next_printout )
			{
				if(m_ShutdownTimer > 60000.0f)
				{
					if( !( (int)(m_ShutdownTimer) % 60000 ) )
						Log.Notice( "Server", "Shutdown in %i minutes.", (int)(m_ShutdownTimer / 60000.0f ) );
				}
				else
				{
					Log.Notice( "Server","Shutdown in %i seconds.", (int)(m_ShutdownTimer / 1000.0f ) );
					stringstream msg;
					msg << "Server Reboot in : " << m_ShutdownTimer/1000 << " sec !";
					objmgr.SendChatMessage(NULL,msg.str());
				}
					
				next_printout = getMSTime() + 1000;
			}

			if( getMSTime() >= next_send )
			{
				next_send = getMSTime() + 1000;
			}
			if( diff >= m_ShutdownTimer )
				break;
			else
				m_ShutdownTimer -= diff;
		}
	}

	Log.Notice("Config", "Removing All Players...");
	objmgr.RemoveAllPlayer();

	n.stop();
	wr->Terminate();
	Log.Notice("WorldServer","Closing.");


	sLog.outString("Closing DB : EndThreads.");
	CharacterDatabase.EndThreads();
	WorldDatabase.EndThreads();

	Log.Debug("Server", "Shutting down random generator.");
	CleanupRandomNumberGenerators();
	mapmgr.Shutdown();
	ThreadPool.Shutdown();

	Log.Notice("LogonServer","Shutdown complete.\n");
	return true;
}

void WorldServer::PrintMACaddress(unsigned char MACData[])
{
	Log.Info("SECURITY","MAC Address: %02X-%02X-%02X-%02X-%02X-%02X",MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
	Log.Success("SECURITY","Address Accepted !");
}
void WorldServer::GetMACaddress()
{
	unsigned char MACData[6];

	UUID uuid;
	UuidCreateSequential( &uuid );				// Ask OS to create UUID

	for (int i=2; i<8; ++i)						// Bytes 2 through 7 inclusive are MAC address
		MACData[i - 2] = uuid.Data4[i];

	PrintMACaddress(MACData);					// Print MAC address
}