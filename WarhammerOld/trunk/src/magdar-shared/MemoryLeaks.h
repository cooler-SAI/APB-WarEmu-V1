#ifndef SERVER_MEMORY_H
#define SERVER_MEMORY_H

#include "Common.h"
#include "Singleton.h"

struct MemoryManager : public Singleton < MemoryManager > {
	MemoryManager();
};

#endif

