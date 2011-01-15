#include "StdAfx.h"

void ClientCom::onUseItem()
{
	Log.Debug("ClientCom","F_USE_ITEM");

}

ItemsInterface::ItemsInterface(Player * Plr) : m_player(Plr) 
{
	m_equiped.SetOwner(Plr);
	m_equiped.m_items.resize(MAX_EQUIPED_SLOT);
	m_equiped.SetMinSlot(0);

	for(int i=0;i<MAX_INVENTORY;++i)
	{
		m_inventory[i].m_items.resize(MAX_INVENTORY_SLOT);
		m_inventory[i].SetOwner(Plr);
		m_inventory[i].SetMinSlot(MAX_INVENTORY_SLOT*i+INVENTORY_START_SLOT);
	}
}
void ItemsInterface::Load()
{
	QueryResult * Result = CharacterDatabase.Query("SELECT guid,character_id,entry,slot_id,counts FROM characters_items WHERE character_id=%u",m_player->GetPlayerID());
	if(Result)
	{
		do{
			Item * Itm = new Item(Result->Fetch()[0].GetUInt32(),m_player);

			if( !Itm->Load(Result->Fetch()) ) continue;

			AddToSlot(Itm,Itm->GetSlot(),false);

		}while(Result->NextRow());
	}
	delete Result;
}
void ItemsInterface::Save()
{
	Log.Debug("ItemsInterface","Saving Items of %s",m_player->GetName().c_str());

	for(int i=0;i<MAX_INVENTORY;++i)
	{
		m_inventory[i].Save();
	}

	m_equiped.Save();
}
void ItemsInterface::HandleMoveItem(uint16 slot,uint16 toslot,uint16 count)
{
	Log.Debug("ItemsInterface","HandleMoveItem %u->%u [%u]",slot,toslot,count);

	if(!slot || !toslot)
		return;

	Item * ToMove = GetItemInSlot(slot);

	// L'item ne fait pas parti de l'inventaire
	if(!ToMove || !ToMove->m_inventory)
		return;

	if(count > ToMove->GetCount())
		count = ToMove->GetCount();

	Log.Debug("ItemsInterface","Moving %s",ToMove->GetProto()->name);

	if(toslot == DELETE_SLOT) // Suppression de l'item
	{
		if(ToMove->m_inventory)
			ToMove->m_inventory->RemoveItem(ToMove);
		
		SendItem(NULL,slot);

		ToMove->Delete(true);
		m_player->sendLocalizedString(ToMove->GetProto()->name,TEXT_DEATH_STATIC_YOU);
		return;
	}

	if(toslot == 600) // Auto Placement
	{
		if(ToMove->IsEquiped())
			toslot = GetFreeSlot();
		else
		{
			toslot = ToMove->GetProto()->slot;
			if(toslot == MAIN)
			{
				toslot = MAIN_DROITE;

				if(GetItemInSlot(MAIN_DROITE))
					toslot = MAIN_GAUCHE;
			}
		}
	}

	if(!toslot)
		return;

	Item * FromSlot = GetItemInSlot(toslot);
	BaseInventory * inv = ToMove->m_inventory;

	if(!FromSlot) // Si il n'y a pas d'item ou la je vais
	{
		if(ToMove->GetCount() > count) // On déplace seulement un  nombre de l'item
		{
			Item * itm = new Item(objmgr.GenerateItemGuid(),m_player); // On créer un nouvel item
			itm->Load(ToMove->GetProto());
			itm->SetCount(count);
			if(AddToSlot(itm,toslot))
				ToMove->RemoveCount(count);

			SendDoubleItem(ToMove,slot,itm,toslot);
		}
		else // On déplace tout l'item
		{
			if(AddToSlot(ToMove,toslot)) 
				SendDoubleItem(NULL,slot,ToMove,toslot);
			else inv->AddToSlot(ToMove,slot,false);
		}
	}
	else // Si il y a un item dans le slot
	{
		BaseInventory * frominv = FromSlot->m_inventory;

		if(m_equiped.CanAddToSlot(ToMove,toslot) && m_equiped.CanAddToSlot(FromSlot,slot))
		{
			if( !ToMove->IsEquiped() && !FromSlot->IsEquiped() && ToMove->GetProto() == FromSlot->GetProto() )
			{
				if( count + FromSlot->GetCount() <= FromSlot->GetProto()->maxstack )
				{
					ToMove->RemoveCount(count);
					FromSlot->AddCount(count);

					if(!ToMove->GetCount())
					{
						SendDoubleItem(NULL,slot,FromSlot,toslot);
						ToMove->Delete(true);
					} else SendDoubleItem(ToMove,slot,FromSlot,toslot);

					return;
				}
			}
			
			inv->RemoveItem(ToMove);
			frominv->RemoveItem(FromSlot);

			if(AddToSlot(ToMove,toslot))
			{
				if( AddToSlot(FromSlot,slot) )
				{
					SendDoubleItem(ToMove,toslot,FromSlot,slot);
				} else frominv->AddToSlot(FromSlot,toslot);
			}
			else inv->AddToSlot(ToMove,slot);

		} else return;
	}

	return;
}
BaseInventory * ItemsInterface::GetInventory(uint16 slot)
{
	if(slot <= MAX_EQUIPED_SLOT)
		return &m_equiped;
	else
	{
		uint8 type=0;
		if(slot <= INVENTORY_START_SLOT+(MAX_INVENTORY_SLOT*MAX_INVENTORY))
		{				
			slot-=INVENTORY_START_SLOT;
			while(slot>=MAX_INVENTORY_SLOT)
			{
				slot-=MAX_INVENTORY_SLOT;
				++type;
			}

			if(type < MAX_INVENTORY)
				return &m_inventory[type];
		}
	}

	return NULL;
}
Item * ItemsInterface::GetItemInSlot(uint16 slot)
{
	BaseInventory * Inv = GetInventory(slot);
	if(!Inv) return NULL;

	else return Inv->GetItemInSlot(slot);
}
uint16 ItemsInterface::GetFreeSlot()
{
	uint8 maxbag = MAX_INVENTORY;
	uint16 freeslot = 0;
	for(uint8 i=0;i<maxbag;++i)
	{
		// On prend uniquement le type 0 pour le moment
		// TODO : géré les type de sac en fonction du type de l'item
		freeslot = m_inventory[i].GetFreeSlot();
		if(freeslot) break;
	}
	return freeslot;
}
uint32 ItemsInterface::GetItemCount(uint32 entry)
{
	uint32 total=0;
	for(int i=0;i<MAX_INVENTORY;++i)
		total+=m_inventory[i].GetItemCount(entry);

	return total;
}
uint32 ItemsInterface::GetCountSlot()
{
	uint32 total=0;
	for(int i=0;i<MAX_INVENTORY;++i)
		total+=m_inventory[i].m_items.size()-m_inventory[i].GetItemCount();

	return total;
}
bool ItemsInterface::RemoveItem(uint32 entry,uint16 count)
{
	if(GetItemCount(entry) < count)
		return false;

	Item * Itm = NULL;
	for(int i=0;i<MAX_INVENTORY && count > 0;++i)
	{
		do
		{
			Itm = m_inventory[i].GetItemEntry(entry);
			if(!Itm) continue;

			if(Itm->GetCount() <= count)
			{
				count-=Itm->GetCount();
				RemoveItem(Itm,Itm->GetCount());
			}
			else
				RemoveItem(Itm,count);

		}while(Itm && count);
	}

	return count <= 0;
}
bool ItemsInterface::RemoveItem(Item * Itm,uint16 count)
{
	if(!Itm) return true;

	if(count > Itm->GetCount())
		return false;

	if(count < Itm->GetCount())
	{
		Itm->RemoveCount(count);
		SendItem(Itm,Itm->GetSlot());
	}
	else
	{
		SendItem(NULL,Itm->GetSlot());
		Itm->Delete(true);
	}

	return true;
}
bool ItemsInterface::RemoveItemInSlot(uint16 slot)
{
	if(!slot) return false;

	BaseInventory * Inv = GetInventory(slot);
	if(!Inv) return false;

	return Inv->RemoveItem(slot);
}
bool ItemsInterface::AddToSlot(Item * itm,uint16 slot,bool send)
{
	if(!itm) return false;
	if(!slot) return false;

	BaseInventory * Inv = GetInventory(slot);
	if(!Inv) return false;

	if(Inv->AddToSlot(itm,slot))
		if(send)
			SendItem(itm,slot);
	else return false;

	return true;
}
bool ItemsInterface::CreateItem(uint32 entry,uint16 count,bool message)
{
	const ItemProto * Proto = objmgr.GetItemProto(entry);
	return CreateItem(Proto,count,message);
}
bool ItemsInterface::CreateItem(const ItemProto * Proto,uint16 count,bool message)
{
	if(!count) return false;
	if(!Proto) return false;

	bool add=false;

	if(Proto->maxstack>1) // Check Maxstack
	{
		Item * itm=NULL;

		for(int i=0;i<MAX_INVENTORY;++i)
		{
			itm = m_inventory[i].AddCountItems(Proto,count);
			
			if(itm != NULL)
			{
				SendItem( itm, itm->GetSlot() );
				add=true;
				break;
			}
		}
	}

	if(!add)
	{
		uint16 freeslot = GetFreeSlot();

		if(!freeslot)  return false;

		uint32 guid = objmgr.GenerateItemGuid();

		Item * itm = new Item(guid,m_player);
		if(!itm->Load(Proto)) 
			return false;

		itm->SetCount(count);

		if(!AddToSlot(itm,freeslot))
			return false;
	}

	if(message)
		m_player->sendLocalizedString(Proto->name,TEXT_YOU_RECEIVE_ITEM_X);

	return true;
}
void ItemsInterface::BuildItemInfo(Buffer *b,const ItemProto * Proto,uint16 count,Item * itm,bool slot)
{
	if(!Proto)
		return;

	if(slot && itm)
		b->write<uint16>(itm->GetSlot());

	b->write<uint8>(0x00); // unk
	b->write<uint32>(Proto->entry);//18
	b->write<uint32>(Proto->modelid);

	b->write<uint16>(Proto->slot);
	b->write<uint8>(Proto->type);
	b->write<uint8>(Proto->min_rank); // Min Rank
	b->write<uint8>(Proto->min_rank+1); // Item Level // 1.3.5
	b->write<uint8>(0x00); // 1.3.5
	b->write<uint8>(Proto->min_renown); // Min Renown
	b->write<uint8>(0x00); // Unique equiped
	b->write<uint8>(Proto->rarity);
	b->write<uint8>(Proto->bind); // bind ?
	b->write<uint8>(Proto->race);
	b->write<uint32>(Proto->career); //36

	b->write<uint32>(0); // 45
	b->write<uint32>(Proto->sellprice);
	b->write<uint16>(count);
	b->write<uint32>(0x00);

	b->write<uint32>(Proto->skill); // skill mask Proto->skill
	b->write<uint16>(Proto->dps ? Proto->dps : (Proto->armor ? Proto->armor : 0));
	b->write<uint16>(Proto->speed);
	b->writeBytePascalString(Proto->name);

	int z=0;
	for(z=0;z<MAX_ITEMS_STATS;z++)
	{
		if( Proto->ItemStat[z].ItemStatType > GameData::ABILITYRESULT_WRONG_WEAPON_TYPE
			|| Proto->ItemStat[z].ItemStatValue <= 0 || Proto->ItemStat[z].ItemStatType == 0)
			break;
	}

	b->write<uint8>(z); // Stats counts
	
	for(int i=0;i<z;++i)
	{
		b->write<uint8>( Proto->ItemStat[i].ItemStatType );
		b->write<uint16>( Proto->ItemStat[i].ItemStatValue );
		b->fill(0,5);
	}

	b->write<uint8>(0x00);
	b->write<uint8>(0x00);// Spells counts
	// uint32 entry , uint16 X ,uint16 Y

	b->write<uint8>(0x00); // artisana info
		// for artisana info , 3 bytes
	b->write<uint8>(0x00);
	b->write<uint8>(Proto->talismanslot);
	if(Proto->talismanslot)
	{
		for(uint8 i=0;i<Proto->talismanslot;++i)
		{
			b->write<uint32>(0);
		}
	}


	b->write<uint8>( strlen(Proto->desc) );
	b->write(reinterpret_cast<const uint8*>(Proto->desc), strlen(Proto->desc) );

	b->write<uint8>(0x00);
	b->write<uint8>(0x00); // dunno but maybe 01
	b->write<uint8>(0x00);
	b->write<uint8>(0x00); 

	b->write<uint16>(0x0302);

	b->fill(0,8);
	b->write<uint8>(0x00); // Type culture , etc etc
	b->write<uint8>(0x00); // type , recipian , soil etc etc
	b->fill(0,11);
}
void ItemsInterface::SendDoubleItem(Item * Itm1,uint16 slot1,Item * Itm2,uint16 slot2)
{
	Buffer *b = new Buffer(0);
	b->write<uint8>(0x02); // Items counts;
	b->write<uint8>(0x00);
	b->write<uint8>(0x00);
	b->write<uint8>(0x00);

	if(!Itm1)
	{
		b->write<uint16>(slot1);
		b->fill(0,5);
	}
	if(!Itm2)
	{
		b->write<uint16>(slot2);
		b->fill(0,5);
	}

	if(Itm1)
		BuildItemInfo(b,Itm1->GetProto(),Itm1->GetCount(),Itm1,true);

	if(Itm2)
		BuildItemInfo(b,Itm2->GetProto(),Itm2->GetCount(),Itm2,true);

	Buffer *d = new Buffer(b->size()+3);
	d->write<uint16>(b->size());
	d->write<uint8>(F_GET_ITEM);
	d->write(b,true);

	m_player->sendPacket(d);
	delete b;
}
void ItemsInterface::SendItem(Item *Itm,uint16 slot)
{
	Buffer *b = new Buffer(0);
	b->write<uint8>(0x01); // Items counts;
	b->write<uint8>(0x00);

	if(Itm)
	{	
		b->write<uint8>(0x00);
		b->write<uint8>(0x00);
		BuildItemInfo(b,Itm->GetProto(),Itm->GetCount(),Itm,true);
	}
	else
	{
		b->write<uint8>(2);
		b->write<uint8>(0);
		b->write<uint16>(slot);
		b->write<uint16>(0);
		b->write<uint16>(0);
		b->write<uint16>(0);
	}

	Buffer *d = new Buffer(b->size()+3);
	d->write<uint16>(b->size());
	d->write<uint8>(F_GET_ITEM);
	d->write(b,true);

	m_player->sendPacket(d);
	delete b;
}
void ItemsInterface::BuildBufferList(Buffer * b,Item * itm ,uint8 &count)
{
	if(!itm) return;

	++count;
	BuildItemInfo(b,itm->GetProto(),itm->GetCount(),itm,true);

	// Si on atteind le max item par buffer alors on le send et on en créer un nouvea
	if(count >= MAX_ITEM_ON_BUFFER)
	{
		Buffer *r = new Buffer(0);
		r->write<uint16>(b->size()+4);
		r->write<uint8>(F_GET_ITEM);

		r->write<uint8>(count);
		r->fill(0,3);

		r->write(b,true);

		m_player->sendPacket(r);

		// Remise a zéro du buffer liste
		delete b;
		b = new Buffer(0);
		count=0;
	}
}
void ItemsInterface::SendMaxInventory()
{
	Buffer *f = new Buffer(17+3);
	f->write<uint16>(17);
	f->write<uint8>(0x95);
	uint8 data[] =
	{
		0x0F,0x50,
		0x00,0x10,
		0x00,
		0x10,0x27,0x00, // Price Swapped BYTE
		0x00,
		0x50,0x00,0x08,
		0x00,0x60,
		0xEA,
		0x00,0x00
	};
	f->write(data,17);
	m_player->sendPacket(f);
}
void ItemsInterface::SendAllItems()
{
	SendMaxInventory();
	Buffer *b = new Buffer(0);

	uint8 count=0;

	// Construction des items équipé
	m_equiped.M_items.Acquire();

	for(int i=0;i<m_equiped.m_items.size();++i)
		BuildBufferList(b,m_equiped.m_items[i],count);

	m_equiped.M_items.Release();

	for(int i=0;i<MAX_INVENTORY;++i)
	{
		// Construction des items de l'inventaire
		m_inventory[i].M_items.Acquire();

		for(int d=0;d<m_inventory[i].m_items.size();++d)
			BuildBufferList(b,m_inventory[i].m_items[d],count);	

		m_inventory[i].M_items.Release();
	}

	if(count)
	{
		Buffer *r = new Buffer(0);
		r->write<uint16>(b->size()+4);
		r->write<uint8>(F_GET_ITEM);

		r->write<uint8>(count);
		r->fill(0,3);

		r->write(b,true);
		m_player->sendPacket(r);
		delete b;
	}
}
void ItemsInterface::SendInspect(Player * Plr)
{
	if(Plr == NULL) return;
	if(Plr == m_player) return;
	
	Log.Debug("ItemsInterface","SendInspect %s -> %s",m_player->GetName().c_str(),Plr->GetName().c_str());


	uint8 counts=0;
	Item * itm=NULL;

	Buffer *b = new Buffer(0);

	m_equiped.M_items.Acquire();
	for(uint16 i=0;i<m_equiped.m_items.size();++i)
	{
		itm = m_equiped.m_items[i];
		if(!itm) continue;

		BuildItemInfo(b,itm->GetProto(),itm->GetCount(),itm);
		counts++;
	}
	m_equiped.M_items.Release();

	uint16 responseSize=b->size()+5;

	Buffer *d = new Buffer(responseSize+3);
	d->write<uint16>(responseSize);
	d->write<uint8>(F_SOCIAL_NETWORK);

	d->write<uint16>(0x0000);
	d->write<uint8>(0x07);
	d->write<uint8>(counts);
	d->write(b);
	d->write<uint8>(0);
	Plr->sendPacket(d);
	delete b;
}
void ItemsInterface::SendAllEquiped(Player * Plr)
{
	if(!m_player) return;

	uint8 counts=0;
	Item * itm=NULL;

	Buffer * b =new Buffer(0);
	
	m_equiped.M_items.Acquire();
	for(uint16 i=0;i<m_equiped.m_items.size();++i)
	{
		itm = m_equiped.m_items[i];
		if(!itm) continue;

		BuildItemInfo(b,itm->GetProto(),itm->GetCount(),itm);
		counts++;
	}
	m_equiped.M_items.Release();


	uint16 responseSize = b->size()+5;
	Buffer *d = new Buffer(responseSize+3);

	d->write<uint16>(responseSize);
	d->write<uint8>(F_PLAYER_INVENTORY);
	d->write<uint16>(m_player->GetOid());
	d->write<uint8>(0);
	d->write<uint8>(counts);
	d->write<uint8>(0);
	d->write(b);

	if(Plr) 
		Plr->sendPacket(d);
	else 
		m_player->dispachPacketInRange(d);

	delete b;
}
uint8 ItemsInterface::GetAttackSpeed()
{
	uint32 speed=0;
	Item * itm = m_equiped.GetItemInSlot(MAIN_GAUCHE);
	if(itm) speed+= itm->GetProto()->speed*0.1;

	itm = m_equiped.GetItemInSlot(MAIN_DROITE);
	if(itm) speed+= itm->GetProto()->speed*0.1;

	Log.Info("ItemInterface","AttackSpeed %u",speed);
	return speed;
}
uint16 ItemsInterface::GetTotalDamage()
{
	uint16 damage=0;
	Item * itm = m_equiped.GetItemInSlot(MAIN_GAUCHE);
	if(itm) damage+= itm->GetProto()->dps;

	itm = m_equiped.GetItemInSlot(MAIN_DROITE);
	if(itm) damage+= itm->GetProto()->dps;

	return damage;
}
uint16 ItemsInterface::GetTotalArmor()
{
	uint16 Armor=0;

	m_equiped.M_items.Acquire();

	for(int i=0;i<m_equiped.m_items.size();++i)
		if( m_equiped.m_items[i] != NULL )
			Armor = m_equiped.m_items[i]->GetProto()->armor;

	m_equiped.M_items.Release();

	return Armor;
}
float ItemsInterface::GetWdps( uint16 slot )
{
	Item * itm = GetItemInSlot(slot);

	if(!itm) return 0;
	else return float(itm->GetProto()->dps)/10;
}
float ItemsInterface::GetWspeed( uint16 slot)
{
	Item * itm = GetItemInSlot(slot);

	if(!itm) return 200;
	else return itm->GetProto()->speed;
}