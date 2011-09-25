#ifndef AUTHWORLD_H
#define AUTHWORLD_H

class Buffer;
class BufferPool;

/// GameServer connection management
class AuthLogon : public Connection
{
public:

    AuthLogon(boost::asio::io_service& IOService, BufferPool* hp);
    void start(void* param=NULL);

    ~AuthLogon();

	// Sended Packet
	void SendHello();
	void SendRegister();
	void SendAccountCheck(string username,string password);
	void SendAccountCharInfo(uint32 AccountID);

	// Handler
	void HandleHello();
	void HandleRegister();
	void HandleAccountList();
	void HandleAccountCheck();

private:

	void onDisconnect() { b_connected=false; };
    void onRead(const boost::system::error_code& e);
    void onWrite(const boost::system::error_code& e);
    void writePacket(Buffer* b);

    static uint32 connectionCount;

    boost::shared_ptr<AuthLogon> GetDerivedPointer()
    {
        return boost::dynamic_pointer_cast<AuthLogon>(shared_from_this());
    }

	bool m_expectData;
    uint16 m_expectedSize;
    uint8  m_opcode;
};



#endif