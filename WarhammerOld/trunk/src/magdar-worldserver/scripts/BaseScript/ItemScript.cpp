#include "../../StdAfx.h"

class TestItem : public ScriptedItem
{
public:
	ADD_ITEM_FACTORY_FUNCTION(TestItem);
	TestItem(Player * Plr,Item * Itm) : ScriptedItem(Plr,Itm)
	{

	}

	void onMoveItem(uint16 toslot) 
	{
	
	}
	void onDeleteItem()
	{
	
	}

	void onUseItem() 
	{
	
	}
	void onSpeack(Unit * unit) 
	{
	
	}
	void onKillUnit(Unit * unit) 
	{
	
	}
	void onGetXp(uint32 &xp) 
	{
	
	}
	void onExpire() 
	{
	
	}
	void onSellItem() 
	{
	
	}
	void onModifyCount(uint32 newcount) 
	{
	
	}
};

void RegisterItemScript(ScriptMgr *Mgr)
{
	Mgr->RegisterItemScript("ItemScript",&TestItem::Create);
}