#include "StdAfx.h"

initialiseSingleton(ChatHandler);

static GameCommandHandler AllianceTable[] = 
{
    {"form", &ChatHandler::noCoded},
    {"invite", &ChatHandler::noCoded},
    {"leave", &ChatHandler::noCoded},
    {"list", &ChatHandler::noCoded},
    {"say", &ChatHandler::noCoded},
	{"", NULL},
};

static GameCommandHandler FriendTable[] = 
{
	{"add", &ChatHandler::onFriendAdd},
    {"list", &ChatHandler::noCoded},
	{"remove", &ChatHandler::onFriendRemove},
    {"toggle", &ChatHandler::onFriendAdd},
	{"", NULL},
};

static GameCommandHandler IgnoreTable[] = 
{
	{"add", &ChatHandler::onIgnoreAdd, NULL },
    {"list", &ChatHandler::noCoded, NULL },
	{"remove", &ChatHandler::onIgnoreRemove, NULL },
    {"toggle", &ChatHandler::noCoded, NULL },
};

static GameCommandHandler ChannelTable[] = 
{
    {"join", &ChatHandler::noCoded, NULL },
    {"leave", &ChatHandler::noCoded, NULL },
    {"list", &ChatHandler::noCoded, NULL },
    {"say", &ChatHandler::noCoded, NULL },
	{"", NULL},
};

static GameCommandHandler WarbandTable[] = 
{
    {"join", &ChatHandler::noCoded},
    {"promote", &ChatHandler::noCoded},
    {"demote", &ChatHandler::noCoded},
    {"move", &ChatHandler::noCoded},
    {"swap", &ChatHandler::noCoded},
    {"leader", &ChatHandler::noCoded},
    {"convert", &ChatHandler::noCoded},
    {"inviteaccept", &ChatHandler::noCoded},
    {"invitedecline", &ChatHandler::noCoded},
    {"password", &ChatHandler::noCoded},
    {"info", &ChatHandler::noCoded},
    {"guildonly", &ChatHandler::noCoded},
    {"allianceonly", &ChatHandler::noCoded},
    {"note", &ChatHandler::noCoded},
    {"list", &ChatHandler::noCoded},
	{"resetinstance", &ChatHandler::noCoded, NULL },
    {"say", &ChatHandler::noCoded, NULL },
	{"", NULL},
};

static GameCommandHandler GuildTable[] = 
{
    {"invite", &ChatHandler::noCoded},
    {"leave", &ChatHandler::noCoded},
    {"quit", &ChatHandler::noCoded},
    {"tax", &ChatHandler::noCoded},
    {"tithe", &ChatHandler::noCoded},
    {"promote", &ChatHandler::noCoded},
    {"demote", &ChatHandler::noCoded},
    {"alliancepromote", &ChatHandler::noCoded},
    {"alliancedemote", &ChatHandler::noCoded},
    {"leader", &ChatHandler::noCoded},
    {"remove", &ChatHandler::noCoded},
    {"kick", &ChatHandler::noCoded},
    {"boot", &ChatHandler::noCoded},
    {"addstandardbearer", &ChatHandler::noCoded},
    {"removestandardbearer", &ChatHandler::noCoded},
    {"releasekeep", &ChatHandler::noCoded},
    {"list", &ChatHandler::noCoded},
    {"motd", &ChatHandler::noCoded},
    {"info", &ChatHandler::noCoded},
    {"details", &ChatHandler::noCoded},
    {"email", &ChatHandler::noCoded},
    {"note", &ChatHandler::noCoded},
    {"officernote", &ChatHandler::noCoded},
    {"playstyle", &ChatHandler::noCoded},
    {"summary", &ChatHandler::noCoded},
    {"webpage", &ChatHandler::noCoded},
    {"ranksetname", &ChatHandler::noCoded},
    {"rankenable", &ChatHandler::noCoded},
    {"rankdisable", &ChatHandler::noCoded},
    {"rankpermission", &ChatHandler::noCoded},
    {"rankpermissionreset", &ChatHandler::noCoded},
    {"interests", &ChatHandler::noCoded},
    {"archetypes", &ChatHandler::noCoded},
    {"playtime", &ChatHandler::noCoded},
    {"timezone", &ChatHandler::noCoded},
    {"atmosphere", &ChatHandler::noCoded},
    {"recruitingstatus", &ChatHandler::noCoded},
    {"recruitlevelrange", &ChatHandler::noCoded},
    {"recruiter", &ChatHandler::noCoded},
	{"officersay", &ChatHandler::noCoded},
	{"say", &ChatHandler::noCoded},
	{"", NULL},
};

static GameCommandHandler commandTable[] = 
{
	{"/afk", &ChatHandler::noCoded, NULL },
    {"/alliance", &ChatHandler::noCoded, AllianceTable },
    {"/a", &ChatHandler::noCoded, NULL },
    {"/as", &ChatHandler::noCoded, NULL },
    {"/allianceofficersay", &ChatHandler::noCoded, NULL },
    {"/ao", &ChatHandler::noCoded, NULL },
    {"/aos", &ChatHandler::noCoded, NULL },
    {"/anonymous", &ChatHandler::noCoded, NULL },
    {"/appeal", &ChatHandler::noCoded, NULL },
    {"/appealview", &ChatHandler::noCoded, NULL },
    {"/appealcancel", &ChatHandler::noCoded, NULL },
    {"/assist", &ChatHandler::noCoded, NULL },
    {"/aid", &ChatHandler::noCoded, NULL },
    {"/bug", &ChatHandler::noCoded, NULL },
    {"/channel", &ChatHandler::noCoded, NULL },
    {"/chan", &ChatHandler::noCoded, ChannelTable },
    {"/2", &ChatHandler::noCoded, NULL },
    {"/3", &ChatHandler::noCoded, NULL },
    {"/4", &ChatHandler::noCoded, NULL },
    {"/5", &ChatHandler::noCoded, NULL },
    {"/6", &ChatHandler::noCoded, NULL },
    {"/7", &ChatHandler::noCoded, NULL },
    {"/8", &ChatHandler::noCoded, NULL },
    {"/9", &ChatHandler::noCoded, NULL },
    {"/channelwho", &ChatHandler::noCoded, NULL },
    {"/cloaktoggle", &ChatHandler::onCloakToggle, NULL },
    {"/cloak", &ChatHandler::noCoded, NULL },
    {"/count", &ChatHandler::noCoded, NULL },
    {"/debugwindow", &ChatHandler::noCoded, NULL },
    {"/duelchallenge", &ChatHandler::noCoded, NULL },
    {"/duel", &ChatHandler::noCoded, NULL },
    {"/duelaccept", &ChatHandler::noCoded, NULL },
    {"/dueldecline", &ChatHandler::noCoded, NULL },
    {"/duelcancel", &ChatHandler::noCoded, NULL },
    {"/duelsurrender", &ChatHandler::noCoded, NULL },
    {"/emote", &ChatHandler::onEmote},
    {"::", &ChatHandler::noCoded, NULL },
    {"/emotelist", &ChatHandler::noCoded, NULL },
    {"/friend", &ChatHandler::noCoded, FriendTable },
    {"/guild", &ChatHandler::noCoded, GuildTable },
    {"/g", &ChatHandler::noCoded, NULL },
    {"/gc", &ChatHandler::noCoded, GuildTable },
    {"/o", &ChatHandler::noCoded, NULL },
    {"/follow", &ChatHandler::noCoded, NULL },
    {"/helmtoggle", &ChatHandler::onHelmToggle, NULL },
    {"/helm", &ChatHandler::noCoded, NULL },
    {"/help", &ChatHandler::noCoded, NULL },
    {"/hide", &ChatHandler::noCoded, NULL },
    {"/ignoreadd", &ChatHandler::noCoded, NULL },
    {"/ignorelist", &ChatHandler::noCoded, NULL },
    {"/ignoreremove", &ChatHandler::noCoded, NULL },
    {"/ignoretoggle", &ChatHandler::noCoded, NULL },
    {"/ignore", &ChatHandler::noCoded, NULL },
    {"/inspect", &ChatHandler::noCoded, NULL },
    {"/inspectable", &ChatHandler::noCoded, NULL },
    {"/inspectablebraggingrights", &ChatHandler::noCoded, NULL },
    {"/inspectabletradeskills", &ChatHandler::noCoded, NULL },
    {"/join", &ChatHandler::noCoded, NULL },
    {"/language", &ChatHandler::noCoded, NULL },
    {"/lfguild", &ChatHandler::noCoded, NULL },
    {"/lfm", &ChatHandler::noCoded, NULL },
    {"/lfp", &ChatHandler::noCoded, NULL },
    {"/lfg", &ChatHandler::noCoded, NULL },
    {"/location", &ChatHandler::noCoded, NULL },
    {"/loc", &ChatHandler::noCoded, NULL },
    {"/lockouts", &ChatHandler::noCoded, NULL },
    {"/logout", &ChatHandler::noCoded, NULL },
    {"/camp", &ChatHandler::noCoded, NULL },
    {"/openlist", &ChatHandler::noCoded, NULL },
    {"/openpartyinterest", &ChatHandler::noCoded, NULL },
    {"/openjoin", &ChatHandler::noCoded, NULL },
    {"/partyroll", &ChatHandler::noCoded, NULL },
    {"/partyrandom", &ChatHandler::noCoded, NULL },
    {"/partyresetinstance", &ChatHandler::noCoded, NULL },
    {"/partynote", &ChatHandler::noCoded, NULL },
    {"/openpartynote", &ChatHandler::noCoded, NULL },
    {"/lfpnote", &ChatHandler::noCoded, NULL },
    {"/partysay", &ChatHandler::noCoded, NULL },
    {"/p", &ChatHandler::onSayParty},
    {"/partyjoin", &ChatHandler::noCoded, NULL },
    {"/partyinvite", &ChatHandler::noCoded, NULL },
    {"/invite", &ChatHandler::onInvite},
    {"/partyinviteopen", &ChatHandler::noCoded, NULL },
    {"/oinvite", &ChatHandler::noCoded, NULL },
    {"/partyremove", &ChatHandler::noCoded, NULL },
    {"/partykick", &ChatHandler::noCoded, NULL },
    {"/partyboot", &ChatHandler::noCoded, NULL },
    {"/kick", &ChatHandler::noCoded, NULL },
    {"/partyleader", &ChatHandler::noCoded, NULL },
    {"/makeleader", &ChatHandler::noCoded, NULL },
    {"/mainassist", &ChatHandler::noCoded, NULL },
    {"/makemainassist", &ChatHandler::noCoded, NULL },
    {"/partyleave", &ChatHandler::noCoded, NULL },
    {"/partyquit", &ChatHandler::noCoded, NULL },
    {"/leave", &ChatHandler::onLeave},
    {"/disband", &ChatHandler::noCoded, NULL },
    {"/partylfwarband", &ChatHandler::noCoded, NULL },
    {"/partylfw", &ChatHandler::noCoded, NULL },
    {"/partyopen", &ChatHandler::noCoded, NULL },
    {"/partyprivate", &ChatHandler::noCoded, NULL },
    {"/partyclose", &ChatHandler::noCoded, NULL },
    {"/partypassword", &ChatHandler::noCoded, NULL },
    {"/partyguildonly", &ChatHandler::noCoded, NULL },
    {"/partyallianceonly", &ChatHandler::noCoded, NULL },
    {"/partylist", &ChatHandler::noCoded, NULL },
    {"/petname", &ChatHandler::noCoded, NULL },
    {"/played", &ChatHandler::noCoded, NULL },
    {"/random", &ChatHandler::noCoded, NULL },
    {"/roll", &ChatHandler::noCoded, NULL },
    {"/reply", &ChatHandler::noCoded, NULL },
    {"/r", &ChatHandler::noCoded, NULL },
    {"/transferguild", &ChatHandler::noCoded, NULL },
    {"/reportgoldseller", &ChatHandler::noCoded, NULL },
    {"/rgs", &ChatHandler::noCoded, NULL },
    {"/rg", &ChatHandler::noCoded, NULL },
    {"/reloadui", &ChatHandler::noCoded, NULL },
    {"/refund", &ChatHandler::noCoded, NULL },
    {"/respec", &ChatHandler::noCoded, NULL },
    {"/rvr", &ChatHandler::noCoded, NULL },
    {"/pvp", &ChatHandler::noCoded, NULL },
    {"/rvrmap", &ChatHandler::noCoded, NULL },
	{"/quit", &ChatHandler::onQuit},
    {"/exit", &ChatHandler::noCoded, NULL },
    {"/q", &ChatHandler::noCoded, NULL },
    {"/say", &ChatHandler::onSay },
    {"/s", &ChatHandler::noCoded, NULL },
    {"'", &ChatHandler::noCoded, NULL },
    {"/scenariosay", &ChatHandler::noCoded, NULL },
    {"/sc", &ChatHandler::noCoded, NULL },
    {"/scenariotell", &ChatHandler::noCoded, NULL },
    {"/sp", &ChatHandler::noCoded, NULL },
    {"/sp1", &ChatHandler::noCoded, NULL },
    {"/sp2", &ChatHandler::noCoded, NULL },
    {"/sp3", &ChatHandler::noCoded, NULL },
    {"/sp4", &ChatHandler::noCoded, NULL },
    {"/sp5", &ChatHandler::noCoded, NULL },
    {"/sp6", &ChatHandler::noCoded, NULL },
    {"/sp7", &ChatHandler::noCoded, NULL },
    {"/sp8", &ChatHandler::noCoded, NULL },
    {"/sp9", &ChatHandler::noCoded, NULL },
    {"/sp10", &ChatHandler::noCoded, NULL },
    {"/script", &ChatHandler::noCoded, NULL },
    {"/shout", &ChatHandler::onShout},
    {"/socialplaystyle", &ChatHandler::noCoded, NULL },
    {"/socialinterests", &ChatHandler::noCoded, NULL },
    {"/socialatmosphere", &ChatHandler::noCoded, NULL },
    {"/socialplaytime", &ChatHandler::noCoded, NULL },
    {"/socialtimezone", &ChatHandler::noCoded, NULL },
    {"/stuck", &ChatHandler::noCoded, NULL },
    {"/target", &ChatHandler::noCoded, NULL },
	{"/tell", &ChatHandler::onWhisp},
    {"/t", &ChatHandler::noCoded, NULL },
	{"/whisper", &ChatHandler::onWhisp},
    {"/w", &ChatHandler::noCoded, NULL },
    {"/msg", &ChatHandler::noCoded, NULL },
    {"/send", &ChatHandler::noCoded, NULL },
    {"/time", &ChatHandler::noCoded, NULL },
    {"/togglecloakheraldry", &ChatHandler::noCoded, NULL },
    {"/togglealwaysformprivate", &ChatHandler::noCoded, NULL },
    {"/warband", &ChatHandler::noCoded, NULL },
    {"/war", &ChatHandler::noCoded, NULL },
    {"/wbc", &ChatHandler::noCoded, WarbandTable },
    {"/ra", &ChatHandler::noCoded, NULL },
    {"/who", &ChatHandler::noCoded, NULL },
    {"/", NULL}
};

GmCommandHandler gmcommandTable[] = 
{
	// SERVER COMMANDS
	{".gps",		"",		0,	&ChatHandler::onGps,			"Affiche votre position" ,							3},
	{".server", "info",		0,	&ChatHandler::onServer,			"Affiche les info serveur" ,						3},
	{".server", "reboot",	1,	&ChatHandler::onReboot,			"Reboot le serveur" ,								3},
	{".announce", "",		1,	&ChatHandler::onAnnounce,		"Boradcast message" ,								3},

	// PLAYER COMMANDS
	{".mod", "speed",	1,		&ChatHandler::onSpeed,			"Modifie votre vitesse : 50-5000" ,					3},
	{".mod", "money",	1,		&ChatHandler::onMoney,			"Ajoute de l'argent" ,								3},
	{".mod", "item",	1,		&ChatHandler::onItem,			"Ajoute un objet" ,									3},
	{".mod", "level",	1,		&ChatHandler::onLevel,			"Modifi le lvl" ,									3},

	// NPC COMMANDS
	{".npc", "info",	0,		&ChatHandler::onNpcInfo,		"Affiche les infos de la creature" ,				3},
	{".npc", "spawn",	2,		&ChatHandler::onNpcSpawn,		"Spawn une creature (entry)(save)" ,						3},
	{".npc", "del",		0,		&ChatHandler::onNpcDelete,		"Supprime la creature selectionnée" ,				3},
	{".npc", "turn",	1,		&ChatHandler::onNpcTurn,		"Modifi la rotation de la creature" ,				3},
	{".npc", "quest",	0,		&ChatHandler::onNpcQuests,		"Affiche la liste des quetes" ,						3},
	{".bytes", "",		2,		&ChatHandler::onNpcBytes,		"Modify les bytes d'un npc" ,						3},

	// GO
	{".go", "spawn",	2,		&ChatHandler::onGoSpawn,		"Spawn une un gameobject (entry)" ,					3},

	// UTILS
	{".rec", "add",		1,		&ChatHandler::onRecallAdd,		"Add un recall (name)" ,							3},
	{".rec", "list",	0,		&ChatHandler::onRecallList,		"Affiche la liste des recall" ,						3},
	{".rec", "rem",		1,		&ChatHandler::onRecallRemove,	"Delete un recall (name)" ,							3},
	{".rec", "port",	1,		&ChatHandler::onRecallPort,		"Teleporte sur un recall (name)" ,					3},
	{".tp", "",			5,		&ChatHandler::onTeleport,		"Teleporte (zone x y z o)" ,						3},

	{".ap", "",			1,		&ChatHandler::onAppear,		"Appear to player (name)" ,								3},
	{".sum", "",		1,		&ChatHandler::onSummon,		"Summon player (name)" ,								3},
	{".kill", "",		0,		&ChatHandler::onKill,		"Kill target" ,											3},
	{".givexp", "",		1,		&ChatHandler::onGiveXp,		"Give xp" ,												3},

	// Quest 
	//{".quest", "info",		1,	&ChatHandler::onQuestInfo,		"Affiche les info de la quète (entry)" ,				3},
	//{".quest", "delete",	1,	&ChatHandler::onQuestDelete,	"Supprime une quète (entry)" ,							3},
	//{".quest", "setprev",	2,	&ChatHandler::onQuestSetPrev,	"Ajoute une ancienne quète a finir (entry)(entryancienne)",		3},

	//{".qobj", "info",		1,	&ChatHandler::onQObjInfo,		"Affiche les info de l'objectif (guid)" ,				3},
	//{".qobj", "delete",		1,	&ChatHandler::onQObjDelete,		"Supprime un objectif de quète (guid)" ,				3},
	//{".qobj", "killnpc",	2,	&ChatHandler::onQObjKillNpc,	"Met un npc a tuer en objectif (guid)(npcnentry)" ,		3},
	//{".qobj", "speaknpc",	2,	&ChatHandler::onQObjSpeakNpc,	"Met un npc a parler en objectif (guid)(npcnentry)" ,	3},

	//{".pq", "info",			1,	&ChatHandler::onPQInfo,		"Affiche les information d'un Public Quest (entry)" ,	3},
	//{".pq", "next",			0,	&ChatHandler::onPQNext,		"Passe au stage suivant de la PQ" ,						3},

	{""	,		"",		0,	NULL,	"" ,					3},
};

ChatHandler::ChatHandler()
{


}
void ExecuteTable(Player * Plr,GameCommandHandler Handler[],string command,string text)
{
	for(int i=0;Handler[i].chatHandler != NULL;++i)
	{
		if( Handler[i].name == command )
		{
			if(Handler[i].m_table)
			{
				string subcommande = text.substr(0,text.find_first_of(' '));
				text.erase(0,subcommande.size()+1);

				ExecuteTable(Plr,Handler[i].m_table,subcommande,text);
			}
			else if(Handler[i].chatHandler)
				(sChat.*Handler[i].chatHandler)(Plr,command,text);
		}
	}
}
void ChatHandler::ExecuteFunction(Player * Plr,string command,string value)
{
	Log.Debug("ChatHandler","ExecuteFunction com(%s),value(%s)",command.c_str(),value.c_str());

	if(command[0]!='/')
		return;

	if(value[0]=='.' && value.size() > 1) // GM command
	{
		string values = value;
		string gmcommand = values.substr(0,values.find_first_of(' '));
		values.erase(0,gmcommand.length()+1);

		if(gmcommand == ".help" || gmcommand == ".?")
		{
			sendCommandList(Plr);
			return;
		}
		else
		{
			vector<string> m_values;

			int a = values.find_first_of(' ');
				while( a != string::npos )
				{
					string val = values.substr(0,a);
					values.erase(0,val.length()+1);

					a = values.find_first_of(' ');

					m_values.push_back(val);
				}

			if(values.size())
				m_values.push_back( values );

			for(int i=0;gmcommandTable[i].chatHandler != NULL;++i)
			{
				if( string(gmcommandTable[i].name).find(gmcommand)  != string::npos ) // Si la commande existe
				{
					if(Plr->GetPermission() < gmcommandTable[i].permission ) // Permission trop basse
						break;

					string subcommand = gmcommandTable[i].subname;
					if(subcommand.size() > 1) // Si il y a un sub commande
					{
						if( m_values.size() && subcommand.find(m_values[0]) != string::npos ) // Si il ya un subcommande et qu'elle correspond
						{
							// speed 500 = 2
							if(int(m_values.size()) >= gmcommandTable[i].minvalues+1)
								(this->*gmcommandTable[i].chatHandler)(Plr,gmcommand,m_values);
							else sendCommandList(Plr,gmcommand,subcommand);
							return;

						} // SI il n'y a pas de subcommand ou qu'elle ne correspond pas on pass au prochain
						else if(!m_values.size()) // Si il n 'y a pas de subcommand just command on envoi la liste des sub
						{
							sendCommandList(Plr,gmcommand);
							return;
						}
						else continue;
					}
					else // Si il n'y a pas de subcommande on execute la fonction
					{
							if(int(m_values.size()) >= gmcommandTable[i].minvalues)
								(this->*gmcommandTable[i].chatHandler)(Plr,gmcommand,m_values);
							else sendCommandList(Plr,gmcommand);
						return;
					}
				}
			}
		}
	}
	

	ExecuteTable(Plr,commandTable,command,value);
}

Buffer * ChatHandler::BuildMessagePacket(ClientCom * Com,uint16 oid,string name,string text,ChatLogFilters type)
{
	if(name.size() <=0)
		name = "Serveur";

	uint16 responseSize = 7+1+name.length()+1+1+1+1+text.length()+1;
	Buffer* b = Com->m_bufferPool->allocateBuffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_CHAT);
	b->write<uint16>( oid );

	b->write<uint8>(type); // Type
	b->write<uint8>(0); // zone
	b->write<uint8>(0); // unk2
	b->write<uint8>(0); // unk3
	b->write<uint8>(0); // unk4

	b->write<uint8>(name.length()+1);
	b->write(reinterpret_cast<const uint8*>(name.c_str()),name.length());
	b->write<uint8>(0); // unk5
	b->write<uint8>(0); // unk6
	b->write<uint8>(text.length()+1);
	b->write(reinterpret_cast<const uint8*>(text.c_str()),text.length());
	b->write<uint8>(0); // unk6
	b->write<uint8>(0); // unk7

	return b;
}
void ChatHandler::BuildSocialInfo(Buffer *b,PlayerInfo *Info,bool troncdeco)
{
	if(!Info) return;

	if(Info->m_player)
		BuildSocialInfo(b,Info->m_player,true);
	else
	{
		b->write<uint32>(Info->character_id);
		b->write<uint16>(Info->name.length()+1);
		b->write(reinterpret_cast<const uint8*>(Info->name.c_str()),Info->name.length());
		b->write<uint16>(0x0000);
		b->write<uint8>(Info->m_player ? 0x01 : 0x00);
		b->write<uint8>(Info->m_player ? 0x01 : 0x00);

		if(troncdeco && !Info->m_player) return;

		b->write<uint8>(Info->level);
		b->write<uint16>(0x0000);
		b->write<uint16>(Info->career);
		b->write<uint16>(Info->zone);
		b->write<uint16>(1); // Guild Size
		b->write<uint8>(0x00);
	}
	//21
}
void ChatHandler::BuildSocialInfo(Buffer *b,Player * Plr,bool connected)
{
	if(Plr == NULL) return;

	b->write<uint32>(Plr->GetPlayerID());
	b->write<uint16>(Plr->GetName().length()+1);
	b->write(reinterpret_cast<const uint8*>(Plr->GetName().c_str()),Plr->GetName().length());
	b->write<uint16>(0x0000);
	b->write<uint8>(connected);
	b->write<uint8>(connected);

	if( !connected ) return;

	b->write<uint8>(Plr->GetLevel());
	b->write<uint16>(0x0000);
	b->write<uint16>(Plr->GetCareer());
	b->write<uint16>(Plr->GetZoneId());
	b->write<uint16>(1); // Guild Size
	b->write<uint8>(0x00);
	//21
}
void ChatHandler::BuildItemInfo(Buffer *b,const ItemProto *Proto,Item *itm)
{
	if(!Proto)
		return;

	b->write<uint8>(0x00); // unk
	b->write<uint32>(Proto->entry);//18
	b->write<uint32>(Proto->modelid);
	b->write<uint16>(Proto->slot);
	b->write<uint8>(Proto->type);
	b->write<uint8>(Proto->min_rank); // Min Rank
	b->write<uint8>(Proto->min_rank); // Min Rank
	b->write<uint8>(0x00); // 1.3.5
	b->write<uint8>(Proto->min_renown); // Min Renown
	b->write<uint8>(0x00); // Unique equiped
	b->write<uint8>(Proto->rarity);
	b->write<uint8>(Proto->bind); // bind ?
	b->write<uint8>(Proto->race);
	b->write<uint32>(Proto->career); //36

	b->write<uint32>(0); // 45
	b->write<uint32>(Proto->sellprice);
	b->write<uint16>(itm ? itm->GetCount() : 1);
	b->write<uint32>(0x00);

	b->write<uint32>(Proto->skill); // skill mask Proto->skill
	b->write<uint16>(Proto->dps ? Proto->dps : (Proto->armor ? Proto->armor : 0));
	b->write<uint16>(Proto->speed);
	b->writeBytePascalString(Proto->name);

	int z=0;
	for(z=0;z<MAX_ITEMS_STATS;z++)
	{
		if( Proto->ItemStat[z].ItemStatType > GameData::ABILITYRESULT_WRONG_WEAPON_TYPE
			|| Proto->ItemStat[z].ItemStatValue <= 0 || Proto->ItemStat[z].ItemStatType == 0)
			break;
	}

	b->write<uint8>(z); // SpellCount
	
	for(int i=0;i<z;++i)
	{
		b->write<uint8>( Proto->ItemStat[i].ItemStatType );
		b->write<uint16>( Proto->ItemStat[i].ItemStatValue );
		b->fill(0,5);
	}

	b->write<uint16>(0x00);
	b->write<uint8>(0x00); // artisana info
		// for artisana info , 3 bytes
	b->write<uint8>(0x00);
	b->write<uint8>(Proto->talismanslot);
	if(Proto->talismanslot)
	{
		for(uint8 i=0;i<Proto->talismanslot;++i)
		{
			b->write<uint32>(0);
		}
	}

	b->write<uint8>( strlen(Proto->desc) );
	b->write(reinterpret_cast<const uint8*>(Proto->desc), strlen(Proto->desc) );
	
	b->write<uint8>(0x00);
	b->write<uint8>(0x00); // dunno but maybe 01
	b->write<uint8>(0x00);
	b->write<uint8>(0x00);

	b->write<uint16>(0x0302);

	b->fill(0,8);
	b->write<uint8>(0x00); // Type culture , etc etc
	b->write<uint8>(0x00); // type , recipian , soil etc etc
	b->fill(0,11);
}
void ChatHandler::noCoded(Player * Plr,string command,string value)
{
	Log.Debug("ChatHandler","No Coded command ![%s]-[%s]",command.c_str(),value.c_str());

}
void ChatHandler::sendCommandList(Player * Plr,string command,string subb)
{
	// Command(mod),subcommand,description;
	map<string,map<string,string>> m_commands;

	int i=0;
	for(int i=0;gmcommandTable[i].chatHandler != NULL;++i)
	{
		string name = gmcommandTable[i].name;
		string sub = gmcommandTable[i].subname;
		string desc = gmcommandTable[i].description;
		
		if(command.size() > 1 && name.find(command) != string::npos )
			continue;
		if(subb.size() > 1 && sub.find(subb) != string::npos)
			continue;

		m_commands[name][sub] = desc;
	}

	Plr->sendStringMessage(Plr->GetOid(),Plr->GetName(),"Liste des commandes :");

	for(map<string,map<string,string>>::iterator itr = m_commands.begin();itr != m_commands.end();++itr)
	{
		for( map<string,string>::iterator itt=itr->second.begin();itt!= itr->second.end();++itt)
		{
			stringstream tt;
			tt << "["<<itr->first<<"]";
			tt << "->[" << itt->first<<"] :" << itt->second;
			Plr->sendStringMessage(Plr->GetOid(),Plr->GetName(),tt.str());
		}
	}


}
void ChatHandler::onSay(Player * Plr,string command,string value)
{
	if( Plr == NULL ) return;
	if( !Plr->IsInWorld() ) return;

	Buffer * b = BuildMessagePacket(Plr->GetCom(),Plr->GetOid(),Plr->GetName(),value);
	if( Plr->GetZoneMgr() ) Plr->GetZoneMgr()->SendMessageToCellPlayers(b,Plr->GetOffX(),Plr->GetOffY(),Plr->GetCom());
	Plr->DisposeBuffer(b);

	sEventHook.ExecuteHook(EVENTS_SEND_CHAT_TEXT,2,Plr,value);
}
void ChatHandler::onShout(Player * Plr,string command,string value)
{
	if( Plr == NULL ) return;
	if( !Plr->IsInWorld() ) return;

	Buffer * b = BuildMessagePacket(Plr->GetCom(),Plr->GetOid(),Plr->GetName(),value,CHATLOGFILTERS_SHOUT);
	if( Plr->GetZoneMgr() ) Plr->GetZoneMgr()->SendMessageToCellPlayers(b,Plr->GetOffX(),Plr->GetOffY(),Plr->GetCom());
	Plr->DisposeBuffer(b);
}
void ChatHandler::onWhisp(Player * Plr,string command,string value)
{
	string receiver = value.substr(0,value.find_first_of(' '));
	value.erase(0,receiver.length()+1);

	Player * Receiver = objmgr.GetPlayerByName(receiver);

	if(Receiver==NULL)	Plr->sendLocalizedString(value,TEXT_SN_LISTS_ERR_PLAYER_NOT_FOUND);
	else				Plr->sendWipeMessage(Receiver,value);
}
void ChatHandler::onEmote(Player *Plr,string command,	string value)
{
	if( Plr == NULL ) return;
	if( !Plr->IsInWorld() ) return;

	Buffer *b = BuildMessagePacket(Plr->GetCom(),Plr->GetOid(),Plr->GetName(),value,CHATLOGFILTERS_EMOTE);
	if( Plr->GetZoneMgr() ) Plr->GetZoneMgr()->SendMessageToCellPlayers(b,Plr->GetOffX(),Plr->GetOffY(),Plr->GetCom(),true);
	Plr->DisposeBuffer(b);
}
void ChatHandler::onQuit(Player * Plr,string command,string value)
{
	objmgr.PlayerLeave(Plr);
}
void ChatHandler::onLeave(Player *Plr,string command,	string value)
{
	if(Plr == NULL) return;

	if(Plr->GetGroup())
		Plr->SetGroup(NULL);
}
void ChatHandler::onSayParty(Player *Plr,string command,	string value)
{
	if(Plr == NULL) return;
	if(!Plr->GetGroup()) return;

	Plr->GetGroup()->sendMessageToGroup(Plr,value);
}
void ChatHandler::onHelmToggle(Player *Plr,string command,	string value)
{
	if(Plr == NULL) return;

	/*if(atoi(value.c_str()))
		Plr->GetItemInterface()->TogglePart(0,true);
	else Plr->GetItemInterface()->TogglePart(0,false);*/
}
void ChatHandler::onCloakToggle(Player *Plr,string command,	string value)
{
	if(Plr == NULL) return;

	/*if(atoi(value.c_str()))
		Plr->GetItemInterface()->TogglePart(1,true);
	else Plr->GetItemInterface()->TogglePart(1,false);*/
}