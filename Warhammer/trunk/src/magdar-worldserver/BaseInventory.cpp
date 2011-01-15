#include "StdAfx.h"

// Selection d'items
void BaseInventory::Save()
{
	M_items.Acquire();

	for(int i=0;i<m_items.size();++i)
		if( m_items[i] != NULL )
			m_items[i]->Save();

	M_items.Release();
}
void BaseInventory::Delete()
{
	M_items.Acquire();

	for(int i=0;i<m_items.size();++i)
		if( m_items[i] != NULL )
			m_items[i]->Delete();

	M_items.Release();
}

uint16 BaseInventory::GetItemCount()
{
	uint16 count=0;
	M_items.Acquire();

	for(int i=0;i<m_items.size();++i)
		if( m_items[i] != NULL )
			++count;

	M_items.Release();

	return count;
}
uint16 BaseInventory::GetFreeSlot()
{
	uint16 slot=0;

	M_items.Acquire();

	for(int i=0;i<m_items.size();++i)
		if( m_items[i] == NULL )
		{
			slot = i;
			break;
		}

	M_items.Release();

	return slot+minslot;
}
Item * BaseInventory::GetItemInSlot(uint16 slot)
{
	if(slot < minslot || slot-minslot > m_items.size())
		return NULL;

	slot-=minslot;

	Item * itm=NULL;

	M_items.Acquire();
	itm = m_items[slot];
	M_items.Release();

	return itm;
}
uint32 BaseInventory::GetItemCount(uint32 entry)
{
	uint16 count=0;
	M_items.Acquire();

	for(int i=0;i<m_items.size();++i)
		if( m_items[i] != NULL  && m_items[i]->GetProto()->entry == entry)
			++count;

	M_items.Release();

	return count;
}

// Ajout d'item
bool BaseInventory::AddToFreeSlot(Item * itm)
{
	if(!itm) return true;

	bool result=false;

	M_items.Acquire();

	uint16 freeslot = GetFreeSlot();
	if(freeslot)
		result = AddToSlot(itm,freeslot);

	M_items.Release();

	return result;

}
bool BaseInventory::AddToSlot(Item *itm,uint16 slot,bool check)
{
	Log.Debug("BaseInventory","AddToSlot %u",slot);
	if(!itm) return true;

	if(check)
		if(!CanAddToSlot(itm,slot))
			return false;

	bool result=false;

	M_items.Acquire();

	Log.Debug("Base","minslot=%u , size = %u",minslot,m_items.size());

	if( slot >= minslot && m_items.size() > slot-minslot)
	{
		uint16 temslot = slot-minslot;
		Item * temp = m_items[temslot];

		if( !temp )
		{
			if(itm->m_inventory)
				itm->m_inventory->RemoveItem(itm);

			if(temp)
			{
				itm->AddCount(temp->GetCount());
				temp->Delete();
			}	

			m_items[temslot] = itm;
			itm->SetSlot(slot);
			itm->m_inventory = this;

			ApplyStats(itm,true);
			result = true;

		} else Log.Error("Base","Error item in slot");
	} else Log.Error("Base","Error slot minslot");
	M_items.Release();

	return result;
}
// Suppression d'item
bool BaseInventory::RemoveItem(Item * itm)
{
	if(!itm) return false;

	bool result=false;

	M_items.Acquire();

	for(int i=0;i<m_items.size();++i)
		if( m_items[i] != NULL  && m_items[i] == itm)
		{
			itm->m_inventory = NULL;
			ApplyStats(itm,false);
			m_items[i] = NULL;
			result=true;
			break;
		}

	M_items.Release();

	return result;
}
bool BaseInventory::RemoveItem(uint16 slot)
{
	M_items.Acquire();
	Item * itm = GetItemInSlot(slot);
	bool result = RemoveItem(itm);
	M_items.Release();

	return result;
}
Item * BaseInventory::GetItemEntry(uint32 entry)
{
	Item * itm=NULL;

	M_items.Acquire();

	for(int i=0;i<m_items.size();++i)
		if( m_items[i] != NULL  && m_items[i]->GetProto()->entry == entry)
		{
			itm = m_items[i];
			break;
		}

	M_items.Release();

	return itm;
}
Item * BaseInventory::AddCountItems(const ItemProto * Proto,uint16 count)
{
	Item * itm=NULL;

	M_items.Acquire();

	for(int i=0;i<m_items.size();++i)
	{
		itm =  m_items[i];
		if( itm != NULL  && itm->GetProto()->entry == Proto->entry)
		{
			if(itm->GetProto()->maxstack >= count+itm->GetCount())
			{
				itm->AddCount(count);
				break;
			} else itm=NULL;
		}
	}
	M_items.Release();

	return itm;
}