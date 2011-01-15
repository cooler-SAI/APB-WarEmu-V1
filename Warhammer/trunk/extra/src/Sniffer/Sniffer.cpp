// Sniffer.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <winsock2.h>
#include <Winbase.h>
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

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

using namespace std;

#define RC4_KEY_OFFSET          0xC096F2 // offset version 1.3.0 warhammer
#define RC4_KEY_LENGTH          256 // Taille en bytes de la clé
#define OPCODE_HEADER_LENGTH    10 // 3 bytes // Nombre aprés la taille du packet pour trouvé l'opcode
#define OPCODE_HEADER_OFFSET    0x00C93E81 // Adresse de la taille du packet
#define MAX_LENGTH_PACKET       65535

static int rc4adresse = 0;
static int sizeadresse = 0;
static int opcodeadresse = 0;
static int packetadresse = 0;
static int maxdiff = 0;
static int typedesc = 0;

static bool bloqued = false;

void dumpByteArray(char* array, int size)
{
	FILE* fp;
	fopen_s(&fp,"sniff.txt", "ab+");
	fprintf(fp, "Cle RC4 : ");
	for(int i=0; i<size;i++)
		fprintf(fp,"%02X ", (unsigned char)array[i]);
	fprintf(fp, "\n");
	fclose(fp);
	ofstream fichier("sniff.txt", ios::in | ios::out | ios::ate);
	fichier << std::endl;
	fichier.close();
}

void dumpPacket(FILE* fp,int opcode,unsigned char* array,int size)
{
	// structure :
	// [<F_CREATE_MONSTER(0XFF)><74>] 04865D56F457DE4554548787
	if(!fp) return;
	const char * opName = opcodeS[opcode];

	fprintf(fp, "[<%s>(0x%02X)<%i>]",opName,opcode,size);
	for(int i=0; i<size;i++)
		fprintf(fp,"%02X ", (unsigned char)array[i]);
	fprintf(fp,"\r\n");
}
bool readSessionKey(char* sessionKey)
{
	HANDLE hToken;
	OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken );
       
	TOKEN_PRIVILEGES tp = { 0 }; 
	LUID luid; 
	DWORD cb = sizeof(TOKEN_PRIVILEGES); 
	if( !LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &luid ) )
	{
		printf("Les privileges n'ont pu etre donne, si vous etes sous windows vista ou seven verifier d'avori lancer l'application en mode administrateur.");
		while(1){}
		return false;
	}
  
	tp.PrivilegeCount = 1; 
	tp.Privileges[0].Luid = luid; 
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
   
	AdjustTokenPrivileges( hToken, FALSE, &tp, cb, NULL, NULL );
	CloseHandle(hToken);

	unsigned long WindowsPID;
    
    HWND WindowsHandle = FindWindow(0,L"Warhammer: Age of Reckoning, Copyright 2001-2010 Electronic Arts, Inc.");
    if (!WindowsHandle)
    {
		printf("L'application du nom : 'Warhammer: Age of Reckoning, Copyright 2001-2010 Electronic Arts, Inc.' n'a pu etre trouver, verifier d'avoir lance le jeu.");
		while(1){}
        return false;
    }

	GetWindowThreadProcessId(WindowsHandle, &WindowsPID);
    HANDLE warh_process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, WindowsPID);
    
    DWORD number_of_read_bytes=0;
    number_of_read_bytes = 0;
    ReadProcessMemory(warh_process_handle, (LPCVOID)(rc4adresse), sessionKey, 256, &number_of_read_bytes);
    CloseHandle(warh_process_handle);
    if(number_of_read_bytes != RC4_KEY_LENGTH)
    {
        printf("Lecture de la cle RC4 impossible,  %u bytes lu aux lieux de %u\n", number_of_read_bytes, RC4_KEY_LENGTH);
        while(1){}
		return false;
    }
    char a = sessionKey[0];
	if(!bloqued)
	{
    printf("Cle RC4 : ");
    dumpByteArray(sessionKey, RC4_KEY_LENGTH);
    printf("\n");
	}
    return true;
}

bool CheckSeparator(unsigned char* data,int opcode)
{

	if(opcode==0x72)
	{
		int separator = data[2] + data[16];
		cout << "Separator = " << separator << endl;
		if( separator )
			return false;
	}
	else if(opcode==0xE9)
	{
		if( data[0] )
			return false;
		else if( data[3] )
			return false;
		else if( data[4] )
			return false;
	}
	return true;
}

int main()
{
	bool m_okop[0xFF+1];
	bool m_ignore[0xFF+1];
	bool ignoring=false;
	bool filter=false;
	memset(m_okop,0,sizeof(m_okop));

    printf("-=-=-=-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	printf("Warhammer: Age of Reckoning, Copyright 2001-2009 Electronic Arts, Inc.\n");
	printf("Memory Sniffer by Hotboss\n");
	printf("Hotboss@hotmail.com\n");
	printf("-=-=-=-=-=-=-=-=-==-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	printf("Chargment du fichier de configuration ...\n");
	int count=0;

	if(!bloqued)
	{
	printf("Ouverture de packet.txt ...\n");
	ifstream fichier("packet.txt");
	if(fichier)
	{
		printf("-> Reussie ...\n");
		std::string ligne;
		while ( std::getline( fichier, ligne ) )
		{
			if(!ligne.size())
				continue;

			if(ligne[0] == '/')
				continue;

			for(int i=0;i<0xFF+1;i++)
			{
				if(ligne.compare( opcodeS[i] ) == 0 )
				{
					m_okop[i] = true;
					filter=true;
					printf("Ajout de l'opcode [%s] (0x%04X) \n",ligne.c_str(),i);
					break;
				}
			}
		}
	}
	else printf("-> Echouer ...\n");

	printf("Ouverture de ignorepacket.txt ...\n");
	ifstream ignore("ignorepacket.txt");
	if(ignore)
	{
		printf("-> Reussie ...\n");
		std::string ligne;
		while ( std::getline( ignore, ligne ) )
		{
			if(!ligne.size())
				continue;

			if(ligne[0] == '/')
				continue;

			for(int i=0;i<0xFF+1;i++)
			{
				if(ligne.compare( opcodeS[i] ) == 0 )
				{
					m_ignore[i] = true;
					ignoring=true;
					printf("Ingorement de l'opcode [%s] (0x%04X) \n",ligne.c_str(),i);
					break;
				}
			}
		}
	}
	else printf("-> Echouer ...\n");
	}
	else
	{
		m_okop[0x72] = true;
		m_okop[0x71] = true;
		filter=true;
	}
	printf("Ouverture de adresse.txt ...\n");
	ifstream adresse("adresse.txt");
	if(adresse)
	{
		printf("-> Reussie ...\n");
		std::string ligne;
		while ( std::getline( adresse, ligne ) )
		{
			if(!ligne.size())
				continue;

			if(ligne[0] == '/')
				continue;

			if(!rc4adresse)
			{
				printf("-> Enregistrement de l'adresse rc4 ... ");
				ligne.erase(0,2);
				rc4adresse = strtoul(ligne.c_str(),0,16);
				printf("--> Adresse : 0x%X\n",rc4adresse);
				continue;
			}
			if(!sizeadresse)
			{
				printf("-> Enregistrement de l'adresse size ... ");
				sizeadresse = strtoul(ligne.c_str(),0,16);
				printf("--> Adresse : 0x%X\n",sizeadresse);
				continue;
			}
			if(!opcodeadresse)
			{
				printf("-> Enregistrement de l'adresse opcode ... ");
				opcodeadresse=atoi(ligne.c_str());
				printf("--> Adresse : %i\n",opcodeadresse);
				continue;
			}
			if(!packetadresse)
			{
				printf("-> Enregistrement de l'adresse packet ... ");
				packetadresse = strtoul(ligne.c_str(),0,16);
				printf("--> Adresse : 0x%X\n",packetadresse);
				continue;
			}
			if(!maxdiff)
			{
				printf("-> Enregistrement du nombre max de diff ... ");
				maxdiff = atoi(ligne.c_str());
				printf("--> Adresse : %u\n",maxdiff);
				continue;
			}

			if(!typedesc)
			{
				printf("-> Enregistrement du type d'écriture... ");
				typedesc = atoi(ligne.c_str());
				printf("--> Type : %u\n",typedesc);
				continue;
			}
		}
	}
	else printf("-> Echouer ...\n");
	printf("Fin du Chargement ...\n");

    char sessionKey[RC4_KEY_LENGTH] = {'\0'};
    if(!readSessionKey(sessionKey))
	{
		printf("La cle RC4 n'a pu etre lu pour divers raisons.");
		while(1){}
        return 1;
	}
	int oldsize=0;

	unsigned long WindowsPID;

	HWND WindowsHandle = FindWindow(0,L"Warhammer: Age of Reckoning, Copyright 2001-2010 Electronic Arts, Inc.");
	GetWindowThreadProcessId(WindowsHandle, &WindowsPID);
	printf( "\n  Process ID        = %d \n", WindowsPID );

    HANDLE warh_process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, WindowsPID);

	FILE* fp;
	fopen_s(&fp,"sniff.txt", "ab+");
	int lastopcode=0;
	int lastsize=0;
	int lastvaleur=0;
	int lastchecksum=0;

	unsigned char lastdata[MAX_LENGTH_PACKET] = {'\0'};
	
	std::map<int,bool> m_oids;

	LPDEBUG_EVENT evt;

	int nombre=0;
	int CheckAdresse = sizeadresse-8;

	while(1)
	{
		// opcode server
		char opcode[OPCODE_HEADER_LENGTH] = {'\0'};
		char Checksum[2] = {'\0'};

		DWORD number_of_read_bytes3=0;
		DWORD number_of_read_bytes2=0;

		ResumeThread(warh_process_handle);
		ReadProcessMemory(warh_process_handle, (LPCVOID)(sizeadresse), opcode, opcodeadresse, &number_of_read_bytes2);
		SuspendThread(warh_process_handle);

		ReadProcessMemory(warh_process_handle, (LPCVOID)(CheckAdresse), Checksum, int(2), &number_of_read_bytes3);
		int checksum = (unsigned char)Checksum[0]*256 + (unsigned char)Checksum[1];

		int pktlen = (unsigned char)opcode[0] * 256 + (unsigned char)opcode[1];
		int opcodeId = (unsigned char)opcode[9];

		if(filter && !m_okop[opcodeId])
			continue;

		if(ignoring && m_ignore[opcodeId])
			continue;

		unsigned short lenght = pktlen;
		unsigned char data[MAX_LENGTH_PACKET] = {'\0'};
		// Check data
		DWORD number_of_read_bytes=0;
		ReadProcessMemory(warh_process_handle, (LPCVOID)(packetadresse), data, pktlen, &number_of_read_bytes); // Adresse ou commence packet

		if(lastopcode==opcodeId && lastsize==pktlen && lastchecksum == checksum)
			continue;

		lastchecksum = checksum;
		lastsize=pktlen;
		lastopcode=opcodeId;

		/*if(lastsize==pktlen && lastopcode==opcodeId)
		{
			int diff=0;

			for(int i=0;i<pktlen;i++)
			{
			 if(lastdata[i] != data[i])
				 diff++;
			}

			if(diff < maxdiff ) // Moins de 10 différence c'est le meme packet
				continue;
		}*/

		/*if(!CheckSeparator(data,opcodeId))
		{
			lastsize=0;
			lastopcode=0;
			continue;
		}*/


		if(bloqued && (opcodeId==0x72 || opcodeId==0x71) ) // check oid et separator
		{
			int oid = data[0]*256+data[1];

			if( m_oids[oid] == true)
				continue;
			else 
				m_oids[oid] = true;
		}

		/*for(int i=0;i<pktlen;i++)
		{
			lastdata[i] = data[i];
		}*/

		printf("-(%i)[%s]||\n",nombre,opcodeS[opcodeId]);

		nombre++;
		if(typedesc == 1 || bloqued) // non lisible
		{
			dumpPacket(fp,opcodeId,data,pktlen);
			continue;
		}

        fprintf(fp, "{SERVER} Packet: (0x%04X) %s PacketSize = %u\r\n", opcodeId,opcodeS[opcodeId] , pktlen);
        fprintf(fp, "|------------------------------------------------|----------------|\r\n");
        fprintf(fp, "|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|\r\n");
        fprintf(fp, "|------------------------------------------------|----------------|\r\n");
		unsigned int countpos = 0;
		unsigned int line = 1;

		if(pktlen > 0)
		{
            fprintf(fp, "|");
            for (unsigned int count = 0 ; count < lenght ; count++)
			{
                if (countpos == 16)
                {
                    countpos = 0;

                    fprintf(fp,"|");

                    for (unsigned int a = count-16; a < count;a++)
                    {
                        if ((data[a] < 32) || (data[a] > 126))
                            fprintf(fp,".");
                        else
						{
                            fprintf(fp,"%c",data[a]);
							printf("%c",data[a]);
						}
                    }

                    fprintf(fp,"|\r\n");

                    line++;
                    fprintf(fp,"|");
                }

                fprintf(fp,"%02X ",data[count]);

                if (count+1 == lenght && lenght <= 16)
                {
                    for (unsigned int b = countpos+1; b < 16;b++)
                        fprintf(fp,"   ");

                    fprintf(fp,"|");

                    for (unsigned int a = 0; a < lenght;a++)
                    {
                        if ((data[a] < 32) || (data[a] > 126))
                            fprintf(fp,".");
                        else
						{
                            fprintf(fp,"%c",data[a]);
							printf("%c",data[a]);
						}
                    }

                    for (unsigned int c = count; c < 15;c++)
                    fprintf(fp," ");

                    fprintf(fp,"|\r\n");
                }

                if (count+1 == lenght && lenght > 16)
                {
                    for (unsigned int b = countpos+1; b < 16;b++)
                        fprintf(fp,"   ");

                    fprintf(fp,"|");

                    unsigned short print = 0;

                    for (unsigned int a = line * 16 - 16; a < lenght;a++)
                    {
                        if ((data[a] < 32) || (data[a] > 126))
                            fprintf(fp,".");
                        else
 						{
                            fprintf(fp,"%c",data[a]);
							printf("%c",data[a]);
						}

                        print++;
                    }

                    for (unsigned int c = print; c < 16;c++)
                    fprintf(fp," ");

					printf("|\r\n");
                    fprintf(fp,"|\r\n");
                }

                countpos++;
            }
		}

        fprintf(fp, "-------------------------------------------------------------------\r\n\r\n");
	}
	CloseHandle(warh_process_handle);
	fclose(fp);
    return 0;
}