#ifndef __EVENTMGR_H
#define __EVENTMGR_H

using namespace SystemData;

enum EventFlags
{
	EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT	= 0x1,
	EVENT_FLAG_DELETES_OBJECT					= 0x2,
	EVENT_FLAG_FIRE_ON_DELETE					= 0x4,
};

class EventableObject;
struct SERVER_DECL TimedEvent
{
	TimedEvent(EventableObject * object, CallbackBase* callback, uint32 type, time_t time, uint32 repeat, uint32 flags) : 
		obj(object), cb(callback), eventType(type), eventFlag(flags), msTime(time), currTime(time), repeats(repeat), deleted(false),ref(0) {}
		
	EventableObject* obj;
	CallbackBase *cb;
	uint32 eventType;
	uint16 eventFlag;
	time_t msTime;
	time_t currTime;
	uint16 repeats;
	bool deleted;
	int instanceId;
	volatile long ref;

	static TimedEvent * Allocate( EventableObject * object, CallbackBase* callback, uint32 flags, time_t time, uint32 repeat);

#ifdef WIN32
	void DecRef()
	{
		InterlockedDecrement(&ref);
		if(ref <= 0)
		{
			delete cb;
			delete this;
		}
	}

	void IncRef() { InterlockedIncrement(&ref); }
#else

	void IncRef() 
	{
		__sync_add_and_fetch(&ref, 1); 
	}

	void DecRef()
	{
		__sync_sub_and_fetch(&ref, 1);

		if( ref <= 0 )
		{
			delete cb;
			delete this;
		}
	}

#endif

};

class EventMgr;
class EventableObjectHolder;
typedef map<int32, EventableObjectHolder*> HolderMap;

class SERVER_DECL EventMgr : public Singleton < EventMgr >
{
	friend class MiniEventMgr;
public:
	template <class Class>
		void AddEvent(Class* obj, void (Class::*method)(), uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP0<Class>(obj, method), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1>
		void AddEvent(Class* obj, void (Class::*method)(P1), P1 p1, uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP1<Class, P1>(obj, method, p1), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2>
		void AddEvent(Class* obj, void (Class::*method)(P1,P2), P1 p1, P2 p2, uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP2<Class, P1, P2>(obj, method, p1, p2), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3>
		void AddEvent(Class* obj,void (Class::*method)(P1,P2,P3), P1 p1, P2 p2, P3 p3, uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP3<Class, P1, P2, P3>(obj, method, p1, p2, p3), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4>
		void AddEvent(Class* obj, void (Class::*method)(P1,P2,P3,P4), P1 p1, P2 p2, P3 p3, P4 p4, uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP4<Class, P1, P2, P3, P4>(obj, method, p1, p2, p3, p4), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4, typename P5>
		void AddEvent(Class* obj, void (Class::*method)(P1,P2,P3,P4,P5), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP5<Class, P1, P2, P3, P4, P5>(obj, method, p1, p2, p3, p4, p5), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class> void RemoveEvents(Class* obj) { obj->event_RemoveEvents(-1); }
	template <class Class> void RemoveEvents(Class* obj, int32 type)
	{
		obj->event_RemoveEvents(type);
	}

	template <class Class> void ModifyEventTimeLeft(Class* obj, uint32 type, uint32 time,bool unconditioned=true)
	{
		obj->event_ModifyTimeLeft(type, time,unconditioned);
	}

	template <class Class> void ModifyEventTimeAndTimeLeft(Class* obj, uint32 type, uint32 time)
	{
		obj->event_ModifyTimeAndTimeLeft(type, time);
	}

	template <class Class> void ModifyEventTime(Class* obj, uint32 type, uint32 time)
	{
		obj->event_ModifyTime(type, time);
	}

	template <class Class> bool HasEvent(Class* obj, uint32 type)
	{
		return obj->event_HasEvent(type);
	}

	EventableObjectHolder * GetEventHolder(int32 InstanceId)
	{
		HolderMap::iterator itr = mHolders.find(InstanceId);
		if(itr == mHolders.end()) return 0;
		return itr->second;
	}

	void AddEventHolder(EventableObjectHolder * holder, int32 InstanceId)
	{
		holderLock.Acquire();
		mHolders.insert( HolderMap::value_type( InstanceId, holder) );
		holderLock.Release();
	}

	void RemoveEventHolder(int32 InstanceId)
	{
		holderLock.Acquire();
		mHolders.erase(InstanceId);
		holderLock.Release();
	}

	void RemoveEventHolder(EventableObjectHolder * holder)
	{
		holderLock.Acquire();
		HolderMap::iterator itr = mHolders.begin();
		for(; itr != mHolders.end(); ++itr)
		{
			if(itr->second == holder)
			{
				mHolders.erase(itr);
				holderLock.Release();
				return;
			}
		}
		holderLock.Release();
	}

protected:
	HolderMap mHolders;
	Mutex holderLock;
};

#define sEventMgr EventMgr::getSingleton()

#endif
