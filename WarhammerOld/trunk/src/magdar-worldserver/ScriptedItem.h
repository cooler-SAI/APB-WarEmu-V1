#ifndef SCRIPTEDITEM_H
#define SCRIPTEDITEM_H


class Player;
class Unit;
class Item;

class SERVER_DECL ScriptedItem
{
public:
	ScriptedItem(Player * Plr,Item * Item) : m_player(Plr), m_item(Item) {}

	virtual void onMoveItem(uint16 toslot) {} // OK
	virtual void onDeleteItem() {} // OK

	virtual void onUseItem() {}
	virtual void onSpeack(Unit * unit) {}
	virtual void onKillUnit(Unit * unit) {}
	virtual void onGetXp(uint32 &xp) {}
	virtual void onExpire() {}
	virtual void onSellItem() {}
	virtual void onModifyCount(uint32 newcount) {} // OK

	void onTradeItem(Player * Plr);
	void Delete();

private:

	Player * m_player;
	Item * m_item;
};


#endif