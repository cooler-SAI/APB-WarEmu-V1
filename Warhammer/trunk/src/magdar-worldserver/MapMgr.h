#ifndef MAPMGR_H
#define MAPMGR_H

class Object;
class Player;
class Creature;

#define MAX_ZONE_ID 999
#define MAX_VISIBILITY_RANGE 300

struct AreaInfo
{
	uint32 areaid;
	uint16 zoneid;

	uint32 influenceid[2];
	uint32 minx[2];
	uint32 miny[2];

	uint32 maxx[2];
	uint32 maxy[2];
};

class MapMgr : public EventableObject, public Singleton< MapMgr >
{
	friend class CellMgr;
	friend class ZoneMgr;
public:
	MapMgr();

	ZoneMgr * GetZoneMgr(Object * obj);
	ZoneMgr * GetZoneMgr(uint16 zoneid);
	ZoneMgr * _CreateZone(uint16 zoneid);

	uint16 CalculPosInCombat(uint16 cb_pos,uint16 offset,uint8 region,bool x);
	uint16 CalculPosInCombat(const RegionOffset * Off,uint16 cb_pos,uint16 offset,bool x);
	uint16 CalculCombatPos(Object * obj,bool x);

	// Nouveau Systeme
	uint16 CalculOffset(uint32 wp);
	uint32 CalculPos(uint8 region,uint32 wp,bool x);
	uint16 CalculPin(uint8 region,uint32 wp,bool x);
	bool IsOnFirstOffSet(uint8 region,uint16 offset,bool x);

	uint16 CalculPin(const RegionOffset * Off,uint32 wp,bool x);
	uint32 CalculPos(const RegionOffset * Off,uint32 wp,bool x);
	bool IsOnFirstOffSet(const RegionOffset * Off,uint16 offset,bool x);

	bool IsOnFirstOffSet(uint16 base,uint16 offset);
	uint16 CalculPin(uint16 base,uint32 wp);
	uint32 CalculPos(uint16 base,uint32 wp);


	uint16 GetBaseOffset(const RegionOffset * Off,uint32 wp,bool x);


	__forceinline  CellSpawns *GetSpawnsList(uint16 zone,uint16 x,uint16 y) { return m_spawns[zone][x][y]; };
	__forceinline  void SetSpawnsList(uint16 zone,uint16 x,uint16 y,CellSpawns * Cs) { m_spawns[zone][x][y]=Cs; };

	void Shutdown();

	void LoadAreaInfo();
	vector<AreaInfo*> GetAreaList(uint16 zoneid) 
	{ 
		vector<AreaInfo*> m_list;

		for(map<uint32, AreaInfo*>::iterator itr=m_areas[zoneid].begin();itr!=m_areas[zoneid].end();++itr)
		{
			if(itr->second)
				m_list.push_back(itr->second);
		}
		return m_list; 
	
	}
	AreaInfo * GetAreaInfo(uint32 areaid,uint16 zoneid=0)
	{
		if(zoneid)
			return m_areas[zoneid][areaid];
		else
		{
			for(map<uint16, map<uint32, AreaInfo*> >::iterator itr=m_areas.begin();itr!=m_areas.end();++itr)
			{
				if( itr->second[areaid] )
					return itr->second[areaid];
			}
		}
		return NULL;
	}

	AreaInfo * CheckAreaInfo(uint32 realm,uint16 zoneid,uint32 pinx,uint32 piny)
	{
		if(realm > 1)
			return NULL;

		AreaInfo * Choice = NULL;
		int mindist=65536;
		for(map<uint32, AreaInfo*>::iterator itr=m_areas[zoneid].begin();itr!=m_areas[zoneid].end();++itr)
		{
			AreaInfo * Info = itr->second;
			if(!Info) continue;

			uint32 minx = Info->minx[realm];
			uint32 maxx = Info->maxx[realm];

			uint32 miny = Info->miny[realm];
			uint32 maxy = Info->maxy[realm];

			if(minx > pinx || miny > piny || maxx < pinx || maxy < piny)
				continue;

			int dist = 0;

			if(minx < 32768)
				dist+= pinx-minx;
			else dist+= maxx-pinx;

			if(miny < 32768)
				dist+= piny-miny;
			else dist+= maxy-piny;

			if(dist < mindist)
			{
				mindist = dist;
				Choice = Info;
			}
		}

		return Choice;
	}

private:
	Mutex M_zones;
	ZoneMgr * m_zones[MAX_ZONE_ID];

	// zoneid , offx , offy , cellspawn
	map<uint16, map<uint16, map<uint16,CellSpawns*> > > m_spawns;

	// zoneid , areaid, areainfo
	map<uint16, map<uint32,AreaInfo*> > m_areas;

};

#define mapmgr MapMgr::getSingleton( )

#endif