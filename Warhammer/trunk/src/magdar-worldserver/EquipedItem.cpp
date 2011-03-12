#include "StdAfx.h"

bool EquipedItems::CanAddToSlot(Item *itm,uint16 slot)
{
	Log.Debug("EquipedItems","CanAddToSlot %u",slot);
	
	if(!itm) return false;
	if(!m_player) return false;
	if(slot > MAX_EQUIPED_SLOT) return true;

	const ItemProto * Proto = itm->GetProto();
	if(!Proto) return false;

	uint16 itemslot = Proto->slot;

	bool can = false;

	if(itemslot != slot)
	{
		if(itemslot==MAIN && (slot==MAIN_DROITE || slot==MAIN_GAUCHE) )
			can =true;
		else if(itemslot >= BIJOUX_1 && itemslot <=BIJOUX_4 && slot >= BIJOUX_1 && slot <=BIJOUX_4)
			can=true;
		else if(itemslot >= TROPHEE_1 && itemslot <=TROPHEE_5 && slot >= TROPHEE_1 && slot <=TROPHEE_5)
			can=true;
		else if(itemslot >= POCHE_1 && itemslot <=POCHE_2 && slot >= POCHE_1 && slot <=POCHE_2)
			can=true;
		else if(slot >= INVENTORY_START_SLOT)
			can =true;

	} else can = true;

	if(!can) return false;

	uint32 playerskill = m_player->GetSkills();
	uint32 playercareer = 1 << (m_player->GetCareerLine()-1);
	uint8 level = m_player->GetLevel();
	uint8 race =  1 << (m_player->GetRace()-1);

	uint32 itmskill = Proto->skill << 1;
	if(Proto->skill && (playerskill &= itmskill) != itmskill )
	{
		Log.Debug("EquipedItems","HasAllRequiredForEquip no skills");
		return false;
	}
	
	if(Proto->career && !(Proto->career & playercareer) )
	{
		Log.Debug("EquipedItems","HasAllRequiredForEquip no career");
		return false;
	}
	
	if(Proto->min_rank && level < Proto->min_rank)
	{
		Log.Debug("EquipedItems","HasAllRequiredForEquip no level");
		return false;
	}

	if(Proto->race &&  !(Proto->race & race) )
	{
		Log.Debug("EquipedItems","HasAllRequiredForEquip no race");
		return false;
	}

	return true;
}
void EquipedItems::ApplyStats(Item *itm,bool add)
{
	Log.Debug("EquipedItems","ApplyStats %u",add);
}
bool EquipedItems::AddToSlot(Item *itm,uint16 slot,bool check)
{
	if(BaseInventory::AddToSlot(itm,slot,check))
	{
		if(m_player != NULL)
			m_player->GetItemInterface()->SendEquipedSlot(itm->GetSlot(),itm);

		return true;
	}

	return false;
}
bool EquipedItems::RemoveItem(Item *itm)
{
	uint16 Slot = itm->GetSlot();
	if(BaseInventory::RemoveItem(itm))
	{
		if(m_player != NULL)
			m_player->GetItemInterface()->SendEquipedSlot(Slot,NULL);

		return true;
	}

	return false;
}