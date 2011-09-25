#include "StdAfx.h"

void ChatHandler::onServer(Player * Plr,string command,	vector<string> m_values)
{
	if( Plr == NULL ) return;

	stringstream ss,tt;
	ss << "Joueurs en ligne : "<<objmgr.GetPlayerOnline();
	tt<<"Uptime : "<< objmgr.ConvertTime(objmgr.GetUptime());

	Plr->sendStringMessage(0,Plr->GetName(),ss.str());
	Plr->sendStringMessage(0,Plr->GetName(),tt.str());
}
void ChatHandler::onReboot(Player * Plr,string command,	vector<string> m_values)
{
	if(Plr == NULL) return;

	uint32 temps = atoi(m_values[1].c_str());

	Log.Debug("ChatHandler","onReboot %u",temps);

	if(m_values.size() > 2)
	{
		stringstream raison;
		raison << "[Reboot] : ";
		for(int i=2;i<m_values.size();++i)
			raison << m_values[i] << " ";

		objmgr.SendChatMessage(Plr,raison.str());
		Log.Debug("ChatHandler","onReboot %s",raison.str().c_str());
	}

	stringstream msg;
	msg << "Server Reboot in : " << temps << " sec !";

	Plr->sendStringMessage(Plr->GetOid(),Plr->GetName(),msg.str());

	sWorldServer.m_Shutdown=true;
	sWorldServer.m_ShutdownTimer=temps*1000;
}
void ChatHandler::onAnnounce(Player * Plr,string command,	vector<string> m_values)
{
	if(!Plr) return;

	stringstream ss;
	for(vector<string>::iterator itr=m_values.begin();itr!=m_values.end();++itr)
		ss << (*itr) << " ";

	if(ss.str().size())
		objmgr.SendChatMessage(Plr,ss.str());


}