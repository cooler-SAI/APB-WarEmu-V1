#ifndef OBJECT_H
#define OBJECT_H

class Player;
class Creature;
class GameObject;
class ZoneMgr;
class Unit;

#define M_PI	   3.14159265358979323846
#define M_H_PI		1.57079632679489661923
#define M_Q_PI		0.785398163397448309615

class SERVER_DECL Object : public EventableObject
{
	friend class ZoneMgr;
	friend class CellMgr;

public:

	typedef unordered_set< Object* > InRangeSet;
	typedef unordered_set< Player* > InRangeSetPlayer;

	Object();
	~Object();

	virtual void Update ( uint32 time ) { }

	//////////////////////////////
	// Accessor
	//////////////////////////////
	uint16 GetOid() { return m_oid; };
	string GetName() { return m_name; };
	string GetSubName() { return m_subname; };
	ObjectType GetType() { return m_type; };
	bool IsObject() { return m_type == TYPEID_OBJECT; };
	bool IsPlayer() { return m_type == TYPEID_PLAYER; };
	bool IsCreature() { return m_type == TYPEID_CREATURE; };
	bool IsGameObject() { return m_type == TYPEID_GAMEOBJECT; };
	bool IsUnit()  {  return (IsPlayer() ? true : (IsCreature() ? true : (IsGameObject() ? true : false))); }

	//////////////////////////////
	// Stats Syteme
	//////////////////////////////
	void SetBaseStat(uint8 type,uint16 value)
	{
		M_stats.Acquire();
		m_basestats[type] = value;
		M_stats.Release();
	}

	uint16 GetBaseStat(uint8 type) // On addition les base + les bonus;
	{	
		uint16 stat=0;
		M_stats.Acquire();
		stat= m_basestats[type];
		M_stats.Release();
		return stat;
	}

	uint16 GetStat(uint8 type) // On addition les base + les bonus;
	{		
		uint16 stat = 0;
		M_stats.Acquire();
		if( m_basestats[type] ) stat+=m_basestats[type];
		if( m_stats[type] ) stat+=m_stats[type];
		M_stats.Release();
		return stat;
	}

	void AddStat(uint8 type,uint16 value)
	{
		M_stats.Acquire();
		if( m_stats[type] )	m_stats[type] += value;
		else				m_stats[type] = value;
		M_stats.Release();
	}

	void RemoveStat(uint8 type,uint16 value)
	{
		M_stats.Acquire();
		if( m_stats[type] && m_stats[type] > value )	m_stats[type] -= value;
		else				m_stats[type] = 0;
		M_stats.Release();
	}

	void ClearStat()
	{
		M_stats.Acquire();
		m_stats.clear();
		M_stats.Release();
	}

	/////////////////////////
	// POSITION SYSTEM
	/////////////////////////
	uint16 GetX()					{ return uint16(m_position.x); };
	uint16 GetY()					{ return uint16(m_position.y); };
	uint16 GetZ()					{ return uint16(m_position.z); };
	uint16 GetO()					{ return uint16(m_position.o); };
	uint32 GetWorldX()				{ return uint32(m_worldposition.x); };
	uint32 GetWorldY()				{ return uint32(m_worldposition.y); };
	uint16 GetWorldZ()				{ return uint16(m_worldposition.z); };
	uint16 GetWorldO()				{ return uint16(m_worldposition.o); };
	uint16 CalcPinX();
	uint16 CalcPinY();
	uint32 CalculWorldX(); 
	uint32 CalculWorldY();
	void CalculOffset();
	void CalculPos();

	uint32 GetZoneX() { return m_zone_x; };
	uint32 GetZoneY() { return m_zone_y; };
	uint16 GetOffX() { return m_OffSetX; };
	uint16 GetOffY() { return m_OffSetY; };
	float GetDistance(Object * obj);
	float GetFacing(Object * target);
	float GetFacing(uint16 x,uint16 y);


	uint8 GetRegion() { return m_region; };
	uint16 GetZoneId() { return m_zone; };
	ZoneMgr * GetZoneMgr() { return m_zoneMgr; };
	CellMgr * GetCellMgr() { return m_cellMgr; };

	void SetRegion(uint8 region) { m_region=region; };
	void SetZoneId(uint16 zone) { m_zone=zone; };
	void SetCellMgr(CellMgr * mgr) { m_cellMgr = mgr; };
	void SetZoneMgr(ZoneMgr * mgr) { m_zoneMgr = mgr; };

	uint32 GetDistance2D(Object * m_object);
	float GetDistance(float x,float y);
	bool isInFront(Unit * target);
	bool isInBack(Unit * target);
	float calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y );
	float calcRadAngle( float Position1X, float Position1Y, float Position2X, float Position2Y );
	float getEasyAngle( float angle );

	void SetPosition(float x,float y,float z,float o,uint32 offx=0,uint32 offy=0,bool forceupdate=false);

	// Convert Player position on Wold position
	void ConvertWorldPos();

	__forceinline float GetDistanceSq(LocationVector & comp)
	{
		return comp.DistanceSq(m_position);
	}

	__forceinline float CalcDistance(LocationVector & comp)
	{
		return comp.Distance(m_position);
	}

	__forceinline const float GetDistanceSq(float x, float y, float z)
	{
		return m_position.DistanceSq(x, y, z);
	}

	__forceinline const float GetDistance2dSq( Object * obj )
	{
		if( obj->GetZoneId() != m_zone )
			return 40000.0f; //enough for out of range
		return m_position.Distance2DSq(float(obj->GetWorldX()),float(obj->GetWorldY()));
	}

	__forceinline float GetDistance2dSq(float x, float y)
	{
		return m_worldposition.Distance2DSq( x, y );
	}

	void AddToWorld(ZoneMgr * Mgr,bool newguid=false);
	void AddToWorld(bool newguid=false);
	virtual void RemoveFromWorld();
	void PushToWorld(ZoneMgr * Zone);
	bool IsInWorld() { return m_zoneMgr != NULL; }
	void SetNewOid(uint16 oid) { m_oid=oid; };

	//////////////////////////////
	// Function
	//////////////////////////////

	void sendRemoved(Player * Plr=NULL);
	void dispachPacketInRange(Buffer *b,bool self=false);
	virtual void SendEquiped(Player * Plr) = 0;
	virtual void SendMeTo(Player * Plr) = 0;
	virtual void SendInterractTo(Player * Plr,Struct_f_interract &interract) = 0;

	//Convertion
	Player * GetPlayer();
	Unit * GetUnit();
	Creature * GetCreature();
	GameObject * GetGameObject();

	bool Active;
	bool CanActivate() { return true; };
	void Activate(ZoneMgr * mgr);
	void Deactivate(ZoneMgr * mgr);

	/////////////////////////////////////
	//	Ranged Function
	/////////////////////////////////////
	__forceinline bool IsInRangeSet( Object* pObj )
	{
		return !( m_objectsInRange.find( pObj ) == m_objectsInRange.end() );
	}
	
	virtual void AddInRangeObject(Object* pObj)
	{
		if( pObj == NULL ) return;

		m_objectsInRange.insert( pObj );

		if( pObj->IsPlayer() )
			m_inRangePlayers.insert( pObj->GetPlayer() );
	}

	__forceinline void RemoveInRangeObject( Object* pObj )
	{
		if( pObj == NULL ) return;

		OnRemoveInRangeObject( pObj );
		m_objectsInRange.erase( pObj );
	}

	__forceinline bool HasInRangeObjects()
	{
		return ( m_objectsInRange.size() > 0 );
	}

	__forceinline bool HasInRangePlayers()
	{
		return ( m_inRangePlayers.size() > 0 );
	}

	virtual void OnRemoveInRangeObject( Object* pObj )
	{
		if( pObj->IsPlayer() )
		{
			m_inRangePlayers.erase( pObj->GetPlayer() );
			//if( IsPlayer() )
				//pObj->sendRemoved(GetPlayer());
		}
	}

	virtual void ClearInRangeSet()
	{
		if(m_objectsInRange.size() > 0)
		m_objectsInRange.clear();

		if(m_inRangePlayers.size() > 0)
		m_inRangePlayers.clear();

		if(m_oppFactsInRange.size() > 0)
		m_oppFactsInRange.clear();
	}

	__forceinline size_t GetInRangeCount() { return m_objectsInRange.size(); }
	__forceinline size_t GetInRangePlayersCount() { return m_inRangePlayers.size();}
	__forceinline InRangeSet::iterator GetInRangeSetBegin() { return m_objectsInRange.begin(); }
	__forceinline InRangeSet::iterator GetInRangeSetEnd() { return m_objectsInRange.end(); }
	__forceinline InRangeSet::iterator FindInRangeSet(Object* obj) { return m_objectsInRange.find(obj); }

	void RemoveInRangeObject(InRangeSet::iterator itr)
	{ 
		OnRemoveInRangeObject(*itr);
		m_objectsInRange.erase(itr);
	}

	__forceinline bool RemoveIfInRange( Object* obj )
	{
		InRangeSet::iterator itr = m_objectsInRange.find(obj);
		if( obj->IsPlayer() )
			m_inRangePlayers.erase( obj->GetPlayer() );

		if( itr == m_objectsInRange.end() )
			return false;
		
		m_objectsInRange.erase( itr );
		return true;
	}

	__forceinline void AddInRangePlayer( Object* obj )
	{
		m_inRangePlayers.insert( obj->GetPlayer() );
	}

	__forceinline void RemoveInRangePlayer( Object* obj )
	{
		m_inRangePlayers.erase( obj->GetPlayer() );
	}

	bool IsInRangeOppFactSet(Object* pObj) { return (m_oppFactsInRange.count(pObj) > 0); }

	__forceinline unordered_set<Object* >::iterator GetInRangeOppFactsSetBegin() { return m_oppFactsInRange.begin(); }
	__forceinline unordered_set<Object* >::iterator GetInRangeOppFactsSetEnd() { return m_oppFactsInRange.end(); }
	__forceinline unordered_set<Player*  >::iterator GetInRangePlayerSetBegin() { return m_inRangePlayers.begin(); }
	__forceinline unordered_set<Player*  >::iterator GetInRangePlayerSetEnd() { return m_inRangePlayers.end(); }
	__forceinline unordered_set<Player*  > * GetInRangePlayerSet() { return &m_inRangePlayers; };

	unordered_set<Object* > m_objectsInRange;
	unordered_set<Player* > m_inRangePlayers;
	unordered_set<Object* > m_oppFactsInRange;
	//////////////////////////////
	//////////////////////////////

	/************************************************************************/
	/* TARGET                                                              */
	/************************************************************************/
	void SetTarget(Unit * Obj) { m_Target = Obj; };

	GameData::TargetTypes GetTargetType() { return m_TargetType; };
	void SetTargetType(GameData::TargetTypes ObjType) { m_TargetType = ObjType; };

	Unit * m_Target;
	GameData::TargetTypes m_TargetType;

protected:

	uint16 m_oid; // Oid unique a chaque object dans le world;

	ObjectType m_type; // Type de l'objet

	string m_name;
	string m_subname;

	/////////////////////////
	// POSITION SYSTEM
	/////////////////////////
	LocationVector m_lastposition;
	LocationVector m_worldposition;
	LocationVector m_position;
	uint16 m_zone;
	uint32 m_region;
	ZoneMgr * m_zoneMgr;
	CellMgr * m_cellMgr;

	uint16 m_OffSetX;
	uint16 m_OffSetY;
	uint32 m_zone_x;
	uint32 m_zone_y;

	/////////////////////////
	// STATS SYSTEM
	/////////////////////////
	FastMutex M_stats;
	map<uint8,uint16> m_basestats; // Stats de base ( non modifiable )
	map<uint8,uint16> m_stats; // Stats ajouter par items,spell etc
};

#endif