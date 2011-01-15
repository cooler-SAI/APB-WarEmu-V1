#include "StdAfx.h"

using namespace GameData;

enum EncryptionStates
{
    NOT_ENCRYPTED = 0x01,
    RSA_ENCRYPTED = 0x02,
    RC4_ENCRYPTED = 0x03
};


uint32 ClientCom::connectionCount = 2; //0 and 1 are reserved

/// ConnectionCount Mutex
static boost::mutex ConnectionCount_mutex;

/// Constructor
ClientCom::ClientCom(boost::asio::io_service& IOService, BufferPool* hp)
        : Connection(IOService,hp)
        , m_ioservice(IOService)
        , m_expectedSize(0)
        , m_expectData(false)
        , m_opcode(0)
        , m_encstate(NOT_ENCRYPTED)
        , m_SIDAssigned(false)
        , m_userStage(NOT_CONNECTED)
        , m_userZone(0)
		, m_lastping(UNIXTIME)
{
	m_Plr=NULL;
	m_accountID=0;
	m_permission=0;

 
}
ClientCom::~ClientCom()
{
	if(m_accountID)
	{
		Log.Debug("ClientCom","[%u] Delete Connection",m_accountID);

		Player * Plr = objmgr.GetPlayerByAccount(m_accountID);
		if(Plr)
			objmgr.DeletePlayer(Plr);
	}
}

typedef void (ClientCom::*ClientHandler)();
static ClientHandler Handlers[0xFF+1] = {
    NULL , // "UNKNOWN",
	&ClientCom::onCode1 , // "UNKNOWN",
	&ClientCom::onQuest , // "F_QUEST",
    NULL , // "F_UPDATE_SIEGE_LOOK_AT",
	&ClientCom::onPlayerExit , // "F_PLAYER_EXIT",
    NULL , // "F_PLAYER_HEALTH",
    NULL , // "F_CHAT",
	&ClientCom::onText , // "F_TEXT",
    NULL , // "UNKNOWN",
    NULL , // "F_OBJECT_STATE",
	NULL , // "F_OBJECT_DEATH", 
	&ClientCom::onPing , // "F_PING",
    NULL , // "F_PLAYER_QUIT",
	&ClientCom::onDumpStatics , // "F_DUMP_STATICS",
	&ClientCom::onUnk14 , // "UNKNOWN",
	&ClientCom::onConnect , // "F_CONNECT",
	&ClientCom::onClientDisconnect , // "F_DISCONNECT",
    NULL , // "F_HEARTBEAT",
    NULL , //  "UNKNOWN",
	&ClientCom::onRequestCharTemplates , // "F_REQUEST_CHAR_TEMPLATES",
    NULL , // "F_HIT_PLAYER",
    NULL , // "F_DEATHSPAM",
	NULL , // "F_REQUEST_INIT_OBJECT",
    &ClientCom::onOpenGame , // "F_OPEN_GAME",
	&ClientCom::onPlayerInfo , // "F_PLAYER_INFO",
    NULL , // "F_WORLD_ENTER",
    NULL , // "F_CAMPAIGN_STATUS",
    NULL , // "F_REQ_CAMPAIGN_STATUS", 
    NULL , // "UNKNOWN",
    NULL , // "F_GUILD_DATA",
    NULL , // "F_MAX_VELOCITY",
    NULL , // "F_SWITCH_REGION",
    NULL , // "F_PET_INFO",
    NULL , // "F_PLAYER_CLEAR_DEATH",
    NULL , // "F_COMMAND_CONTROLLED",
    NULL , // "UNKNOWN",
    NULL , // "UNKNOWN",
    NULL , // "F_GUILD_COMMAND",
    NULL , // "UNKNOWN",                
    NULL , // "F_REQUEST_TOK_REWARD",
    NULL , // "F_SURVEY_BEGIN",
    NULL , // "F_SHOW_DIALOG",
    NULL , // "F_PLAYERORG_APPROVAL",
    NULL , //  "F_QUEST_INFO",
	&ClientCom::onRandomNameListInfo , // "F_RANDOM_NAME_LIST_INFO",
    NULL , // "UNKNOWN",
    NULL , // "UNKNOWN",
    NULL , //F_INVITE_GROUP",
    NULL , //F_JOIN_GROUP",
    NULL , //F_PLAYER_DEATH",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
   &ClientCom::onDumpArenasLarge , //F_DUMP_ARENAS_LARGE",
    NULL , //UNKNOWN",
	&ClientCom::onGroupCommand ,	//F_GROUP_COMMAND",
    &ClientCom::onJumpZone ,		//F_ZONEJUMP",
    NULL , //F_PLAYER_EXPERIENCE",
    NULL , //F_XENON_VOICE",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
	&ClientCom::onRequestWorldLarge , //F_REQUEST_WORLD_LARGE",
    NULL , //F_ACTION_COUNTER_INFO",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //F_ACTION_COUNTER_UPDATE",
    NULL , //UNKNOWN",
    NULL , //F_PLAYER_STATS",
    NULL , //F_MONSTER_STATS",
    NULL , //F_PLAY_EFFECT",
    NULL , //F_REMOVE_PLAYER",
    NULL , //F_ZONEJUMP_FAILED",
	&ClientCom::onTradeStatus , //F_TRADE_STATUS",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //F_PLAYER_RENOWN",
    NULL , //F_MOUNT_UPDATE",
    NULL , //F_PLAYER_LEVEL_UP",
    NULL , //F_ANIMATION",
    NULL , //F_PLAYER_WEALTH",
    NULL , //F_TROPHY_SETLOCATION",
	&ClientCom::onRequestChar , //F_REQUEST_CHAR",
    NULL , //F_REQUEST_CHAR_RESPONSE",
    NULL , //F_REQUEST_CHAR_ERROR",
    NULL , //F_CHARACTER_PREFS",
    NULL , //F_SEND_CHARACTER_RESPONSE",
    NULL , //F_SEND_CHARACTER_ERROR",
    NULL , //F_PING_DATAGRAM",
    NULL , //UNKNOWN",
	&ClientCom::onEncryptKeyRequest , //F_ENCRYPTKEY",
    NULL , //F_PQLOOT_TRIGGER",
    NULL , //F_SET_TARGET",
    NULL , //UNKNOWN",
    NULL , //F_MYSTERY_BAG",
    NULL , //F_PLAY_SOUND",
	&ClientCom::onPlayerState2 , //F_PLAYER_STATE2",
    NULL , //F_QUERY_NAME",
    NULL , //F_QUERY_NAME_RESPONSE",
    NULL , //F_ADD_NAME",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
	&ClientCom::onDeleteName , //F_DELETE_NAME",
    NULL , //UNKNOWN",
    NULL , //F_CHECK_NAME",
    NULL , //F_CHECK_NAME_RESPONSE",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //F_LOCALIZED_STRING",
    NULL , //F_KILLING_SPREE",
    NULL , //F_CREATE_STATIC",
    NULL , //F_CREATE_MONSTER",
    NULL , //F_PLAYER_IMAGENUM",
    NULL , //UNKNOWN",
	&ClientCom::onTransfertItem , //F_TRANSFER_ITEM",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //F_CRAFTING_STATUS",
    NULL , //F_REQUEST_LASTNAME",
    NULL , //UNKNOWN",
    &ClientCom::onInitPlayer , //F_INIT_PLAYER",
    &ClientCom::onRequestInitPlayer , //F_REQUEST_INIT_PLAYER",
    NULL , //F_SET_ABILITY_TIMER",
    NULL , //UNKNOWN",
    NULL , //S_PID_ASSIGN",
    NULL , //S_PONG",
    NULL , //S_CONNECTED",
	&ClientCom::onWorldSent , //S_WORLD_SENT",
    NULL , //S_NOT_CONNECTED",
    NULL , //S_GAME_OPENED",
    &ClientCom::onMail , //F_MAIL",
    NULL , //S_DATAGRAM_ESTABLISHED",
    NULL , //S_PLAYER_INITTED",
    NULL , //S_PLAYER_LOADED",
    NULL , //F_RECEIVE_ENCRYPTKEY",
    NULL , //UNKNOWN",
    NULL , //F_MORALE_LIST",
    NULL , //F_SURVEY_ADDQUESTION",
    NULL , //F_SURVEY_END", 
    NULL , //F_SURVEY_RESULT",
    &ClientCom::onEmote , //F_EMOTE",
	&ClientCom::onCreateCharacter , //F_CREATE_CHARACTER",
    &ClientCom::onDeleteCharacter , //F_DELETE_CHARACTER",
    NULL , //F_GFX_MOD",
    NULL , //F_INSTANCE_INFO",
    NULL , //UNKNOWN",
    NULL , //F_KEEP_STATUS",
    NULL , //F_PLAY_TIME_STATS",
    NULL , //F_CATAPULT",
    NULL , //F_GRAVITY_UPDATE",
    NULL , //F_HELP_DATA",
    NULL , //F_UPDATE_LASTNAME",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //F_GET_CULTIVATION_INFO",
    NULL , //F_CRASH_PACKET",
    NULL , //F_LOGINQUEUE",
	&ClientCom::onInterrupt , //F_INTERRUPT",
    NULL , //F_INSTANCE_SELECTED",
    NULL , //F_ACTIVE_EFFECTS", 
    NULL , //UNKNOWN",
    NULL , //F_SERVER_INFO",
    NULL , //F_START_SIEGE_MULTIUSER",
    NULL , //F_SIEGE_WEAPON_RESULTS",
	&ClientCom::onInterractQueue , //"F_INTERACT_QUEUE",  
    NULL , //F_UPDATE_HOT_SPOT",
    NULL , //F_GET_ITEM",
    NULL , //F_DUEL",
    NULL , //F_PLAYER_JUMP",
    NULL , //F_INTRO_CINEMA", 
    NULL , //F_MAGUS_DISC_UPDATE",
    NULL , //F_FIRE_SIEGE_WEAPON",
    NULL , //F_GRAPHICAL_REVISION",
    NULL , //UNKNOWN",
    NULL , //F_AUCTION_POST_ITEM",
    NULL , //F_CAST_PLAYER_EFFECT",
    NULL , //F_AUCTION_SEARCH_QUERY",
	&ClientCom::onFlight , //F_FLIGHT", 
	&ClientCom::onSocialNetwork , //"F_SOCIAL_NETWORK",
    NULL , //F_AUCTION_SEARCH_RESULT",
	&ClientCom::onPlayerEnterFull , //F_PLAYER_ENTER_FULL",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //F_AUCTION_BID_ITEM",
    NULL , //F_ESTABLISH_DATAGRAM",
    NULL , //F_PLAYER_INVENTORY",
    NULL , //F_CHARACTER_INFO",
    NULL , //F_INIT_STORE",
    NULL , //F_STORE_BUY_BACK",
    NULL , //F_OBJECTIVE_INFO",
    NULL , //F_OBJECTIVE_UPDATE",
    NULL , //F_SCENARIO_INFO",
    NULL , //F_SCENARIO_POINT_UPDATE",
    NULL , //F_OBJECTIVE_STATE",
    NULL , //F_REALM_BONUS",
    NULL , //F_OBJECTIVE_CONTROL",
    NULL , //F_INTERFACE_COMMAND",
    NULL , //F_SCENARIO_PLAYER_INFO",
    NULL , //F_FLAG_OBJECT_STATE",
    NULL , //F_FLAG_OBJECT_LOCATION",
	NULL , //F_CITY_CAPTURE",
    NULL , //F_ZONE_CAPTURE",
    NULL , //F_SALVAGE_ITEM",
    NULL , //F_AUCTION_BID_STATUS",
    NULL , //F_PUNKBUSTER",
    NULL , //F_ITEM_SET_DATA",
	&ClientCom::onInterract , //"F_INTERACT",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
	&ClientCom::onDoAbility , //"F_DO_ABILITY",
    NULL , //F_SET_TIME",
    NULL , //F_INIT_EFFECTS",
    NULL , //F_GROUP_STATUS",
	&ClientCom::onUseItem , //F_USE_ITEM",
    NULL , //F_USE_ABILITY",
	&ClientCom::onInfluenceDetail , //F_INFLUENCE_DETAILS",
	&ClientCom::onAttack , //"F_SWITCH_ATTACK_MODE",
    NULL , //F_BUG_REPORT",
    NULL , //F_OBJECT_EFFECT_STATE",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //UNKNOWN",
    NULL , //F_EXPERIENCE_TABLE",
    NULL , //F_CREATE_PLAYER",
    NULL , //F_UPDATE_STATE",
    NULL , //F_UI_MOD",
    NULL , //UNKNOWN",
    NULL , //F_RVR_STATS",
    NULL , //F_CLIENT_DATA",
	NULL , //"F_INTERACT_RESPONSE",
    NULL , //F_QUEST_LIST",
    NULL , //F_QUEST_UPDATE",
	&ClientCom::onRequestQuest , //F_REQUEST_QUEST",
    NULL , //F_QUEST_LIST_UPDATE",
    NULL , //F_CAREER_CATEGORY",
    NULL , //F_PLAYER_INIT_COMPLETE",
    NULL , //UNKNOWN",
    NULL , //F_CAREER_PACKAGE_UPDATE",
    NULL , //F_BUY_CAREER_PACKAGE",
    NULL , //F_CAREER_PACKAGE_INFO",
	NULL , //F_PLAYER_RANK_UPDATE
    NULL , //F_DO_ABILITY_AT_POS",
    NULL , //F_CHANNEL_LIST",
    NULL , //F_TACTICS",
    NULL , //F_TOK_ENTRY_UPDATE",
	NULL , //"F_TRADE_SKILL_UPDATE",
	NULL , //"F_RENDER_PRIMITIVE",    
    NULL , //F_INFLUENCE_UPDATE",
    NULL , //F_INFLUENCE_INFO",
    NULL , //F_KNOCKBACK",
	&ClientCom::onPlayerVoiceOver , //F_PLAY_VOICE_OVER",
    NULL , //UNKNOWN"
};

/// Accept the connection and wait for information
void ClientCom::start(void* param)
{
    AsyncRead();

    boost::mutex::scoped_lock lock(ConnectionCount_mutex);
    m_connectionID = static_cast<uint16>(connectionCount); ///\TODO will serve as session ID. Be sure not to assign twice the same (when counter returns to 0)
    connectionCount++;
	b_connected=true;
	Log.Debug("ClientCom","New Game connection accepted ID: %u",m_connectionID);
}

/// Read data from the connection
void ClientCom::onDisconnect()
{
	if( m_Plr )
		objmgr.DeletePlayer(m_Plr);

	sendNotConnected();
}
void ClientCom::onRead(const boost::system::error_code& e)
{
    if (!e)
    {
        uint32 bytesLeft = m_readBuffer.size();
        uint16 sessionID;
        uint16 unk1;
        uint8 unk2;

        ///- While there is some bytes in the received buffer
        while (bytesLeft)
		{
            ///- Read up to the opcode
            if (!m_expectData)
            {
                if (bytesLeft < 2)
                {
					Log.Error("ClientCom","[ID: %u] Incomplete packet header sent (size=%u)",m_connectionID,bytesLeft);
					break;
                }

				m_readBuffer >> m_expectedSize;
                bytesLeft-=2;

				 if (bytesLeft < static_cast<uint16>(m_expectedSize + 10))
                {
					Log.Error("ClientCom","[ID: %u] Incomplete packet header sent (size=%u)",m_connectionID,bytesLeft);
					break;
                }

                /// \todo for later: only decrypt header and pass to zone servers to decrypt the rest.
                if (m_encstate == RC4_ENCRYPTED)
                    m_readBuffer.decryptWARRC4Packet(m_expectedSize+10,m_encryptionKey, 256);

                m_readBuffer >> m_sequenceID >> sessionID >> unk1 >> unk2 >> m_opcode;
                bytesLeft-=8;

				if (bytesLeft > static_cast<uint32>(m_expectedSize + 2)) //+2 for the CRC
                {
					//Log.Debug("ClientCom","Packet contains multiple opcodes (bytesLeft - m_expectedSize = %u)",bytesLeft - m_expectedSize);
                }
                m_expectData = true;

            } //!m_expectedData
            else
            {
				
                m_expectData = false;
                if (bytesLeft >= static_cast<uint32>(m_expectedSize + 2))
                {
 					//Log.Notice("ClientCom","[ID: %u] Received %u [%s]",m_connectionID,static_cast<int>(m_opcode),opcodeS[m_opcode]);
					m_readBuffer.pushReaderPos();
                    if (m_userStage < AUTHENTICATED && m_opcode != F_CONNECT && m_opcode != F_ENCRYPTKEY && m_opcode != F_PING && m_opcode != F_DISCONNECT && m_opcode!=F_SERVER_INFO)
                    {
						Log.Warning("ClientCom","[ID: %u] Hack try. Trying to issue commands while not authenticated. Command %u %s",m_connectionID,static_cast<int>(m_opcode),opcodeS[m_opcode]);
					}
                    else
                    {
                       		if(m_opcode < MAX_GAME_OPCODE && Handlers[m_opcode] != NULL)
							{
								//Log.Success("ClientCom", "Received Opcode [%s]",opcodeS[m_opcode]);
								(this->*Handlers[m_opcode])();
							}
							else
							{
								Log.Error("ClientCom", "Unknown opcode %u [%s]", m_opcode,(m_opcode < MAX_GAME_OPCODE) ? opcodeS[m_opcode] : "TO BIG" );
								//m_readBuffer.Trace();
							}
                    }

                   m_readBuffer.popReaderPos();
                   m_readBuffer.skip(m_expectedSize);

                    // We *may* check the packet checksum here, but usefulness is not obvious (except spending time)
                    uint16 checksum;
                    m_readBuffer >> checksum;

                    bytesLeft-= m_expectedSize + 2;
                }
                else
                {
					Log.Error("ClientCom","[ID: %u] Did not receive the correct amount of data... Remains: %u  instead of %u",bytesLeft,m_expectedSize+2);
                    break;
                }
            } //m_expectedData
        } // while bytesLeft
        AsyncRead();
	
    } //!e
    else
    {
		b_connected=false;
        if (e.value() == 2)
        {
			Log.Debug("ClientCom","[ID: %u] Connection closed (%s)",m_connectionID,e.message().c_str());
        }
        else
        {
			Log.Debug("ClientCom","[ID: %u] Socket read problem (%s)",m_connectionID,e.message().c_str());
        }
        disconnect();
    }
}

/// Report error after a failed write
void ClientCom::onWrite(const boost::system::error_code& e)
{
    if (e)
    {
		Log.Error("ClientCom","[ID: %u] Socket write problem (%s)",m_connectionID,e.message().c_str());
        disconnect();
    }
}

/// Calculate the packet checksum
uint16 ClientCom::CalculateChecksum(char* packet, int dataOffset, int dataSize)
{
    uint8 val1 = 0x7E;
    uint8 val2 = 0x7E;
    int i = dataOffset;
    int len = i+dataSize;

    while (i < len)
    {
        val1 += packet[i++];
        val2 += val1;
    }

    return (uint16)(val2 - ((val1 + val2) << 8));
}

/// Send a packet in the connection
void ClientCom::writePacket(Buffer *b, bool dispose)
{
	if( !b_connected ) return;
	if( !b ) return;

	uint16 originalPacketLength;
	originalPacketLength = b->peek<uint16>(0);
    uint8 opcode = b->peek<uint8>(2);

    if (b->remains()-3 != originalPacketLength )
    {
		Log.Error("ClientCom","[ID: %u] Fishy Packet %u < %u. opcode[%u]",m_connectionID,b->remains()-3,originalPacketLength,opcode);
        return;
    }

    if (m_encstate == RC4_ENCRYPTED)
    {
        b->pushReaderPos();
        b->skip(2);
        b->encryptWARRC4Packet(m_encryptionKey, 256);
        b->popReaderPos();
    }

    AsyncWrite(b,b->size());
    if (dispose)
        m_bufferPool->disposeBuffer(b);
}
void ClientCom::writePacket(uint8 opcode,Buffer *b)
{
	Buffer* d = m_bufferPool->allocateBuffer(b->size()+3);
	d->write<uint16>(b->size());
	d->write<uint8>(opcode);
	d->write(b,true);
	writePacket(d,true);
	delete b;
}
/// Handle Encryption Key request
void ClientCom::onEncryptKeyRequest()
{
	Log.Debug("ClientCom","onEncryptKeyRequest()");
    uint8 cipherOrder;
    uint16 responseSize = opcodeSize[F_RECEIVE_ENCRYPTKEY][SC];//opcodeTable[F_RECEIVE_ENCRYPTKEY].PacketSizeSC;
    uint8 application , major, minor, revision, unk1;

    m_readBuffer >> cipherOrder >> application >> major >> minor >> revision >> unk1;

	stringstream ss;
	ss << uint32(major) << "." << uint32(minor) << "." << uint32(revision);

	Log.Debug("ClientCom","Client Version : '%s'.%u - Supported : '%s'",ss.str().c_str(),application,objmgr.ClientVersion.c_str());

	if(objmgr.ClientVersion != ss.str())
	{
		Log.Error("ClientCom","Version Not supported ! Disconnecting .");
		disconnect();
		return;
	}

    switch (cipherOrder)
    {
    case 0:
    {
		Log.Debug("ClientCom","Writing RSA key");
        Buffer* b = m_bufferPool->allocateBuffer(1+3);
        b->write<uint16>(1);
        b->write<uint8>(F_RECEIVE_ENCRYPTKEY);
        b->write<uint8>(1);  /// \todo send a RSA key instead
        writePacket(b);

        m_encstate=RSA_ENCRYPTED;
    }break;
    case 1:
    {
		Log.Debug("ClientCom","Reading Key");
        if(!m_readBuffer.read(m_encryptionKey,256))
			return;

        m_encstate=RC4_ENCRYPTED;
    }break;
    default:
        //LOG_ERROR(logger, "[ID: " << m_connectionID << "] Unknown cipher ordered received: " << cipherOrder);
        Log.Error("ClientCom","[ID: %u] Unknown cipher ordered received: %u",m_connectionID,cipherOrder);
		break;
    }

}
void ClientCom::onConnect()
{    
	//Fixed buffer size already checked.

    uint8 unk1;
    uint8 authToken[80+1];
    authToken[80] = '\0';
    uint8 usernameTmp[23];

    m_readBuffer >> unk1;
	m_readBuffer.skip(4); // Client version
    m_readBuffer.skip(3); // all zeroes
    m_readBuffer >> m_tag;
    m_readBuffer.read(authToken,80);
    m_readBuffer.skip(21); //all zeroes
    m_readBuffer.read(usernameTmp,23);

    m_username = reinterpret_cast<const char*>(usernameTmp);
    m_authToken = reinterpret_cast<const char*>(authToken);
	
	if(objmgr.m_Logon && objmgr.m_Logon->b_connected)
	{
		objmgr.m_accountcheck[m_username] = this;
		objmgr.m_Logon->SendAccountCheck(m_username,m_authToken);
	}
}
void ClientCom::onClientDisconnect()
{
    //Fixed buffer size already checked.

    uint8 reason;
    m_readBuffer >> reason;
    // 4 more (null) bytes in buffer
    m_userStage = NOT_CONNECTED;
	Log.Debug("ClientCom","Disconnecting [%u] : %u",m_connectionID,reason);

	if(m_Plr != NULL)
	{
		m_Plr->m_leaved = true;
		objmgr.DeletePlayer(m_Plr);
		m_Plr = NULL;
	}
}
void ClientCom::onPlayerExit()
{
    //Fixed buffer size already checked.

	Log.Debug("ClientCom","Player Exiting : [%u]",m_connectionID);

	if( m_Plr != NULL && !m_Plr->m_teleporting)
	{
		objmgr.DeletePlayer(m_Plr);
		m_Plr=NULL;
	}
}
void ClientCom::CheckAccount(uint32 AccountId,uint8 permission)
{
		objmgr.m_accountcheck[m_username] = NULL;
		m_permission = permission;
		m_accountID = AccountId;

		Log.Debug("ClientCom","Account [%s] : permission=%u",m_username.c_str(),m_permission);
		//uint16 responseSize = 4+4+1+4+1+m_username.size()+1+objmgr.m_realm->Name.size()+1+0+2;
		Buffer* b = new Buffer(0);
		//b->write<uint16>(responseSize);
		//b->write<uint8>(S_CONNECTED);
		b->write<uint32>(0x00000000);
		b->write<uint32>(m_tag);
		b->write<uint8>(objmgr.m_realm->RealmID);
		b->write<uint32>(0x00000001); //seen 0x00000000 also...
		b->writeBytePascalString(m_username);
		b->writeBytePascalString(objmgr.m_realm->Name);
		b->write<uint8>(0);
		b->fill(0,2);
		writePacket(S_CONNECTED,b);
		m_userStage = AUTHENTICATED;
}
void ClientCom::onPlayerEnterFull()
{
	uint16 SID;
    uint8 unk1,serverID, characterSlot;
    uint8 characterName[24];
    uint8 language[2];

    m_readBuffer >> SID >> unk1 >> serverID;
    m_readBuffer.read(characterName,24);
    m_readBuffer.skip(2);
    m_readBuffer.read(language,2);
    m_readBuffer.skip(4);
    m_readBuffer >> characterSlot;
    // 11 bytes (0's) are remaining.

    m_characterName = reinterpret_cast<const char*>(characterName);

	if (serverID != objmgr.m_realm->RealmID)
    {
		Log.Error("ClientCom","[ID: %u]  Server IDs do not match! I am server %u  and receive orders for server %u",m_connectionID,objmgr.m_realm->RealmID,serverID);
		disconnect();
		return;
    }

        // Should we also check the language?

		sendPidAssign();
        m_SIDAssigned =  true;
}
void ClientCom::sendPidAssign()
{
        Buffer* b = new Buffer(0);
        b->write<uint8>(m_connectionID & 0xFF);
        b->write<uint8>(m_connectionID >> 8);
        writePacket(S_PID_ASSIGN,b);
}
void ClientCom::sendNotConnected()
{
	Buffer* b =  new Buffer(0);
	b->write<uint8>(m_connectionID & 0xFF);
    b->write<uint8>(m_connectionID >> 8);
	b->fill(0,3);
	writePacket(S_NOT_CONNECTED,b);
}
void ClientCom::onPing()
{
    //Fixed buffer size already checked.

	/*if(m_Plr)
	{
		if(m_lastping+4 > UNIXTIME)
		{
			objmgr.DeletePlayer(m_Plr);
			return;
		}
		else
			m_lastping=UNIXTIME;
	}*/

    uint32 timestamp;
    m_readBuffer >> timestamp;
    // still some stuff in the packet. See doc.

    Buffer* b = new Buffer(0);
    b->write<uint32>(timestamp);
    b->write<uint64>(UNIXTIME);
    b->write<uint32>(m_sequenceID+1);
    b->write<uint32>(0x00); // unknown
    writePacket(S_PONG,b);
}
void ClientCom::onRandomNameListInfo()
{
    GameData::Races race = (GameData::Races)m_readBuffer.read<uint8>();
    uint8 unk1,slot;
    m_readBuffer >> unk1 >> slot;

    ///\todo make a random name generator by race.
    std::vector<std::string> nameList;
	std::vector<std::string>::iterator it;
	uint32 random = RandomUInt(MAXLISTNAME-1);

	for(uint32 i=random;i<MAXLISTNAME;++i)
	{
		nameList.push_back(m_ListName[i]);
	}
	for(uint32 i=0;i<random;++i)
	{
		nameList.push_back(m_ListName[i]);
	}

    Buffer* b = new Buffer(0);
    b->write<uint8>(0x00);
    b->write<uint8>(unk1);
    b->write<uint8>(slot);
    b->write<uint16>(0);
    b->write<uint8>(nameList.size());
    for (it = nameList.begin(); it != nameList.end(); ++it)
    {
        b->write(reinterpret_cast<const uint8*>(it->c_str()),it->length());
        b->write<uint8>(0);
    }
    writePacket(F_RANDOM_NAME_LIST_INFO,b);
}
/// really should be onCheckName...
void ClientCom::onDeleteName()
{
    uint8 charName[30+1];
    charName[30] = '\0';
    uint8 userName[20+1];
    userName[20] = '\0';

    m_readBuffer.read(charName,30);
    m_readBuffer.read(userName,20);

    std::string sCharName = reinterpret_cast<const char*>(charName);
    std::string sUserName = reinterpret_cast<const char*>(userName);

	Log.Debug("CGameConnection","OnDeleteName %s name for username: %s",sCharName.c_str(),sUserName.c_str());

    uint8 badName = 0;

    if (sCharName.length() == 0)
        badName=1;
    else
    {
		QueryResult * Result = CharacterDatabase.Query("SELECT 1 FROM `characters` WHERE name='%s'",sCharName.c_str());
        if (Result) badName = 1;
		delete Result;
    }

    Buffer* b = new Buffer(0);
    b->write(charName,30);
    b->write(userName,20);

    // name is good
    b->write<uint8>(badName); /// \todo test if <> 0
    b->fill(0x00,3);
    writePacket(F_CHECK_NAME,b);
}
////////////////////
// SENDER
////////////////////
void ClientCom::sendServerInfo()
{
    Buffer* b = new Buffer(0);
    b->write<uint8>(1);
    b->write<uint16>(9);
    b->write<uint16>(0);
    b->write<uint16>(0x1FFF);
    writePacket(F_SERVER_INFO,b);
}
void ClientCom::sendRealmBonus()
{
    Buffer* b = new Buffer(0);
    b->write<uint8>(m_realm); //to be tested. not sure at all
    b->write<uint8>(0xF); //? seen 0 too
    b->write<uint8>(0xF); //?
    writePacket(F_REALM_BONUS,b);
}
void ClientCom::sendKeepStatus()
{
    uint16 responseSize = opcodeSize[F_KEEP_STATUS][SC];
    // Slightly not thread-safe (keep number should not change)
    std::vector<Keep>::iterator it;
    int keepNumber=1;
    for (it=objmgr.m_keeps.begin();it!=objmgr.m_keeps.end();++it)
    {
        Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
        b->write<uint16>(responseSize);
        b->write<uint8>(F_KEEP_STATUS);
        b->write<uint32>(keepNumber);
        b->write<uint8>(it->keepStatus);
        b->fill(0,7); // To be found out
        b->write<uint8>(it->keepRealm);
        b->fill(0,15); // To be found out
        b->write(reinterpret_cast<const uint8*>(it->guildName.c_str()),it->guildName.length());
        b->fill(0,50-it->guildName.length());
        b->write<uint16>(it->keepTactics[0]);
        b->write<uint16>(it->keepTactics[1]);
        b->write<uint16>(it->keepTactics[2]);
        b->write<uint8>(it->keepRank);
        b->write<uint8>(0);
        writePacket(b);
        keepNumber++;
    }
}
void ClientCom::sendObjectiveStatus()
{
	if(!m_Plr) return;

	ZoneMgr * Zone = mapmgr.GetZoneMgr(m_userZone);
	if(!Zone) return;

	if(Zone->m_pquests.size())
	{
		for(list<PQuest*>::iterator itr = Zone->m_pquests.begin();itr!=Zone->m_pquests.end();++itr)
		{
			Buffer* b = m_bufferPool->allocateBuffer(12+3);
			b->write<uint16>(12);
			b->write<uint8>(F_OBJECTIVE_STATE);
			b->write<uint32>((*itr)->GetEntry());
			b->write<uint32>(0xFFFFFFFF);
			b->write<uint8>(!(*itr)->GetRealm());
			b->write<uint8>(0);
			b->write<uint16>(0);
			b->Trace();
			writePacket(b);
		}
	}

}
void ClientCom::sendCampaignStatus()
{
    uint16 responseSize = opcodeSize[F_CAMPAIGN_STATUS][SC];
    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_CAMPAIGN_STATUS);
    /// \todo reverse engineer this packet
    uint8 buffer[0x82]=
    {
        0x00,0x05,0x00,0x6E,0x00,0xCD,0x00,0x16,0x07,0x46,0x0A,0x2A,0x2E,0x12,0x0D,0x43,
        0x1F,0x0B,0x39,0x10,0x2C,0x27,0x2C,0x0B,0x2B,0x17,0x04,0x48,0x46,0x00,0x00,0x15,
        0x0C,0x42,0x21,0x0D,0x34,0x13,0x05,0x4A,0x22,0x0C,0x32,0x00,0x04,0x01,0x03,0x00,
        0x04,0x00,0x02,0x03,0x01,0x01,0x02,0x00,0x04,0x00,0x00,0x00,0x03,0x02,0x02,0x00,
        0x04,0x00,0x01,0x10,0x11,0x5F,0x00,0x00,0x2C,0x1D,0x03,0x4B,0x15,0x27,0x35,0x00,
        0x19,0x4E,0x01,0x00,0x01,0x00,0x00,0x00,0x01,0x02,0x01,0x00,0x01,0x00,0x00,0x00,
        0x00,0x00,0x01,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0x01,0x01,0x00,0x02,0x02,0x00,0x02,0x02,0x02,0x02,0x01,0x01,0x00,0x02,
        0x02,0x00
    };
    b->write(buffer, responseSize);
    writePacket(b);
}
void ClientCom::sendExperienceTable()
{
    uint8 maxLevel = 40;
    uint8 items=0;
    Buffer* bTemp = m_bufferPool->allocateBuffer(maxLevel*3*5);
    for (uint8 i=0;i<=maxLevel;++i)
    {
		const XpInfo * Info = objmgr.GetXpInfo(i);
		if(!Info) continue;

		if (Info->adv1 !=0)
        {
            bTemp->write<uint8>(Info->adv1);
            bTemp->write<uint32>(0);
            items++;
        }
        if (Info->adv2 !=0)
        {
            bTemp->write<uint8>(Info->adv2);
            bTemp->write<uint32>(0);
            items++;
        }
        bTemp->write<uint8>(0);
		bTemp->write<uint32>(Info->xp);
        items++;
    }

    uint16 responseSize = 1+bTemp->size()+1;
    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_EXPERIENCE_TABLE);
    b->write<uint8>(items);
    b->write(bTemp);
    b->write<uint8>(0);
    writePacket(b);

    m_bufferPool->disposeBuffer(bTemp);
}
void ClientCom::sendGuildData()
{
	uint16 responseSize = 2;
	Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_GUILD_DATA);
	b->write<uint16>(0x0000);
	writePacket(b);
}
void ClientCom::sendChannels()
{
	uint16 responseSize = 2;
	Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_CHANNEL_LIST);
	b->write<uint8>(0);
	b->write<uint8>(0);
	writePacket(b);
}
void ClientCom::sendMailCount()
{
	//maybe call it from playerEnter (wont't give the same packet orders, though.)
	uint16 responseSize = 4;
	Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_MAIL);
	b->write<uint8>(0x9); // Display unread messages count
	b->write<uint8>(GameData::MAILBOXTYPE_PLAYER);
	b->write<uint16>(0); // number of unread messages
	writePacket(b,false);

	b->reset();
	b->write<uint16>(responseSize);
	b->write<uint8>(F_MAIL);
	b->write<uint8>(0x9); // Display unread messages count
	b->write<uint8>(GameData::MAILBOXTYPE_AUCTION); // auction mailbox
	b->write<uint16>(0); // number of unread messages
	writePacket(b);
}
void ClientCom::sendCityCapture()
{
    /// \todo finish reversing this
    uint16 responseSize = opcodeSize[F_CITY_CAPTURE][SC];

    std::vector<City>::iterator it;
    for (it=objmgr.m_cities.begin();it!=objmgr.m_cities.end();++it)
    {
        Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
        b->write<uint16>(responseSize);
        b->write<uint8>(F_CITY_CAPTURE);
        b->write<uint8>(0x73); //??
        b->write<uint32>(it->cityID);
        b->fill(0,3);
        b->write<uint16>(0xFFFF);
        b->write<uint8>(it->cityState);
        b->write<uint8>(0);
        b->write<uint8>(it->cityRealm); //not sure
        b->write<uint8>(it->cityRealm); //not sure
        b->write<uint16>(0); //not sure
        writePacket(b);
    }
}
void ClientCom::onRequestWorldLarge()
{
	if(m_Plr) m_Plr->onRequestWorldLarge();
}
void ClientCom::onText()
{
    uint8 unk1;
    m_readBuffer >> unk1;
    std::string text = reinterpret_cast<const char*>(m_readBuffer.constBuffer());

	if (text[0]=='&') text[0]='/';

	std::string command = text.substr(0,text.find_first_of(' '));
	text.erase(0,command.size()+1);

	if( m_Plr )
	{
		sChat.ExecuteFunction(m_Plr,command,text);
	}

}
void ClientCom::onWorldSent()
{

}
void ClientCom::onCode1()
{

}
void ClientCom::onTransfertItem()
{
    uint16 oid;
	uint16 slot,toslot,count;

    m_readBuffer >> oid;
	m_readBuffer >> slot;
	m_readBuffer >> toslot;
	m_readBuffer >> count;

	Log.Notice("ClientCom","Move Item on Slot(%u)",slot);
	if(m_Plr) m_Plr->GetItemInterface()->HandleMoveItem(toslot,slot,count);
}
void ClientCom::onRequestInitPlayer()
{
	uint16 oid=0;
	m_readBuffer >> oid;
	if(m_Plr)
	{
		if( m_Plr->HasInRangePlayers() )
		{
			unordered_set<Player*  >::iterator itr;
			for(itr=m_Plr->GetInRangePlayerSetBegin();itr!=m_Plr->GetInRangePlayerSetEnd();++itr)
			{
				if( (*itr) )
				{
					(*itr)->SendMeTo(m_Plr);
					(*itr)->SendEquiped(m_Plr);
				}
			}
		}
	}

}
void ClientCom::onEmote()
{
	Log.Debug("ClientCom","OnEmote()");
	if(m_Plr == NULL)
		return;

	uint16 emote,unk=0;
	uint32 unk2;
	m_readBuffer >> unk >> emote >> unk2;
	Buffer * b = m_bufferPool->allocateBuffer(8+3);
	b->write<uint16>(8);
	b->write<uint8>(F_EMOTE);
	b->write<uint16>(m_Plr->GetOid());
	b->write<uint16>(emote);
	b->write<uint32>(unk2);
	m_Plr->dispachPacketInRange(b);
	m_Plr->sendPacket(b);
}
void ClientCom::onUnk14()
{
	Log.Debug("ClientCom","OnUnk14");
	m_readBuffer.Trace();

	return; //TODO vérifier cette merde de packet
	if(!m_Plr) return;

	Buffer *b = new Buffer(295+3);
	b->write<uint16>(295);
	b->write<uint8>(0x0E);

	uint8 data[295] = 
	{
		0x01,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x80,0x00,0x00,0x03,0xBD,0x02,0x03,
		0x01,0x00,0x00,0x02,0xFC,0x00,0x00,0x00,0xE4,0x00,0x00,0x00,0x6A,0x00,0x00,0x03,
		0xC0,0x02,0x03,0x01,0x00,0x00,0x02,0x52,0x00,0x00,0x13,0xE9,0x00,0x00,0x00,0x6A,
		0x00,0x00,0x03,0xAA,0x02,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6A,0x00,
		0x00,0x00,0x6A,0x00,0x00,0x03,0xB9,0x02,0x03,0x02,0x00,0x00,0x00,0x36,0x00,0x00,
		0x00,0xF0,0x00,0x00,0x00,0x65,0x00,0x00,0x03,0xBF,0x02,0x05,0x03,0x00,0x00,0x00,
		0x50,0x00,0x00,0x00,0x0D,0x00,0x00,0x00,0x6C,0x00,0x00,0x03,0xBE,0x02,0x03,0x03,
		0x00,0x00,0x00,0x2F,0x00,0x00,0x13,0xE2,0x00,0x00,0x00,0x66,0x00,0x00,0x03,0xBC,
		0x02,0x03,0x04,0x00,0x00,0x00,0x22,0x00,0x00,0x00,0x9C,0x00,0x00,0x00,0x09,0x00,
		0x00,0x00,0x05,0x02,0x0A,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6A,0x00,0x00,
		0x00,0x6A,0x00,0x00,0x00,0x06,0x02,0x0A,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x65,0x00,0x00,0x00,0x65,0x00,0x00,0x00,0x07,0x02,0x0A,0x03,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x6C,0x00,0x00,0x00,0x6C,0x00,0x00,0x00,0x08,0x02,0x0A,0x04,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x01,0x01,
		0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6A,0x00,0x00,0x00,0x6A,0x00,0x00,
		0x00,0x02,0x01,0x08,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x65,0x00,0x00,0x00,
		0x65,0x00,0x00,0x00,0x03,0x01,0x08,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6C,
		0x00,0x00,0x00,0x6C,0x00,0x00,0x00,0x04,0x01,0x08,0x04,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x67,0x00,0x00,0x00,0x67
	};

	b->write(data,295);

	m_Plr->sendPacket(b);

}