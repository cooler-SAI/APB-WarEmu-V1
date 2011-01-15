#ifndef ITEM_H
#define ITEM_H

class ScriptedItem;
class BaseInventory;
struct ItemProto;

enum EquipSlot
{
	MAIN_DROITE = 10,
	MAIN_GAUCHE = 11,
	ARME_DISTANCE = 12,
	MAIN = 13,
	ETENDARD = 14,

	TROPHEE_1 = 15,
	TROPHEE_2 = 16,
	TROPHEE_3 = 17,
	TROPHEE_4 = 18,
	TROPHEE_5 = 19,

	CORPS = 20,
	GANTS = 21,
	CHAUSSURE = 22,
	HEAUME = 23,
	EPAULE = 24,
	POCHE_1 = 25,
	POCHE_2 = 26,
	DOS = 27,
	CEINTURE = 28,

	BIJOUX_1 = 31,
	BIJOUX_2 = 32,
	BIJOUX_3 = 33,
	BIJOUX_4 = 34,
};

class Item
{
public:
	
	Item(uint32 guid,Player * player);
	const ItemProto * GetProto() { return Proto; };

	bool Load(uint32 entry);
	bool Load(const ItemProto *proto);
	bool Load(Field * Fetch);
	bool LoadFromGuid();

	void Save(uint16 slot=0);

	void Delete(bool remove=true);

	bool IsEquiped();

	uint32 GetGuid()				{ return m_guid; };
	void SetGuid(uint32 guid)		{ m_guid=guid; };

	Player * GetOwner()				{ return m_player; };
	void SetOwner(Player * Plr)		{ m_player=Plr; };
	
	uint16 GetCount()				{ return m_count; };
	void SetCount(uint16 count);

	uint16 GetSlot() { return m_slot; };
	void SetSlot(uint16 slot);

	void RemoveCount(uint16 remove)	{ SetCount(m_count-remove); };
	void AddCount(uint16 add)		{ SetCount(m_count+add); };

	uint16 GetTradeSlot()			{ return m_tradeslot; };
	void SetTradeSlot(uint16 slot)	{ m_tradeslot=slot; };

	uint32 CalculBuyBackPrice(uint16 count);

	uint16 GetStat(uint16 type);

	ScriptedItem * GetScript() { return m_script; };

	BaseInventory * m_inventory;
private:

	// id of Slot on player or inventory
	uint32 m_guid;
	Player * m_player;
	uint16 m_slot;
	uint16 m_tradeslot;
	uint16 m_count;
	ObjectType Type;

	const ItemProto* Proto;

	ScriptedItem * m_script;
};
#endif