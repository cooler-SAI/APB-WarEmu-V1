#ifndef UNIT_H
#define UNIT_H

class Player;
class AIInterface;
class CombatManager;
class Ability;
class PQuest;
class Loot;

struct AbilityInfo;
struct LootInfo;
struct QuestInfo;
struct AreaInfo;

#define HP_REGEN_TIME 4000
#define AP_REGEN_TIME 1000
#define AREA_UPDATE_TIME 5000

class SERVER_DECL Unit : public Object
{
public:
	Unit();
	virtual void Update( uint32 time );

	void SetLevel(uint8 Level) { m_level = Level; };
	void SetScale(uint8 Scale) { m_scale = Scale; };

	uint16	GetSpeed()		{ return m_speed; };
	uint8	GetLevel()		{ return m_level; };
	uint8	GetTrait(int id){ return m_trait[id]; };
	uint16	GetModel()		{ return m_model; };
	uint8	GetScale()		{ return m_scale; };
	uint8	GetSex()		{ return m_sex; };

	virtual void RemoveFromWorld();
	bool IsMoving()				{ return m_ismoving; };
	void SetMoving(bool move)	{ m_ismoving=move; };
	
	void SendGraphicalRevision();

	/************************/
	/* COMBAT               */
	/************************/
	uint8 m_Ranklevel;
	void SendAttackState(Unit * Victim,uint16 damage,uint16 mitiged,GameData::CombatEvent Event = GameData::COMBATEVENT_HIT,AbilityInfo * Info=NULL);
	void Strike( Unit * pVictim );
	void DealDamage(Unit * Victim,uint16 damage,AbilityInfo * Info=NULL);
	bool canReachWithAttack(Unit * pVictim);
	virtual Unit * GetTarget() = 0;

	/* Health */
	uint32 GetHealth()		{ return m_health; };
	uint32 GetBaseHealth()	{ return m_maxhealth; };
	uint8 GetHealthPct()	{ return IsDead() ? 0 : (m_health* 100)/GetTotalHealth(); };
	bool IsDead()			{ return (m_health <= 0); };
	uint32 GetTotalHealth() { return m_maxhealth+m_bonushealth; };
	
	uint32 m_nextHpRegen;
	uint32 m_nextApRegen;
	void RegenerateHealth();
	void RegenerateAP();

	uint16 GetAP() { return m_actionpoints; }
	uint16 GetMaxAP() { return m_maxactionpoints; }
	bool RemoveAP(uint16 counts);

	void setInFront(Object * target);

	void SetUnitDeath(Unit * killer);
	
	AIInterface * m_aiInterface;
	AIInterface * GetAIInterface()	{ return m_aiInterface; };
	CombatManager * CombatStatus;

	
	/* Loot */
	Loot * GetLoot()			{ return m_loot; };
	void SetLoot(Loot * loot)	{ m_loot=loot; };
	void SendLootTo(Player * Plr,Struct_f_interract interract);
	void SetLootable(bool loot,Player * Plr);

	void RezUnit();

	/************************************************************************/
	/* QUEST																*/
	/************************************************************************/
	// Starter
	list<QuestInfo*> * m_quests;
	QuestInfo * GetQuest(uint32 questid);
	bool HasQuests()		{ return m_quests != NULL; };
	uint32 NumOfQuests()	{ return uint32(m_quests->size()); }
	list<QuestInfo *>::iterator QuestsBegin()		{ return m_quests->begin(); };
	list<QuestInfo *>::iterator QuestsEnd()			{ return m_quests->end(); };
	void SetQuestList(list<QuestInfo*>* qst_lst)	{ m_quests = qst_lst; };

	bool HasQuestForPlayer(Player * Plr);
	bool HasQuestDoneForPlayer(Player * Plr);
	bool HasQuestToDoneForPlayer(Player * Plr);

	list<QuestInfo*> * m_finish;
	bool HasFinisher() { return m_finish != NULL; }
	uint32 NumOfFinisher() { return uint32(m_finish->size()); }
	void SetQuestFinish(list<QuestInfo*>* qst_lst)	{ m_finish = qst_lst; };
	list<QuestInfo *>::iterator QuestsFinishBegin()	{ return m_finish->begin(); };
	list<QuestInfo *>::iterator QuestsFinishEnd()	{ return m_finish->end(); };

	uint32 m_updatetime;

	PQuest * m_pq;
	void SetPublicQuest(PQuest * pq) { m_pq=pq; }
	PQuest * GetPublicQuest() { return m_pq; }

	AreaInfo * m_Area;
	AreaInfo * GetArea() { return m_Area; }
	void SetArea(AreaInfo * Info) { m_Area=Info; }

	/************************************************************************/
	/* ABILITY																*/
	/************************************************************************/
	Ability * GetCurrentAbility();
	bool IsCasting() { return GetCurrentAbility() != NULL; }


	/************************************************************************/
	/* AREA																	*/
	/************************************************************************/
	void UpdateArea(uint32 tdiff);


protected:

	uint16	m_speed;	// Vitesse de déplacement
	uint8	m_level;	// Niveau de l'unité
	uint8	m_trait[8]; // Trait du visage
	uint16	m_model;	// ModelID de l'unité
	uint8	m_scale;	// Taille du personnage
	uint8	m_sex;		// 0 = Homme, 1 = Femme

	uint32	m_health;	// Vie Actuel de l'unité
	uint32	m_maxhealth;// Vie de base de l'unité;
	uint32	m_bonushealth;	// Bonus de vie de l'unitée
	uint16	m_actionpoints;	// Nombre de points d'action
	uint16	m_maxactionpoints;	// Nombre Maximum de points d'action

	uint32	m_nextgraphical;	// TODO
	bool	m_ismoving;		// Si l'unité est en mouvement
	GameData::InteractType m_interract;	// Interract type , Vendors , Trainer , Flight master ....
	Loot * m_loot;		// Loot de l'unité

	uint32 m_nextrespawn;// Temps de respawn de l'unité

	uint32 m_nextareaupdate;	// Temps de check des Quest/chapter/etcetc
};


#endif