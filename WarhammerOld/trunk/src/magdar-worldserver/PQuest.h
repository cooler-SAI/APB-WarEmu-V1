#ifndef PQUEST_H
#define PQUEST_H

class Unit;
class Player;
class PQScript;
struct ItemProto;
struct InfluenceInfo;

struct QuestObjectif;
enum Quest_Objectif;

#define TIME_EACH_STAGE 10000	// 10 secondes d'intervalle entre chaque stage
#define TIME_PQ_RESET 160		// 2 Minutes entre chaque reset

struct PQStageInfo // Contient tous les objectifs d'un stage
{
	uint32 entry;
	string stageName;
	string description;

	vector<QuestObjectif*> Objectifs;

	uint32 tok_completed;
	bool done;

	void AddObjectif(QuestObjectif* Obj);
	void Reset();
	void CheckDone();
};

struct PQInfo // Contient toutes les information de la quètes
{
	uint32 entry;
	string name;
	uint32 realm;
	uint32 level;
	uint16 zoneid;
	uint32 pinx,piny;
	uint32 tok_discovered;
	uint32 tok_unlocked;
	InfluenceInfo * Info;

	vector<PQStageInfo*> m_stages;
};

struct PQScore // Contient toutes les informations des joueurs pour générer les récompenses
{
	uint32 character_id;
	uint32 killcount;
	uint32 healcounts;
	uint32 bonus;
	uint32 damages;

	bool participe; // True si le joueurs participe au butin
};

class PQuest
{
public:
	PQuest(PQInfo *Info,ZoneMgr * Zone);

	// Stage Event
	void Start();		// Démmare la quète
	void NextStage();	// Démmare le prochain stage
	void End();			// Génère les récompenses;

	void Update(uint32 tdiff); // Update de la quètes;
	PQStageInfo * GetStageInfo() { return m_currentstage; }

	// Players
	void AddPlayer(Player * Plr);		// Ajoute un joueur a la quète
	void RemovePlayer(Player * Plr);	// Supprime un joueur de la quète
	bool HasPlayer(Player *Plr);		// True si le joueur est dans la quète
	bool IsInrange(Object *Obj);		// True si l'objet est dans le range de la quète
	uint16 GetPlayerCounts();			// Retourne le nombre de joueurs

	void HandleEvent(Quest_Objectif objectif,uint32 count,Unit * unit=NULL,ItemProto * Proto=NULL,Player * Plr=NULL);
	void PlayerKillUnit(Player *Plr,Unit *unit);
	void PlayerDie(Player *Plr,Unit *Killer);
	void PlayerDealHeal(Player *Plr,Unit *Healed,uint32 count);
	void PlayerDealDamage(Player *Plr,Unit *target,uint32 counts);
	void UnitDie(Unit *unit,Unit *Killer);

	// Accesseur
	string GetName()	{ return m_info->name;	}
	uint32 GetEntry()	{ return m_info->entry; }
	uint32 GetRealm()	{ return m_info->realm; }
	uint32 GetLevel()	{ return m_info->level; }
	uint32 GetZoneId()	{ return m_info->zoneid; }
	uint32 GetPinX()	{ return m_info->pinx; }
	uint32 GetPinY()	{ return m_info->piny; }
	InfluenceInfo * GetInfluenceInfo() { return m_info->Info; };
	PQScore * GetPQScore(uint32 charid) 
	{ 
		PQScore * Score = NULL;
		M_players.Acquire();
		Score = m_scores[charid]; 
		M_players.Release();
		return Score;
	};

	// Sender
	void sendPQuest(Player *Plr);		// On envoi les informations de la quètes
	void sendPQuestUpdate(Player *Plr,uint32 guid,uint8 count); // On envoi les informations de l'objectif
	void sendLeave(Player *Plr);		// Packet de leave

private:
	bool m_started;			// True si la quète a démarée,false si aucun joueur n'est encore entré dedans
	uint32 m_endtime;		// Temps avant reset de la quète
	uint32 m_stagenum;		// Numéro du stage

	PQInfo * m_info;		// Informations sur la quète
	ZoneMgr * m_zoneMgr;	// Zone sur laquelle se trouve la quète

	Mutex M_players;
	map<uint32,Player*> m_players; // Listes des joueurs
	map<uint32,PQScore*> m_scores; // Score des joueurs pour tirage de fin;
	
	PQStageInfo * m_currentstage; // Stage en cours
	vector<PQStageInfo*> m_stages; // Liste des stages

	PQScript *m_script;
};



#endif