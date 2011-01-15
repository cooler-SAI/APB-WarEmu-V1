#include "StdAfx.h"

initialiseSingleton(EventHook);


void EventHook::RegisterHook(SystemData::EventTypes type, void* function)
{
	Log.Debug("EventHook","Registering function for %u",type);
	return;
	M_hooks.Acquire();
	m_hooks[type].push_back(function);
	M_hooks.Release();

}
void EventHook::ExecuteHook(SystemData::EventTypes type,uint32 ParamCount,...)
{
	//Log.Debug("EventHook","Executing Function Event=%u , ParamCount=%u",type,ParamCount );
	return;
	if(m_hooks[type].size())
	{
		M_hooks.Acquire();

		va_list args;
		va_start(args,ParamCount);

		for(list<void*>::iterator itr = m_hooks[type].begin();itr != m_hooks[type].end();++itr)
		{
			((OnHook)*itr)(ParamCount,args);
		}

		va_end(args);
		
		M_hooks.Release();
	}
}
