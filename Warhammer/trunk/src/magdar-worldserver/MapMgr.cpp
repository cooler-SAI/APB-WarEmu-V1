#include "StdAfx.h"

initialiseSingleton(MapMgr);

MapMgr::MapMgr()
{
	for(int i=0;i<MAX_ZONE_ID;++i)
	{
		m_zones[i]=NULL;
	}
}
ZoneMgr * MapMgr::GetZoneMgr(Object * obj)
{
	if(!obj) return NULL;
	uint16 zone = obj->GetZoneId();
	if(!zone) return NULL;

	ZoneMgr * Zone=NULL;

	M_zones.Acquire();
	Zone = m_zones[zone];
	if(!Zone) Zone = _CreateZone(zone);

	M_zones.Release();
	return Zone;
}
ZoneMgr * MapMgr::GetZoneMgr(uint16 zoneid)
{
	if(!zoneid) return NULL;

	ZoneMgr * Zone=NULL;

	M_zones.Acquire();
	Zone = m_zones[zoneid];
	M_zones.Release();

	return Zone;
}
ZoneMgr * MapMgr::_CreateZone(uint16 zoneid)
{
	M_zones.Acquire();

	ZoneMgr * Zone = new ZoneMgr(zoneid);
	ThreadPool.ExecuteTask(Zone);

	m_zones[zoneid] = Zone;

	M_zones.Release();
	return Zone;
}
void MapMgr::Shutdown()
{
	M_zones.Acquire();

	ZoneMgr * Zone=NULL;
	for(int i=0;i<MAX_ZONE_ID;++i)
	{
		Zone = m_zones[i];
		if( Zone!=NULL )
		{
			Zone->KillThread();
			delete Zone;
		}
	}
	M_zones.Release();

	hfmgr.DestroyMap(0);
}

bool MapMgr::IsOnFirstOffSet(uint8 region,uint16 offset,bool x)
{
	const RegionOffset * Info = objmgr.GetRegionOffset(region);
	uint16 OffStart = 0;
	if(x) OffStart+= Info ? Info->OffX : 0; // 8
	else  OffStart+= Info ? Info->OffY : 0;

	return IsOnFirstOffSet(OffStart,offset);
}

uint16 MapMgr::CalculPosInCombat(uint16 cb_pos,uint16 offset,uint8 region,bool x)
{
	cb_pos/=2;
	if(!IsOnFirstOffSet(region,offset,x))
		cb_pos+=32768;

	return cb_pos;
}
uint16 MapMgr::CalculPosInCombat(const RegionOffset * Off,uint16 cb_pos,uint16 offset,bool x)
{
	cb_pos/=2;

	uint16 OffStart = 0;
	if(Off) OffStart+= x ? Off->OffX : Off->OffY;

	if(!IsOnFirstOffSet(OffStart,offset))
		cb_pos+=32768;

	return cb_pos;
}
// Nouveau Systeme
uint16 MapMgr::CalculOffset(uint32 wp)
{
	return (wp >> 12);
}
uint32 MapMgr::CalculPos(uint8 region,uint32 wp,bool x)
{
	// On récupère le pin position et on la divise par deux;
	const RegionOffset * Info = objmgr.GetRegionOffset(region);
	uint16 OffStart = 0;
	if(Info) OffStart+= x ? Info->OffX : Info->OffY;

	return CalculPos(OffStart,wp);
}
uint16 MapMgr::CalculPin(uint8 region,uint32 wp,bool x)
{
	// wp = 851283

	const RegionOffset * Info = objmgr.GetRegionOffset(region);
	uint16 OffStart = 0;
	if(Info) OffStart+= x ? Info->OffX : Info->OffY;

	return CalculPin(OffStart,wp);
}
uint16 MapMgr::CalculPin(const RegionOffset * Off,uint32 wp,bool x)
{
	uint16 base = 0;
	if(Off) base = x ? Off->OffX : Off->OffY;

	return CalculPin(base,wp);
}
uint32 MapMgr::CalculPos(const RegionOffset * Off,uint32 wp,bool x)
{
	uint16 base = 0;
	if(Off) base = x ? Off->OffX : Off->OffY;

	return CalculPos(base,wp);
}
bool MapMgr::IsOnFirstOffSet(const RegionOffset * Off,uint16 offset,bool x)
{
	uint16 base = 0;
	if(Off) base = x ? Off->OffX : Off->OffY;

	return IsOnFirstOffSet(base,offset);
}

uint16 MapMgr::CalculPin(uint16 base,uint32 wp)
{
	uint16 OffSet = CalculOffset(wp); // 207
	uint16 OffDiff =0;

	// On part de l'offset qui démare la première zone de 32768
	OffDiff = OffSet-base; // 207-8 = 199

	while(OffDiff>=16) { OffDiff-=16; }; // 7

	wp-=( (OffSet-OffDiff) * 4096 ); // 207-7 = 200*4096 = 851283-819200 = 32083

	return wp;
}
uint16 MapMgr::GetBaseOffset(const RegionOffset * Off,uint32 wp,bool x)
{
	/*uint16 OffSet = CalculOffset(wp); // 207
	uint16 OffDiff =0;

	// On part de l'offset qui démare la première zone de 32768
	OffDiff = OffSet-base; // 207-8 = 199

	while(OffDiff>=16) { OffDiff-=16; }; // 7*/
	return 0;
}
uint32 MapMgr::CalculPos(uint16 base,uint32 wp)
{
	// On récupère le pin position et on la divise par deux;
	uint32 pos = CalculPin(base,wp);
	if(pos >= 32768)
		pos-=32768;

	pos*=2;

	return pos;
}
bool MapMgr::IsOnFirstOffSet(uint16 base,uint16 offset)
{
	uint16 OffSet = offset; // 207
	uint16 OffDiff =0;

	// On part de l'offset qui démare la première zone de 32768
	OffDiff = OffSet-base; // 207-8 = 199

	while(OffDiff>=8) { OffDiff-=8; }; // 7

	if( OffDiff < 4)
		return true;

	return false;
}
void MapMgr::LoadAreaInfo()
{
	Log.Debug("MapMgr","Loading Area Info...");

	uint32 counts=0;
	QueryResult * Result = WorldDatabase.Query("SELECT * FROM zone_area;");
	if(Result)
	{	
		do{
			uint32 areaid = Result->Fetch()[0].GetUInt32();
			uint16 zoneid = Result->Fetch()[1].GetUInt32();

			AreaInfo * Info = GetAreaInfo(areaid,zoneid);
			if(!Info) 
			{
				Info = new AreaInfo;
				Info->areaid = areaid;
				Info->zoneid = zoneid;
				m_areas[zoneid][areaid] = Info;
			}

			for(int i=0;i<2;++i)
			{
				Info->influenceid[i] = Result->Fetch()[2+(5*i)].GetUInt32();;
				Info->minx[i] = Result->Fetch()[3+(5*i)].GetUInt32();;
				Info->miny[i] = Result->Fetch()[4+(5*i)].GetUInt32();;
				Info->maxx[i] = Result->Fetch()[5+(5*i)].GetUInt32();;
				Info->maxy[i] = Result->Fetch()[6+(5*i)].GetUInt32();;
			}

			++counts;
		}while(Result->NextRow());
	}
	delete Result;

	Log.Success("MapMgr","Loaded %u Zone Area",counts);

};