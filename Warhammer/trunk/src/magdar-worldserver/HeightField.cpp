#include "StdAfx.h"

initialiseSingleton(HeightFieldMgr);

ZoneHeight*  HeightFieldMgr::LoadMap(int ZoneID)
{
	stringstream ss,dd;
	ss << "zones/zone";
	if(ZoneID < 100) ss << "0";
	else if(ZoneID < 10) ss <<"00";

	ss << ZoneID << "/";

	dd << ss.str();
	dd << "offset.png";
	ss << "terrain.png";

	Log.Debug("HFMgr","[%u] [->--> Loading Height <--<-]",ZoneID);
	sf::Image* pTerrain = new sf::Image();
	sf::Image* pOffset = new sf::Image();

	if(!pTerrain->LoadFromFile(ss.str()) || !pOffset->LoadFromFile(dd.str()))
	{
		Log.Error("HfMgr","Can not Load Height !");
		return NULL;
	}
	else
	{
		ZoneHeight * Height = new ZoneHeight;
		Height->terrain = pTerrain;
		Height->offset = pOffset;
		Height->zoneid = ZoneID;

		M_heightmaps.Acquire();
		m_heightmaps[ZoneID] = Height;
		M_heightmaps.Release();
		return Height;
	}
}
ZoneHeight* HeightFieldMgr::GetImage(int ZoneId)
{
	ZoneHeight* Image=NULL;

	M_heightmaps.Acquire();
	Image=m_heightmaps[ZoneId];
	M_heightmaps.Release();

	if(!Image)
		Image = LoadMap(ZoneId);

	return Image;
}
int HeightFieldMgr::GetHeight(ZoneHeight* Height, int CoordX, int CoordY)
{
	if(!Height) return -1;
	if(!Height->offset) return -1;
	if(!Height->terrain) return -1;

		// Conversion coordonnées ému <-> image
	float fTmpX = (float)CoordX / 64.f; // Humm kaldo ^^
	float fTmpY = (float)CoordY / 64.f; //

	CoordX = (int)fTmpX;
	CoordY = (int)fTmpY;
	float fZValue=0;
	{
		sf::Color iColor = Height->offset->GetPixel(CoordX, CoordY);
		fZValue+=iColor.r*31; // 0 -> 30
	}
	{
		sf::Color iColor = Height->terrain->GetPixel(CoordX, CoordY);
		fZValue+=iColor.r;
	}
	fZValue*=32;

	return (int)fZValue;
}

void HeightFieldMgr::DestroyMap(int ZoneID)
{
	Log.Debug("HfMgr","Deleting All Height...");
	if(!ZoneID)
	{
		for(std::map<int, ZoneHeight*>::iterator itr=m_heightmaps.begin();itr!=m_heightmaps.end();++itr)
			if(itr->second)
				delete itr->second;

		m_heightmaps.clear();
	}
	else
	{
		delete m_heightmaps[ZoneID];
		m_heightmaps[ZoneID] = NULL;
	}
}


