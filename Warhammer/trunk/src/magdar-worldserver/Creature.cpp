#include "StdAfx.h"

//Vendors
VendorItem::VendorItem(const ItemProto * proto,uint32 _cost, string reqitem) : m_proto(proto), cost(_cost) 
{
	if(reqitem.size())
	{
		// (1,25155)(5,18548)
		int a = reqitem.find("(");
		while(a != string::npos)
		{
			int b = reqitem.find(")");
			if(b != string::npos)
			{
				string req = reqitem.substr(a+1,b);
				// 1,25155
				int c = req.find(",");
				uint16 count = atoi( req.substr( 0,c ).c_str() ); // 1
				uint32 entry = atoi( req.substr( c+1,req.size() ).c_str() ); // 25155

				const ItemProto * Proto = objmgr.GetItemProto(entry);
				if(Proto)
					m_reqitem.insert( make_pair(count,Proto) );

				reqitem.erase(0,b+1);
			}
			a = reqitem.find("(");
		}
	}
}
bool VendorItemData::RemoveItem( uint32 item_id )
{
    for(vector<VendorItem*>::iterator i = m_items.begin(); i != m_items.end(); ++i )
    {
		if((*i)->m_proto->entry==item_id)
        {
            m_items.erase(i);
            return true;
        }
    }
    return false;
}

size_t VendorItemData::FindItemSlot(uint32 item_id) const
{
    for(size_t i = 0; i < m_items.size(); ++i )
		if(m_items[i]->m_proto->entry==item_id)
            return i;
    return m_items.size();
}

VendorItem const* VendorItemData::FindItem(uint32 item_id) const
{
    for(vector<VendorItem*>::const_iterator i = m_items.begin(); i != m_items.end(); ++i )
		if((*i)->m_proto->entry==item_id)
            return *i;
    return NULL;
}

//
VendorItemData const* Creature::GetVendorItems()
{
		return objmgr.GetNpcVendorItemList(GetEntry());
}
CreatureEquipement const* Creature::GetEquipedItems()
{
	return objmgr.GetNpcEquipement(GetEntry());
}
CreatureLoot const* Creature::GetCreatureLoot()
{
	return objmgr.GetCreatureLoot(GetEntry());
}
bool Creature::Load()
{
	if( !m_entry || !m_cs) return false;

	const CreatureProto * Info = objmgr.GetCreatureProto( m_entry );
	const CreatureSpawn * sp = m_cs;
	if( !Info ) return false;

	m_guid = m_cs->guid;
	m_name = Info->name;
	m_zone = m_cs->zone;
	m_region = objmgr.GetRegionByZone(m_zone);

	m_worldposition.ChangeCoords(m_cs->WorldX,m_cs->WorldY,m_cs->WorldZ,m_cs->WorldO);
	CalculPos();
	CalculOffset();

	m_speed=100;

	if(Info->level[0] != Info->level[1] && Info->level[1] > Info->level[0])
		m_level = Info->level[0]+RandomUInt(Info->level[1]-Info->level[0]);
	else m_level = Info->level[1];

	string bytes = m_cs->bytes;
	int z = bytes.find(";");
	while( z != string::npos )
	{
		m_bytes.push_back(strtoul(bytes.substr(0,z).c_str(),0,16));
		bytes.erase(0,z+1);
		
		z = bytes.find(";");
	}

	RemoveByte(3); // dead
	RemoveByte(5); // Questgiver

	if( Info->displayid[0] && Info->displayid[1])
		m_model = Info->displayid[RandomUInt(1)];
	else m_model = Info->displayid[0];

	if(Info->scale[0] != Info->scale[1] && Info->scale[1] > Info->scale[0])
		m_scale = Info->scale[0]+RandomUInt(Info->scale[1]-Info->scale[0]);
	else m_scale = Info->scale[1];

	m_faction = Info->faction;
	uint8 faction = m_faction;

	while( faction >= 8 )
		faction-=8;

	if(faction == 2 || faction == 3) m_Ranklevel=1;
	else if(faction == 4 || faction==5) m_Ranklevel=2;
	else if(faction == 6 || faction==7) m_Ranklevel=3;

	m_cp = Info;
	m_cs = sp;

	m_nextobjectstate=getMSTime()+UPDATE_GRAPHICAL_INTERVAL;

	m_equipement = GetEquipedItems();
	m_health = objmgr.GenerateHealth(this);
	m_maxhealth = m_health;

	m_interract = objmgr.GetInterractType(m_cs->title);
	m_aiInterface->Init(this);

	SetQuestList(questmgr.GetCreatureQuestList(m_cp->entry));
	SetQuestFinish(questmgr.GetCreatureQuestFinisher(m_cp->entry));

	m_ScriptName = std::string(m_cp->scriptname);

	if(m_ScriptName != "")
	{
		m_script = scriptmgr.GetAiScript(m_ScriptName,this);
	}

	return true;
}
Unit * Creature::GetTarget()
{
	return m_aiInterface->GetTarget();
}
void Creature::Reload()
{
	/*if(!m_guid || !m_entry) return;
	CreatureSpawn * Sp = objmgr.GetCreatureSpawn(m_guid);
	if(!Sp) return;

	Load(Sp);*/
}
void Creature::SendEquiped(Player * Plr)
{
	if( !Plr ) return;

	if(!m_equipement) return;

	uint16 responseSize = 5;

	for(int i=0;i<m_equipement->GetItemCount();++i)
	{
		EquipedItem * Equi = m_equipement->GetItem(i);
		if(!Equi) continue;

		responseSize+=5;
		if(Equi->effect)
			responseSize+=4;
	}

	Buffer* b = Plr->Allocate(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_PLAYER_INVENTORY);

	b->write<uint16>(GetOid());
	b->write<uint16>(m_equipement->GetItemCount());

	for(int i=0;i<m_equipement->GetItemCount();++i)
	{
		EquipedItem * Equi = m_equipement->GetItem(i);
		if(!Equi) continue;

		b->write<uint8>( Equi->effect ? 1 : 0 );
		b->write<uint16>( Equi->slotid );
		b->write<uint16>( Equi->modelid );
		if( Equi->effect )
			b->write<uint32>( Equi->effect );
	}
	b->write<uint8>(0);
	Plr->sendPacket(b);
}
void Creature::SendMeTo(Player * Plr)
{
		if(!Plr ) return;
		//Log.Debug("Creature","%s SendMeTo %s",GetName().c_str(),Plr->GetName().c_str());
		
		const CreatureProto * cp = GetCreatureProto();
		const CreatureSpawn * cs = GetCreatureSpawn();

		if(!cs || !cp ) return;

		list<uint8> tmpbytes;
		if(HasQuestDoneForPlayer(Plr))
			tmpbytes.push_back(4);
		else if(HasQuestForPlayer(Plr))
			tmpbytes.push_back(5);
		else if(HasQuestToDoneForPlayer(Plr))
			tmpbytes.push_back(7);



		uint16 responseSize = 114+m_bytes.size()+GetName().length()+tmpbytes.size();
		Buffer* b = Plr->Allocate(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_CREATE_MONSTER);
		b->write<uint16>(GetOid());
		b->write<uint16>(0);

		b->write<uint16>(GetWorldO());
		b->write<uint16>(GetWorldZ());
		b->write<uint32>(GetWorldX());
		b->write<uint32>(GetWorldY());
		b->write<uint16>(0x0000); // SpeedZ
		// 18
		b->write<uint16>(m_model);
		b->write<uint8>(m_scale);
		b->write<uint8>(m_level);
		b->write<uint8>(cp->faction);

		b->fill(0,4);
		b->write<uint8>(cs->emote);
		b->fill(0,2);
		b->write<uint8>(cs->Unk[0]);
		b->write<uint8>(0x00);
		b->write<uint16>(cs->Unk[1]);
		b->write<uint16>(cs->Unk[2]);
		b->write<uint16>(cs->Unk[3]);
		b->write<uint16>(cs->Unk[4]);
		b->write<uint16>(cs->Unk[5]);
		b->write<uint16>(cs->title);
		b->write<uint8>( m_bytes.size()+tmpbytes.size() ); // Count
		//45
		for(list<uint8>::iterator itr=m_bytes.begin();itr!=m_bytes.end();++itr)
			b->write<uint8>( (*itr) );

		for(list<uint8>::iterator itr=tmpbytes.begin();itr!=tmpbytes.end();++itr)
			b->write<uint8>( (*itr) );

		b->write<uint8>(0x00);

		b->write(reinterpret_cast<const uint8*>(GetName().c_str()),GetName().length());

		b->write<uint8>(0);//0x00
		b->write<uint8>(0x01);//0x00
		b->write<uint8>(0x0A);//0x00

		b->write<uint8>(0);//0x00
		//50
		b->write<uint16>(0);//0x00
		b->write<uint8>(cs->icone);//
		b->write<uint8>(cs->Unk1);//

		b->write<uint8>(0x00);//
		//55
		b->fill(0,8); // flags
		//63

		b->write<uint8>(100);// Health

		b->write<uint16>(GetZoneId());

		b->fill(0,4);

		b->fill(0,8);
		//78
		b->write<uint8>(0x00);//0x04 ? MP ? HP
		b->write<uint8>(0x00);//0x58
		b->write<uint8>(0x00);//0x00
		b->fill(0,33); // Equipmodel
		//114
		Plr->sendPacket(b);
		SendObjectState(Plr);
}
void Creature::SendText(string text,ChatLogFilters type)
{
	if(HasInRangePlayers())
	{
		unordered_set<Player*  >::iterator itr;
		for(itr=GetInRangePlayerSetBegin();itr!=GetInRangePlayerSetEnd();++itr)
		{
			(*itr)->sendStringMessage(GetOid(),GetName(),text,type);
		}
	}
}
void Creature::SendObjectState(Player * Plr)
{
	if(!IsInWorld())
		return;

	m_nextobjectstate = getMSTime()+UPDATE_GRAPHICAL_INTERVAL;

		uint16 x = CalcPinX();
		uint16 y = CalcPinY();
		uint16 z = GetWorldZ();
		uint16 o = GetWorldO();

		uint8 r1 = o &0xFF;
		uint8 r2 = o >>8;

		if(Plr)
		{
			Buffer * b =Plr->Allocate(18+3);
			b->write<uint16>(18);
			b->write<uint8>(F_OBJECT_STATE);
			b->write<uint16>(GetOid());
			b->write<uint16>( x ); // X = PinX-32000..
			b->write<uint16>( y ); // Y
			b->write<uint16>( z ); // Z
			b->write<uint8>(GetHealthPct()); // VIE
			b->write<uint16>(GetZoneId());
			b->fill(0,5);
			b->write<uint8>( r1 ); 
			b->write<uint8>( r2 );
			Plr->sendPacket(b);
			return;
		}

	Plr=NULL;
	if(HasInRangePlayers())
	{  
		for(unordered_set<Player*  >::iterator itr=GetInRangePlayerSetBegin();itr!=GetInRangePlayerSetEnd();++itr)
		{
			Plr = (*itr);

			if( (*itr) == NULL ) continue;

			if( !(*itr)->IsInRangeSet(this) )
				continue;
			if( (*itr)->GetDistance( this ) > MAX_VISIBILITY_RANGE)
				continue;

			uint16 responseSize = 18;
			bool ismove = false;//IsMoving();
			if(ismove)
				responseSize+=11;

			Buffer * b =(*itr)->Allocate(responseSize+3);
			b->write<uint16>(responseSize);
			b->write<uint8>(F_OBJECT_STATE);
			b->write<uint16>(GetOid());
			if(ismove)
			{
				Log.Debug("Creature","Sending ObjectStat Moving");
				b->write<uint16>( x ); // X = PinX-32000..
				b->write<uint16>( y ); // Y
				b->write<uint16>( z ); // Z
				
				b->write<uint32>(0x64036A00); // unk1 -- SMSG_MONSTER_MOVE Authentifie le packet en tant que déplacement de créature ?
				b->fill(0, 7);

				b->write<uint8>(0x50); // Rapport à l'orientation ?

				b->write<uint16>(uint16(GetAIInterface()->m_dest.x));
				b->write<uint16>(uint16(GetAIInterface()->m_dest.y));
				
				b->write<uint16>(0x1B6A); // unk2 Z ?

				b->write<uint8>(0xA9); // unk3
				b->write<uint8>(0x08); // unk4 -- Changer cette valeur ne change rien ??

				b->write<uint8>(0x0A); // unk5 -- Changer cette valeur provoque des problèmes d'orientation et de téléportation sur les monstres, il semblerait qu'elle soit la pour indiquer que le mouvement est fini ou non ?
			}
			else
			{
				b->write<uint16>( x ); // X = PinX-32000..
				b->write<uint16>( y ); // Y
				b->write<uint16>( z ); // Z
				b->write<uint8>(GetHealthPct()); // VIE
				b->write<uint16>(GetZoneId());
				b->fill(0,5);
				b->write<uint8>( r1 ); 
				b->write<uint8>( r2 );
			}
			(*itr)->sendPacket(b);
		}
	}
}
void Creature::Update( uint32 p_time )
{
	Unit::Update( p_time );

	uint32 mstime = getMSTime();

	if(m_nextobjectstate <= mstime)
	{
		SendObjectState();
		m_nextobjectstate = mstime+UPDATE_GRAPHICAL_INTERVAL;
	}

	//_UpdateMovement();

	if(GetAIInterface())
		GetAIInterface()->Update(p_time);

	/*if(IsTotem() && isDead())
	{
		RemoveFromWorld(false, true);
		Destructor();
		return;
	}

	if(m_corpseEvent)
	{
		sEventMgr.RemoveEvents(shared_from_this());
		if(proto==NULL)
			sEventMgr.AddEvent(TO_CREATURE(shared_from_this()), &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else if (creature_info->Rank == ELITE_WORLDBOSS)
			sEventMgr.AddEvent(TO_CREATURE(shared_from_this()), &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_BOSSCORPSE, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else if ( creature_info->Rank == ELITE_RAREELITE || creature_info->Rank == ELITE_RARE)
			sEventMgr.AddEvent(TO_CREATURE(shared_from_this()), &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_RARECORPSE, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else
			sEventMgr.AddEvent(TO_CREATURE(shared_from_this()), &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_CORPSE, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		m_corpseEvent=false;
	}*/
}
void Creature::SendVendorItems(Player * Plr)
{
	if(!Plr) return;


	VendorItemData const* vItems = GetVendorItems();

    if (!vItems)
    {
		Log.Error("Creature","No vendors found !");
    }
	else
	{
		uint32 size = vItems->GetItemCount();

		Log.Debug("Creature","SendVendors Items size=%u",size);

		uint32 counts=0;
		uint8 page=0;
		while(counts < size)
		{
			uint32 diff = size-counts;
			if(diff > MAX_ITEM_PAGE)
				diff=MAX_ITEM_PAGE;

			SendVendorItemsList(Plr,vItems,counts,counts+diff,page);
			counts+=diff;
			page++;
		}
	}
	Plr->SendBuyBack();

	sEventHook.ExecuteHook(EVENTS_INTERACT_SHOW_STORE,2,Plr,this);
}
void Creature::SendVendorItemsList(Player * Plr,VendorItemData const* vItems,uint32 depart,uint32 fin,uint8 page)
{
	if(!fin) return;
	if(depart==fin || fin < depart) return;

	uint8 size = fin-depart;

	list<Buffer*> m_buffers;

	uint16 responseSize=7;
	uint32 counts=0;

	for(int i=depart;i<fin;++i)
	{
		VendorItem * Itm = vItems->GetItem(i);
		if(!Itm) continue;

		const ItemProto * proto = Itm->m_proto;
		if(!proto) continue;

		Buffer *d = new Buffer(300); //TODO -FIX THIS SHIT

		if( Itm->cost < proto->sellprice) //TODO ; vérifier avant
			Itm->cost = proto->sellprice*3;

		d->write<uint8>(counts);
		d->write<uint8>(0x03);
		d->write<uint32>( Itm->cost );
		sChat.BuildItemInfo(d, proto );

		d->write<uint8>(Itm->m_reqitem.size()); // Items required
		for(map<uint16,const ItemProto*>::iterator itr=Itm->m_reqitem.begin();itr!=Itm->m_reqitem.end();++itr)
		{
			d->write<uint32>(itr->second->entry);
			d->write<uint16>(itr->second->modelid);
			d->writeBytePascalString(string(itr->second->name));
			d->write<uint16>(itr->first);
			d->fill(0,18);
			break;
		}

		/*
		if(01)
		32(entry);
		32(modelid);
		name size
		name
		00
		01
		fill 18
		*/

		responseSize+=d->size();
		m_buffers.push_back(d);
		counts++;
	}

	Buffer* b = Plr->Allocate(responseSize+3);
	b->resize(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_INIT_STORE);

	b->write<uint8>(0x00);
	b->write<uint8>(page);
	b->write<uint8>(size);
	b->write<uint8>(page ? 0x00 : 0x01);
	b->write<uint8>(0x01);
	b->write<uint8>(0x00);

	for(list<Buffer*>::iterator itr=m_buffers.begin();itr!=m_buffers.end();++itr)
		b->write( (*itr) );

	//b->Trace();
	b->write<uint8>(0x00);
	Plr->sendPacket(b);

	for(list<Buffer*>::iterator itr=m_buffers.begin();itr!=m_buffers.end();++itr)
		delete (*itr);

	m_buffers.clear();
}
void Creature::BuyVendorItem(Player * Plr,Struct_f_interract interract)
{
	if(!Plr) return;

	uint32 num = interract.page*MAX_ITEM_PAGE+interract.itemnum;
	

    VendorItemData const* vItems = GetVendorItems();

    if(!vItems || vItems->Empty())
    {
        return;
    }

	VendorItem * Itm = vItems->GetItem(num);
	if(!Itm) return;

	const ItemProto *Proto = Itm->m_proto;
	if(!Proto)
		return;


	uint16 freeslot = Plr->GetItemInterface()->GetFreeSlot();
	if(!freeslot)
	{
		Plr->sendLocalizedString("",TEXT_MERCHANT_INSUFFICIENT_SPACE_TO_BUY);
		return;
	}

	if( !Plr->RemoveWealth(Itm->cost) )
	{
		// Vous n'avez pas asser d'argent pour acheter
		Plr->sendLocalizedString(string(""),TEXT_MERCHANT_INSUFFICIENT_MONEY_TO_BUY);
		return;
	}
	if(Itm->m_reqitem.size())
	{
		for(map<uint16,const ItemProto*>::iterator itr=Itm->m_reqitem.begin();itr!=Itm->m_reqitem.end();++itr)
		{
			if(Plr->GetItemInterface()->GetItemCount(itr->second->entry) < itr->first)
			{
				Plr->sendLocalizedString(string(""),TEXT_MERCHANT_FAIL_PURCHASE_REQUIREMENT);
				return;
			}
		}
		for(map<uint16,const ItemProto*>::iterator itr=Itm->m_reqitem.begin();itr!=Itm->m_reqitem.end();++itr)
		{
			if( !Plr->GetItemInterface()->RemoveItem(itr->second->entry,itr->first) )
			{
				Plr->sendLocalizedString(string(""),TEXT_MERCHANT_FAIL_PURCHASE_REQUIREMENT);
				return;
			}
		}
	}

	stringstream ss;
	ss << "1";
	stringstream zz;
	zz << Itm->cost;
	uint16 responseSize = 44+ss.str().length()+zz.str().length()+strlen(Proto->name)+GetName().length();
	Buffer *b= Plr->Allocate(responseSize);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_LOCALIZED_STRING);
	b->write<uint16>(0x0220);
	b->write<uint16>(0x0000);
	b->write<uint16>(0x0214);
	b->fill(0,3);
	b->write<uint16>(0x0401);
	b->write<uint16>(strlen(Proto->name)+1);
	b->write(reinterpret_cast<const uint8*>(Proto->name),strlen(Proto->name));
	b->write<uint16>(0x0001);
	b->write<uint16>(GetName().length()+1);
	b->write(reinterpret_cast<const uint8*>(GetName().c_str()),GetName().length());
	b->write<uint16>(0x0001);
	b->write<uint16>(ss.str().length()+1);
	b->write(reinterpret_cast<const uint8*>(ss.str().c_str()),ss.str().length());
	b->fill(0,4);
	b->write<uint16>(0x047E);
	b->fill(0,3);
	b->write<uint16>(0x0201);
	b->write<uint16>(zz.str().length()+1);
	b->write(reinterpret_cast<const uint8*>(zz.str().c_str()),zz.str().length());
	b->fill(0,4);
	b->write<uint16>(0x032A);
	b->fill(0,4); //44
	Plr->sendPacket(b);

	Plr->GetItemInterface()->CreateItem(Proto,1,false);

	sEventHook.ExecuteHook(EVENTS_INTERACT_BUY_ITEM,3,Plr,this,Proto);
}
void Creature::SellVendorItem(Player * Plr,Struct_f_interract interract)
{
	if(!Plr) return;

	uint16 slot = interract.itemnum+(interract.page*256);

	Log.Debug("Creature","Selling To Vendor Item in Slot %u count = %u",slot,interract.selcount);


	uint16 selcount = interract.selcount;

	Item * itm = Plr->GetItemInterface()->GetItemInSlot(slot);
	if(!itm) return;
	if(!itm->GetProto()->sellprice)
	{
		return;
	}

	if(selcount==0)
		selcount=itm->GetCount();

	uint32 price = selcount*itm->GetProto()->sellprice;
	Plr->AddWealth(price);
	const ItemProto * proto = itm->GetProto();

	sEventHook.ExecuteHook(EVENTS_INTERACT_SELL_ITEM,2,Plr,itm);

	if(selcount>=itm->GetCount())
	{
		Plr->GetItemInterface()->RemoveItemInSlot(itm->GetSlot());
		Plr->AddBuyBack(itm);
	}
	else
	{
		// Si on vend une partie de l'item , alors on créé un nouvel item , on le mec au buyback et on reduit l'item actuel
		uint32 guid = objmgr.GenerateItemGuid();
		Item * nitem = new Item(guid,Plr);
		nitem->Load(proto);
		nitem->SetCount(selcount);
		itm->SetCount(itm->GetCount()-selcount);
		Plr->GetItemInterface()->SendItem(itm,itm->GetSlot());
		Plr->AddBuyBack(nitem);
	}
}
void Creature::BuyBackVendorItem(Player * Plr,Struct_f_interract interract)
{
	if(!Plr) return;
	uint16 slot = interract.itemnum+(interract.page*256);

	Log.Debug("Creature","BuyBack To Vendor Item in Slot %u count=%u",slot,interract.selcount);

	uint32 counts=0;
	uint16 selcount = interract.selcount;
	Item * itm = NULL;
	Plr->M_buyback.Acquire();
	for(list<Item*>::iterator itr= Plr->m_buyback.begin();itr!=Plr->m_buyback.end();++itr)
	{
		if(counts == slot)
		{
			itm =(*itr);
			break;
		}
		else counts++;
	}
	Plr->M_buyback.Release();

	if(!itm) return;

	if(selcount==0)
		selcount=itm->GetCount();

	uint16 freeslot = Plr->GetItemInterface()->GetFreeSlot();

	if(!freeslot)
	{
		Plr->sendLocalizedString("",TEXT_OVERAGE_CANT_BUY_ITEMS);
		return;
	}
	if(selcount > itm->GetCount())
		selcount = itm->GetCount();

	uint32 price = itm->CalculBuyBackPrice(selcount);
	if(!Plr->RemoveWealth(price))
	{
		// Vous n'avez pas asser d'argent pour acheter
		Plr->sendLocalizedString(string(""),0x02D0);
		return;
	}

	if(selcount < itm->GetCount())
	{
		Plr->GetItemInterface()->CreateItem(itm->GetProto(),selcount,false);
		itm->SetCount(itm->GetCount()-selcount);
		Plr->SendBuyBack();
	}
	else 
	{
		Plr->GetItemInterface()->AddToSlot(itm,freeslot);
		Plr->RemoveBuyBack(itm);
	}
}
void Creature::SendDyeMenu(Player * Plr,Struct_f_interract interract)
{
	if(!Plr) return;
	Log.Debug("Creature","SendDyeMenu");
	Buffer *b = Plr->Allocate(86+3);
	b->write<uint16>(86);
	b->write<uint8>(F_INTERACT_RESPONSE);
	b->write<uint16>(Plr->GetOid());
	
	uint8 data[84] = 
	{ // Color , price
		0x00,0xBE,0x00,0x00,0x01,0xF4,
		0x00,0xB7,0x00,0x00,0x01,0xF4,
		0x00,0xCF,0x00,0x00,0x03,0xE8,
		0x00,0xCC,0x00,0x00,0x03,0xE8,
		0x00,0xC1,0x00,0x00,0x03,0xE8,
		0x00,0xD0,0x00,0x00,0x07,0xD0,
		0x00,0xCB,0x00,0x00,0x07,0xD0,
		0x00,0xC5,0x00,0x00,0x0B,0xB8,
		0x00,0xC2,0x00,0x00,0x0B,0xB8,
		0x00,0xC3,0x00,0x00,0x0F,0xA0,
		0x00,0xDB,0x00,0x00,0x0F,0xA0,
		0x00,0xC0,0x00,0x00,0x0F,0xA0,
		0x00,0xB3,0x00,0x00,0x13,0x88,
		0x00,0xD7,0x00,0x00,0x13,0x88
	};
	b->write(data,84);
	Plr->sendPacket(b);

}
void Creature::MoveTo(uint16 x,uint16 y,uint16 z)
{

}
void Creature::Save()
{
	Log.Debug("Creature","Save %u",m_cs->guid);

	stringstream ss;
	ss << "INSERT INTO creature_spawns (";
	ss << "guid,entry,zone,WorldX,WorldY,WorldZ,WorldO,";
	ss << "bytes,ico,Unk,emote,Unk1,Unk2,Unk3,Unk4,Unk5,Unk6,title,Flags)";
	ss << " VALUES (";

	ss << "'" << m_cs->guid << "',";
	ss << "'" << m_cs->entry << "',";
	ss << "'" << m_cs->zone << "',";
	ss << "'" << m_cs->WorldX << "',";
	ss << "'" << m_cs->WorldY << "',";
	ss << "'" << m_cs->WorldZ << "',";
	ss << "'" << m_cs->WorldO << "',";
	ss << "'" << m_cs->bytes << "',";
	ss << "'" << m_cs->icone << "',";
	ss << "'" << m_cs->Unk1 << "',";
	ss << "'" << m_cs->emote << "',";

	for(int i=0;i<6;++i)
		ss << "'" << m_cs->Unk[i] << "',";

	ss << "'" << m_cs->title << "',";
	ss << "'" << m_cs->flags << "')";

	WorldDatabase.Execute(ss.str().c_str());
}