#ifndef CREATURE_H
#define CREATURE_H

class Player;
struct CreatureSpawn;
struct CreatureInterract;
struct CreatureProto;
struct CreatureLoot;
struct ItemProto;
class ScriptedAI;

#define UPDATE_GRAPHICAL_INTERVAL 20000
#define MAX_ITEM_PAGE 20

//vendors

struct VendorItem
{
	VendorItem(const ItemProto * proto,uint32 _cost, string reqitem);

	const ItemProto * m_proto;
	uint32 cost;

	// count, entry
	map<uint16,const ItemProto*> m_reqitem;
};

struct VendorItemData
{
	vector<VendorItem*> m_items;

	VendorItem* GetItem(uint32 slot) const
    {
        if(slot>=m_items.size()) return NULL;
        return m_items[slot];
    }

	bool Empty() const { return m_items.empty(); }
	uint8 GetItemCount() const { return m_items.size(); }
	void AddItem( const ItemProto * proto, uint32 cost, string reqitem)
    {
        m_items.push_back(new VendorItem(proto, cost,reqitem ));
    }
    bool RemoveItem( uint32 item_id );
    VendorItem const* FindItem(uint32 item_id) const;
    size_t FindItemSlot(uint32 item_id) const;

    void Clear()
    {
        for (vector<VendorItem*>::const_iterator itr = m_items.begin(); itr != m_items.end(); ++itr)
            delete (*itr);
        m_items.clear();
    }


};

//Equipement
struct EquipedItem
{
	EquipedItem(uint32 _slotid,uint32 _modelid, uint32 _effect) : slotid(_slotid), modelid(_modelid), effect(_effect) {}

	uint32 slotid;
	uint32 modelid;
	uint32 effect;
};

struct CreatureEquipement
{
	vector<EquipedItem*> m_items;

	EquipedItem* GetItem(uint32 slot) const
    {
        if(slot>=m_items.size()) return NULL;
        return m_items[slot];
    }

	bool Empty() const { return m_items.empty(); }
	uint8 GetItemCount() const { return m_items.size(); }
	void AddItem( uint32 slot, uint32 model , uint32 effect)
    {
        m_items.push_back( new EquipedItem(slot, model, effect ) );
    }

	bool RemoveItem( uint32 slot )
	{
		for(vector<EquipedItem*>::iterator i = m_items.begin(); i != m_items.end(); ++i )
		{
			if((*i)->slotid==slot)
			{
				m_items.erase(i);
				return true;
			}
		}
		return false;
	}

    void Clear()
    {
        for (vector<EquipedItem*>::const_iterator itr = m_items.begin(); itr != m_items.end(); ++itr)
            delete (*itr);
        m_items.clear();
    }
};

class SERVER_DECL Creature : public Unit
{
public:
	Creature(uint32 entry,const CreatureSpawn * Sp) 
	{
		m_type = TYPEID_CREATURE;
		m_entry=entry;
		m_cs=Sp; 
		m_script = NULL;
		m_ScriptName = "";
		m_Target = NULL;
	};

	void Update( uint32 time );

	bool Load();
	void Reload();

	/************************************************************************/
	/* CREATURE																*/
	/************************************************************************/
	uint32 GetEntry()	{ return m_entry; };
	uint32 GetGuid()	{ return m_guid; };
	const CreatureProto * GetCreatureProto() { return m_cp; };
	const CreatureSpawn * GetCreatureSpawn() { return m_cs; };
	void Save();


	/************************************************************************/
	/* INTERRACT															*/
	/************************************************************************/
	CreatureEquipement const* GetEquipedItems();
	void SendEquiped(Player * Plr);
	void SendMeTo(Player * Plr);
	void SendInterractTo(Player * Plr,Struct_f_interract &interract);


	void SayText(string text) { SendText(text,CHATLOGFILTERS_MONSTER_SAY); }
	void ShoutText(string text) { SendText(text,CHATLOGFILTERS_MONSTER_SAY); }
	void SendText(string text,ChatLogFilters type);

	/************************************************************************/
	/* VENDORS																*/
	/************************************************************************/
	VendorItemData const* GetVendorItems();
	void SendVendorItems(Player * Plr);
	void SendVendorItemsList(Player * Plr,VendorItemData const* vItems,uint32 depart,uint32 fin,uint8 page);
	void BuyVendorItem(Player * Plr,Struct_f_interract interract);
	void SellVendorItem(Player * Plr,Struct_f_interract interract);
	void BuyBackVendorItem(Player * Plr,Struct_f_interract interract);

	void SendDyeMenu(Player * Plr,Struct_f_interract interract);


	/************************************************************************/
	/* BYTES																*/
	/************************************************************************/
	void AddByte(uint8 bytes) { m_bytes.push_front(bytes); }
	void RemoveByte(uint8 bytes) { m_bytes.remove(bytes); }
	void ClearByte() { m_bytes.clear(); }
	bool HasByte(uint8 bytes)
	{
		for(list<uint8>::iterator itr=m_bytes.begin();itr!=m_bytes.end();++itr)
			if((*itr) == bytes) return true;

		return false;
	}

	/************************************************************************/
	/* LOOT																	*/
	/************************************************************************/
	CreatureLoot const* GetCreatureLoot();
	Unit * GetTarget();

	/* Movement */
	void SendObjectState(Player * Plr=NULL);
	void MoveTo(uint16 x,uint16 y,uint16 z);
	uint32 m_nextobjectstate;

	ScriptedAI* m_script;

protected:

	uint32 m_entry;
	uint32 m_guid;

	const CreatureSpawn *m_cs;
	const CreatureProto *m_cp;

	//CreatureInterract * m_interract;

	uint32 m_faction;
	uint32 m_npcflags;

	string npc_text;
	map<uint16,uint16> m_items;

	list<uint8> m_bytes;
	const CreatureEquipement * m_equipement;

	std::string m_ScriptName;
};


#endif