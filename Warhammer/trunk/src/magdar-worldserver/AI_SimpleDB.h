#ifndef AI_SIMPLEDB_H
#define AI_SIMPLEDB_H

class SERVER_DECL Simple_DB_AI : public ScriptedAI
{
	enum Simple_DB_AI_Event_List
	{
		EVENT_UPDATE = 0,
		EVENT_ENTERCOMBAT = 1,
		EVENT_DAMAGETAKEN = 2,
		EVENT_LEAVECOMBAT = 3,
		EVENT_DEATH = 4,
		EVENT_KILLEDUNIT = 5
	};

	// TODO : Ajouter plus de types de params
	enum Simple_DB_AI_ActionParam
	{
		PARAM_DOMELEE = 0,
		PARAM_DORANGED = 1,
		PARAM_TIMED = 2,
		PARAM_HEALTH_PERCENT = 3,
		PARAM_MANA_PERCENT = 4,
		PARAM_ENNEMI_COUNT = 5,
		PARAM_TARGET_HEALTH_PERCENT = 6,
		PARAM_RANDOM = 7
	};

	enum Simple_DB_AI_ActionType
	{
		ACTION_CAST = 0,
		ACTION_MELEE = 1,
		ACTION_SAY = 2
	};

	enum Simple_DB_AI_TargetType
	{
		TARGET_SELF = 0,
		TARGET_ENNEMY = 1
	};

	struct Simple_DB_AI_Event
	{
		uint8 EventType;
		uint8 EventParam;

		uint8 EventActionType;

		uint32 EventData1;
		uint32 EventData2;
		uint32 EventData3;
		uint32 EventData4;
		uint32 EventData5;
		uint32 EventData6;

		std::string StringData1;
		std::string StringData2;
		std::string StringData3;

		boost::timer tTimer;
	};

private:
	std::vector<Simple_DB_AI_Event*> m_eventstack;

public:

	Simple_DB_AI(Creature* c) : ScriptedAI(c) {}

	void LoadFromDB(uint32 CreatureID);
	void DoAction(Simple_DB_AI_Event* Evt);
	void HandleEvent(Simple_DB_AI_Event* Evt);
	void OnUpdate();
	void OnEnterCombat();
	void OnDamageTaken();
	void OnLeaveCombat();
	void OnDeath();
	void OnKilledUnit();
};
/*
inline void Get_SimpleDBAI(Creature* c)
{
	Simple_DB_AI* pDBAI = new Simple_DB_AI(c);
	pDBAI->LoadFromDB(c->GetEntry());

	ScriptedAI* pAI = (ScriptedAI*)(pDBAI);
	c->m_script = pAI;
}

inline void AddAI_SimpleDB()
{
	scriptmgr.RegisterScript("db_creature", Get_SimpleDBAI);
};
*/
#endif
