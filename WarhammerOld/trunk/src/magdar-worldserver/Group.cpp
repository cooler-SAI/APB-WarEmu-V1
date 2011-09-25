#include "StdAfx.h"

GroupInvitation::GroupInvitation(uint16 own,uint16 inv,Player * Own,Player * Inv)
{ 
	Log.Debug("ChatHandler","new Group");
	owner=own;
	invited=inv;
	Owner=Own;
	Invited=Inv; 
	SendInvitation();
	expire=UNIXTIME+65;
};

void GroupInvitation::SendInvitation()
{
	Log.Debug("ChatHandler","Sending invitation");
	if(!Owner || !Invited)
		return;

	Owner->Invitation = this;
	Invited->Invitation = this;

	Owner->sendLocalizedString(Invited->GetName(),TEXT_GROUP_INVITE_BEGIN);
	Invited->sendLocalizedString(Owner->GetName(),TEXT_GROUP_YOU_WERE_INVITED);
	Invited->sendShowDialog(Owner->GetName(),0x0002);
}

void GroupInvitation::DeclineInvitation()
{
	if(Owner && Invited)
	Owner->sendLocalizedString(Invited->GetName(),TEXT_ERROR_INVITE_DECLINED);

	Log.Debug("ClientCom","Decline");
	Destroy();

	sEventHook.ExecuteHook(EVENTS_GROUP_DECLINE_INVITATION,2,Owner,Invited);
}

void GroupInvitation::AcceptInvitation()
{
	if(Owner==NULL || Invited==NULL)
		return;

	Group * group=NULL;

	if(Owner->GetGroup())
		group = Owner->GetGroup();
	else // on créer le groupe
	{
		group = new Group;
		group->SetLeader(Owner);
	}

	if(group->IsFull())
	{
		Log.Debug("ClientCom","Groupe is FULL");
		Invited->sendLocalizedString(string(""),TEXT_PARTY_IS_FULL);
		Destroy();
		return;
	}

	sEventHook.ExecuteHook(EVENTS_GROUP_ACCEPT_INVITATION,2,Owner,Invited);
	Invited->SetGroup(group);
	Owner->SetGroup(group);

	Destroy();
}

void GroupInvitation::Destroy()
{
	if(Owner) Owner->Invitation = NULL;
	if(Invited) Invited->Invitation = NULL;
	delete this;
}

Group::Group()
{
	m_GroupMembers.clear();
	m_Leader=NULL;
}
void Group::Update()
{
	Log.Debug("Group","Updating Group");

	uint32 counts=0;
	uint16 responseSize = 5;

	bool lead=false;

	M_GroupMembers.Acquire();
	for( GroupMembersSet::iterator itr = m_GroupMembers.begin(); itr != m_GroupMembers.end(); itr++ )
	{
		if(!(*itr)) continue;

		counts++;
		responseSize+=( 40+(*itr)->GetName().length() );
		if( (*itr) == m_Leader ) lead=true;

	}
	M_GroupMembers.Release();

	// Pas de Lead , pas de membre ou seulement le leader
	if(!m_Leader || (counts < 1) || (counts == 1 && lead) )
	{
		Delete();
		return;
	}


	Buffer * b = new Buffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_CHARACTER_INFO);
	b->write<uint16>(0x0602); // Groupe info
	b->write<uint16>(0xB536); // Groupe id
	b->write<uint8>(counts);

	M_GroupMembers.Acquire();
	for( GroupMembersSet::iterator itr = m_GroupMembers.begin(); itr != m_GroupMembers.end(); ++itr )
	{
		if( !(*itr) ) continue;

			uint8 data1[4] = { 0xA8,0xBF,0x3B,0x0C };
			b->write(data1,4);

			b->write<uint8>( (*itr)->GetModel() );
			b->write<uint8>( (*itr)->GetRace() );
			b->write<uint8>( (*itr)->GetLevel() );
			b->write<uint16>( (*itr)->GetCareerLine() );
			b->write<uint8>(1);
			b->write<uint16>(0);
			b->write<uint8>(1);
			b->write<uint8>( (*itr)->GetName().length() );
			b->fill(0,3);
			b->write(reinterpret_cast<const uint8*>((*itr)->GetName().c_str()),(*itr)->GetName().length());
			b->fill(0,4);

			// X/Y ....
			uint8 data[16] =
			{
				0x84,0x7A,0x90,0x95,0x03,0xE2,0x36,0x50,0xE2,0x50,
				0x40,0x01,0x02,0xD4,0x01,0x01,
			};

			b->write(data,16);
			b->write<uint8>( (*itr)->GetHealthPct() );
			b->write<uint8>( 0x64 );
			b->write<uint8>( 0 );
	}
	M_GroupMembers.Release();


	DispachToGroup(b);

	sEventHook.ExecuteHook(EVENTS_GROUP_UPDATED,1,this);
}
void Group::DispachToGroup(Buffer * b)
{
	M_GroupMembers.Acquire();
	for( GroupMembersSet::iterator itr = m_GroupMembers.begin(); itr != m_GroupMembers.end(); ++itr )
		if( (*itr) != NULL )
			(*itr)->dispachPacket(b);
	M_GroupMembers.Release();
}
void Group::SetLeader(Player * Plr) 
{ 
	if(Plr == NULL) return;
	
	Log.Debug("Group","SetLeader %s",Plr->GetName().c_str());
	m_Leader=Plr; 
	Plr->sendLocalizedString(string(""),TEXT_YOU_NOW_PARTY_LEADER);// Vous etes désormais responsable du groupe;
}

void Group::AddMember(Player * Plr) 
{
	if( Plr == NULL ) return;
	if( Plr->GetGroup() && Plr->GetGroup() != this)
		Plr->GetGroup()->RemoveMember(Plr);

	Log.Debug("Group","AddMember %s",Plr->GetName().c_str());

	M_GroupMembers.Acquire();
	m_GroupMembers.push_back(Plr); 
	M_GroupMembers.Release();

	Plr->sendLocalizedString(string(""),TEXT_YOU_JOIN_PARTY);

	if(Plr != m_Leader)
		sendWhosLeader(Plr);


	sEventHook.ExecuteHook(EVENTS_GROUP_PLAYER_ADDED,2,Plr,this);

	Update();
};
void Group::RemoveMember(Player * Plr) 
{ 
	if(Plr == NULL) return;


	if(!m_GroupMembers.size()) return;

	Log.Debug("Group","RemoveMember %s",Plr->GetName().c_str());

	M_GroupMembers.Acquire();
	m_GroupMembers.remove(Plr); 
	sendNullGroup(Plr);
	for( GroupMembersSet::iterator itr = m_GroupMembers.begin(); itr != m_GroupMembers.end(); ++itr )
		if( (*itr) != NULL )
			(*itr)->sendLocalizedString(Plr->GetName(),TEXT_X_LEFT_PARTY); // x a quité le groupe
	M_GroupMembers.Release();

	if(Plr == m_Leader) m_Leader = NULL;

	Update();
}
void Group::sendNullGroup(Player * Plr)
{
	if(Plr == NULL) return ;

	Plr->sendLocalizedString("",TEXT_YOU_LEFT_PARTY); // vous quittez le groupe

	Buffer *b = Plr->GetCom()->m_bufferPool->allocateBuffer(5+3);
	b->write<uint16>(5);
	b->write<uint8>(F_CHARACTER_INFO);
	b->write<uint32>(0x06028C11);
	b->write<uint8>(0x00);
	Plr->sendPacket(b);

	sEventHook.ExecuteHook(EVENTS_GROUP_LEAVE,2,Plr,this);
}

bool Group::HasMember(uint32 playerid)
{
	bool ok=false;

	M_GroupMembers.Acquire();
	for( GroupMembersSet::iterator itr = m_GroupMembers.begin(); itr != m_GroupMembers.end(); ++itr )
		if( (*itr) != NULL && (*itr)->GetPlayerID() == playerid)
			{ ok=true;break; }
	M_GroupMembers.Release();
	return ok;
}


void Group::sendWhosLeader(Player * Plr) // Envoi le nom du leader du groupe;
{
	if( m_Leader==NULL ) return;

	Plr->sendLocalizedString(m_Leader->GetName(),TEXT_NEW_PARTY_LEADER);
}
void Group::sendNewMember(Player * Plr) // Envoi a tous le groupe le nouveau membre qui a rejoins;
{
	M_GroupMembers.Acquire();
	for(GroupMembersSet::iterator itr=m_GroupMembers.begin();itr!=m_GroupMembers.end();++itr)
	{
		if( (*itr)==NULL ) continue;
		(*itr)->sendLocalizedString(Plr->GetName(),TEXT_PLAYER_JOIN_PARTY);
	}
	M_GroupMembers.Release();
}
void Group::sendMessageToGroup(Player * Sender,string message)
{
	if(!Sender) return;

	Buffer * b = sChat.BuildMessagePacket(Sender->GetCom(),Sender->GetOid(),Sender->GetName(),message,CHATLOGFILTERS_GROUP);
	DispachToGroup(b);
	Sender->GetCom()->m_bufferPool->disposeBuffer(b);
}
void Group::UpdatePlayerPosition(Player * Plr)
{
	if( Plr==NULL ) return;

}
void Group::Delete()
{
	Log.Debug("Group","Deleting Group");


	M_GroupMembers.Acquire();
	Player * Plr = NULL;
	for( GroupMembersSet::iterator itr = m_GroupMembers.begin(); itr != m_GroupMembers.end(); ++itr )
	{
		Plr = (*itr);
		if( Plr == NULL ) continue;

		sendNullGroup( Plr );
		Plr->SetGroup(NULL);

		if( Plr == m_Leader) 
			m_Leader = NULL;
	}
	m_GroupMembers.clear();
	M_GroupMembers.Release();

	delete this;
}