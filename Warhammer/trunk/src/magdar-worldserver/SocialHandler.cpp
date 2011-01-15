#include "StdAfx.h"

void ClientCom::onSocialNetwork()
{
	Log.Debug("ClientCom","onSocialNetwork()");
	uint8 type=0;
	m_readBuffer >> type;

	if(!m_Plr) return;

	switch(type)
	{
	case 11: // Inspect
		{
		Unit * target = m_Plr->GetTarget();
		if(target == NULL || !target->IsPlayer())
			return;
		Player * ptarget = target->GetPlayer();
		ptarget->GetItemInterface()->SendInspect(m_Plr);
		}
		break;
	case 8: // Search Players social menu
		{
		uint8 namesize,guildsize,minlevel,maxlevel,unk=0;
		uint16 career,zoneid=0;
		string name,guildname;

		uint8 usernameTmp[23];
		uint8 guildnameTmp[28];

		m_readBuffer.skip(1);
		m_readBuffer>>namesize;
		m_readBuffer.skip(1);
		m_readBuffer.read(usernameTmp,namesize);
		m_readBuffer >> guildsize;
		m_readBuffer.skip(1);
		m_readBuffer.read(guildnameTmp,guildsize);
		m_readBuffer.skip(1);
		m_readBuffer >> career;
		m_readBuffer.skip(4);
		m_readBuffer >> zoneid;

		while(zoneid > 256)
			zoneid-=256;

		m_readBuffer >> unk;
		while(unk != 0xFF)
			m_readBuffer >> unk;

		m_readBuffer.skip(2+(zoneid==255 ? 0 : 1));

		m_readBuffer >> minlevel >> maxlevel;

		name = reinterpret_cast<const char*>(usernameTmp);
		guildname = reinterpret_cast<const char*>(guildnameTmp);
		name.resize(namesize);
		guildname.resize(guildsize);
		guildname = Utils::strToLowerCase(guildname);
		name = Utils::strToLowerCase(name);

		Log.Debug("Social","user=%s,guild=%s,career=%u,zoneid=%u,minlvl=%u,maxlvl=%u",name.c_str(),guildname.c_str(),career,zoneid,minlevel,maxlevel);
		objmgr.SendPlayerOnline(m_Plr,name,guildname,career,zoneid,minlevel,maxlevel);
		}
		break;
	};
}

// Ajoute/Suppr un ami
void ChatHandler::onFriendAdd(Player *Plr,string command,	string value)
{
	if(Plr == NULL) return;
	if(!value.size()) return;

	Plr->Social_AddFriend(value);
}
void ChatHandler::onFriendRemove(Player *Plr,string command,	string value)
{
	if(Plr == NULL) return;
	if(!value.size()) return;

	Plr->Social_RemoveFriend(value);
}
void ChatHandler::onIgnoreAdd(Player *Plr,string command,	string value)
{
	if(Plr == NULL) return;
	if(!value.size()) return;

	Plr->Social_AddIgnore(value);
}
void ChatHandler::onIgnoreRemove(Player *Plr,string command,	string value)
{
	if(Plr == NULL) return;
	if(!value.size()) return;

	Plr->Social_RemoveIgnore(value);
}

void Player::Social_AddIgnore(string name)
{
	if(!name.size()) return;

	Log.Debug("Player","Social_AddIgnore %s",name.c_str());
}
void Player::Social_RemoveIgnore(string name)
{
	if(!name.size()) return;

	Log.Debug("Player","Social_RemoveIgnore %s",name.c_str());
}
void Player::Social_AddFriend(string name)
{
	if(!name.size()) return;

	Log.Debug("Player","Social_AddFriend %s",name.c_str());

	name = Utils::strToLowerCase(name);

	// Même nom
	if(Utils::strToLowerCase( GetName()).compare( name ) == 0)
	{
		// Vous ne pouvez pas vous ajouter vous même
		sendLocalizedString("",TEXT_SN_IGNORELIST_ERR_ADD_SELF);
		return;
	}

	PlayerInfo * Info = objmgr.GetPlayerInfoByName(name);
	if(!Info)
	{
		// Impossible de trouver ce joueur
		sendLocalizedString("",TEXT_SN_LISTS_ERR_PLAYER_NOT_FOUND);
		return;
	}

	if(Info->realm != m_realm)
	{
		//enemy
		return;
	}

	/*if(Info->account == GetAccountID)
		return;*/

	M_social.Acquire();
	if(m_friend.find(Info->character_id) != m_friend.end())
	{
		M_social.Release();
		return;
	}

	m_friend.insert(Info->character_id);


	Buffer *d=new Buffer(0);
	sChat.BuildSocialInfo(d,Info,true);

	uint16 responseSize=5+d->size();

	Buffer *b = GetCom()->m_bufferPool->allocateBuffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_SOCIAL_NETWORK);

	b->write<uint16>(0);
	b->write<uint8>(1);
	b->write<uint8>(1);
	b->write<uint8>(0);
	b->write(d);
	sendPacket(b);
	delete d;
	// Vous avez ajouté x en amis
	sendLocalizedString(Info->name,TEXT_SN_FRIENDSLIST_ADD_SUCCESS);

	if(Info->m_player)
	{
		Player * Inv = Info->m_player;
		// x vous a ajouter en ami , ajoutez le aussi
		Buffer *b = Inv->GetCom()->m_bufferPool->allocateBuffer(13+GetName().length()+3);
		b->write<uint16>(13+GetName().length());
		b->write<uint8>(0x1C);

		b->write<uint8>(GetPlayerID() & 0x000000FF);
		b->write<uint8>(GetPlayerID() & 0x0000FF00);
		b->write<uint8>(GetPlayerID() & 0x00FF0000);
		b->write<uint16>(GetCareerLine());
		b->write<uint16>(GetLevel());
		b->write<uint16>(GetName().length());
		b->write<uint16>(GetName().length()+1);
		b->write(reinterpret_cast<const uint8*>(GetName().c_str()),GetName().length());
		b->write<uint16>(0x00);
		Inv->sendPacket(b);
		Inv->Social_AddHasFriend(this);
	}
	M_social.Release();
	SaveSocial();
}
void Player::Social_RemoveFriend(string name)	
{
	if(!name.size()) return;
	if(name.size() < 3) return;

	name.resize(name.length()-2);
	name = Utils::strToLowerCase(name);
	Log.Debug("Player","Social_RemoveFriend %s",name.c_str());
	PlayerInfo * Info = objmgr.GetPlayerInfoByName(name);
	if(!Info)
		return;

	M_social.Acquire();

	if(m_friend.find(Info->character_id) == m_friend.end())
	{
		M_social.Release();
		return;
	}

	m_friend.erase(Info->character_id);
	M_social.Release();


	uint16 responseSize=9;
	Buffer *b = GetCom()->m_bufferPool->allocateBuffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_SOCIAL_NETWORK);
	/*00 00 01 01 01 00 09 0E 86*/
	b->write<uint16>(0x0000);
	b->write<uint8>(0x01);
	b->write<uint8>(0x01);
	b->write<uint8>(0x01);
	b->write<uint32>(Info->character_id);
	sendPacket(b);
	// Vous avez supprimé x en amis
	sendLocalizedString(Info->name,TEXT_SN_FRIEND_REMOVE);

	if(Info->m_player)
		Info->m_player->Social_RemoveHasFriend(this);

	SaveSocial();
}
// Kelkun m'ajoute/me suppr de ces amis
void Player::Social_RemoveHasFriend(Player * Plr)
{
	if(Plr == NULL) return;

	Log.Debug("Player","Social_RemoveHasFriend %s",Plr->GetName().c_str());

	M_social.Acquire();
	m_hasfriends.erase( Plr->GetPlayerID() );
	M_social.Release();
}
void Player::Social_AddHasFriend(Player * Plr)
{
	if(Plr == NULL) return;

	Log.Debug("Player","Social_AddHasFriend %s",Plr->GetName().c_str());

	M_social.Acquire();
	m_hasfriends.insert( Plr->GetPlayerID() );
	M_social.Release();
}
void Player::Social_SetOnline()
{
	M_social.Acquire();
	if(!m_hasfriends.size())
	{
		M_social.Release();
		return;
	}

	for(set<uint32>::iterator itr=m_hasfriends.begin();itr!=m_hasfriends.end();++itr)
	{
		// X vien de se connecter
		Player * Plr = objmgr.GetPlayer( (*itr) );
		if(Plr) 
		{
			Plr->sendLocalizedString(GetName(),TEXT_SN_FRIEND_LOGON);

			uint16 responseSize=5+20+GetName().length();
			Buffer *b = Plr->GetCom()->m_bufferPool->allocateBuffer(responseSize+3);
			b->write<uint16>(responseSize);
			b->write<uint8>(F_SOCIAL_NETWORK);

			b->write<uint16>(0x0000);
			b->write<uint8>(0x01);
			b->write<uint8>(0x01);
			b->write<uint8>(0x00);
			sChat.BuildSocialInfo(b,this,true);
			Plr->sendPacket(b);
		}
	}
	M_social.Release();

}
void Player::Social_SetOffline()
{
	M_social.Acquire();
	if(!m_hasfriends.size())
	{
		M_social.Release();
		return;
	}

	for(set<uint32>::iterator itr=m_hasfriends.begin();itr!=m_hasfriends.end();++itr)
	{
		// X vien de se deconnecter
		Player * Plr = objmgr.GetPlayer( (*itr) );
		if(Plr) 
		{
			Plr->sendLocalizedString(GetName(),TEXT_SN_FRIEND_LOGOFF);

			uint16 responseSize=5+10+GetName().length();
			Buffer *b = Plr->GetCom()->m_bufferPool->allocateBuffer(responseSize+3);
			b->write<uint16>(responseSize);
			b->write<uint8>(F_SOCIAL_NETWORK);

			b->write<uint16>(0x0000);
			b->write<uint8>(0x01);
			b->write<uint8>(0x01);
			b->write<uint8>(0x00);
			sChat.BuildSocialInfo(b,this,false);
			Plr->sendPacket(b);
		}
	}
	M_social.Release();
}
void Player::Social_SendFriends()
{
	Log.Debug("Player","Social_SendFriends %s",GetName().c_str());

	uint8 counts=0;
	Buffer *d=new Buffer(0);
	
	M_social.Acquire();
	for(set<uint32>::iterator itr=m_friend.begin();itr!=m_friend.end();++itr)
	{
		if((*itr)==m_character_id) continue;
		PlayerInfo * Inf = objmgr.GetPlayerInfo( (*itr) );

		if(Inf)
		{
			sChat.BuildSocialInfo(d,Inf,true);
			counts++;
		}
	}
	M_social.Release();

	uint16 responseSize=5+d->size();

	Buffer *b = GetCom()->m_bufferPool->allocateBuffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_SOCIAL_NETWORK);
	b->write<uint16>(0);
	b->write<uint8>(1);
	b->write<uint8>(counts);
	b->write<uint8>(0);
	b->write(d);
	sendPacket(b);
	delete d;
}