#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

class ScriptedAI;
class ScriptedQuest;
class ScriptedItem;
class PQScript;
class Player;
class Item;
class Creature;

#define ADD_CREATURE_FACTORY_FUNCTION(cl) static ScriptedAI * Create(Creature* c) { return new cl(c); }
#define ADD_ITEM_FACTORY_FUNCTION(cl) static ScriptedItem * Create(Player* Plr,Item * Itm) { return new cl(Plr,Itm); }
#define ADD_PQUEST_FACTORY_FUNCTION(cl) static PQScript * Create(PQuest * Quest) { return new cl(Quest); }


typedef ScriptedItem*(*pItemScript)(Player* Plr,Item* Itm);
typedef ScriptedAI*(*pAiScript)(Creature* pCreature);
typedef ScriptedQuest*(*pQuestScript)();
typedef PQScript*(*pPQScript)(PQuest * Quest);

class SERVER_DECL ScriptMgr : public Singleton< ScriptMgr >
{
public:
	void LoadScript();

	void RegisterAiScript(std::string name, pAiScript callback);
	void RegisterItemScript(std::string name,pItemScript Isc);

	void RegisterQuestScript(uint32 entry,ScriptedQuest * Sc);
	void RegisterPQuestScript(uint32 entry,pPQScript Sc);

	ScriptedAI * GetAiScript(std::string name,Creature * Crea);
	ScriptedItem * GetItmScript(std::string name,Player * Plr,Item * itm);
	PQScript * GetPQScript(uint32 entry,PQuest * Quest);

private:
	list<void*> _handles;

	std::map<std::string, pAiScript> m_AiScript;
	std::map<std::string, pItemScript> m_ItmScript;
	std::map<uint32, pPQScript> m_PQScript;

	set<ScriptedQuest*> m_QuestScript;
};

#define scriptmgr ScriptMgr::getSingleton( )

#endif