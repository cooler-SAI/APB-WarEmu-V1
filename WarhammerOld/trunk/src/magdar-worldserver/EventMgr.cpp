#include "StdAfx.h"

initialiseSingleton( EventMgr );

TimedEvent * TimedEvent::Allocate(EventableObject* object, CallbackBase* callback, uint32 flags, time_t time, uint32 repeat)
{
	return new TimedEvent(object, callback, flags, time, repeat, 0);
}