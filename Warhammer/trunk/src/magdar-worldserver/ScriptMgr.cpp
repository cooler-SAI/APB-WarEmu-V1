#include "StdAfx.h"

initialiseSingleton(ScriptMgr);

typedef void(*exp_script_register)(ScriptMgr * mgr);
struct ScriptingEngine
{
	HMODULE Handle;
	exp_script_register InitializeCall;
};

void ScriptMgr::LoadScript()
{
	Log.Notice("ScriptMgr","Loading External Script Libraries..." );
	int32 bcklvl = Log.log_level;
	Log.log_level = 3;
	string start_path = "script\\";
	string search_path = start_path + "*.";

	vector< ScriptingEngine > ScriptEngines;

	/* Loading system for win32 */
	search_path += "dll";

	WIN32_FIND_DATA data;
	uint32 count = 0;
	HANDLE find_handle = FindFirstFile( search_path.c_str(), &data );
	if(find_handle == INVALID_HANDLE_VALUE)
		Log.Error("ScriptMgr","No external scripts found! Server will start up with limited functionality." );
	else
	{
		do
		{
			string full_path = start_path + data.cFileName;
			HMODULE mod = LoadLibrary( full_path.c_str() );
			if( mod == 0 )
				Log.Error("ScriptMgr","Loading %s failed, crc=0x%p", data.cFileName, reinterpret_cast< uint32* >( mod ));
			else
			{	
				std::stringstream cmsg; 
				cmsg << "Loading " << data.cFileName << ", crc:0x" << reinterpret_cast< uint32* >( mod );

				// find version import
				exp_script_register rcall = (exp_script_register)GetProcAddress(mod, "_exp_script_register");
				if(rcall == 0)
				{
					Log.Error("ScriptMgr","Loading %s failed, version info not found", data.cFileName);
					FreeLibrary(mod);
				}
				else
				{
						_handles.push_back(((void*)mod));
						rcall(this);;
						Log.Success("ScriptMgr",cmsg.str().c_str());
						++count;
				}
			}
		}
		while(FindNextFile(find_handle, &data));
		FindClose(find_handle);
		Log.Notice("ScriptMgr","Loaded %u external libraries.", count);
		Log.Success("ScriptMgr","Done loading script engines...");
	}

}
void ScriptMgr::RegisterAiScript(std::string name,pAiScript callback)
{
	m_AiScript[name] = callback;
}
void ScriptMgr::RegisterItemScript(std::string name,pItemScript Isc)
{
	m_ItmScript[name] = Isc;
}
ScriptedAI * ScriptMgr::GetAiScript(std::string name,Creature * Crea)
{
	if(!name.size())
		return NULL;

	pAiScript function_ptr = NULL;
	if(m_AiScript[name])
		function_ptr= m_AiScript[name];
	else return NULL;

	return (function_ptr)(Crea);
}
ScriptedItem * ScriptMgr::GetItmScript(std::string name,Player * Plr,Item * itm)
{
	if(!name.size())
		return NULL;

	pItemScript function_ptr = NULL;
	if(m_ItmScript[name])
		function_ptr= m_ItmScript[name];
	else return NULL;

	return (function_ptr)(Plr,itm);
}
PQScript * ScriptMgr::GetPQScript(uint32 entry,PQuest * Quest)
{
	if(!entry)
		return NULL;

	pPQScript function_ptr = NULL;
	if(m_PQScript[entry])
		function_ptr= m_PQScript[entry];
	else return NULL;

	return (function_ptr)(Quest);
}
void ScriptMgr::RegisterQuestScript(uint32 entry,ScriptedQuest * Sc)
{
	QuestInfo * Info = questmgr.GetQuestInfo(entry);
	if(Info)
		Info->m_script = Sc;

	m_QuestScript.insert( Sc );
}
void ScriptMgr::RegisterPQuestScript(uint32 entry,pPQScript Sc)
{
	if(!entry) return;

	m_PQScript[entry] = Sc;
}