#include "StdAfx.h"

void ClientCom::onInterract()
{
	Struct_f_interract interract;

	m_readBuffer >> interract.toid;
	m_readBuffer >> interract.menu;
	m_readBuffer >> interract.page;
	m_readBuffer >> interract.itemnum;
	m_readBuffer >> interract.unk;
	m_readBuffer >> interract.count;
	m_readBuffer >> interract.selcount;

	if(interract.toid && m_Plr && m_Plr->IsInWorld())
	{
		Unit * unit = m_Plr->GetZoneMgr()->GetUnit(interract.toid);
		if(!unit) return;
		
		float dist = m_Plr->GetDistance( unit );
		if(dist > 10.0f) return;

		unit->SendInterractTo(m_Plr,interract);
	}
}
void Player::SendInterractTo(Player * Plr,Struct_f_interract &interract)
{
	

}
void GameObject::SendInterractTo(Player *Plr,Struct_f_interract &interract) 
{ 
	Log.Debug("Gameobject","SendInterractTo(%s) Type=%u",Plr->GetName().c_str(),m_interract);

	if(IsDead())
		return;

	if(GetName() == "Mailbox")
		Plr->GetMailInterface()->sendMailBox(this);
}
void Creature::SendInterractTo(Player * Plr,Struct_f_interract &interract)
{
	Log.Debug("Creature","SendInterractTo(%s) Type=%u",Plr->GetName().c_str(),m_interract);

	Plr->m_interract=this;

	if(Plr->GetQuestInterface())
		Plr->GetQuestInterface()->onHandleEvent(QUEST_SPEACK_TO,1,this,NULL,Plr);

	if(Plr->GetPublicQuest())
		Plr->GetQuestInterface()->onHandleEvent(QUEST_SPEACK_TO,1,this,NULL,Plr);

	if(IsDead()) // Loot
	{
		SendLootTo(Plr,interract);
		return;
	}

	switch(m_interract)
	{
	case GameData::INTERACTTYPE_DYEMERCHANT:	//////////////////////////////////////////////
		{
			switch(interract.menu)
			{
			case 9: // Liste des ventes
				SendVendorItems(Plr);
				break;
			case 11: // Achète un item
				BuyVendorItem(Plr,interract);
				break;
			case 14: // Vend un item
				SellVendorItem(Plr,interract);
				break;
			case 36: // Achète un item du buyback
				BuyBackVendorItem(Plr,interract);
				break;
			default:
				{
			npc_text="Wot ya want?";

			uint16 responseSize = 10+npc_text.length();
			Buffer* b =  Plr->Allocate(responseSize+3);
			b->write<uint16>(responseSize);
			b->write<uint8>(F_INTERACT_RESPONSE);
			b->write<uint8>(0x00);
			b->write<uint16>(GetOid());
			b->write<uint16>(0x0000);

			b->write<uint8>(0x40); // Dye
			b->write<uint8>(0x22); // Vendors

			b->write<uint8>(0);
			b->write<uint8>(npc_text.length());
			b->write(reinterpret_cast<const uint8*>(npc_text.c_str()),npc_text.length());
			b->write<uint8>(0x00);
			Plr->sendPacket(b);

			sEventHook.ExecuteHook(EVENTS_INTERACT_DYE_MERCHANT,2,Plr,this);
				}break;
			};
		}break;

	case GameData::INTERACTTYPE_TRAINER:	//////////////////////////////////////////////
		{
			switch(interract.menu)
			{
			case 7: // Liste des sorts
				{
					uint16 responseSize = 5;
					Buffer* b =  Plr->Allocate(responseSize+3);
					b->write<uint16>(responseSize);
					b->write<uint8>(F_INTERACT_RESPONSE);
					b->write<uint16>(0x0529);
					b->write<uint8>(0x06);
					b->write<uint16>(0x00);
					Plr->sendPacket(b);
					sEventHook.ExecuteHook(EVENTS_INTERACT_SHOW_TRAINING,2,Plr,this);
				}break;

			default:
				{
				npc_text="Wot ya want?";
				uint16 responseSize = 10+npc_text.length();
				Buffer* b =  Plr->Allocate(responseSize+3);
				b->write<uint16>(responseSize);
				b->write<uint8>(F_INTERACT_RESPONSE);
				b->write<uint8>(0x00);
				b->write<uint16>(GetOid());
				b->fill(0,3);
				b->write<uint8>(0x21);
				b->write<uint16>(npc_text.length());
				b->write(reinterpret_cast<const uint8*>(npc_text.c_str()),npc_text.length());
				b->write<uint8>(0x06);
				Plr->sendPacket(b);
				}
				break;
			};

		}break;

	case GameData::INTERACTTYPE_FLIGHT_MASTER: //////////////////////////////////////////////
		{
		Plr->CheckZoneAcces(this);

		Buffer *b = new Buffer(0);
		b->write<uint16>(0x0A12);
		Plr->BuildZoneAcces(b);

		uint8 data[62] =
		{
			0x01,0xF4,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0x42,0x39,0x00,0x00,0x00,0xC0,0xE3,
			0x03,0x39,0xA0,0xD1,0x6F,0x00,0xC8,0xA8,0x1D,0x37,0x28,0x94,0x79,0x33,0xB2,0x24,
			0x32,0x44,0xDB,0xD7,0x1C,0x5D,0x18,0x5D,0xDD,0x1C,0xA4,0x0D,0x00,0x00,0xA8,0x6B,
			0x21,0x36,0x11,0x00,0x00,0x00,0xC8,0xD0,0xAF,0x3A,0x78,0xD1,0x6F,0x00
		};

		b->write(data,62);

		uint16 responseSize = b->size();
		Buffer *d =  Plr->Allocate(responseSize+3);
		d->write<uint16>(responseSize);
		d->write<uint8>(F_INTERACT_RESPONSE);
		d->write(b);
		d->Trace(3);
		Plr->sendPacket(d);
		delete b;

		sEventHook.ExecuteHook(EVENTS_INTERACT_SHOW_FLIGHTMASTER,2,Plr,this);
		}break;

	default:
		questmgr.BuildInterractQuest(this,Plr);
		break;

	};
}