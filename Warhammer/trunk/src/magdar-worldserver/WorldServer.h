#ifndef WORLDSERVER_H
#define WORLDSERVER_H


extern bool mrunning;

class WorldServer;
class Network;
class WorldServer : public Singleton< WorldServer >
{
public:
	bool Run(int argc, char ** argv);
	void Stop();

	// Prints the MAC address stored in a 6 byte array to stdout
	void PrintMACaddress(unsigned char MACData[]);
	// Fetches the MAC address and prints it
	void GetMACaddress();

	bool m_Shutdown;
	uint32 m_ShutdownTimer;
private:
	bool m_stopEvent;
};

#define sWorldServer WorldServer::getSingleton()


#endif