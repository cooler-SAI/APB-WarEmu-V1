#include <stdarg.h>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

using namespace std;

static const char * opcodeS[0xFF+1] =
{
    "UNKNOWN",
    "UNKNOWN",
    "F_QUEST",
    "F_UPDATE_SIEGE_LOOK_AT",
    "F_PLAYER_EXIT",
    "F_PLAYER_HEALTH",
    "F_CHAT",
    "F_TEXT",
    "UNKNOWN",
    "F_OBJECT_STATE",
	"F_OBJECT_DEATH", 
	"F_PING",
    "F_PLAYER_QUIT",
    "F_DUMP_STATICS",
    "UNKNOWN",
    "F_CONNECT",
    "F_DISCONNECT",
    "F_HEARTBEAT",
    "UNKNOWN",
    "F_REQUEST_CHAR_TEMPLATES",
    "F_HIT_PLAYER",
    "F_DEATHSPAM",
	"F_REQUEST_INIT_OBJECT",
    "F_OPEN_GAME",
	"F_PLAYER_INFO",
    "F_WORLD_ENTER",
    "F_CAMPAIGN_STATUS",
    "F_REQ_CAMPAIGN_STATUS", 
    "UNKNOWN",
    "F_GUILD_DATA",
    "F_MAX_VELOCITY",
    "F_SWITCH_REGION",
    "F_PET_INFO",
    "F_PLAYER_CLEAR_DEATH",
    "F_COMMAND_CONTROLLED",
    "UNKNOWN",
    "UNKNOWN",
    "F_GUILD_COMMAND",
    "UNKNOWN",                
    "F_REQUEST_TOK_REWARD",
    "F_SURVEY_BEGIN",
    "F_SHOW_DIALOG",
    "F_PLAYERORG_APPROVAL",
    "F_QUEST_INFO",
    "F_RANDOM_NAME_LIST_INFO",
    "UNKNOWN",
    "UNKNOWN",
    "F_INVITE_GROUP",
    "F_JOIN_GROUP",
    "F_PLAYER_DEATH",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "F_DUMP_ARENAS_LARGE",
    "UNKNOWN",
    "F_GROUP_COMMAND",
    "F_ZONEJUMP",
    "F_PLAYER_EXPERIENCE",
    "F_XENON_VOICE",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "F_REQUEST_WORLD_LARGE",
    "F_ACTION_COUNTER_INFO",
    "UNKNOWN",
    "UNKNOWN",
    "F_ACTION_COUNTER_UPDATE",
    "UNKNOWN",
    "F_PLAYER_STATS",
    "F_MONSTER_STATS",
    "F_PLAY_EFFECT",
    "F_REMOVE_PLAYER",
    "F_ZONEJUMP_FAILED",
    "F_TRADE_STATUS",
    "UNKNOWN",
    "UNKNOWN",
    "F_PLAYER_RENOWN",
    "F_MOUNT_UPDATE",
    "F_PLAYER_LEVEL_UP",
    "F_ANIMATION",
    "F_PLAYER_WEALTH",
    "F_TROPHY_SETLOCATION",
    "F_REQUEST_CHAR",
    "F_REQUEST_CHAR_RESPONSE",
    "F_REQUEST_CHAR_ERROR",
    "F_CHARACTER_PREFS",
    "F_SEND_CHARACTER_RESPONSE",
    "F_SEND_CHARACTER_ERROR",
    "F_PING_DATAGRAM",
    "UNKNOWN",
    "F_ENCRYPTKEY",
    "F_PQLOOT_TRIGGER",
    "F_SET_TARGET",
    "UNKNOWN",
    "F_MYSTERY_BAG",
    "F_PLAY_SOUND",
    "F_PLAYER_STATE2",
    "F_QUERY_NAME",
    "F_QUERY_NAME_RESPONSE",
    "F_ADD_NAME",
    "UNKNOWN",
    "UNKNOWN",
    "F_DELETE_NAME",
    "UNKNOWN",
    "F_CHECK_NAME",
    "F_CHECK_NAME_RESPONSE",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "F_LOCALIZED_STRING",
    "F_KILLING_SPREE",
    "F_CREATE_STATIC",
    "F_CREATE_MONSTER",
    "F_PLAYER_IMAGENUM",
    "UNKNOWN",
    "F_TRANSFER_ITEM",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "F_CRAFTING_STATUS",
    "F_REQUEST_LASTNAME",
    "UNKNOWN",
    "F_INIT_PLAYER",
    "F_REQUEST_INIT_PLAYER",
    "F_SET_ABILITY_TIMER",
    "UNKNOWN",
    "S_PID_ASSIGN",
    "S_PONG",
    "S_CONNECTED",
    "S_WORLD_SENT",
    "S_NOT_CONNECTED",
    "S_GAME_OPENED",
    "F_MAIL",
    "S_DATAGRAM_ESTABLISHED",
    "S_PLAYER_INITTED",
    "S_PLAYER_LOADED",
    "F_RECEIVE_ENCRYPTKEY",
    "UNKNOWN",
    "F_MORALE_LIST",
    "F_SURVEY_ADDQUESTION",
    "F_SURVEY_END", 
    "F_SURVEY_RESULT",
    "F_EMOTE",
    "F_CREATE_CHARACTER",
    "F_DELETE_CHARACTER",
    "F_GFX_MOD",
    "F_INSTANCE_INFO",
    "UNKNOWN",
    "F_KEEP_STATUS",
    "F_PLAY_TIME_STATS",
    "F_CATAPULT",
    "F_GRAVITY_UPDATE",
    "F_HELP_DATA",
    "F_UPDATE_LASTNAME",
    "UNKNOWN",
    "UNKNOWN",
    "F_GET_CULTIVATION_INFO",
    "F_CRASH_PACKET",
    "F_LOGINQUEUE",
    "F_INTERRUPT",
    "F_INSTANCE_SELECTED",
    "F_ACTIVE_EFFECTS", 
    "UNKNOWN",
    "F_SERVER_INFO",
    "F_START_SIEGE_MULTIUSER",
    "F_SIEGE_WEAPON_RESULTS",
	"F_INTERACT_QUEUE",  
    "F_UPDATE_HOT_SPOT",
    "F_GET_ITEM",
    "F_DUEL",
    "F_PLAYER_JUMP",
    "F_INTRO_CINEMA", 
    "F_MAGUS_DISC_UPDATE",
    "F_FIRE_SIEGE_WEAPON",
    "F_GRAPHICAL_REVISION",
    "UNKNOWN",
    "F_AUCTION_POST_ITEM",
    "F_CAST_PLAYER_EFFECT",
    "F_AUCTION_SEARCH_QUERY",
    "F_FLIGHT", 
	"F_SOCIAL_NETWORK",
    "F_AUCTION_SEARCH_RESULT",
    "F_PLAYER_ENTER_FULL",
    "UNKNOWN",
    "UNKNOWN",
    "F_AUCTION_BID_ITEM",
    "F_ESTABLISH_DATAGRAM",
    "F_PLAYER_INVENTORY",
    "F_CHARACTER_INFO",
    "F_INIT_STORE",
    "F_STORE_BUY_BACK",
    "F_OBJECTIVE_INFO",
    "F_OBJECTIVE_UPDATE",
    "F_SCENARIO_INFO",
    "F_SCENARIO_POINT_UPDATE",
    "F_OBJECTIVE_STATE",
    "F_REALM_BONUS",
    "F_OBJECTIVE_CONTROL",
    "F_INTERFACE_COMMAND",
    "F_SCENARIO_PLAYER_INFO",
    "F_FLAG_OBJECT_STATE",
    "F_FLAG_OBJECT_LOCATION",
	"F_CITY_CAPTURE",
    "F_ZONE_CAPTURE",
    "F_SALVAGE_ITEM",
    "F_AUCTION_BID_STATUS",
    "F_PUNKBUSTER",
    "F_ITEM_SET_DATA",
	"F_INTERACT",
    "UNKNOWN",
    "UNKNOWN",
	"F_DO_ABILITY",
    "F_SET_TIME",
    "F_INIT_EFFECTS",
    "F_GROUP_STATUS",
    "F_USE_ITEM",
    "F_USE_ABILITY",
    "F_INFLUENCE_DETAILS",
	"F_SWITCH_ATTACK_MODE",
    "F_BUG_REPORT",
    "F_OBJECT_EFFECT_STATE",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "F_EXPERIENCE_TABLE",
    "F_CREATE_PLAYER",
    "F_UPDATE_STATE",
    "F_UI_MOD",
    "UNKNOWN",
    "F_RVR_STATS",
    "F_CLIENT_DATA",
	"F_INTERACT_RESPONSE",
    "F_QUEST_LIST",
    "F_QUEST_UPDATE",
    "F_REQUEST_QUEST",
    "F_QUEST_LIST_UPDATE",
    "F_CAREER_CATEGORY",
    "F_PLAYER_INIT_COMPLETE",
    "UNKNOWN",
    "F_CAREER_PACKAGE_UPDATE",
    "F_BUY_CAREER_PACKAGE",
    "F_CAREER_PACKAGE_INFO",
	"F_PLAYER_RANK_UPDATE",
    "F_DO_ABILITY_AT_POS",
    "F_CHANNEL_LIST",
    "F_TACTICS",
    "F_TOK_ENTRY_UPDATE",
	"F_TRADE_SKILL_UPDATE",
	"F_RENDER_PRIMITIVE",    
    "F_INFLUENCE_UPDATE",
    "F_INFLUENCE_INFO",
    "F_KNOCKBACK",
    "F_PLAY_VOICE_OVER",
    "UNKNOWN"
};

list<string> m_packets;

void ConvertFile(ifstream & fichier)
{
	cout << "Convertion ... " << endl;

    std::string ligne; // variable contenant chaque ligne lue

	stringstream packet;

	FILE * fp = fopen("out.txt","ab+");

    while ( std::getline( fichier, ligne ) )
    {
		if(!ligne.size()) continue; // espace
		if(ligne[0] == 'C') continue; // Clef Rc4

		if(ligne[0] == '|' && ligne[1] == '-') continue;
		if(ligne[0] == '-') // fin du packet
		{
			packet << "\n";
			fprintf(fp,packet.str().c_str());
			packet.str().erase(0,packet.str().size());
			packet.str("");
		}
		
		if( ligne[0] == '{' )
		{
			int i = ligne.find("(");
			int j = ligne.find(") ");
			int k = ligne.find("PacketSize");
			int l = ligne.find_last_of(' ');

			string opcode = ligne.substr(i+5,2);
			string opcodeName = ligne.substr(j+2,k-j-3);
			string size = ligne.substr(l+1,ligne.size()-l);
			cout << opcode << endl;
			cout << opcodeName << endl;
			cout << size << endl;

			packet << "[<"<<opcodeName<<">(0x"<<opcode<<")<"<<size<<">]";
		}

		if(ligne[0] == '|')
		{
			int a =  ligne.find("|0123456789ABCDEF|");
			if( a != string::npos) continue;

			ligne.erase(0,1);

			for(int i=0;i<ligne.size();i++)
			{
				if( ligne[i] == '|' ) break;

				if( ligne[i] == ' ' && ligne[i+1] == ' ') break;

				packet << ligne[i];
			}
		}
    }

	fclose(fp);
	cout << "Fin Convertion" << endl;
}
int main()
{
	cout << "Chargement des Fichiers a convertir" << endl;

	int count = 0;
	while(1)
	{
		stringstream filename;
		filename<<"sniffconvert"<<count<<".txt";
		cout << "Chargement de " << filename.str().c_str() << std::endl;
		ifstream fichier(filename.str().c_str());
		if(!fichier) break;
		ConvertFile(fichier);
		count++;
	}

	cout << "Fin du programme..."<<endl;

	cin >> count;
};