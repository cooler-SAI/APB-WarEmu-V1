#include "StdAfx.h"


Item::Item(uint32 guid,Player * player)
{
	m_player = player;
	m_guid = guid;
	m_slot=0;
	m_script = NULL;
	Proto = NULL;
	m_inventory = NULL;
}
bool Item::Load(uint32 entry)
{
	const ItemProto * itm = objmgr.GetItemProto(entry);
	return Load(itm);
}

bool Item::Load(const ItemProto *proto)
{
	if(!proto) 
	{
		Log.Error("Item","Error on Load : proto==NULL");
		return false;
	}

	if(!proto->entry)
	{
		Log.Error("Item","Invalid item Proto %u.Loading Failled",proto->entry);
		return false;
	}

	Proto = proto;

	m_script = scriptmgr.GetItmScript(string(Proto->scriptname),m_player,this);

	return true;
}
bool Item::Load(Field * Fetch)
{
	m_guid =		Fetch[0].GetUInt32();
	uint32 entry =	Fetch[2].GetUInt32();
	m_slot =		Fetch[3].GetUInt32();
	m_count =		Fetch[4].GetUInt32();

	return Load(entry);
}
bool Item::LoadFromGuid()
{
	QueryResult * Result = CharacterDatabase.Query("SELECT entry,counts FROM characters_items WHERE guid=%u",m_guid);
	if(!Result) return false;

	uint32 entry =	Result->Fetch()[0].GetUInt32();
	m_count =		Result->Fetch()[1].GetUInt32();
	delete Result;

	return Load(entry);
}
void Item::Save(uint16 slot)
{
	uint16 tslot = slot;
	if(!tslot) tslot=m_slot;

	std::stringstream ssSQL;
	ssSQL << "REPLACE INTO `characters_items` (`guid`,`character_id`,`entry`,`slot_id`,`counts`) VALUES ('";
	ssSQL << GetGuid() << "','";
	ssSQL << (GetOwner() ? GetOwner()->GetPlayerID() : 0) << "','";
	ssSQL << GetProto()->entry << "','";
	ssSQL << uint32(tslot) << "','";
	ssSQL << uint32(GetCount()) << "')";

	CharacterDatabase.Execute(ssSQL.str().c_str());

	m_slot = tslot;

}
void Item::SetCount(uint16 count)
{
	if(count < 0)
		count=0;

	if(m_script)
		m_script->onModifyCount(count);

	m_count=count;

}
void Item::SetSlot(uint16 slot)
{
	m_slot = slot;
}
void Item::Delete(bool remove)
{
	if(m_inventory != NULL)
		m_inventory->RemoveItem(this);

	std::stringstream ssSQL;
	ssSQL << "DELETE FROM characters_items WHERE guid=";
	ssSQL << GetGuid() ;
	CharacterDatabase.Execute(ssSQL.str().c_str());
	if(remove)
	{
		if(m_script)
		{
			m_script->onDeleteItem();
			m_script->Delete();
			m_script = NULL;

		}

		delete this;
	}
}
bool Item::IsEquiped()
{
	return m_slot < INVENTORY_START_SLOT;
}
uint32 Item::CalculBuyBackPrice(uint16 count)
{ 
	 return Proto->sellprice*count; 
 
}
uint16 Item::GetStat(uint16 type)
{
	uint16 value=0;
	for(int i=0;i<MAX_ITEMS_STATS;++i)
	{
		if(GetProto()->ItemStat[i].ItemStatType == type)
			value+=GetProto()->ItemStat[i].ItemStatValue;
	}
	return value;
}