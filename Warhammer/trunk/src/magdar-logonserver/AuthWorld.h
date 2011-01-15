#ifndef AUTHWORLD_H
#define AUTHWORLD_H

class Buffer;
class BufferPool;
class AuthClient;

/// GameServer connection management
class AuthWorld : public Connection
{
public:

    AuthWorld(boost::asio::io_service& IOService, BufferPool* hp);
    void start(void* param=NULL);

    ~AuthWorld();

	// Handler
	void HandleHello();
	void HandleRegister();
	void HandleCharInfo();
	void HandleAccountCheck();

	// Sender
	void SendAccountId(Account * Acct);
	void sendClient();

	Realm * m_realm;
private:

	void onDisconnect();
    void onRead(const boost::system::error_code& e);
    void onWrite(const boost::system::error_code& e);
    void writePacket(Buffer* b);
	uint32 readSize(uint32* bytesLeft);

    static uint32 connectionCount;

    boost::shared_ptr<AuthWorld> GetDerivedPointer()
    {
        return boost::dynamic_pointer_cast<AuthWorld>(shared_from_this());
    }

	bool m_expectData;
    uint16 m_expectedSize;
    uint8  m_opcode;

	bool m_authentified;
};



#endif