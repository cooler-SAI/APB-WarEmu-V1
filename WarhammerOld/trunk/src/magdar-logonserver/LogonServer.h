#ifndef _LOGON_SERVER_H
#define _LOGON_SERVER_H

extern bool mrunning;

class LogonServer;
class Network;
class LogonServer : public Singleton< LogonServer >
{
public:
	void Run(int argc, char ** argv);
	void Stop();

private:
	bool m_stopEvent;
};

#endif