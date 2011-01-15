#include "StdAfx.h"

void ScriptedItem::onTradeItem(Player * Plr)
{
	m_player = Plr;
}

void ScriptedItem::Delete() 
{ 
	delete this; 
};