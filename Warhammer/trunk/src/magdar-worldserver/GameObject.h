#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct GameObjectProto;
struct GameObjectSpawn;
class Unit;

class SERVER_DECL GameObject : public Unit
{
public:

	GameObject(uint32 entry,const GameObjectSpawn * gos);

	bool Load();
	void Save();

	void Update(uint32 tdiff);

	/************************************************************************/
	/* INTERRACT															*/
	/************************************************************************/
	void SendMeTo(Player * Plr);
	void SendEquiped(Player *Plr) {};
	void SendInterractTo(Player *Plr,Struct_f_interract &interract);

	Unit * GetTarget();

private:

	uint32 m_entry;
	uint32 m_guid;

	const GameObjectProto * m_gon;
	const GameObjectSpawn *m_gos;
};

#endif