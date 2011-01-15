#ifndef OBJMGR_H
#define OBJMGR_H

extern SQLStorage sXpTable;
extern SQLStorage sTokTable;
extern SQLStorage sItemsTable;
extern SQLStorage sRenownTable;

extern SQLStorage sCreatureProto;
extern SQLStorage sCreatureSpawn;
extern SQLStorage sGameObjectProto;
extern SQLStorage sGameObjectSpawn;
extern SQLStorage sRegionOffset;

struct VendorItemData;
struct CreatureEquipement;
struct AbilityInfo;
class AuthLogon;

struct TokInfo
{
	uint32 entry;
	char * description;
	uint32 exp;
	uint32 tomesection;
	uint32 index;
	uint32 flag1;
	char * event_descs;
};

struct ItemInfluence
{
	const ItemProto * Proto;
	uint32 influence;
	uint32 tier;
};

struct InfluenceInfo
{
	uint32 chapterid; // Numéro du chapitre
	uint16 zoneid; // Numéro de la zone du chapitre
	string name; // Nom du chapitre
	const CreatureProto * npc; // Npc qui donne les récompense
	uint32 influenceid; // Numéro de l'influence
	string race;
	uint32 number;
	uint32 pinx;
	uint32 piny;
	uint32 tok_entry;
	uint32 tok_explore;
	uint8 realm;

	// Career , List des items pour la carrière
	vector<ItemInfluence*> m_rewards[3];

	uint32 maxinflu;
};

struct Account
{
	uint32 AccountId;
	uint8 Permission;
	uint8 locked;
	
	string username;
	string KeyToken;
};

enum LootType
{
	LOOT_DROPS=0,
	LOOT_SCAREVENGING=1,
	LOOT_BUTCHERING=2,
};

struct ItemLoot
{
	ItemLoot(const ItemProto * proto,float chance,uint32 type) : m_proto(proto), m_chance(chance), m_type(type) {}

	bool Generate();
	const ItemProto * m_proto;
	float m_chance;
	uint32 m_type;
};

struct CreatureLoot
{
	vector<ItemLoot*> m_loots;

	uint32 GetLootCount() const { return m_loots.size(); }
	void AddLoot(const ItemProto * proto,float chance,uint32 type)
    {
        m_loots.push_back( new ItemLoot(proto, chance, type) );
    }
    void Clear()
    {
        for (vector<ItemLoot*>::const_iterator itr = m_loots.begin(); itr != m_loots.end(); ++itr)
            delete (*itr);
        m_loots.clear();
    }

	vector<const ItemProto*> GenerateLoot()
	{
		vector<const ItemProto*> m_list;

		for (vector<ItemLoot*>::const_iterator itr = m_loots.begin(); itr != m_loots.end(); ++itr)
		{
			if((*itr)->Generate())
				m_list.push_back((*itr)->m_proto);

			if(m_list.size() >= 50)
				break;
		}
		return m_list;
	}
};

struct RegionOffset
{
	uint32 Region;
	uint32 OffX;
	uint32 OffY;
};

struct RenownsLevel
{
	uint32 RenownLevel;
	uint32 RenownToLevel;
};

struct Characterinfo_item
{
	uint32 entry;
	uint16 slot;
	uint32 count;
};

struct Characterinfo
{
	uint8 careerline;
	uint8 career;
	string career_name;
	uint8 realm;

	uint32 region;
	int16 zone;
	uint32 pos_x;
	uint32 pos_y;
	uint16 pos_z;
	uint16 heading;

	uint16 rallypo;
	uint32 skills;

	// slot,(entry,count)
	map<uint16,Characterinfo_item*> m_items;

	// type , type_lvl
	map<uint32,float> m_stats;

	list<uint16> m_spells;
};

struct _ItemStat
{
    uint32  ItemStatType;
    int32   ItemStatValue;
};

struct ItemProto
{
	uint32 entry;
	char * name;
	char * desc;
	uint32 type;
	uint32 race ;
	uint32 modelid;
	uint32 slot;
	uint32 rarity;
	uint32 career;
	uint32 skill;
	uint32 icon;
	uint32 bind;
	uint32 armor;
	uint32 spell_id;
	uint32 dps;
	uint32 speed;
	uint32 min_rank;
	uint32 min_renown;
	uint32 start_quest;
	
	_ItemStat ItemStat[MAX_ITEMS_STATS];

	uint32 sellprice;
	uint32 talismanslot;
	uint32 maxstack;

	char * scriptname;
};

struct XpInfo
{
	uint32 level;
	uint32 xp;
	uint32 adv1;
	uint32 adv2;
};

struct PlayerInfo
{
	uint32	character_id;
	uint32	account;
	uint8	slot;
	string	name;
	uint8	model;
	uint8	level;
	uint8	career;
	uint8	careerline;
	uint8	realm;
	uint32	heldleft;
	uint8	race;
	uint32	last_time_played;
	uint8	traits[8];
	uint32	wealth;
	uint32	speed;

	int16	zone;
	uint32	region;
	LocationVector Position;

	uint32 xp;
	uint32 restxp;
	uint32 renown;
	uint8 renownrank;
	uint16 rallyPoint;
	uint32 playedTime;
	uint32 health;
	uint32 maxhealth;
	uint16 actionpoints;
	uint16 maxactionpoints;
	uint8 sex;
	uint32 m_skills;

	// slot,entry;
	map<uint32,uint32> m_items;
	map<uint8,uint16> m_stats;
	vector<ZoneInfo*> m_zonesacces;

	set<uint32> friends;
	set<uint32> ignores;
	set<uint32> hasfriends;

	Player * m_player;
};

struct CostItem
{
	const ItemProto * Proto;
	uint32 price;
};

struct CreatureProto
{
	uint32 entry;
	char * name;
	uint32 displayid[2];
	uint32 scale[2];
	uint32 level[2];
	uint32 faction;
	uint32 ranged;
	uint32 attacktime;
	uint32 respawntime;
	uint32 money;
	char * bytes;
	uint32 icone;
	uint32 Unk1;
	uint32 emote;
	uint32 Unk[6];
	uint32 title;
	char * flags;
	char * scriptname;
};

struct CreatureInterract
{
	uint32 guid;
	uint16 unk1;
	string text;
};

struct CreatureSpawn
{
	uint32 guid;
	uint32 entry;
	uint32 zone;
	uint32 WorldX;
	uint32 WorldY;
	uint32 WorldZ;
	uint32 WorldO;
	char * bytes;
	uint32 icone;
	uint32 Unk1;
	uint32 emote;
	uint32 Unk[6];
	uint32 title;
	char * flags;
};

struct GameObjectProto
{
	uint32 entry;
	char * name;
	uint32 displayid;
	uint32 scale;
	uint32 level;
	uint32 faction;
	uint32 healthpoints;
	uint32 attacktime;
	uint32 damage[2];
	uint32 respawntime;
	uint32 Unk[6];
};

struct GameObjectSpawn
{
	uint32 guid;
	uint32 entry;
	uint32 zone;
	uint32 WorldX;
	uint32 WorldY;
	uint32 WorldZ;
	uint32 WorldO;
	uint32 displayid;
	uint32 Unk[6];
};

struct ZoneInfo
{
	uint16 zoneid;
	string zonename;
	uint8 minlevel;
	uint8 maxlevel;
	uint8 type;
	uint8 tier;

	LocationVector m_tp[3];
	uint16 price;

	uint8 region;

	uint32 minwx;
	uint32 minwy;

	const RegionOffset * Offset;
};

struct LevelInfo
{
	uint16 careerline;
	uint8 level;

	uint32 stats[GameData::STATS_COUNT];
};

struct CareerLevelInfo
{
	// Level,Career,Info
	map<uint8, map<uint16,LevelInfo*> > m_infos;

	void AddLevelInfo(LevelInfo * info)
	{
		m_infos[info->level][info->careerline] = info;
	}
	
	LevelInfo * GetLevelInfo(uint8 level,uint16 careerline)  { return m_infos[level][careerline]; }
};

typedef tr1::unordered_map<uint32, VendorItemData> CacheVendorItemMap;
typedef tr1::unordered_map<uint32, CreatureEquipement> CacheCreatureItems;
typedef tr1::unordered_map<uint32, CreatureLoot> CacheCreatureLoot;
typedef tr1::unordered_map<uint32, InfluenceInfo*> CacheInfluence;

class ObjectMgr : public Singleton< ObjectMgr >
{
public:
	Realm * m_realm;

	ObjectMgr();
	~ObjectMgr();

	void LoadRealmConfig();
	void LoadItemProto();
	void LoadRenowLevel();
	void LoadXpInfo();
	void LoadTokInfo();
	void LoadZoneInfo();
	void LoadZoneWp();
	void LoadRegionOffset();
	void LoadCharacterInfo();
	void LoadCharacterInfoItems(Characterinfo * Info);
	void LoadCharacterInfoStats(Characterinfo * Info);
	void LoadCharacterInfoSpells(Characterinfo * Info);
	void LoadCharacterInfoLevel();
	void LoadMaxGuid();
	void LoadCreatureProto();
	void LoadCreatureSpawn();
	void LoadGameObjectName();
	void LoadGameObjectSpawn();
	void LoadCreatureItems();
	void LoadCellSpawns();
	void LoadCreatureVendors();
	void LoadCreatureLoot();
	void LoadAbilityInfo();
	void LoadInfluence();

	void RemoveAllPlayer();

	bool registered; // true si le serveur est enregistré sur le logon
	bool opened; // true si le port du jeu est ouvert;
	bool loaded; // true si tous le serveur est chargé;

	////////////////////////////////
	// General
	////////////////////////////////
	map<string,list<string>> m_errors;

	void CleanDbErrors()
	{
		Log.Debug("ObjMgr","Error : World=%u | Characters=%u",m_errors["World"].size(),m_errors["Character"].size());

		if(!m_cleandb)
		{
			m_errors.clear();
			return;
		}

		Log.Debug("ObjMgr","Cleaning Errors...",m_errors.size());
		for(map<string,list<string>>::iterator itr=m_errors.begin();itr!=m_errors.end();++itr)
		{
			for(list<string>::iterator itt=itr->second.begin();itt!=itr->second.end();++itt)
			{
				if(itr->first.compare("World")==0)
					WorldDatabase.Execute( (*itt).c_str() );
				else if(itr->first.compare("Character")==0)
					CharacterDatabase.Execute( (*itt).c_str() );
			}
		}

		m_errors.clear();
	}


	////////////////////////////////
	// Accounts
	////////////////////////////////
	void	AddAccount(Account * Acct);
	uint32	GetAccountID(string name);
	uint8	GetAccountPermission(string name);
	Account * GetAccount(string name) 
	{ 
		Account * Acct=NULL;

		M_accountname.Acquire();
		Acct = m_accountname[name]; 
		M_accountname.Release();

		return Acct;
	}

	map<string,ClientCom*> m_accountcheck;

	////////////////////////////////
	// Items
	////////////////////////////////
	const ItemProto * GetItemProto(uint32 entry)
	{
		const ItemProto * Proto=NULL;
		Proto = sItemsTable.LookupEntry<ItemProto>(entry);
		return Proto;
	}

	////////////////////////////////
	// Characters
	////////////////////////////////
	Characterinfo * GetCharacterInfo(uint32 career);
	uint32 GetRenownToLevel(uint8 rank);

	const XpInfo * GetXpInfo(uint32 level)
	{
		const XpInfo * Info = sXpTable.LookupEntry<XpInfo>(level+1);
		return Info;
	}

	list<PlayerInfo*>	GetPlayerInfoList(uint32 accountid); // Récupère la liste de id en db
	GameData::Realm		GetRealmAccount(uint32 accountid);

	PlayerInfo * GetPlayerInfo(uint32 characterid,bool all=false); // Récupère les info en DB
	PlayerInfo * GetPlayerInfoBySlot(uint32 accountid,uint32 slot); //
	PlayerInfo * GetPlayerInfoByName(string name)
	{
		PlayerInfo * Info=NULL;
		QueryResult * Result = CharacterDatabase.Query("SELECT character_id FROM characters WHERE name='%s'",name.c_str());
		if(Result) Info = GetPlayerInfo(Result->Fetch()[0].GetUInt32());
		delete Result;
		return Info;
	}
	void RemoveCharacerBySlot(uint32 account,uint32 slot);

	////////////////////////////////
	// Players
	////////////////////////////////
	Player * CreatePlayer(uint32 character_id,ClientCom * Com);
	Player * GetPlayer(uint32 character_id);
	Player * GetPlayerByName(string name);
	Player * GetPlayerByOid(uint16 oid);
	Player * GetPlayerByAccount(uint32 acctid);

	void PlayerLeave(Player * Plr);
	void DeletePlayer(Player * Plr);
	void DistributePacketToAll(Buffer * b);
	void SendChatMessage(Player * Sender,string text,ChatLogFilters type=CHATLOGFILTERS_SHOUT);
	void SendPlayerOnline(Player * Plr,string name,string guildname,uint8 career,uint16 zoneid,uint8 minlvl,uint8 maxlvl);
	void SaveAllPlayers();

	uint32 GetPlayerOnline()
	{
		uint32 counts=0;
		M_players.Acquire();
		counts=m_players.size();
		M_players.Release();
		return counts;
	}

	////////////////////////////////
	// GameObject
	////////////////////////////////
	const GameObjectProto * GetGameObjectProto(uint32 entry)
	{
		const GameObjectProto * Proto;
		Proto = sGameObjectProto.LookupEntry<GameObjectProto>(entry);
		return Proto;
	}
	const GameObjectSpawn * GetGameObjectSpawn(uint32 guid)
	{
		const GameObjectSpawn * Spawn;
		Spawn = sGameObjectSpawn.LookupEntry<GameObjectSpawn>(guid);
		return Spawn;
	}


	////////////////////////////////
	// Creature
	////////////////////////////////
	const CreatureProto * GetCreatureProto(uint32 entry)
	{
		const CreatureProto * Proto;
		Proto = sCreatureProto.LookupEntry<CreatureProto>(entry);
		return Proto;
	}
	const CreatureSpawn * GetCreatureSpawn(uint32 guid)
	{
		const CreatureSpawn * Spawn;
		Spawn = sCreatureSpawn.LookupEntry<CreatureSpawn>(guid);
		return Spawn;
	}
	const CreatureSpawn * GetCreatureSpawnByProto(const CreatureProto * Proto)
	{
		const CreatureSpawn * Spawn;
		for(uint32 i = 1; i < sCreatureSpawn.MaxEntry; ++i)
		{
			Spawn = sCreatureSpawn.LookupEntry<CreatureSpawn>(i);
			if(Spawn && Spawn->entry == Proto->entry)
				return Spawn;
		}
		return NULL;
	}


	////////////////////////////////
	// Cities
	////////////////////////////////
	void LoadKeep();
	vector<Keep> m_keeps;
    vector<Objective> m_objectives;
    vector<City> m_cities;

	////////////////////////////////
	// Guid
	////////////////////////////////
	uint32 itemguid;
	uint32 characterguid;
	uint32 spawnguid;
	uint32 goguid;
	uint32 mailguid;

	uint32 GenerateItemGuid()		{ return ++itemguid; }
	uint32 GenerateCharacterGuid()	{ return ++characterguid; }
	uint32 GenerateSpawnGuid()		{ return ++spawnguid; }
	uint32 GenerateGoGuid()			{ return ++goguid; }
	uint32 GenerateMailGuid()		{ return ++mailguid; }


	////////////////////////////////
	// Calculation
	////////////////////////////////
	uint32 GenerateHealth(Unit * unit);

	void GenerateLoot(Unit * unit,Player * Plr);
	void CalculXpToGive(Unit * attacker,Unit * victim);
	

	////////////////////////////////
	// Player
	////////////////////////////////
	uint32 GetUptime()
	{
		uint32 time= uint32(UNIXTIME)-m_starttime;
		return time;
	};
	string ConvertTime(uint32 secondes)
	{
		stringstream ss;
		int seconds =  secondes;
		int mins=0;
		int hours=0;
		int days=0;
		if(seconds >= 60)
		{
			mins = seconds / 60;
			if(mins)
			{
				seconds -= mins*60;
				if(mins >= 60)
				{
					hours = mins / 60;
					if(hours)
					{
						mins -= hours*60;
						if(hours >= 24)
						{
							days = hours/24;
							if(days)
								hours -= days*24;
						}
					}
				}
			}

		}

		ss << hours <<" heure(s) "<<mins<<" minute(s) "<<seconds<<" seconde(s)";
		return ss.str();
	}

    VendorItemData const* GetNpcVendorItemList(uint32 entry)
    {
        CacheVendorItemMap::const_iterator  iter = m_mCacheVendorItemMap.find(entry);
        if(iter == m_mCacheVendorItemMap.end())
            return NULL;

        return &iter->second;
    }
	CreatureEquipement const* GetNpcEquipement(uint32 entry)
    {
        CacheCreatureItems::const_iterator  iter = m_mCacheCreatureItem.find(entry);
        if(iter == m_mCacheCreatureItem.end())
            return NULL;

        return &iter->second;
    }

	CreatureLoot * GetCreatureLoot(uint32 entry)
    {
        CacheCreatureLoot::iterator  iter = m_mCacheCreatureLoot.find(entry);
        if(iter == m_mCacheCreatureLoot.end())
            return NULL;

        return &iter->second;
    }

	GameData::InteractType GetInterractType(uint32 title);

	////////////////////////////////
	// Zone
	////////////////////////////////
	ZoneInfo * GetZoneInfo(uint16 zoneid)
	{
		ZoneInfo * Info=NULL;
		M_zoneinfo.Acquire();
		Info=m_zoneinfo[zoneid];
		M_zoneinfo.Release();
		return Info;
	}
	const RegionOffset * GetRegionOffset(uint32 region)
	{
		return sRegionOffset.LookupEntry<RegionOffset>(region);
	}

	uint8 GetRegionByZone(uint16 zone)
	{
		const ZoneInfo * Info = GetZoneInfo(zone);
		if(Info) return Info->region;
		else return 0;
	}

	list<CreatureSpawn*> GetCellSpawn(uint16 x,uint16 y)
	{
		list<CreatureSpawn*> m_list;

		M_spawnmap.Acquire();
		stringstream ss;
		ss << x << "," << y;
		m_list = m_spawnmap[ss.str()];
		M_spawnmap.Release();

		return m_list;
	}

	LevelInfo * GetLevelInfo(uint16 careerline,uint8 level)
	{
		return InfoLevel.GetLevelInfo(level,careerline);
	}

	uint16 GetNewZoneID(uint32 offx,uint32 offy)
	{
		uint32 wpx = offx;
		uint32 wpy = offy;
		uint16 zoneid=0;

		ZoneInfo * Info=NULL;
		M_zoneinfo.Acquire();

		for(map<uint16,ZoneInfo*>::iterator itr=m_zoneinfo.begin();itr!=m_zoneinfo.end();++itr)
		{
			Info = itr->second;

			if(!Info) continue;
			if(!Info->minwx || !Info->minwy) continue;

			if(wpx >= Info->minwx && wpx < Info->minwx+65536)
				if(wpy >= Info->minwy && wpy < Info->minwy+65536)
				{
					zoneid=itr->first;
					break;
				}
		}
		M_zoneinfo.Release();

		return zoneid;
	}

	////////////////////////////////
	// Ability
	////////////////////////////////
	AbilityInfo * GetAbilityInfo(uint32 entry,uint8 level)
	{
		return m_abilityinfo[entry][level];
	}

	////////////////////////////////
	// Influence
	////////////////////////////////
	InfluenceInfo * GetInfluenceInfo(uint32 chapterid)
	{
		return InfoInfluence[chapterid];
	}
	InfluenceInfo * GetInfluenceInfo(uint16 influenceid)
	{
		for(CacheInfluence::iterator itr=InfoInfluence.begin();itr!=InfoInfluence.end();++itr)
		{
			if(!itr->second) continue;

			if(itr->second->influenceid == influenceid)
				return itr->second;
		}

		return NULL;
	}
	vector<InfluenceInfo*> GetInfluenceList(uint16 zoneid)
	{
		vector<InfluenceInfo*> m_list;
		for(CacheInfluence::iterator itr=InfoInfluence.begin();itr!=InfoInfluence.end();++itr)
		{
			if(!itr->second) continue;

			if(itr->second->zoneid == zoneid)
				m_list.push_back(itr->second);
		}

		return m_list;
	}
	const TokInfo * GetTokInfo(uint32 tokid)
	{
		return sTokTable.LookupEntry<TokInfo>(tokid);
	}

	////////////////////////////////
	// RealmInfo
	////////////////////////////////
	string m_Welcome;
	string ClientVersion;
	int m_maxplayer;
	int RezTime;
	bool m_discotimeforgm;
	bool m_cleandb;
	int m_savetime;
	int loot_rate;
	int xp_rate;
	int gold_rate;

	AuthLogon * m_Logon;

private:
	uint32 m_starttime;

	// careerline,info
	FastMutex M_characterinfo;
	map<uint32,Characterinfo*> m_characterinfo;

	// entry,proto
	FastMutex M_itemproto;
	map<uint32,ItemProto*> m_itemproto;

	// level,renowtolvl
	FastMutex M_renownlevel;
	map<uint8,uint32> m_renownlevel;

	// levelactu,xpinfo
	FastMutex M_xpinfo;
	map<uint32,XpInfo*> m_xpinfo;

	// AccountName,AccountId,permission
	FastMutex M_accountname;
	map<string,Account*> m_accountname;

	// Character_id,Info
	FastMutex M_players;
	map<uint32,Player*> m_players;

	//Lorsqu'un cell se charge , il récupère la list de ces spawns;
	// string(x,y), list<spawn>
	FastMutex M_spawnmap;
	map<string, list<CreatureSpawn*> > m_spawnmap;

	// ZoneId,ZoneInfo
	FastMutex M_zoneinfo;
	map<uint16,ZoneInfo*> m_zoneinfo;

	CacheVendorItemMap m_mCacheVendorItemMap;
	CacheCreatureItems m_mCacheCreatureItem;
	CacheCreatureLoot m_mCacheCreatureLoot;
	CareerLevelInfo InfoLevel;
	CacheInfluence InfoInfluence;

	// entry , level du spell , Info
	tr1::unordered_map<uint32 , tr1::unordered_map< uint8 , AbilityInfo*> > m_abilityinfo;
};

#define objmgr ObjectMgr::getSingleton( )

#endif