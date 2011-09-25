#include "StdAfx.h"

initialiseSingleton(StatsMgr);

StatsMgr::StatsMgr()
{
	m_day = *localtime(&UNIXTIME);

	string day = GetDayString();
	Log.Debug("STATS","DAY = %s",day.c_str());
	WorldDatabase.Execute("INSERT IGNORE realm_stats (day) VALUE (ADDDATE('%s',0))",day.c_str());
	WorldDatabase.Execute("UPDATE realm_stats SET reboots=reboots+1 WHERE day='%s'",day.c_str());
}
void Update()
{
	uint32 actuco = objmgr.GetPlayerOnline();

}