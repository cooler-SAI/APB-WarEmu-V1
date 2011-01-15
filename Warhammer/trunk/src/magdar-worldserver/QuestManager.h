#ifndef QUESTMANAGER_H
#define QUESTMANAGER_H

class Player;
class Creature;
class ScriptedQuest;

struct ItemProto;
struct CreatureProto;
struct PQInfo;

enum Quest_Objectif
{
	QUEST_UNKNOWN=0,
	QUEST_SPEACK_TO=1,
	QUEST_KILL_MOB=2,
	QUEST_USE_GO=3,
	QUEST_GET_ITEMS=4,
	QUEST_KILL_PLAYERS=5,
	QUEST_PROTECT_UNIT=6,
};

struct QuestObjectif
{
	Quest_Objectif objectif;

	uint32 guid;		// Guid de l'objectif pour sauvegarder l'avancement des joueurs;

	string Objective;	// Le texte de l'objectif

	const CreatureProto* CreaProto;
	const CreatureSpawn* CreaSpawn;
	const ItemProto*	 ItemProto;

	uint8 count;
	uint8 num;
	bool done;

	QuestObjectif * m_base; // Peut contenir un objectif de comparaison pour check si il est fini

	void CheckDone() // Vérifie si tous les objectif de comparaison sont accomplis;
	{
		if(m_base)
			if(count >= m_base->count)
				done=true;
	}

	void CopyFromBase()
	{
		objectif =	m_base->objectif;
		ItemProto = m_base->ItemProto;
		CreaSpawn = m_base->CreaSpawn;
		CreaProto = m_base->CreaProto;
		done =		false;
		num =		m_base->num;
		count =		0;
		Objective =	m_base->Objective;
		guid =		m_base->guid;
	}

};

struct QuestInfo
{
	uint32	entry;
	string	name;
	uint32	type;
	uint8	level;
	uint32	prevquest;
	string	description;
	string	particular;
	
	// Rewards
	uint32	m_xp;
	uint32	m_gold;
	vector<const ItemProto*> m_given;
	vector<const ItemProto*> m_choice;

	vector<QuestObjectif*> m_objectif;
	uint8 obj_count;

	ScriptedQuest * m_script;
};

class SERVER_DECL QuestMgr : public Singleton< QuestMgr >
{
	public:
		QuestMgr();

		void GererateObjectif(QuestObjectif *obj,uint32 objentry);


		//////////////////////
		// Basic Quests
		//////////////////////
		void LoadQuests();
		void LoadQuestsObjectives();
		void LoadQuestsStarter();
		void LoadQuestsFinisher();

		bool CanStartQuest(Player * Plr,QuestInfo * Info);		// Check si le joueurs peut commencer la quète
		void BuildPreQuest(Buffer *b,QuestInfo *Info,bool particular=true);
		void BuildQuestInfo(Buffer *b,QuestInfo *Info,Player * Plr);
		void BuildInterractQuest(Creature * Crea,Player * Plr);
		void BuildQuestForPlayer(uint32 questid,Creature * Crea,Player * Plr);
		list<QuestInfo*> * GetCreatureQuestList(uint32 entry)		{ return &m_starter[entry]; }	// Renvoi la liste des quètes de la créature
		list<QuestInfo*> * GetCreatureQuestFinisher(uint32 entry)	{ return &m_finisher[entry]; }	// Renvoi la liste des quètes a finir par cette créature
		QuestInfo * GetQuestInfo(uint32 questid) { return m_quests[questid]; }		// Renvoi l'info de la quète
		vector<const ItemProto*> GetItemChoice(Player * Plr,QuestInfo * Info);		// Génère les items de la quète

		//////////////////////
		// Public Quests
		//////////////////////
		void LoadPQuests();
		list<PQuest*> GeneratePQuest(ZoneMgr * Zone);		// Génère les class pour la zone
		PQInfo * GetPQInfo(uint32 questid,uint16 zoneid=0);	// Renvoi les info d'une quète


	private:

	//questentry,QuestInfo
	tr1::unordered_map<uint32, QuestInfo*>		m_quests;	// Liste des quètes (entry,quest)
	tr1::unordered_map<uint32,list<QuestInfo*> > m_starter;	// Liste des starters (entry,quests)
	tr1::unordered_map<uint32,list<QuestInfo*> > m_finisher;	// Liste des finishers (entry,quests)
	tr1::unordered_map<uint16,list<PQInfo*> >	m_pquests;	// Liste des quètes public (zone,quests)
};

#define questmgr QuestMgr::getSingleton( )

#endif