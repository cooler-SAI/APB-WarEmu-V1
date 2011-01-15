#include "StdAfx.h"

void CharacterMail::Save()
{
	stringstream ssSql;
	ssSql << "REPLACE INTO characters_mails ("
	<< "guid,receiver,sender,receivername,sendername,title,content,money,cr,opened,items) VALUE ("
	<< "'" << guid << "',"
	<< "'" << receiver << "',"
	<< "'" << sender << "',"
	<< "'" << receivername << "',"
	<< "'" << sendername << "',"
	<< "'" << title << "',"
	<< "'" << content << "',"
	<< "'" << money << "',"
	<< "'" << cr << "',"
	<< "'" << opened << "','";

	for(vector<Item*>::iterator itr=m_items.begin();itr!=m_items.end();++itr)
	{
		ssSql << (*itr)->GetGuid() << ";";
		(*itr)->Save();
	}

	ssSql << "')";

	CharacterDatabase.Execute(ssSql.str().c_str());
};
void CharacterMail::Delete()
{
	CharacterDatabase.Execute("DELETE FROM characters_mails WHERE guid=%u",guid);
	if(m_items.size())
	{
		for(vector<Item*>::iterator itr=m_items.begin();itr!=m_items.end();++itr)
		{
			if(!(*itr)) continue;

			CharacterDatabase.Execute("DELETE FROM characters_items WHERE guid=%u",(*itr)->GetGuid());
		}
	}
	delete this;
};

void ClientCom::onMail()
{
	if(!m_Plr) return;
	
	Log.Debug("ClientCom","onMail");

	uint8 type=0;
	m_readBuffer.Trace();
	m_readBuffer >> type;

	switch(type)
	{
	case 0: // A chaque fermeture de fenètre
		if(m_Plr->m_interract != NULL)
		{
			m_Plr->m_interract = NULL;
		}

		sEventHook.ExecuteHook(EVENTS_INTERACT_CLOSE,1,m_Plr);
		break;

	case 1: // Envoi d'un mail !
		{
			m_Plr->GetMailInterface()->BuildMail(m_readBuffer);
		}
		break;

	case 2: // Ouvrir un Mail
	case 3:	// Renvoyer le mail
	case 4: // Supprimer le mail
	case 5:	// Marquer comme lu/non lu
	case 7:	// Prendre un objet
	case 8:	// Prendre l'argent
		{
			uint8 page,num=0;
			m_readBuffer >> page >> num;
			if(num) --num;

			CharacterMail * Mail = m_Plr->GetMailInterface()->GetMail(num);
			if(!Mail) return;

			switch(type)
			{
				case 2:
					m_Plr->GetMailInterface()->sendMail(Mail);
					break;
				case 3:
					//TODO
					break;
				case 4:
					m_Plr->GetMailInterface()->RemoveMail(Mail);
					m_Plr->GetMailInterface()->sendMailCounts();
					m_Plr->GetMailInterface()->sendMailBox();
					break;
				case 5:
					{
						m_readBuffer.skip(7);
						uint8 opened=0;
						m_readBuffer >> opened;
						Mail->opened = opened;

						m_Plr->GetMailInterface()->sendMailBox();
						m_Plr->GetMailInterface()->sendMailCounts();

						Mail->Save();
					}break;
				case 7:
					{
						m_readBuffer.skip(7);
						uint8 itmnum=0;
						m_readBuffer >> itmnum;
						if(Mail->m_items.size() < itmnum+1)
							return;

						Item * itm = Mail->m_items[itmnum];
						if(!itm) return;
						uint16 freeslot = m_Plr->GetItemInterface()->GetFreeSlot();
						if(!freeslot)
						{
							m_Plr->sendLocalizedString("",TEXT_OVERAGE_CANT_TAKE_ATTACHMENTS);
							return;
						}

						m_Plr->GetItemInterface()->AddToSlot(itm,freeslot);
						Mail->m_items.erase(Mail->m_items.begin()+itmnum);

						m_Plr->GetMailInterface()->sendMailUpdate(Mail);

						Mail->Save();
					}break;
				case 8:
					{
						if(!Mail->money)
							return;
						m_Plr->AddWealth(Mail->money);
						Mail->money = 0;
						m_Plr->GetMailInterface()->sendMailUpdate(Mail);

						Mail->Save();
					}
			};
		}break;
	};
}

void MailInterface::Load()
{
	if(!m_player) return;

	Log.Debug("MailInterface","Loading Mail for %u",m_player->GetPlayerID());
	m_mails.clear();
	m_nextsend=0;

	QueryResult * Result = CharacterDatabase.Query("SELECT guid,receiver,sender,receivername,sendername,title,content,money,cr,opened,items FROM characters_mails WHERE receiver=%u",m_player->GetPlayerID());
	if(Result)
	{
		do{
			CharacterMail * Mail = new CharacterMail;
			Mail->guid =		Result->Fetch()[0].GetUInt32();
			Mail->receiver =	Result->Fetch()[1].GetUInt32();
			Mail->sender =		Result->Fetch()[2].GetUInt32();
			Mail->receivername=	Result->Fetch()[3].GetString();
			Mail->sendername =	Result->Fetch()[4].GetString();
			Mail->title =		Result->Fetch()[5].GetString();
			Mail->content =		Result->Fetch()[6].GetString();
			Mail->money =		Result->Fetch()[7].GetUInt32();
			Mail->cr =			Result->Fetch()[8].GetUInt32();
			Mail->opened =		Result->Fetch()[9].GetUInt32();
			string items =		Result->Fetch()[10].GetString();
			Mail->num =			m_mails.size();

			if(items.size())
			{
				int a = items.find(";");
				while(a != string::npos)
				{
					uint32 guid = atoi(items.substr(0,a).c_str());
					items.erase(0,a+1);

					Item * itm = new Item(guid,NULL);
					if(itm->LoadFromGuid())
					{
						Log.Debug("Mail","[%u] Add Item %u",Mail->guid,guid);
						Mail->m_items.push_back(itm);
					}

					a = items.find(";");
				}
			}
			m_mails.push_back(Mail);
		}while(Result->NextRow());
	}
	delete Result;
}
void MailInterface::BuildMail(Buffer &m_readBuffer)
{
	if(!m_player) return;

	if(m_nextsend >= UNIXTIME) // 5secondes
	{
		sendResult(TEXT_MAIL_RESULT6);
		return;
	}

	if( m_player->GetWealth() < MAIL_PRICE)
	{
		sendResult(TEXT_MAIL_RESULT8);
		return;
	}

	// Lecture du destinataire
	uint8 namesize=0;
	uint8 usernameTmp[255];
	m_readBuffer.skip(1);
	m_readBuffer >> namesize;
	m_readBuffer.read(usernameTmp,namesize);

	string receiver = reinterpret_cast<const char*>(usernameTmp);

	if(receiver == m_player->GetName())
	{
		m_player->sendLocalizedString("",TEXT_PLAYER_CANT_MAIL_YOURSELF);
		return;
	}

	PlayerInfo * Info = objmgr.GetPlayerInfoByName(receiver);
	if(!Info)
	{
		// Impossible de trouver ce joueur
		sendResult(TEXT_MAIL_RESULT7);
		return;
	}

	/*if(Info->realm != m_realm)  // TODO : vérifier qu'on puisse send des mails
	{
		//enemy
		return;
	}*/

	// Lecture du Titre
	uint8 titlesize=0;
	uint8 TitleTmp[255];
	m_readBuffer >> titlesize;
	m_readBuffer.skip(1);
	m_readBuffer.read(TitleTmp,titlesize);
	string title = reinterpret_cast<const char*>(TitleTmp);

	// Lecture du Contenus
	uint8 contentsize=0;
	uint8 ContentTmp[999];
	m_readBuffer >> contentsize;
	m_readBuffer.skip(1);
	m_readBuffer.read(ContentTmp,contentsize);
	string content = reinterpret_cast<const char*>(ContentTmp);

	// Lecture de l'argent envoyé
	uint32 money=0;
	m_readBuffer >> money;
	SwapByte::Swap<uint32>(money);

	// Recommandé ou non
	uint8 cr = 0;
	m_readBuffer >> cr;

	// items
	uint8 itemcounts=0;
	m_readBuffer >> itemcounts;


	if(!cr && !m_player->RemoveWealth(money))
	{
		sendResult(TEXT_MAIL_RESULT8);
		return;
	}

	if(!m_player->RemoveWealth(MAIL_PRICE))
		return;

	CharacterMail * Mail	= new CharacterMail;

	// Destinataire && Envoyeur
	Mail->guid				= objmgr.GenerateMailGuid();
	Mail->sender			= m_player->GetPlayerID();
	Mail->receiver			= Info->character_id;
	Mail->sendername		= CharacterDatabase.EscapeString(m_player->GetName());
	Mail->receivername		= CharacterDatabase.EscapeString(Info->name);

	// Contenant
	Mail->title				= CharacterDatabase.EscapeString(title);
	Mail->content			= CharacterDatabase.EscapeString(content);
	Mail->money				= money;

	// Contenus
	Mail->cr				= cr;
	Mail->opened			= false;

	Log.Debug("Mail","itemcounts %u cr %u",itemcounts,cr);

	for(uint8 i=0; i < itemcounts; ++i)
	{
		uint16 itmslot = 0;
		m_readBuffer >> itmslot;
		m_readBuffer.skip(2);

		SwapByte::Swap<uint16>(itmslot);

		Item * itm = m_player->GetItemInterface()->GetItemInSlot(itmslot);
		if(itm)
		{
			Mail->m_items.push_back(itm);
			m_player->GetItemInterface()->RemoveItemInSlot(itmslot);
			itm->SetOwner(NULL);
		}
		else sendResult(TEXT_MAIL_RESULT9);
	}

	sendResult(TEXT_MAIL_RESULT4);

	Mail->Save();

	if(Info->m_player)
		Info->m_player->GetMailInterface()->AddMail(Mail);

	sendMailBox(NULL);
	m_nextsend = UNIXTIME+5; // 5secondes entre chaque envoi
}
void MailInterface::BuildPreMail(Buffer *d,CharacterMail * Mail)
{
	if(!Mail) return;

	d->write<uint32>(0);
	d->write<uint32>( Mail->guid );
	d->write<uint16>( Mail->opened );
	d->write<uint8>(0x64); // Icone ID

	d->write<uint32>(0xFFE4D486); // Le temps d'envoi
	d->write<uint32>(0xFFE4D486); // Heure d'envoi

	d->write<uint32>( Mail->sender ); // id de l'envoyur

	d->write<uint8>(0); // 1 = localized name

	/*
	if( localized )
		d->write<uint32>(localizedid);
		d->write<uint32>(0);
	else
	*/

	d->write<uint8>(0);
	d->write<uint8>(Mail->sendername.size()+1);
	d->write((const uint8*) &(Mail->sendername[0]),Mail->sendername.size());
	d->write<uint8>(0);

	d->write<uint8>(0);

	d->write<uint8>(Mail->receivername.size()+1);
	d->write((const uint8*) &(Mail->receivername[0]),Mail->receivername.size());
	d->write<uint8>(0);

	d->write<uint8>(0);
	d->write<uint8>(Mail->title.size()+1);
	d->write((const uint8*) &(Mail->title[0]),Mail->title.size());
	d->write<uint8>(0);

	d->write<uint32>(0);

	d->write<uint32>(Mail->money);
	d->write<uint16>(Mail->m_items.size());	// Object dedans
	if(Mail->m_items.size())
	{
		d->write<uint8>(0); // Nombre d'items récup
		// 1 , 2 , 4 , 8 c'est un flag pour foutre l'item en gris
		//

		for(vector<Item*>::iterator itr=Mail->m_items.begin();itr!=Mail->m_items.end();++itr)
		{
			if(!(*itr)) continue;
			d->write<uint32>((*itr)->GetProto()->modelid);
		}
	}
}
void MailInterface::sendMailCounts()
{
	if(!m_player) return;

	uint16 responseSize = 4;
	Buffer* b = m_player->Allocate(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_MAIL);
	b->write<uint8>(0x9);
	b->write<uint8>(GameData::MAILBOXTYPE_PLAYER);

	uint16 counts=0;

	M_mails.Acquire();
	for(vector<CharacterMail*>::iterator itr=m_mails.begin();itr!=m_mails.end();++itr)
		if( (*itr) && !(*itr)->opened)
			++counts;
	M_mails.Release();

	b->write<uint16>(counts);
	m_player->sendPacket(b,false);

	b->reset();
	b->write<uint16>(responseSize);
	b->write<uint8>(F_MAIL);
	b->write<uint8>(0x9); // Display unread messages count
	b->write<uint8>(GameData::MAILBOXTYPE_AUCTION); // auction mailbox
	b->write<uint16>(0); // number of unread messages
	m_player->sendPacket(b,false);
}
void MailInterface::sendMailBox(Unit * sender)
{
	if(!m_player) return;

	{
		Buffer *b = new Buffer(3);
		b->write<uint16>(3);
		b->write<uint8>(F_MAIL);
		b->write<uint16>(0);
		b->write<uint8>(1);
		m_player->sendPacket(b);
	}

	{
		Buffer *b = new Buffer(10);
		b->write<uint16>(10);
		b->write<uint8>(F_MAIL);
		b->write<uint32>(0x0E000000);
		b->write<uint32>(0x001E0AD7);
		b->write<uint16>(0xA33C);
		m_player->sendPacket(b);
	}

	{
		Buffer *d = new Buffer(0);

		d->write<uint8>(0x0A);
		d->write<uint16>(0);

		M_mails.Acquire();
		d->write<uint8>(m_mails.size());
		for(vector<CharacterMail*>::iterator itr=m_mails.begin();itr!=m_mails.end();++itr)
		{
			BuildPreMail(d,(*itr));
		}
		
		d->write<uint16>( m_mails.size() );
		M_mails.Release();

		Buffer *b = new Buffer(d->size()+3);
		b->write<uint16>(d->size());
		b->write<uint8>(F_MAIL);
		b->write(d);
		m_player->sendPacket(b);
	}
	
	/*{
		Buffer *b = new Buffer(6);
		b->write<uint16>(6);
		b->write<uint8>(F_MAIL);
		b->write<uint32>(0x0A010000);
		b->write<uint16>(0);
		m_player->sendPacket(b);
	}*/


}
void MailInterface::AddMail(CharacterMail * Mail)
{
	if(!Mail) return;

	M_mails.Acquire();
	m_mails.push_back(Mail);
	M_mails.Release();

	sendMailCounts();
}
void MailInterface::sendMailUpdate(CharacterMail * Mail)
{
	if(!Mail) return;
	
	Buffer *b = new Buffer(0);
	b->write<uint8>(0x0B);
	b->write<uint8>(0);
	BuildPreMail(b,Mail);

	Buffer *d = new Buffer(3+b->size());
	d->write<uint16>(b->size());
	d->write<uint8>(F_MAIL);
	d->write(b);
	d->Trace();

	m_player->sendPacket(d);
}
void MailInterface::sendMail(CharacterMail * Mail)
{
	if(!Mail) return;
	
	Buffer *b = new Buffer(0);
	b->write<uint8>(0x0D);
	b->write<uint8>(0);
	BuildPreMail(b,Mail);
	b->write<uint16>(Mail->content.length()+1);
	b->write((const uint8*) &(Mail->content[0]),Mail->content.size());
	b->write<uint8>(0);

	b->write<uint8>(Mail->m_items.size()); // Items counts;
	if(Mail->m_items.size())
	{
		for(vector<Item*>::iterator itr=Mail->m_items.begin();itr!=Mail->m_items.end();++itr)
		{
			if(!(*itr)) continue;
			sChat.BuildItemInfo(b,(*itr)->GetProto(),(*itr));
		}
	}

	Buffer *d = new Buffer(3+b->size());
	d->write<uint16>(b->size());
	d->write<uint8>(F_MAIL);
	d->write(b);
	d->Trace();

	m_player->sendPacket(d);

	if(!Mail->opened)
	{
		Mail->opened = true;
		sendMailBox();
		sendMailCounts();
	}
}
CharacterMail * MailInterface::GetMail(uint32 guid)
{
	CharacterMail * Mail=NULL;

	M_mails.Acquire();
	for(vector<CharacterMail*>::iterator itr=m_mails.begin();itr!=m_mails.end();++itr)
		if( (*itr) && (*itr)->guid == guid)
		{
			Mail = (*itr);
			break;
		}
	M_mails.Release();

	return Mail;
}
void MailInterface::sendResult(MailResult Result)
{
	if(!m_player) return;

	Buffer *b=new Buffer(3+3);
	b->write<uint16>(3);
	b->write<uint8>(F_MAIL);
	b->write<uint8>(0x0F);
	b->write<uint16>(Result);
	m_player->sendPacket(b);
}
CharacterMail * MailInterface::GetMail(uint8 num)
{
	CharacterMail * Mail=NULL;

	M_mails.Acquire();

	for(vector<CharacterMail*>::iterator itr=m_mails.begin();itr!=m_mails.end();++itr)
		if( (*itr) && (*itr)->num == num)
		{
			Mail = (*itr);
			break;
		}
	M_mails.Release();

	return Mail;
}