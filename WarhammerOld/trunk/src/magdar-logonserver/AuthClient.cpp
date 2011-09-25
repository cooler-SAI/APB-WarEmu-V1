#include "LogonStdAfx.h"

static boost::mutex AuthConnectionCount_mutex;

uint32 AuthClient::connectionCount = 0;

/// Constructor
AuthClient::AuthClient(boost::asio::io_service& IOService, BufferPool* hp)
        :Connection(IOService,hp)
        , m_expectData(false)
        , m_expectedSize(0)
        , m_opcode(0)
        , m_IOService(IOService)
        , m_charRequested(false)
{
}

/// Accept the connection and wait for information
void AuthClient::start(void* param)
{
    AsyncRead();

    boost::mutex::scoped_lock lock(AuthConnectionCount_mutex);
    m_connectionID = connectionCount;
    connectionCount++;

	Log.Debug("AuthClient","New Client %u [%s]",m_connectionID, GetStringIp().c_str());
}
typedef void (AuthClient::*AuthHandler)();
static AuthHandler Handlers[MAX_OPCODE] = {
		NULL,									// 0
		&AuthClient::onConnect,					// CMSG_CONNECT
		NULL,									// SMSG_CONNECT_RESPONSE
		&AuthClient::onUserLogin,				// CMSG_USER_LOGIN
		NULL,									// SMSG_USER_LOGIN_RESPONSE
		&AuthClient::onAuthenticate,			// CMSG_AUTHENTICATE
		NULL,									// SMSG_AUTHENTICATE_RESPONSE
		&AuthClient::onGetServerList,			// CMSG_GET_SERVER_LIST
		NULL,									// SMSG_GET_SERVER_LIST_RESPONSE
		&AuthClient::onGetCharacterSummaries,	// CMSG_GET_CHARACTER_SUMMARIES
		NULL,									// SMSG_GET_CHARACTER_SUMMARIES_RESPONSE
		&AuthClient::onRefreshAuthentication,	// 11
		NULL,									// 12
		NULL,	// 13
		NULL,									// 14
		&AuthClient::onLoginClearence,			// CMSG_LOGIN_CLEARANCE
		NULL,									// SMSG_LOGIN_CLEARANCE_RESPONSE
		&AuthClient::onGetProductAccountPropertyList,	// CMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST
};
/// Read data from the connection
void AuthClient::onDisconnect()
{ 
	Log.Debug("AuthClient","Client Disconnectig [%u]",m_connectionID);
	sInfoCore.RemoveClient(this); 
}
void AuthClient::onRead(const boost::system::error_code& e)
{
    if (!e)
    {
        //LOG_TRACE(logger, "[ID: " << m_connectionID << "] Received " << m_readBuffer.size() << " bytes");

        uint32 bytesLeft = m_readBuffer.size();

        ///- While there is some bytes in the received buffer
        while (bytesLeft)
        {
            ///- Read the payload size and the opcode
            if (!m_expectData)
            {
                m_expectedSize = readSize(&bytesLeft);
                if (!m_expectedSize)
                {
                    //LOG_ERROR(logger, "[ID: " << m_connectionID << "] Cannot read packet size. Discarding packet.");
                    return;
                }
                if (!bytesLeft)
                {
                    //LOG_ERROR(logger, "[ID: " << m_connectionID << "] Cannot read opcode. Discarding packet.");
                    return;
                }
                m_readBuffer >> m_opcode;
                bytesLeft--;

                if (m_opcode > MAX_OPCODE)
                {
                    //LOG_ERROR(logger,"[ID: " << m_connectionID << "] Out of range opcode received by AUTH connection: "<< static_cast<int>(m_opcode) << ". Discarding packet.");
                    return;
                }

                if ((PacketSize[m_opcode] !=  UNKNOWN_SIZE) && (PacketSize[m_opcode] != m_expectedSize))
                {
                    //LOG_WARN(logger,"[ID: " << m_connectionID << "] Unexpected payload size received by AUTH connection for opcode " << static_cast<int>(m_opcode) << ". Received " << m_expectedSize << " instead of " << PacketSize[m_opcode]);
                }

                //LOG_TRACE(logger, "[ID: " << m_connectionID << "] Received AUTH Header. Opcode: " << static_cast<int>(m_opcode) << " Payload size: " << m_expectedSize);

                if (bytesLeft > m_expectedSize)
                {
                    //LOG_DEBUG(logger, "Packet contains multiple opcodes");
                }
                m_expectData = true;
            } //!m_expectedData
            else
            {
                ///- Read the payload and call the appropriate handler
                m_expectData = false;
                if (bytesLeft >= m_expectedSize)
                {
                    //LOG_TRACE(logger,"[ID: " << m_connectionID << "] Received AUTH Payload. Opcode: " << static_cast<int>(m_opcode) << " size: " << m_expectedSize);
                    m_readBuffer.pushReaderPos();
					if(m_opcode < MAX_OPCODE && Handlers[m_opcode] != NULL)
						(this->*Handlers[m_opcode])();
					else
						Log.Notice("AuthSocket", "Unknown cmd %u", m_opcode);

                    bytesLeft-= m_expectedSize;
                    // Whatever the handler reads (or not), we follow the sender logic.
                    m_readBuffer.popReaderPos();
                    m_readBuffer.skip(m_expectedSize);
                }
                else
                {
                    //LOG_ERROR(logger, "[ID: " << m_connectionID << "] Did not receive the correct amount of data... Remains: " << bytesLeft << " instead of " << m_expectedSize<< ". Discarding packet.");
                    return;
                }
            } //m_expectedData
        } // while bytesLeft
        AsyncRead();
    } //!e
    else
    {
        if (e.value() == 2)
        {
            //LOG_DEBUG(logger,"[ID: " << m_connectionID << "] Connection closed (" << e.message() << ")");
        }
        else
        {
            //LOG_ERROR(logger,"[ID: " << m_connectionID << "] Socket read problem (" << e.message() << ")");
        }
        disconnect();
    }
}

/// Report error after a failed write
void AuthClient::onWrite(const boost::system::error_code& e)
{

}

/// Read 7Bits encoded packet size
uint32 AuthClient::readSize(uint32* bytesLeft)
{
    uint32 mSize = 0;
    int mByteCount = 0;
    uint8 mByte = 0;

    m_readBuffer >> mByte;
    (*bytesLeft)--;
    while (mByte & 0x80)
    {
        mByte ^= 0x80;
        mSize = (mSize | (mByte << (7 * mByteCount)));

        if (!(*bytesLeft))
            return 0;

        m_readBuffer >> mByte;
        (*bytesLeft)--;
        mByteCount++;
    }
    mSize = (mSize | (mByte << (7 * mByteCount)));
    return mSize;
}

/// Send a packet in the connection
void AuthClient::writePacket(Buffer* b, int opcode)
{
    ///- Prepare the packet header
    //We can't tell how big the header packet is going to be, make it 5 bytes for now, resize later
    Buffer* bHeader = m_bufferPool->allocateBuffer(5);
    size_t itemcount=1;
    unsigned long long sz = b->size();
    while (sz > 0x7f)
    {
        bHeader->write<uint8>(static_cast<uint8>(sz) | 0x80);
        sz >>= 7;

        itemcount++;
        if (itemcount >= bHeader->capacity())
            bHeader->resize(bHeader->capacity()+10);
    }
    bHeader->write<uint8>(static_cast<uint8>(sz));
    bHeader->write<uint8>(opcode);

		bHeader->tracePrint(0);
		b->tracePrint(0);
    ///- Send the packet
    AsyncWrite(bHeader,bHeader->size());
    AsyncWrite(b,b->size());
    m_bufferPool->disposeBuffer(bHeader);
    m_bufferPool->disposeBuffer(b);
}

////////////////////////////////////////////////////////////
// Handlers
////////////////////////////////////////////////////////////

std::string AuthClient::generateKeyBits()
{
    uint8 keyBits[24];
    uint8 X1[8];
    uint8 K3[8];
    uint8 X2[8];
    Utils::asciiHexStringtoByteArray(m_strX2,X2);

    srand(int(time(NULL)));
    for (int i=0; i<8; i++)
    {
        X1[i] = rand() % 256;
        K3[i] = rand() % 256;

        keyBits[i] = X2[i] ^K3[i];
        keyBits[i+8] = X1[i] ^ K3[i];
        keyBits[16+i] = K3[i];
    }
    m_strX1 = Utils::byteArrayToAsciiString(X1,8);
    m_strK3 = Utils::byteArrayToAsciiString(K3,8);
    
    return Utils::byteArrayToAsciiString(keyBits,24);
}


AuthClient::~AuthClient()
{

}
void AuthClient::onConnect()
{
	m_readBuffer.tracePrint(0);
    uint32 sequence;
    uint32 unknown1;
    m_readBuffer >> sequence >> unknown1;

    Buffer* b = m_bufferPool->allocateBuffer(PacketSize[SMSG_CONNECT_RESPONSE]);
    b->write<uint32>(sequence);
    b->write<uint16>(0x0000);
    b->write<uint32>(unknown1); // No other behavior detected by changing this value

    writePacket(b,SMSG_CONNECT_RESPONSE);
}
void AuthClient::onUserLogin()
{
    uint32 sequence;
    uint16 unknown1;

	m_readBuffer >> sequence;

	Log.Error("AuthClient","Old Launcher Used for connection ! [%s]", GetStringIp().c_str());
    // Differs from real behavior. The LoginServer sends back to KeyBits whatever the username.
    Buffer* b = m_bufferPool->allocateBuffer(PacketSize[SMSG_USER_LOGIN_RESPONSE]);
    b->write<uint32>(sequence);
    b->write<uint16>(0x0001);
    b->write<uint32>(0x00000018);
    b->fill(0, 0x18);
    writePacket(b, SMSG_USER_LOGIN_RESPONSE);
	disconnect();
}
void AuthClient::onAuthenticate()
{
	Log.Debug("AuthClient","onAuthenticate()");

    uint32 sequence;
    std::string sAuthHash;
	std::string username;

    AuthResult result = AUTH_ACCT_BAD_USERNAME_PASSWORD;

    m_readBuffer >> sequence >> username >> sAuthHash;

	if(!m_username.size())
		m_username = username;

	Log.Debug("AuthClient","Acct = [%s]",m_username.c_str());
	Log.Debug("AuthClient","Pass = [%s]",sAuthHash.c_str());

	Account* Acct = sAccountMgr.GetAccount(m_username);

	if(Acct && Acct->Password == sAuthHash)
	{
		m_acct=Acct;
		m_accountID = m_acct->AccountId;
		result =  AUTH_SUCCESS;
		sLogonSQL->Execute("UPDATE accounts SET last_login=now() WHERE id=%u",m_accountID);
		if(m_acct && m_acct->locked)
			result=AUTH_ACCT_SUSPENDED;
	}
	else
	{
		result =  AUTH_ACCT_BAD_USERNAME_PASSWORD;
		if(Acct)
			sLogonSQL->Execute("UPDATE accounts SET failed_login_attempts=failed_login_attempts+1 WHERE id=%u",Acct->AccountId);
	}

	Log.Debug("AuthClient","onAuthenticate() [%s] Result=%u",username.c_str(),result);

    Buffer* b = m_bufferPool->allocateBuffer(PacketSize[SMSG_AUTHENTICATE_RESPONSE]);
    b->write<uint32>(sequence);
    b->write<uint16>(result);
    b->write<uint8>(0x00);			//Stray extra byte... could be something useful...
    writePacket(b,SMSG_AUTHENTICATE_RESPONSE);

	if(result == AUTH_SUCCESS)
		m_acct = sAccountMgr.GetAccount(m_username);
	else disconnect();
}
void AuthClient::onLoginClearence()
{
	Log.Debug("AuthClient","OnLoginClearence()");

	uint32 sequence;
	m_readBuffer >> sequence;

    Buffer* b = m_bufferPool->allocateBuffer(16);
	b->write<uint32>(sequence);
    b->write<uint32>(0);
	b->write<uint16>(1);
	b->write<uint16>(0x1);
	b->write<uint32>(1);
    writePacket(b,SMSG_LOGIN_CLEARANCE_RESPONSE);
}
void AuthClient::onTokenAuthenticate()
{
	Log.Debug("AuthClient","onTokenAuthenticate()");
    uint32 sequence;
	m_readBuffer >> sequence;

    /*std::string sAuthHash;
    TokenAuthResult result = TOK_USER_NOT_FOUND;
	m_readBuffer.Trace();
    m_readBuffer >> sequence >> sAuthHash;

	bool check = sAccountMgr.checkTokenLogin(m_username, sAuthHash, m_strK3);

    Buffer* b = m_bufferPool->allocateBuffer(PacketSize[SMSG_TOKEN_AUTHENTICATE_RESPONSE]);
    b->write<uint32>(sequence);
	check ? result = TOK_SUCCESS : result = TOK_AUTH_FAILED;
	if(m_acct && m_acct->locked)
			result=TOK_ACCT_SUSPENDED;
    b->write<uint16>(static_cast<uint16>(result));		//Successful login
    b->write<uint8>(0x00);
    writePacket(b,SMSG_TOKEN_AUTHENTICATE_RESPONSE);

	Log.Debug("AuthClient","onTokenAuthenticate() %u",check);

	if(!check) return;

	Account * Acct = sAccountMgr.GetAccount( m_username );
	if(Acct) 
	{
		m_accountID = Acct->AccountId;
		m_permission = Acct->Permission;
		sInfoCore.AddClient(this);
	}*/
}
void AuthClient::onGetProductAccountPropertyList()
{
	Log.Debug("AuthClient","onGetProductAccountPropertyList()");

    uint32 sequence;
    m_readBuffer >> sequence;
    Buffer* b = m_bufferPool->allocateBuffer(28);
    b->write<uint32>(sequence);
    b->write<uint16>(0x0000);
    b->write<uint32>(0x00000003); // number of properties
    b->write<uint16>(0x001c);     // First property ??
    b->write<uint32>(0x00000001); // Value??
    b->write<uint16>(0x001d);     // Second property ??
    b->write<uint32>(0x00000001); //Value??
    b->write<uint16>(0x0035);     // Third property ??
    b->write<uint32>(0x00000001); //Value??

    // 0x3e = trial account
    //Notes: Sending an empty list also works. Sending property values of 0 also works... to be figured out.
    writePacket(b,SMSG_GET_PRODUCT_ACCOUNT_PROPERTY_LIST_RESPONSE);
}
void AuthClient::onGetServerList()
{
	if(m_acct && m_acct->AccountId)
		sInfoCore.SendAccountToRealm(m_acct);

	Log.Debug("AuthClient","onGetServerList()  [%s]",m_username.c_str());
	uint32 sequence;
    m_readBuffer >> sequence;
    Buffer* b = m_bufferPool->allocateBuffer(10000);
    b->write<uint32>(sequence);
    b->write<uint16>(0x0000);

	if( sInfoCore.GetRealmCount() > 0 )
		sInfoCore.SendRealm(b);
	else 
		b->write<uint32>( 0x0000 );

	writePacket(b,SMSG_GET_SERVER_LIST_RESPONSE);
}
void AuthClient::onGetCharacterSummaries()
{
	uint32 sequence;
	m_readBuffer >> sequence;
	sInfoCore.SendCharList(sequence,this);
}
void AuthClient::onRefreshAuthentication()
{
	Log.Debug("AuthClient","onRefreshAuthentication()  [%s]",m_username.c_str());

    uint32 sequence;
    m_readBuffer >> sequence;
    Buffer* b = m_bufferPool->allocateBuffer(PacketSize[SMSG_REFRESH_AUTHENTICATION_RESPONSE]);
    b->write<uint32>(sequence);
    b->write<uint8>(0x00);
    writePacket(b,SMSG_REFRESH_AUTHENTICATION_RESPONSE);

}