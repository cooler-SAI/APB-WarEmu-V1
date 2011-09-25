#include "StdAfx.h"

void ChatHandler::onInvite(Player *Plr,string command,	string value)
{
	if(Plr == NULL) return;

	Log.Debug("ChatHandler","[%s] Invite [%s] in group",Plr->GetName().c_str(),value.c_str());
	
	if(Plr->GetGroup() && Plr->GetGroup()->IsFull())
	{
		Plr->sendLocalizedString(string(""),TEXT_PARTY_IS_FULL);
		return;
	}
	
	Player * Receiver = objmgr.GetPlayerByName(value);
	if(Receiver)
	{
		if(Receiver == Plr)
		{
			Plr->sendLocalizedString("",TEXT_GROUP_INVITE_ERR_SELF);
			return;
		}

		if(Receiver->GetRealm() != Plr->GetRealm())
		{
			Plr->sendLocalizedString("",TEXT_GROUP_INVITE_ERR_ENEMY);
			return;
		}

		if( Receiver->GetGroup() )
		{
			Plr->sendLocalizedString("",TEXT_BG_PLAYER_IN_ANOTHER_GROUP);
			return;
		}
		
		sEventHook.ExecuteHook(EVENTS_GROUP_INVITE_PLAYER,2,Plr,Receiver);

		Log.Debug("ChatHandler","1");
		if( Receiver->Invitation == NULL)
			GroupInvitation * Inv = new GroupInvitation(Plr->GetOid(),Receiver->GetOid(),Plr,Receiver);
		else Plr->sendLocalizedString("",TEXT_BG_PLAYER_PENDING_ANOTHER);
	}
	else Plr->sendLocalizedString("",TEXT_SN_LISTS_ERR_PLAYER_NOT_FOUND);

}

void ClientCom::onGroupCommand()
{
	uint32 unk;
	uint8 state;
	m_readBuffer >> unk >> state;

	Log.Debug("ClientCom","onGroupCommand %u",state);
	if(m_Plr)
	{
		if(state == 2 && m_Plr->Invitation) // Accepte
		{
			Log.Debug("ClientCom","Accepte");
			m_Plr->Invitation->AcceptInvitation();

		}else if(state == 6 && m_Plr->Invitation) // decline
		{
			Log.Debug("ClientCom","Decline");
			m_Plr->Invitation->DeclineInvitation();
		}
		else if(state == 3 && m_Plr->GetGroup() ) // quite le groupe
		{
			Log.Debug("ClientCom","Quite");
			m_Plr->GetGroup()->RemoveMember(m_Plr);
			m_Plr->SetGroup(NULL);
		}
	}
}
