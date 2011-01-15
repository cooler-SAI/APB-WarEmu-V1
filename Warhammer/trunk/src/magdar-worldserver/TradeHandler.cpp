#include "StdAfx.h"


TradeInfo::TradeInfo(Player * owner,Player * invited)
{
	Log.Debug("TradeInfo","New Trade !");
	Owner = owner;
	Invited = invited;

	for(int i=0;i<2;++i)
	{
		for(int j=0;j<MAXTRADESLOT;j++)
			m_itemstrade[i][j]=NULL;
		
		m_money[i]=0;
		m_accept[i] = 1;
	}
	changed = false;
	close=false;
	oktotrade[0]=false;
	oktotrade[1]=false;

	Owner->Trade = this;
	Invited->Trade = this;

	Update();
}
void TradeInfo::SendTradeInfo(Player * Plr)
{
	if(Plr == NULL) return;

	uint32 slot=GetPlayerSlot(Plr);

	if(slot>=2) return;

	Log.Debug("TradeInfo","SendTradeInfo to %s",Plr->GetName().c_str());
	for(int i=0;i<2;++i)
	{

		if(i==slot && m_accept[i]==2)
			continue;

		uint16 responseSize = 28;
		Buffer* b = Plr->GetCom()->m_bufferPool->allocateBuffer(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_TRADE_STATUS);
		b->write<uint8>(m_accept[i]);
		b->write<uint8>(0x00);
		
		if(i==slot)
			b->write<uint16>(0x0000);
		else 
			b->write<uint16>( slot==OWNER ? Invited->GetOid() : Owner->GetOid());
		
		if(m_accept[i]==2)
			b->fill(0,24);
		else
		{
			b->write<uint32>(m_money[i]);
			b->write<uint8>(m_update[i]);

			uint8 counts=0;
			for(int j=0;j<MAXTRADESLOT;j++)
			{
				if(m_itemstrade[i][j]!=NULL)
					counts++;
			}

			b->write<uint8>(counts);

			for(int j=0;j<MAXTRADESLOT;j++)
			{
				if(m_itemstrade[i][j]!=NULL)
					b->write<uint16>(m_itemstrade[i][j]->GetSlot());
			}

			while(counts<MAXTRADESLOT)
			{
				b->write<uint16>(0x00);
				counts++;
			}
		}
		Plr->sendPacket(b);
	}
}
void TradeInfo::Update()
{
	Log.Debug("TradeInfo","Update");

	SendTradeInfo(Owner);
	SendTradeInfo(Invited);
	changed=false;
}
void TradeInfo::AddItemToSlot(Player * Plr,Item * itm,uint16 tradeslot)
{
	if(!itm || Plr == NULL)
		return;

	uint32 slot=GetPlayerSlot(Plr);

	if(slot>=2) return;

	m_itemstrade[slot][tradeslot]=itm;

	Log.Debug("TradeInfo","AddItemToSlot[%s]->%u",itm->GetProto()->name,tradeslot);
	itm->SetTradeSlot(tradeslot);

	SendItemInfo();
	changed=true;
	oktotrade[slot]=false;
}
void TradeInfo::SendItemInfo()
{
	for(int j=0;j<2;j++)
	{
		Player * Plr = NULL;

		if(j==OWNER) Plr = Owner;
		else if(j==INVITED) Plr = Invited;

		if(Plr == NULL) continue;

		Log.Debug("TradeInfo","Sending All Item Trading to %s",Plr->GetName().c_str());
		
		for(int k=0;k<2;k++)
		{
			for(uint16 i=0;i<MAXTRADESLOT;++i)
			{
				Item * itm = m_itemstrade[k][i];
				if(itm==NULL) continue;

				Buffer *b  = new Buffer(0);

				// Calcul du slot de l'item
				uint16 tradeslot=0;
				if(Plr == itm->GetOwner())	tradeslot=START_TRADE_SLOT;
				else						tradeslot=MIDLE_TRADE_SLOT;
				tradeslot+=i;

				b->write<uint16>(tradeslot);
				ItemsInterface::BuildItemInfo(b,itm->GetProto(),itm->GetCount(),itm,false);

				uint16 responseSize=4+b->size();
				Buffer *d =  new Buffer(responseSize+3);
				d->write<uint16>(responseSize);
				d->write<uint8>(F_GET_ITEM);
				d->write<uint8>(0x01);
				d->fill(0,3);
				d->write(b);
				Plr->sendPacket(d);

				Plr->GetCom()->m_bufferPool->disposeBuffer(b);
			}
		}

	}
}
void TradeInfo::RemoveItemFromSlot(Player * Plr,uint16 tradeslot)
{
	if(Plr == NULL) return;

	uint32 slot=GetPlayerSlot(Plr);

	if(slot>=2) return;

	Item * itm = m_itemstrade[slot][tradeslot];
	if(itm) 
	{
		Log.Debug("TradeInfo","RemoveItemFromSlot = %u",tradeslot);

		Plr->GetItemInterface()->SendItem(NULL,tradeslot+START_TRADE_SLOT);
		if(Plr == Owner) Invited->GetItemInterface()->SendItem(NULL,tradeslot+MIDLE_TRADE_SLOT);
		else Owner->GetItemInterface()->SendItem(NULL,tradeslot+MIDLE_TRADE_SLOT);
		
		itm->SetTradeSlot(0);
		m_itemstrade[slot][tradeslot] = NULL;
		SendItemInfo();
		changed=true;
		oktotrade[slot]=false;
	}
}
void TradeInfo::TradeItems()
{
	Log.Debug("TradeInfo","TradeItems !!!!");

	map< Player*,list<Item*> > m_items;
	map< Player*,list<Item*> >::iterator itr;
	list<Item*>::iterator itrr;

	//TEXT_TRADE_ERR_INSUFFICIENT_MONEY
	//TEXT_OVERAGE_PARTNER_CANT_TRADE
	//TEXT_OVERAGE_CANT_TRADE
	// Vérification des place+argent

	uint32 counts[2] = { 0, 0 };
	for(int p=0;p<2;p++)
	{
		uint32 togive=m_money[p];

		if(p==OWNER)
		{
			if(!Owner->RemoveWealth(togive))
			{
				Owner->sendLocalizedString("",TEXT_TRADE_ERR_INSUFFICIENT_MONEY);
				return;
			}
		}
		else if(p==INVITED)
		{
			if(!Invited->RemoveWealth(togive))
			{
				Invited->sendLocalizedString("",TEXT_TRADE_ERR_INSUFFICIENT_MONEY);
				return;
			}
		}

		for(int i=0;i<MAXTRADESLOT;++i) // Chaque Items
		{
			if(m_itemstrade[p][i]!=NULL)
			{
				counts[p]++;
			}
		}
	}

	for(int p=0;p<2;p++)
	{
		for(int j=0;j<2;j++)
		{
			if(j==p) continue;

			if(p==OWNER)
			{
				if(counts[p] > Invited->GetItemInterface()->GetFreeSlot() )
				{
					Invited->sendLocalizedString("",TEXT_OVERAGE_CANT_TRADE);
					Owner->sendLocalizedString("",TEXT_OVERAGE_PARTNER_CANT_TRADE);
					return;
				}
			}
			else if(p==INVITED)
			{
				if(counts[p] > Owner->GetItemInterface()->GetCountSlot() )
				{
					Invited->sendLocalizedString("",TEXT_OVERAGE_PARTNER_CANT_TRADE);
					Owner->sendLocalizedString("",TEXT_OVERAGE_CANT_TRADE);
					return;
				}
			}
		}
	}

	for(int p=0;p<2;p++) // chaque slot
	{
		uint32 togive=m_money[p];

		for(int j=0;j<2;j++)
		{
			if(j==p) continue;

			if(j==OWNER) Owner->AddWealth( togive );
			else if(j==INVITED) Invited->AddWealth( togive );
		}

		for(int i=0;i<MAXTRADESLOT;++i) // Chaque Items
		{
			if(m_itemstrade[p][i]!=NULL)
			{
				Item * itm = m_itemstrade[p][i];
				Player * Toget = itm->GetOwner();
				Toget->GetItemInterface()->RemoveItemInSlot(itm->GetSlot());

				if(Toget==Owner) m_items[Invited].push_back(itm);
				else m_items[Owner].push_back(itm);
			}
		}
	}

	for(itr=m_items.begin();itr!=m_items.end();++itr)
	{
		for(itrr=itr->second.begin();itrr!=itr->second.end();itrr++)
		{
			uint16 freeslot = itr->first->GetItemInterface()->GetFreeSlot();
			(*itrr)->SetSlot(0);
			itr->first->GetItemInterface()->AddToSlot((*itrr),freeslot);
		}
	}
}
bool TradeInfo::HasItemFromSlot(Player * Plr,uint16 playerslot)
{
	if(Plr == NULL) return true;

	uint32 slot=GetPlayerSlot(Plr);

	if(slot>=2) return true;

	for(int i=0;i<MAXTRADESLOT;++i)
	{
		if(!m_itemstrade[slot][i]) continue;

		if(m_itemstrade[slot][i]->GetSlot() == playerslot)
			return true;
	}

	return false;
}
void TradeInfo::SetMoney(Player * Plr,uint32 money)
{
	if(Plr == NULL) return;

	uint32 slot=2;
	if(Plr == Owner)		slot=OWNER;
	else if(Plr == Invited)	slot=INVITED;

	if(slot>=2) return;

	Log.Debug("TradeInfo","SetMoney = %u",money);

	if(money > Plr->GetWealth())
	{
		Buffer *b = Plr->GetCom()->m_bufferPool->allocateBuffer(10);
		b->write<uint16>(10);
		b->write<uint8>(F_LOCALIZED_STRING);
		uint8 data[10] =
		{
			0x02,0x20,0x00,0x00,0x03,0x3C,0x00,0x00,0x00,0x00
		};
		b->write(data,10);
		Plr->sendPacket(b);
		money=0;
	}

	uint32 ancienmoney = m_money[slot];
	m_money[slot] = money;

	if(ancienmoney != money)
	{
		oktotrade[slot]=false;
		changed=true;
	}
}
void TradeInfo::SetAccept(Player * Plr,uint8 accept)
{
	if(Plr == NULL) return;

	uint32 slot=2;
	if(Plr == Owner)		slot=OWNER;
	else if(Plr == Invited)	slot=INVITED;

	if(slot>=2) return;

	Log.Debug("TradeInfo","SetStatus = %u",accept);
	uint8 ancienslot = m_accept[slot];
	m_accept[slot] = accept;

	if(accept==3)
	{
		Log.Debug("ClientCom","Status Closing !");
		close=true;
		return;
	}

	if(accept==2)
		oktotrade[slot]=true;
	else if(accept==1)
		oktotrade[slot]=false;

	if(ancienslot!=accept)
	{
		changed=true;
	}
}
void TradeInfo::SendDistanceMessage(Player * Plr)
{
	if(Plr == NULL) return;

	Log.Debug("ClientCom","SendDistanceMessage");
	Buffer *b = Plr->GetCom()->m_bufferPool->allocateBuffer(10);
	b->write<uint16>(10);
	b->write<uint8>(F_LOCALIZED_STRING);
	uint8 data[10] =
	{
		0x02,0x20,0x00,0x00,0x03,0x38,0x00,0x00,0x00,0x00
	};
	b->write(data,10);
	Plr->sendPacket(b);
}
void TradeInfo::CheckDistance()
{
	float distance = Owner->GetDistance(Invited);
	if(distance > 15.0f)
	{
		Log.Debug("ClientCom","Distance too far, Closing !");
		SendDistanceMessage(Owner);
		SendDistanceMessage(Invited);
		close=true;
	}
}
void TradeInfo::SendTradeClose(Player * Plr,uint16 oid)
{
	Log.Debug("ClientCom","SendTradeClose");
	Buffer *b = Plr->GetCom()->m_bufferPool->allocateBuffer(28+3);
	b->write<uint16>(28);
	b->write<uint8>(F_TRADE_STATUS);
	b->write<uint8>(0x03);
	b->write<uint8>(0x00);
	b->write<uint16>(oid);
	b->fill(0,24);
	Plr->sendPacket(b);
}
void TradeInfo::SendAlreadyTrade(Player * Plr)
{
	Log.Debug("ClientCom","SendAlreadyTrade");
	Buffer *b = Plr->GetCom()->m_bufferPool->allocateBuffer(10);
	b->write<uint16>(10);
	b->write<uint8>(F_LOCALIZED_STRING);
	uint8 data[10] =
	{
		0x02,0x20,0x00,0x00,0x03,0x37,0x00,0x00,0x00,0x00
	};
	b->write(data,10);
	Plr->sendPacket(b);
}
void ClientCom::onTradeStatus()
{
	if(!m_Plr) return;
	
	Log.Debug("ClientCom","onTradeStatus %s",m_Plr->GetName().c_str());

	uint8 status=0;
	uint8 unk=0;
	uint16 toid=0;
	m_readBuffer >> status >> unk >> toid;

	if(status==5 || status==4)
		return;

	if(!toid)
	{
		Log.Debug("TradeInfo","NO TARGET !");
		TradeInfo::SendTradeClose(m_Plr,toid);
		return;
	}

	Player * target = objmgr.GetPlayerByOid(toid);
	if(target==NULL && m_Plr->Trade==NULL)
	{
		TradeInfo::SendTradeClose(m_Plr,toid);
		return;
	}

	if(m_Plr->Trade && target == NULL)
	{
		if(m_Plr->Trade->Owner == m_Plr) target = m_Plr->Trade->Invited;
		else target = m_Plr->Trade->Owner;
	}

	if(target==NULL || target && target==m_Plr)
	{
		TradeInfo::SendTradeClose(m_Plr,toid);
		return;
	}

	if(target->Trade && target->Trade != m_Plr->Trade)
	{
		Log.Debug("TradeInfo","Already in Trade !");
		TradeInfo::SendAlreadyTrade(m_Plr);
		TradeInfo::SendTradeClose(m_Plr,target->GetOid());
		return;
	}

	if(m_Plr->GetDistance(target) > 15.0f)
	{
		Log.Debug("ClientCom","Too FAR !!");
		TradeInfo::SendDistanceMessage(m_Plr);
		TradeInfo::SendTradeClose(m_Plr,target->GetOid());
		return;
	}
	if(m_Plr->Trade && m_Plr->Trade->close)
		return;

	TradeInfo * trad = NULL;
	if(m_Plr->Trade && !m_Plr->Trade->close) trad = m_Plr->Trade;
	else trad = new TradeInfo(m_Plr,target);

	if(!trad) return;

	trad->Lock();

	uint32 money=0;;
	uint8 update=0;
	uint8 itemcounts=0;
	uint16 items[MAXTRADESLOT];
	m_readBuffer >> money >> update >> itemcounts;

	for(int i=0;i<MAXTRADESLOT;++i)
		m_readBuffer >> items[i];

	/*if(!update)
	{
		Log.Debug("TradeInfo","!update");
		trad->UnLock();
		return;
	}*/

	trad->SetAccept(m_Plr,status);
	trad->SetUpdate(m_Plr,update);

	if(trad->oktotrade[0] && trad->oktotrade[1])
	{
		Log.Debug("TradeInfo","Trade Accepted !!");
		trad->TradeItems();
		trad->close=false;
		trad->changed=false;
		trad->SendTradeClose(trad->Owner,trad->Invited->GetOid());
		trad->SendTradeClose(trad->Invited,trad->Owner->GetOid());
		trad->Owner->Trade=NULL;
		trad->Invited->Trade=NULL;
		trad->Destroy();
		return;
	}

	if(status!=2)
		trad->SetMoney(m_Plr,money);

	for(int i=0;i<MAXTRADESLOT;++i)
	{
		if(!items[i] && status!=2) 
		{
			trad->RemoveItemFromSlot(m_Plr,i);
			continue;
		}

		Item * itm = m_Plr->GetItemInterface()->GetItemInSlot(items[i]);
		if(!itm)
		{
			trad->RemoveItemFromSlot(m_Plr,i);
			continue;
		}

		if(itm->GetProto()->bind)
		{
			trad->RemoveItemFromSlot(m_Plr,i);
			m_Plr->sendLocalizedString("",TEXT_TRADE_ERR_OBJECT_CANT_BE_TRADED);
			continue;
		}

		// Si le joueur n'a pas d'item déja dans le slot i , on ajoute celui la
		if(!trad->HasItemFromSlot(m_Plr,items[i]))
			trad->AddItemToSlot(m_Plr,itm,i);
	}

	trad->UnLock();
}
void TradeInfo::Destroy()
{
	Log.Debug("ClientCom","Destroy");
	if(Owner && Invited)
	{
		Log.Debug("ClientCom","Destroy Owner");
		SendTradeClose(Owner,Invited->GetOid());
		Owner->Trade=NULL;
		//Owner->GetItemInterface()->SendAllItem();
	}
	if(Invited && Owner)
	{
		Log.Debug("ClientCom","Destroy Invited");
		SendTradeClose(Invited,Owner->GetOid());
		Invited->Trade=NULL;
		//Invited->GetItemInterface()->SendAllItem();
	}
	close=false;
	changed=false;
	UnLock();
	delete this;
}