#ifndef LOOT_H
#define LOOT_H

class Unit;
class Player;
class Group;

static uint32 m_lastloot=0;

static uint32 GenerateLootId() 
{ 
	return ++m_lastloot; 
}

struct LootInfo
{
	uint8 num; // Numéro de l'item dans le loot;

	uint32 entry;
	const ItemProto * Proto;

	Player * Looter; // Numéro du joueur qui veux loot
};


class Loot
{
public:
	Loot(Unit * unit) : 
	m_unit(unit)
	{ 
		lootentry = GenerateLootId();
		m_looters.clear();
	}
	

	bool GenerateLoot(); // True si ya quelque chose a looter
	LootInfo * GetItemLoot(uint8 num);
	void RemoveItemLoot(uint8 num);
	uint8 GetLootCount();
	bool IsValid();		// True si ya des items ou de l'agent;


	void AddPlayerToLoot(uint32 entry);
	void RemovePlayerToLoot(uint32 entry);
	bool HasPlayerToLoot(uint32 entry);

	void HandleInteract(Player * Plr,Struct_f_interract &interract);

	void RefreshLoot(); // Rafraichi le loot pour tous les joueurs qui le voyent


	void SendLoot(Player * Plr);

	Player * m_looter; // Player qui peut use le loot;
	Group * m_group; // Groupe qui peut use le loot;

private:

	uint32 lootentry;	// Numéro du loot
	Unit * m_unit;		// Unité qui loot

	Mutex M_loot;
	uint32 m_money;	// Argent à loot;
	vector<LootInfo*> m_loot; // Items du loot

	Mutex M_looters;
	tr1::unordered_map<uint32,Player*> m_looters; // Liste des personnes qui affiche le loot
};


#endif