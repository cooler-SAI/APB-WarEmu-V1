#ifndef CELLMGR_H
#define CELLMGR_H

class Player;
class Object;
class Creature;
struct CreatureSpawn;
struct GameObjectSpawn;

typedef std::vector<const CreatureSpawn*> CreatureSpawnList;
typedef std::vector<const GameObjectSpawn*> GOSpawnList;

typedef struct
{
	CreatureSpawnList CreatureSpawns;
	GOSpawnList GOSpawns;
}CellSpawns;


class CellMgr
{
	friend class ZoneMgr;
	friend class MapMgr;

	typedef unordered_set<Object* > ObjectSet;
public:
	CellMgr(uint16 x,uint16 y,ZoneMgr * Zone);

	//Object Managing
	void AddObject(Object* obj); 
	void RemoveObject(Object* obj);
	bool HasObject(Object* obj) { return (_objects.find(obj) != _objects.end()); }
	bool HasPlayers() { return ((_playerCount > 0) ? true : false); }
	__forceinline size_t GetObjectCount() { return _objects.size(); }
	void RemoveObjects();
	__forceinline ObjectSet::iterator Begin() { return _objects.begin(); }
	__forceinline ObjectSet::iterator End() { return _objects.end(); }

	//State Related
	void SetActivity(bool state);

	__forceinline bool IsActive() { return _active; }
	__forceinline bool IsLoaded() { return _loaded; }

	//Object Loading Managing
	void LoadObjects(CellSpawns * sp);
	__forceinline uint32 GetPlayerCount() { return _playerCount; }

	__forceinline bool IsUnloadPending() { return _unloadpending; }
	__forceinline void SetUnloadPending(bool up) { _unloadpending = up; }
	void QueueUnloadPending();
	void CancelPendingUnload();
	void Unload();

	void SetPermanentActivity(bool val) { _forcedActive = val; }
	bool IsForcedActive() { return _forcedActive; }

	uint16 GetPositionX() { return m_x; }
	uint16 GetPositionY() { return m_y; }

	ObjectSet _respawnObjects;
public:
	ZoneMgr * m_zone;
	uint16 m_x,m_y;
	ObjectSet _objects;
	bool _active, _loaded,_forcedActive;
	bool _unloadpending;

	uint16 _playerCount;
};



#endif