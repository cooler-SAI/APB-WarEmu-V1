#include "StdAfx.h"

bool Inventory::CanAddToSlot(Item *itm,uint16 slot) 
{ 
	if(!itm) return true;

	Item * From = GetItemInSlot(slot);

	if(!From) return true;

	if(From && From->GetProto() == itm->GetProto())
	{
		if(From->GetCount()+itm->GetCount() <= itm->GetProto()->maxstack)
			return true;
		else return false;
	}

	return false;
}