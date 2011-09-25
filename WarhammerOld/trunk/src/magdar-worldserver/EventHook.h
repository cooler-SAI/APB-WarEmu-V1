#ifndef EVENTHOOK_H
#define EVENTHOOK_H

typedef map<SystemData::EventTypes, list<void*> > HookList;

#define F_UNIT "U"
#define F_OBJECT "O"
#define F_PLAYER "P"
#define F_UINT32 "u"


class SERVER_DECL EventHook : public Singleton<EventHook>
{
public:
	typedef void(*OnHook)(uint32 ParamCount,va_list args);

	void RegisterHook(SystemData::EventTypes type, void* function);
	void ExecuteHook(SystemData::EventTypes type,uint32 ParamCount,...);
private:

	// Register function for specialtype
	Mutex M_hooks;
	HookList m_hooks;
};

#define sEventHook EventHook::getSingleton()

#endif