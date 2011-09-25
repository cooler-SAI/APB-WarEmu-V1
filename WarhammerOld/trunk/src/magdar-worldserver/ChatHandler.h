#ifndef CHATHANDLER_H
#define CHATHANDLER_H

class ChatHandler : public Singleton< ChatHandler >
{
public :

	ChatHandler();

	void ExecuteFunction(Player * Plr,string command,string value);

	// Game Packet Handler
	void noCoded(Player * Plr,string command,	string value);
	void onSay(Player * Plr,string command,		string value);
	void onShout(Player * Plr,string command,	string value);
	void onWhisp(Player * Plr,string command,	string value);
	void onQuit(Player * Plr,string command,	string value);
	void onInvite(Player *Plr,string command,	string value);
	void onEmote(Player *Plr,string command,	string value);
	void onLeave(Player *Plr,string command,	string value);
	void onSayParty(Player *Plr,string command,	string value);


	// Item
	void onHelmToggle(Player *Plr,string command,	string value);
	void onCloakToggle(Player *Plr,string command,	string value);


	// Social Command
	void onFriendAdd(Player *Plr,string command,	string value);
	void onFriendRemove(Player *Plr,string command,	string value);
	void onIgnoreAdd(Player *Plr,string command,	string value);
	void onIgnoreRemove(Player *Plr,string command,	string value);

	/*********************************************************/
	/* GM COMMAND
	/*********************************************************/
	void onGps(Player * Plr,string command,		vector<string> m_values);
	void onServer(Player * Plr,string command,	vector<string> m_values);
	void onSpeed(Player * Plr,string command,	vector<string> m_values);
	void onMoney(Player * Plr,string command,	vector<string> m_values);
	void onItem(Player * Plr,string command,	vector<string> m_values);
	void onLevel(Player * Plr,string command,	vector<string> m_values);
	void onKill(Player * Plr,string command,	vector<string> m_values);
	void onReboot(Player * Plr,string command,	vector<string> m_values);
	void onGiveXp(Player * Plr,string command,	vector<string> m_values);
	void onAnnounce(Player * Plr,string command,	vector<string> m_values);

	// NPC command
	void onNpcMod(Player * Plr,string command,	vector<string> m_values);
	void onNpcInfo(Player * Plr,string command,	vector<string> m_values);
	void onNpcDelete(Player * Plr,string command,	vector<string> m_values);
	void onNpcSpawn(Player * Plr,string command,	vector<string> m_values);
	void onNpcTurn(Player * Plr,string command,	vector<string> m_values);
	void onNpcQuests(Player * Plr,string command,	vector<string> m_values);
	void onNpcBytes(Player * Plr,string command,	vector<string> m_values);

	// GO
	void onGoSpawn(Player * Plr,string command,	vector<string> m_values);

	//Teleport Command
	void onRecallAdd(Player * Plr,string command,	vector<string> m_values);
	void onRecallList(Player * Plr,string command,	vector<string> m_values);
	void onRecallRemove(Player * Plr,string command,	vector<string> m_values);
	void onRecallPort(Player * Plr,string command,	vector<string> m_values);
	void onTeleport(Player * Plr,string command,	vector<string> m_values);
	void onAppear(Player * Plr,string command,	vector<string> m_values);
	void onSummon(Player * Plr,string command,	vector<string> m_values);

	// Quests
	//void onQuestInfo(Player * Plr,string command,	vector<string> m_values);
	//void onQuestDelete(Player * Plr,string command,	vector<string> m_values);
	//void onQuestSetPrev(Player * Plr,string command,	vector<string> m_values);
	
	void onQObjInfo(Player * Plr,string command,	vector<string> m_values);
	void onQObjDelete(Player * Plr,string command,	vector<string> m_values);
	void onQObjKillNpc(Player * Plr,string command,	vector<string> m_values);
	void onQObjSpeakNpc(Player * Plr,string command,	vector<string> m_values);

	void onPQInfo(Player * Plr,string command,	vector<string> m_values);
	void onPQNext(Player * Plr,string command,	vector<string> m_values);

	void sendCommandList(Player * Plr,string command="",string subb="");

	Buffer * BuildMessagePacket(ClientCom * Com,uint16 oid,string name,string text,ChatLogFilters type=CHATLOGFILTERS_SAY);

	void BuildSocialInfo(Buffer *b,PlayerInfo *Info,bool troncdeco=false);
	void BuildSocialInfo(Buffer *b,Player * Plr,bool connected=true);

	void BuildItemInfo(Buffer *b,const ItemProto *Proto,Item *itm=NULL);
};

struct GameCommandHandler
{
    char const* name;
    void (ChatHandler::*chatHandler)(Player * Plr,string command,string value);
	GameCommandHandler * m_table;
};

struct GmCommandHandler
{
    char const* name;
	char const* subname;
	int minvalues;
    void (ChatHandler::*chatHandler)(Player * Plr,string command,vector<string> m_values);
	char const* description;
	uint8 permission;
};


#define sChat ChatHandler::getSingleton( )

#endif

