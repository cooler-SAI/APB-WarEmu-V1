#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include "StdAfx.h"

struct ZoneHeight
{
	~ZoneHeight()
	{
		delete terrain;
		delete offset;
	}
	int zoneid;
	sf::Image * terrain;
	sf::Image * offset;
};

class HeightFieldMgr : public Singleton< HeightFieldMgr >
{
public:
	ZoneHeight* LoadMap(int ZoneID);
	ZoneHeight* GetImage(int ZoneId);
	static int GetHeight(ZoneHeight * Height, int CoordX, int CoordY);
	void DestroyMap(int ZoneID=0);
private:
	Mutex M_heightmaps;
	std::map<int, ZoneHeight*> m_heightmaps;
};

#define hfmgr HeightFieldMgr::getSingleton( )

#endif
