#include "LogonStdAfx.h"

uint32 AuthWorld::connectionCount = 0;

/// ConnectionCount Mutex
static boost::mutex AuthWorld_mutex;

typedef void (AuthWorld::*AuthHandler)();
static AuthHandler Handlers[LSWS_MAX_OPCODE] = {
		NULL,									// 0
		&AuthWorld::HandleHello,				// WS_CONNEXION_REALM
		NULL,									// LS_CONNEXION_RESPONSE
		NULL,									// 3
		NULL,									// 4
		&AuthWorld::HandleRegister,				// WS_REGISTER_REALM
		&AuthWorld::HandleCharInfo,				// LS_REGISTER_STATUT
		&AuthWorld::HandleAccountCheck,			// WS_ACCOUNT_CHECK =			0x07,
		NULL,									// LS_ACCOUNT_RESULT =			0x08,
};

/// Constructor
AuthWorld::AuthWorld(boost::asio::io_service& IOService, BufferPool* hp) : Connection(IOService,hp)
        , m_expectData(0)
        , m_expectedSize(0)
        , m_opcode(0)
{
	m_authentified=false;
    m_readBuffer.resize(900*10); ///\ todo temp fix for long IntercomOpcodes::GSLS_CHAR_LIST_RESPONSE
}

/// Accepts the connection and wait for information
void AuthWorld::start(void* startParam)
{
    AsyncRead();

    boost::mutex::scoped_lock lock(AuthWorld_mutex);
    m_connectionID = connectionCount;
    connectionCount++;
	b_connected=true;

   Log.Debug("AuthClient","Nouvelle AuthWorld connexion %u",m_connectionID);
}

/// Read data from the connection
void AuthWorld::onRead(const boost::system::error_code& e)
{
    if (!e)
    {
		Log.Notice("AuthWorld","Received %u bytes",m_readBuffer.size());

        uint32 bytesLeft = m_readBuffer.size();
        ///- While there is some bytes in the received buffer
        while (bytesLeft)
        {
            ///- Read the payload size and the opcode
            if (!m_expectData)
            {
                if (bytesLeft < 2)
                {
					Log.Error("AuthWorld","Cannot read packet size. Discarding packet.");
                    return;
                }
                m_readBuffer >> m_expectedSize;
                bytesLeft-=2;

                if (!bytesLeft)
                {
					Log.Error("AuthWorld","Cannot read opcode. Discarding packet.");
                    return;
                }
                m_readBuffer >> m_opcode;
                bytesLeft--;

                if (m_opcode > LSWS_MAX_OPCODE)
                {
					Log.Error("AuthWorld","Out of range opcode received by GSLS connection: %u ",(uint32)m_opcode);
                    return;
                }

                if ((InterOpcodeSize[m_opcode] !=  UNKNOWN_SIZE) && (InterOpcodeSize[m_opcode] != m_expectedSize))
                {
					Log.Warning("AuthWorld","Unexpected payload size received by GSLS connection for opcode %u .Received %u  instead of %u",(uint32)m_opcode,m_expectedSize,PacketSize[m_opcode]);
				}

				//Log.Notice("AuthWorld","Received GSLS Header. Opcode: %u  Payload size: %u",m_opcode,m_expectedSize);

                if (bytesLeft > m_expectedSize)
                {
					Log.Debug("AuthWorld","Packet contains multiple opcodes");
                }
                m_expectData = true;
            } //!m_expectedData
            else
            {
                ///- Call the appropriate handler
                m_expectData = false;
                if (bytesLeft >= m_expectedSize)
                {
                    // So few cases do not mandate a handler function pointers table.
					//Log.Notice("AuthWorld","Received GSLS Payload. Opcode: %u Playload size: %u",m_opcode,m_expectedSize);
                    m_readBuffer.pushReaderPos();

					if(m_opcode < LSWS_MAX_OPCODE && Handlers[m_opcode] != NULL)
						(this->*Handlers[m_opcode])();
					else
						Log.Notice("AuthSocket", "Unknown opcode %u", m_opcode);

                    bytesLeft-= m_expectedSize;
                    // Whatever the handler reads (or not), we follow the sender logic.
                    m_readBuffer.popReaderPos();
                    m_readBuffer.skip(m_expectedSize);
                }
                else
                {
					Log.Error("AuthWorld","Did not receive the correct amount of data... Remains %u in the packet instead of %u",bytesLeft,m_expectedSize);
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
			Log.Debug("AuthWorld","Connection closed (%s)",e.message().c_str());
        }
        else
        {
			Log.Debug("AuthWorld","Socket read problem (%s)",e.message().c_str());
        }
        disconnect();
    }
}
/// Read 7Bits encoded packet size
uint32 AuthWorld::readSize(uint32* bytesLeft)
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
/// Report error after a failed write
void AuthWorld::onWrite(const boost::system::error_code& e)
{

}

/// Send a packet in the connection
void AuthWorld::writePacket(Buffer* b)
{
    AsyncWrite(b);
    m_bufferPool->disposeBuffer(b);
}
void AuthWorld::HandleHello()
{
	string req_password;
	m_readBuffer >> req_password;
	uint16 responseSize = InterOpcodeSize[LS_CONNEXION_RESPONSE];
    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(LS_CONNEXION_RESPONSE);



	if( req_password.size() && sInfoCore.m_password.size() ) // Si il y a un pass on check
		if( sInfoCore.m_password.compare( req_password ) == 0 )
			m_authentified=true;

		if(m_authentified)
			b->write<uint32>( 0x0001 );
		else 
			b->write<uint32>( 0x0000 );

    writePacket(b);
}
void AuthWorld::HandleRegister()
{
	Log.Debug("AuthWorld","HandleRegister()");
	if(!m_authentified)
	{
		Log.Error("AuthWorld","Non authentified Register Attemp !");
		return;
	}
	Realm * realm = new Realm;
	realm->connected = true;

	m_readBuffer >> realm->Adresse;
	m_readBuffer >> realm->Language;
	m_readBuffer >> realm->Legacy;
	m_readBuffer >> realm->Name;
	m_readBuffer >> realm->OpenRVR;
	m_readBuffer >> realm->Port;
	m_readBuffer >> realm->RealmID;
	m_readBuffer >> realm->Region;
	m_readBuffer >> realm->RP;
	m_readBuffer >> realm->Transfert;
	realm->Auth = this;

	m_realm = realm;
	sendClient();
	sInfoCore.SetRealm(realm);
}
void AuthWorld::sendClient()
{
	/*map<uint32,bool> m_already;
	for(list<AuthClient*>::iterator itr = sInfoCore.m_clients.begin();itr!=sInfoCore.m_clients.end();itr++)
	{
		if( !(*itr) ) continue;
		if( !(*itr)->m_username.size() ) continue;
		if(m_already[(*itr)->m_accountID]) continue;
		if(m_charinfo[ Acct->AccountId() ].size())
			return;
		SendAccountId((*itr)->m_acct);
		m_already[(*itr)->m_accountID] = true;
	}*/
}
void AuthWorld::SendAccountId(Account * Acct)
{
	Log.Debug("AuthWorld","SendAccountId(%u)",Acct->AccountId);

	uint16 responseSize = 6+(Acct->Username.length()+4)+(4+Acct->Password.length());

    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(LS_REGISTER_STATUT);

	// accountid + 4 + lenght;
	b->write<uint32>( Acct->AccountId );
	b->write<uint8>( Acct->Permission );
	b->write<uint8>( Acct->locked );
	b->write( Acct->Username );
	b->write( Acct->Password );

    writePacket(b);
}
void AuthWorld::HandleCharInfo()
{
	Log.Debug("AuthWorld","HandleCharInfo()");
	uint8 size=0;
	uint32 accountid;
	m_readBuffer >> accountid >> size;
	Log.Debug("AuthWorld","Account [%u] : [%u] Characters loading.",accountid,size);
	list<CharInfo*> m_list;
	for(uint32 i=0;i<size;i++)
	{
		CharInfo * Info = new CharInfo;
		uint32 done=0;
		m_readBuffer >> Info->character_id;
		m_readBuffer >> Info->career;
		m_readBuffer >> Info->heldleft;
		m_readBuffer >> Info->last_time_played;
		m_readBuffer >> Info->level;
		m_readBuffer >> Info->model;
		m_readBuffer >> Info->name;
		m_readBuffer >> Info->race;
		m_readBuffer >> Info->realm;
		m_readBuffer >> Info->ServerID;
		m_readBuffer >> Info->zone;
		m_readBuffer >> Info->equipped_1;
		m_readBuffer >> done;

		m_list.push_back( Info );
		Log.Debug("sInfoCore","Registering Char : [%u]-[%s][%u]-[%u]",Info->character_id,Info->name.c_str(),Info->ServerID,Info->realm);
	}

	sInfoCore.AddCharInfo(accountid,m_list);
}
void AuthWorld::onDisconnect() 
{ 
	m_realm->connected=false;
	m_realm->Auth = NULL;
	sInfoCore.SetRealm(m_realm);
}
void AuthWorld::HandleAccountCheck()
{
	Log.Debug("AuthWorld","Handle Account Check");

	string username;
	string password;
	m_readBuffer >> username >> password;

	Log.Debug("AuthWorld","Account=%s Pass=%s",username.c_str(),password.c_str());

	Account * Acct = sAccountMgr.GetAccount(username);

	uint16 responseSize = username.length()+4+1+4+1;

	Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(LS_ACCOUNT_RESULT);

	b->write(username);
	if(Acct && !Acct->locked && Acct->Password == password)
	{
		b->write<uint8>(1);
		b->write<uint32>(Acct->AccountId);
		b->write<uint8>(Acct->Permission);
	}
	else b->write<uint8>(0);

	writePacket(b);
}
// tries to remove the server form the list on disconnection
AuthWorld::~AuthWorld()
{


}

