#ifndef ZONEMGR_H
#define ZONEMGR_H

class Object;
class Unit;
class Player;
class Creature;
class GameObject;
class ClientCom;
class PQuest;

struct RegionOffset;
struct ZoneInfo;

#define MAX_OFFSET 16 // Nombre d'offset sur un axe
#define TAILLE_OFFSET 4096 // Taille d'un offset
#define TAILLE_ZONE TAILLE_OFFSET*MAX_OFFSET
#define ZONE_UPDATE_PERIOD 100 // Temps d'update du zonemgr
#define ZONE_INACTIVE_MOVE_TIME 10 // Minute avant qu'une celle ne soit désactivée
#define MAX_CELLS_POSITION 999 // Nombre max de numéro de cell

typedef unordered_set<Object*>	ObjectSet;
typedef unordered_set<Player*>	PlayerSet;
typedef unordered_set<Creature*> CreatureSet;
typedef unordered_set<GameObject*> GameObjectSet;

class ZoneMgr : public EventableObject,public ThreadContext
{
	friend class CellMgr;
	friend class MapMgr;
public:
	ZoneMgr(uint16 zone_id);
	~ZoneMgr();

	void PushObject(Object * Obj);
	void _UpdateInRange();
	void _UpdateObjects();
	EventableObjectHolder eventHolder;

	Mutex m_objectinsertlock;
	ObjectSet m_objectinsertpool;
	void AddObject(Object * obj,bool newguid=false);
	void RemoveObject(Object * obj, bool free_guid);

	void ChangeObjectLocation(Object * obj); // update inrange lists
	bool _CellActive(uint16 x, uint16 y);
	void UpdateInRangeSet(Object * obj, Player * plObj, CellMgr * cell);
	void SendMessageToCellPlayers(Buffer *b,uint32 offx,uint32 offy,ClientCom * Com=NULL,bool self=true,uint32 cell_radius = 2);
	uint32 GetZoneId() { return m_zoneid; }

	//! Mark object as updated
	void ObjectUpdated(Object * obj);
	void UpdateCellActivity(uint16 x, uint16 y, int radius);

	bool run();
	bool Do();
	void _PerformObjectDuties();
	uint32 mLoopCounter;
	uint32 lastGameobjectUpdate;
	uint32 lastUnitUpdate;

	// kill the worker thread only
	void KillThread()
	{
		thread_kill_only = true;
		Terminate();
		while(thread_running)
		{
			Sleep(100);
		}
	}

	CellMgr * Create(uint16 x,uint16 y);
	CellMgr * GetCell(uint16 x,uint16 y);

	//////////////////////////////////////////////////////////
	// Local (mapmgr) storage of players for faster lookup
	//////////////////////////////////////////////////////////
	uint16	GenerateNewOid() { return ++m_lastoid; };
	Object *GetObjectByOid(uint16 oid);
	Player *GetPlayer(uint16 oid);
	Unit * GetUnit(uint16 oid);
	Creature * GetCreature(uint16 oid);


//////////////////////////////////////////////////////////
// Local (mapmgr) storage of players for faster lookup
////////////////////////////////

	// double typedef lolz// a compile breaker..
	typedef HM_NAMESPACE::hash_map<const uint32, Player*> PlayerStorageMap;

	PlayerStorageMap::iterator __player_iterator;
	PlayerStorageMap m_PlayerStorage;
	__inline Player * GetPlayer(const uint32 guid)
	{
		PlayerStorageMap::iterator itr = m_PlayerStorage.find(guid);
		if (itr != m_PlayerStorage.end())
		{
			return itr->second;
		}
		return NULL;
	}

	PlayerStorageMap::iterator GetPlayersBegin() { return m_PlayerStorage.begin(); }
	PlayerStorageMap::iterator GetPlayersEnd() { return m_PlayerStorage.end(); }

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of Creature for faster lookup
////////////////////////////////
	CreatureSet::iterator __creature_iterator;
	HM_NAMESPACE::unordered_map<const uint32,Creature*> m_CreatureStorage;
	Creature * CreateCreature(const CreatureSpawn * Sp);
	Creature * SpawnCreature(uint32 entry,uint32 wx,uint32 wy,uint32 wz,uint32 wo,bool addtoworld);

	__inline Creature * GetCreature(const uint32 guid)
	{
		return m_CreatureStorage[guid] ? m_CreatureStorage[guid] : NULL;
	}
//////////////////////////////////////////////////////////
// Local (mapmgr) storage of GameObject for faster lookup
////////////////////////////////
	GameObjectSet::iterator __gameobject_iterator;
	HM_NAMESPACE::unordered_map<const uint32,GameObject*> m_GameObjectStorage;
	GameObject * CreateGameObject(const GameObjectSpawn * Gs);
	GameObject * SpawnGameObject(uint32 entry,uint32 wx,uint32 wy,uint32 wz,uint32 wo,bool addtoworld);

	__inline GameObject * GetGameObject(const uint32 guid)
	{
		return m_GameObjectStorage[guid] ? m_GameObjectStorage[guid] : NULL;
	}

	GameObjectSet activeGameObjects;
	CreatureSet activeCreatures;
	PlayerSet activePlayers;

	const RegionOffset * GetBaseRegion();
	ZoneInfo * GetZoneInfo() { return m_Info; };

	int GetHeight(uint32 x,uint32 y);
	int GetHeight(Object * obj);

	//////////////////////////////////////////////////////////
	// PUBLIC QUEST
	/////////////////////////////////////////////////////////
	list<PQuest*>::iterator __quest_iterator;
	list<PQuest*> m_pquests;
	void CheckInRangePq(Object *obj); // On vérifi si l'unit est dans une quète public
	void CheckInRangeArea(Player *Plr); // On vérifi si le player entre dans une area
	void CheckInRangeInfluence(Player *Plr); // On vérifi si le joueur entre dans un chapitre

private:
	bool forced_expire;
	bool thread_kill_only;
	bool thread_running;
#ifdef WIN32
	DWORD threadid;
#endif

	//////////////////
	uint16 m_zoneid;

	//map<uint16,map<uint16,CellMgr*>> m_cells;
	CellMgr * m_cells[MAX_CELLS_POSITION][MAX_CELLS_POSITION];

	time_t InactiveMoveTime;

	Mutex M_object;
	map<uint16,Object*> m_object;

	uint16 m_lastoid;

	ZoneInfo * m_Info;
	ZoneHeight* m_height;

	vector<InfluenceInfo*> m_influencelist;
};

#endif