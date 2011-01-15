#include "StdAfx.h"


void ChatHandler::onGoSpawn(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	if(!Plr->IsInWorld()) return;

	uint32 entry = atoi(m_values[1].c_str());
	uint32 save = atoi(m_values[2].c_str());

	if(!entry) return;

	GameObject * Go = Plr->GetZoneMgr()->SpawnGameObject(entry,Plr->GetWorldX(),Plr->GetWorldY(),Plr->GetWorldZ(),Plr->GetWorldO(),true);

	if(save && Go)
		Go->Save();
}