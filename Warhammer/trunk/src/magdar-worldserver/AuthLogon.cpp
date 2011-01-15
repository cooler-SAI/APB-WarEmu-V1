#include "StdAfx.h"

uint32 AuthLogon::connectionCount = 0;

/// ConnectionCount Mutex
static boost::mutex AuthLogon_mutex;

typedef void (AuthLogon::*AuthHandler)();
static AuthHandler Handlers[LSWS_MAX_OPCODE] = {
		NULL,									// 0
		NULL,									// WS_CONNEXION_REALM
		&AuthLogon::HandleHello,				// LS_CONNEXION_RESPONSE
		NULL,									// 3
		NULL,									// 4
		NULL,									// 5
		&AuthLogon::HandleAccountList,			// LS_REGISTER_STATUT
		NULL,									// WS_ACCOUNT_CHECK =			0x07,
		&AuthLogon::HandleAccountCheck,		// LS_ACCOUNT_RESULT =			0x08,
};

/// Constructor
AuthLogon::AuthLogon(boost::asio::io_service& IOService, BufferPool* hp) : Connection(IOService,hp)
        , m_expectData(0)
        , m_expectedSize(0)
        , m_opcode(0)
{
    m_readBuffer.resize(900*10); ///\ todo temp fix for long IntercomOpcodes::GSLS_CHAR_LIST_RESPONSE
}

/// Accepts the connection and wait for information
void AuthLogon::start(void* startParam)
{
	 AsyncRead();
	 Log.Info("AuthLogon","World Server connected to Login Server");

	 SendHello();
}

/// Read data from the connection
void AuthLogon::onRead(const boost::system::error_code& e)
{
    if (!e)
    {
        uint32 bytesLeft = m_readBuffer.size();

        ///- While there is some bytes in the received buffer
        while (bytesLeft)
        {
            ///- Read the payload size and the opcode
            if (!m_expectData)
            {
                if (bytesLeft < 2)
                {
					Log.Error("AuthLogon","Cannot read packet size. Discarding packet.");
                    return;
                }
                m_readBuffer >> m_expectedSize;
                bytesLeft-=2;

                if (!bytesLeft)
                {
					Log.Error("AuthLogon","Cannot read opcode. Discarding packet.");
                    return;
                }
                m_readBuffer >> m_opcode;
                bytesLeft--;

                if (m_opcode > LSWS_MAX_OPCODE)
                {
					Log.Error("AuthLogon","Out of range opcode received by GSLS connection: %u ",(uint32)m_opcode);
                    return;
                }

                if ((InterOpcodeSize[m_opcode] !=  UNKNOWN_SIZE) && (InterOpcodeSize[m_opcode] != m_expectedSize))
                {
					Log.Warning("AuthLogon","Unexpected payload size received by GSLS connection for opcode %u .Received %u  instead of %u",(uint32)m_opcode,m_expectedSize,InterOpcodeSize[m_opcode]);
				}

				//Log.Notice("AuthLogon","Received GSLS Header. Opcode: %u  Payload size: %u",m_opcode,m_expectedSize);

                if (bytesLeft > m_expectedSize)
                {
					Log.Debug("AuthLogon","Packet contains multiple opcodes");
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
					//Log.Notice("AuthLogon","Received GSLS Payload. Opcode: %u Playload size: %u",m_opcode,m_expectedSize);
                    m_readBuffer.pushReaderPos();

					if(m_opcode < LSWS_MAX_OPCODE && Handlers[m_opcode] != NULL)
						(this->*Handlers[m_opcode])();
					else
						Log.Notice("AuthLogon", "Unknown opcode %u", m_opcode);

                    bytesLeft-= m_expectedSize;
                    // Whatever the handler reads (or not), we follow the sender logic.
                    m_readBuffer.popReaderPos();
                    m_readBuffer.skip(m_expectedSize);
                }
                else
                {
					Log.Error("AuthLogon","Did not receive the correct amount of data... Remains %u in the packet instead of %u",bytesLeft,m_expectedSize);
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
			Log.Debug("AuthLogon","Connection closed (%s)",e.message().c_str());
        }
        else
        {
			Log.Debug("AuthLogon","Socket read problem (%s)",e.message().c_str());
        }
        disconnect();
    }
}

/// Report error after a failed write
void AuthLogon::onWrite(const boost::system::error_code& e)
{
    if (e)
    {
		Log.Error("AuthLogon","socket write problem (%s)",e.message().c_str());
        disconnect();
    }
}

/// Send a packet in the connection
void AuthLogon::writePacket(Buffer* b)
{
    AsyncWrite(b);
    m_bufferPool->disposeBuffer(b);
}

void AuthLogon::SendHello()
{
	Log.Debug("AuthLogon","SendHello()");

	string password;
	Config.MainConfig.GetString("LogonInfo","Password",&password);

	uint16 responseSize = 4+password.length();
    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(WS_CONNEXION_REALM);

	b->write(password);

    writePacket(b);
}
void AuthLogon::HandleHello()
{
	Log.Debug("AuthLogon","HandleHello()");

	uint32 m_ok;
	m_readBuffer >> m_ok;

	if( m_ok )
		SendRegister();
	else 
	{
		Log.Error("AuthLogon","Invalid password ! Restart after changed.");
	}
}
void AuthLogon::SendRegister()
{
	Log.Debug("AuthLogon","Registering Realm on logon server !");

	uint32 realmid = int32abs2uint32(objmgr.m_realm->RealmID);

	uint16 responseSize = 4+objmgr.m_realm->Adresse.size()+4+objmgr.m_realm->Language.size()+4+objmgr.m_realm->Legacy.size()+4+objmgr.m_realm->Name.size()
		+4+objmgr.m_realm->OpenRVR.size()+4+objmgr.m_realm->Port.size()+sizeof(realmid)+4+objmgr.m_realm->Region.size()+4+objmgr.m_realm->RP.size()+4+objmgr.m_realm->Transfert.size();
    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(WS_REGISTER_REALM);

	b->write( objmgr.m_realm->Adresse );
	b->write( objmgr.m_realm->Language );
	b->write( objmgr.m_realm->Legacy );
	b->write( objmgr.m_realm->Name );
	b->write( objmgr.m_realm->OpenRVR );
	b->write( objmgr.m_realm->Port );
	b->write<uint32>( realmid );
	b->write( objmgr.m_realm->Region );
	b->write( objmgr.m_realm->RP );
	b->write( objmgr.m_realm->Transfert );
    writePacket(b);

	objmgr.m_Logon=this;
	objmgr.registered=true;
}
void AuthLogon::HandleAccountList()
{
	Log.Debug("AuthLogon","HandleAccountList()");

	string name;
	string key;
	uint32 accountid;
	uint8 permission;
	uint8 lock;

	m_readBuffer >> accountid >> permission >> lock;
	m_readBuffer >> name;
	m_readBuffer >> key;

	Account * Acct = objmgr.GetAccount(name);
	if(!Acct)
		Acct = new Account;

	Acct->AccountId = accountid;
	Acct->locked = lock;
	Acct->Permission = permission;
	Acct->username = name;
	Acct->KeyToken = key;

	objmgr.AddAccount(Acct);
	SendAccountCharInfo(accountid);
}
void AuthLogon::SendAccountCharInfo(uint32 AccountID)
{
	Log.Debug("AuthLogon","SendAccountCharInfo() of account(%u)",AccountID);

	if( ! AccountID ) return;

	// accountid + size
	uint16 responseSize = 5;
	uint32 counts=0;

	list<PlayerInfo*> m_list = objmgr.GetPlayerInfoList(AccountID);
	list<CharInfo*> m_char;

	if(m_list.size())
	{
		for(list<PlayerInfo*>::iterator itr=m_list.begin();itr != m_list.end(); ++itr)
		{
			if( !(*itr) ) continue;

			CharInfo * Info = new CharInfo;
			Info->character_id = (*itr)->character_id;
			Info->career = (*itr)->career;
			Info->heldleft = (*itr)->heldleft;
			Info->last_time_played = (*itr)->last_time_played;
			Info->level = (*itr)->level;
			Info->model = (*itr)->model;
			Info->name = (*itr)->name;
			Info->race = (*itr)->race;
			Info->realm = (*itr)->realm;
			Info->ServerID = objmgr.m_realm->RealmID;
			Info->zone = (*itr)->zone;
			Info->equipped_1 = 0;

			Log.Debug("AuthLogon","Sending : [%s]-[%u]",Info->name.c_str(),Info->character_id);
			m_char.push_back(Info);

			responseSize+=28+4+(*itr)->name.length()+4;
			counts++;
		};
	}

	Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(LS_REGISTER_STATUT);

	b->write<uint32>( AccountID );
	b->write<uint8>( counts );

	for(list<CharInfo*>::iterator itt = m_char.begin();itt != m_char.end(); ++itt)
	{
		if( !(*itt) ) continue;

		b->write<uint32>( (*itt)->character_id );
		b->write<uint8>( (*itt)->career);
		b->write<uint32>( (*itt)->heldleft );
		b->write<uint32>( (*itt)->last_time_played );
		b->write<uint8>( (*itt)->level );
		b->write<uint16>( (*itt)->model );
		b->write( (*itt)->name );
		b->write<uint8>( (*itt)->race );
		b->write<uint8>( (*itt)->realm );
		b->write<uint32>( (*itt)->ServerID );
		b->write<uint16>( (*itt)->zone );
		b->write<uint32>( (*itt)->equipped_1 );

		b->write<uint32>( 0x00 );
	}
	writePacket(b);
}
// tries to remove the server form the list on disconnection
AuthLogon::~AuthLogon()
{

}
void AuthLogon::SendAccountCheck(string username,string password)
{
	Log.Debug("AuthLogon","SendAccountCheck() of account(%s)",username.c_str());
	// accountid + size
	uint16 responseSize = username.length()+4+password.length()+4;
	Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(WS_ACCOUNT_CHECK);
	b->write(username);
	b->write(password);
	writePacket(b);
}
void AuthLogon::HandleAccountCheck()
{
	Log.Debug("AuthLogon","HandleAccountCheck()");

	uint8 result;
	string username;

	m_readBuffer >> username >> result;

	ClientCom * Com=NULL;
	Com = objmgr.m_accountcheck[username];

	if(!Com) return;

	if(!result)
	{
		Log.Error("AuthLogon","Invalid Result for %s",username.c_str());
		Com->disconnect();
	}
	else 
	{
		uint32 accountid;
		uint8 permission;
		m_readBuffer >> accountid >> permission;

		Log.Success("AuthLogon","Account OK %s",username.c_str());
		Com->CheckAccount(accountid,permission);
	}
}