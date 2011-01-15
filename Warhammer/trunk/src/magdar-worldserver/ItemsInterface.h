#ifndef ITEMSINTERFACE_H
#define ITEMSINTERFACE_H

class Player;
class Item;
class ItemsInterface;

#define MAX_INVENTORY_SLOT 16
#define MAX_EQUIPED_SLOT 35
#define MAX_INVENTORY 5
#define MAX_INVENTORY_TYPE 4
#define MAX_ITEM_ON_BUFFER 250
#define INVENTORY_PRICE 10000

#define EQUIPED_START_SLOT 10
#define INVENTORY_START_SLOT 40
#define ARTISANAT_START_SLOT 400
#define DEVISE_START_SLOT 500

#define START_TRADE_SLOT 232
#define MIDLE_TRADE_SLOT 241
#define END_TRADE_SLOT 249

#define DELETE_SLOT 1000

class BaseInventory
{
	friend class ItemsInterface;

public:

	void SetOwner(Player * Info) { m_player=Info; }
	Player * GetPlayer() { return m_player; }
	void Save();
	void Delete();

	// Gestion des slots
	void SetMinSlot(uint16 slot) { minslot=slot; }
	uint16 GetMinSlot() { return minslot; };

	// Selection d'items
	uint16 GetItemCount();								// Renvoi le nombre d'item dans l'inventaire
	uint16 GetFreeSlot();								// Renvoi un slot libre
	Item * GetItemInSlot(uint16 slot);					// Renvoi l'item se situant sur se slot
	uint32 GetItemCount(uint32 entry);					// Renvoi le nombre d'item d'un certain entry
	Item * GetItemEntry(uint32 entry);

	// Ajout d'item
	void HandleMoveItem(uint16 slot,uint16 toslot,uint16 count);

	virtual bool CanAddToSlot(Item *itm,uint16 slot) = 0;	// Check si l'on peut l'ajouter au slot
	virtual void ApplyStats(Item *itm,bool add) = 0;			// Ajoute des stats si necessaire
	bool AddToFreeSlot(Item * itm);						// Tente d'ajouter un items , si il n'y a pas de place return false
	bool AddToSlot(Item *itm,uint16 slot,bool check=true);				// Ajoute un items dans un slot préci
	bool TransfertSlot(uint16 FromSlot,uint16 ToSlot);	// Echange les place des items
	Item * AddCountItems(const ItemProto * Proto,uint16 count); // On retourn l'item sur lequel a été ajouté les counts


	// Suppression d'item
	bool RemoveItem(Item * itm);			// Supprime l'item du sac
	bool RemoveItem(uint16 slot);

protected:
	Player * m_player;
	// Slot de départ dans l'inventaire , Slot maximum de l'inventaire
	uint16 minslot;

	Mutex M_items;
	vector<Item*> m_items;
};

class Inventory : public BaseInventory
{
	friend class ItemsInterface;
	friend class BaseInventory;
public:
	bool CanAddToSlot(Item *itm,uint16 slot);			// Check si l'on peut l'ajouter au slot
	void ApplyStats(Item *itm,bool add) { };			// Ajoute des stats si necessaire

private:
};

class EquipedItems : public BaseInventory
{
	friend class ItemsInterface;
	friend class BaseInventory;
public:
	bool CanAddToSlot(Item *itm,uint16 slot);	// Check si l'on peut l'ajouter au slot
	void ApplyStats(Item *itm,bool add);			// Ajoute des stats si necessaire

private:
};

class ItemsInterface
{
public:
	ItemsInterface(Player * Plr);
	void Load();
	void Save();

	BaseInventory * GetInventory(uint16 slot);	// Renvoi l'inventaire pour le slot
	Item * GetItemInSlot(uint16 slot);			// Renvoi l'item dans le slot
	uint16 GetFreeSlot();						// Renvoi un slot libre
	uint32 GetItemCount(uint32 entry);			// Renvoi le nombre d'item dans l'inventaire
	uint32 GetCountSlot();						// Renvoi le nombre de slot libre
	bool RemoveItem(uint32 entry,uint16 count=1);	// Supprime un nombre d'item
	bool RemoveItem(Item * Itm,uint16 count=1);	// Supprime l'item sélectionné
	bool RemoveItemInSlot(uint16 slot);			// Supprime un item du slot

	// Créer un item
	bool AddToSlot(Item * itm,uint16 slot,bool send=true);	// Ajoute l'item dans un slot
	bool CreateItem(uint32 entry,uint16 count=1,bool slot=true);
	bool CreateItem(const ItemProto * Proto,uint16 count=1,bool message=true);

	// Sender
	static void BuildItemInfo(Buffer *b,const ItemProto * Proto,uint16 count=1,Item * Itm=NULL,bool slot=true);
	void SendDoubleItem(Item * Itm1,uint16 slot1,Item * Itm2,uint16 slot2);
	void SendItem(Item *Itm,uint16 slot);

	void BuildBufferList(Buffer * b,Item * itm ,uint8 &count);
	void SendMaxInventory(); // 1.3.5
	void SendAllItems(); // Envoi la liste de tous les items

	void SendAllEquiped(Player * Plr=NULL);
	void SendInspect(Player * Plr);

	void HandleMoveItem(uint16 slot,uint16 toslot,uint16 count);

	uint8 GetAttackSpeed();
	uint16 GetTotalDamage();
	uint16 GetTotalArmor();
	float GetWdps( uint16 slot );
	float GetWspeed( uint16 slot);

private:
	Player * m_player;

	EquipedItems m_equiped;
	Inventory m_inventory[MAX_INVENTORY];
};


#endif