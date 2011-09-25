#ifndef WORLDSOCKET_H
#define WORLDSOCKET_H

class Buffer;
class Mutex;
class ClientCom;

struct PackToGet
{
	uint8 opcode;
	ClientCom * com;
	Buffer * packet;
};

struct PackToSend
{
	ClientCom * com;
	Buffer* packet;
	bool dispose;
};

class WorldSocket : public Singleton< WorldSocket >
{
public :
	WorldSocket() { };

	void Update();

	__forceinline void QueuePacket(PackToGet* ptg)
	{
		_packets.Push(ptg);
	}

	__forceinline void QueuePacketToSend(PackToSend* pts)
	{
		_tosend.Push(pts);
	}

	FastQueue<PackToGet*, Mutex> _packets;
	FastQueue<PackToSend*, Mutex> _tosend;
};

#define sWorldSocket WorldSocket::getSingleton( )


#endif