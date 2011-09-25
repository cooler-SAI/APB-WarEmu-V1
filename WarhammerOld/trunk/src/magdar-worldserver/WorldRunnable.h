#ifndef __WORLDRUNNABLE_H
#define __WORLDRUNNABLE_H

class WorldRunnable : public ThreadContext
{
public:
	WorldRunnable();
	bool run();

	private:
	EventableObjectHolder * eventholder;

};

#endif