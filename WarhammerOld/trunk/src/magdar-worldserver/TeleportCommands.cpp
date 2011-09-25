#include "StdAfx.h"

void ChatHandler::onTeleport(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	uint16 zone = atoi(m_values[1].c_str());
	uint32 x = atoi(m_values[2].c_str());
	uint32 y = atoi(m_values[3].c_str());
	uint16 z = atoi(m_values[4].c_str());
	uint16 o = atoi( m_values[5].c_str());

	ZoneInfo * Info = objmgr.GetZoneInfo( zone );
	if(!Info)
		return;

	Plr->SafeTeleport(Info->zoneid,Info->region,x,y,z,o);
}
void ChatHandler::onRecallAdd(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	stringstream name;

	for(int i=1;i<m_values.size();++i)
		name << m_values[i] << " ";

	uint32 x = Plr->GetWorldX();
	uint32 y = Plr->GetWorldY();
	uint32 z = Plr->GetWorldZ();
	uint32 o = Plr->GetWorldO();
	uint16 zone = Plr->GetZoneId();
	WorldDatabase.Execute("INSERT INTO recall (name,zone,positionX,positionY,positionZ,positionO) VALUES ('%s','%u','%u','%u','%u','%u')",name.str().c_str(),zone,x,y,z,o);
	stringstream ss;ss << "Recall ["<<name.str().c_str()<<"] added";
	Plr->sendStringMessage(0,"Serveur",ss.str());
}
void ChatHandler::onRecallList(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	QueryResult * Result = WorldDatabase.Query("SELECT id,name FROM recall");
	if(Result)
	{	
		do{
			stringstream ss;
			ss << "["<<Result->Fetch()[1].GetString()<<"] : id "<<Result->Fetch()[0].GetUInt32();
			Plr->sendStringMessage(0,"Serveur",ss.str());
		}while(Result->NextRow());
	}
	else Plr->sendStringMessage(0,"Serveur","Aucun Recall");
	delete Result;
}
void ChatHandler::onRecallRemove(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	string name = m_values[1];

	WorldDatabase.Execute("DELETE FROM recall WHERE name='%s'",name.c_str());
	stringstream ss;ss << "Recall ["<<name<<"] removed";
	Plr->sendStringMessage(0,"Serveur",ss.str());
}
void ChatHandler::onRecallPort(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	string name = m_values[1];

	QueryResult * Result = WorldDatabase.Query("SELECT zone,positionX,positionY,positionZ,positionO FROM recall WHERE name like '%s%s'",name.c_str(),"%");
	if(Result)
	{
		uint16 zone = Result->Fetch()[0].GetUInt32();
		uint32 x = Result->Fetch()[1].GetUInt32();
		uint32 y = Result->Fetch()[2].GetUInt32();
		uint32 z = Result->Fetch()[3].GetUInt32();
		uint32 o = Result->Fetch()[4].GetUInt32();

		if(zone == Plr->GetZoneId())	Plr->SafeTeleport(x,y,z,o);
		else		
		{
			Log.Debug("ChatHandler","Safe Teleporting to new zone");
			ZoneInfo * Info = objmgr.GetZoneInfo(zone);
			if(!Info) return;

			Plr->SafeTeleport(zone,Info->region,x,y,z,o);
		}
	}
	else Plr->sendStringMessage(0,"Serveur","Recall not find");
}
void ChatHandler::onAppear(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	string name = m_values[0];

	Player * Target = objmgr.GetPlayerByName(name);
	if(Target)
		Plr->SafeTeleportToPlayer(Target);
	else
		Plr->sendLocalizedString("",0x02E8); // Impossible de trouver le joueur

}
void ChatHandler::onSummon(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	string name = m_values[0];

	Player * Target = objmgr.GetPlayerByName(name);
	if(Target)
		Target->SafeTeleportToPlayer(Plr);
	else
		Plr->sendLocalizedString("",0x02E8); // Impossible de trouver le joueur
}