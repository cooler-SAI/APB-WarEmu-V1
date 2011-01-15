#ifndef CLIENTCOM_H
#define CLIENTCOM_H

#include <Common.h>

#include <boost/asio/io_service.hpp>
#include "../magdar-shared/Network/Connection.h"
#include <list>


class Buffer;
class BufferPool;

enum UserStage
{
    NOT_CONNECTED,
    AUTHENTICATED,
    INITIALIZING,
    IN_GAME
};

/// Game client connection management
class ClientCom : public Connection
{
public:
    ClientCom(boost::asio::io_service& IOService, BufferPool* hp);
    void start(void* param=NULL);
	uint16 getConnectionID() { return m_connectionID; };
    UserStage getUserStage();

    ~ClientCom();

	// Responder
	void onCode1();
	void onEncryptKeyRequest();
	void onConnect();
	void onClientDisconnect();
	void onPlayerExit();
	void CheckAccount(uint32 AccountId,uint8 permission);
	void onPlayerEnterFull();
	void onRequestChar();
	void onRequestCharTemplates();
	void onPing();
	void onRandomNameListInfo();
	void onDeleteName();
	void onCreateCharacter();
	void onDeleteCharacter();
	void onDumpArenasLarge();
	void onOpenGame();
	void onInitPlayer();
	void onRequestWorldLarge();
	void onPlayerState2();
	void onDumpStatics();
	void onText();
	void onPlayerInfo();
	void onPlayerVoiceOver();
	void onInterract();
	void onAttack();
	void onWorldSent();
	void onTransfertItem();
	void onRequestInitPlayer();
	void onGroupCommand();
	void onEmote();
	void onSocialNetwork();
	void onTradeStatus();
	void onMail();
	void onDoAbility();
	void onFlight();
	void onJumpZone();
	void onQuest();
	void onRequestQuest();
	void onInterrupt();
	void onInterractQueue();
	void onUseItem();
	void onInfluenceDetail();
	void onUnk14();

	// Sender
	void sendServerInfo();
	void sendRealmBonus();
	void sendKeepStatus();
	void sendCampaignStatus();
	void sendExperienceTable();
	void sendGuildData();
	void sendSocialNetwork();
	void sendChannels();
	void sendObjectiveStatus();
	void sendCityCapture();
	void sendMailCount();
	void sendPidAssign();
	void sendNotConnected();

	void onDisconnect();
    void onRead(const boost::system::error_code& e);
    void onWrite(const boost::system::error_code& e);
    void writePacket(Buffer *b, bool dispose = true);
	void writePacket(uint8 opcode,Buffer *b);

    static uint16 CalculateChecksum(char* packet, int dataOffset, int dataSize); // taken from DoL source.

    boost::asio::io_service& m_ioservice;

    uint8 m_opcode;
    uint16 m_expectedSize;
    bool m_expectData;
    uint8 m_encstate;

    uint8 m_encryptionKey[256];

    bool m_SIDAssigned;

    std::string m_username;
    std::string m_authToken;

    uint32 m_tag;
    uint16 m_sequenceID; //Needed for PING/PONG
    UserStage m_userStage;
    std::string m_characterName;
    uint16 m_userZone;

    uint32 m_accountID;
	uint8 m_permission;
	GameData::Realm m_realm;

    static uint32 connectionCount;
    static const uint32 PACKET_HEADER_SIZE = 12;		//size bytes == packetsize - 12 (10 at front, 2 at rear)

	Player * m_Plr;
	uint32 m_lastping;

};

#define MAXLISTNAME 100
static const char * m_ListName[MAXLISTNAME] = 
{
	"Grialath",
	"Jerardok",
	"Beinn",
	"Biladus",
	"Uligoth",
	"Rhikor",
	"Aelajan",
	"Wicoe",
	"Rhiremond",
	"Aloev",
	"Kedaeth",
	"Adwardoseth",
	"Haonnon",
	"Nauron",
	"Gwarewyn",
	"Boannon",
	"Theif",
	"Onirak",
	"Tromas",
	"Larareron",
	"Grendald",
	"Wedrijar",
	"Eowaeg",
	"Gekon",
	"Tilared",
	"Yigomar",
	"Galerrabaen",
	"Ethat",
	"Adya",
	"Ona",
	"Ocirawyn",
	"Gleliwyn",
	"Hairash",
	"Zeranyth",
	"Wilajar",
	"Gwohar",
	"Agrof",
	"Vokin",
	"Unec",
	"Thinyth",
	"Thardokoth",
	"Gelaa",
	"Legaup",
	"Afirach",
	"Edigan",
	"Seviawyth",
	"Ocire",
	"Trelar",
	"Vaekon",
	"Rorenydd",
	"Woea",
	"Vowyr",
	"Lynnon",
	"Ereacan",
	"Kerigan",
	"Acedri",
	"Ocorebwyn",
	"Acoia",
	"Ereav",
	"Loalith",
	"Leiwin",
	"Aeravudd",
	"Diref",
	"Vaokin",
	"Thedrith",
	"Astohan",
	"Walet",
	"Lothireth",
	"Unendaand",
	"Oligodan",
	"Vuloth",
	"Eraec",
	"Eowendalath",
	"Legoand",
	"Ariadan",
	"Deri",
	"Ocomadric",
	"Corekith",
	"Kaliric",
	"Nydiep",
	"Ethoath",
	"Gwomath",
	"Veidon",
	"Firannon",
	"Astilibard",
	"Lotheriric",
	"Ybaew",
	"Saetlan",
	"Credon",
	"Ibaetlan",
	"Daeder",
	"Agrirakon",
	"Thigov",
	"Unevudd",
	"Aliwin",
	"Driegord",
	"Ibenda",
	"Ocomac",
	"Haardoloth",
	"Rorem",
};
#endif
