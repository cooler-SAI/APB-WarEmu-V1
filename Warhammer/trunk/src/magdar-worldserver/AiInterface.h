#ifndef AIINTERFACE_H
#define AIINTERFACE_H

class Object;
class Creature;
class Unit;
class Player;

#define UNIT_MOVEMENT_INTERPOLATE_INTERVAL 400/*750*/ // ms smoother server/client side moving vs less cpu/ less b/w
#define TARGET_UPDATE_INTERVAL 600 // ms
#define oocr 50.0f // out of combat range
#define OUT_TIME 5

enum AI_State
{
	STATE_IDLE,
	STATE_ATTACKING,
	STATE_CASTING,
	STATE_FLEEING,
	STATE_FOLLOWING,
	STATE_EVADE,
	STATE_MOVEWP,
	STATE_FEAR,
	STATE_WANDER,
	STATE_STOPPED,
	STATE_SCRIPTMOVE,
	STATE_SCRIPTIDLE
};

enum AiEvents
{
	EVENT_ENTERCOMBAT,
	EVENT_LEAVECOMBAT,
	EVENT_DAMAGETAKEN,
	EVENT_FEAR,
	EVENT_UNFEAR,
	EVENT_FOLLOWOWNER,
	EVENT_WANDER,
	EVENT_UNWANDER,
	EVENT_UNITDIED,
};

struct AttackerInfo
{
	uint32 AggroValue; // Valeur d'aggro courante
	uint16 Class; // Classe du joueur à stocker si on veut faire des modifier d'aggro selon la classe
	bool Taunt; // Bloque l'aggro sur celui qui taunt le mob 
	void* UserData; // Utilisable pour les scripts pour stocker des données dans la liste d'attacker
};

class SERVER_DECL AIInterface
{
public:

	AIInterface();
	~AIInterface();

	AI_State m_state;
	void Init(Unit * Owner) { m_Unit=Owner;m_state=STATE_IDLE; };

	void Update(uint32 ptime); // Met tous ce qu'il faut a jours;
	void _UpdateTarget(uint32 ptime); // Cherche un target;
	void _UpdateCombat(uint32 ptime); // Met a jour le combat , et attaque si il faut;
	void _UpdateMovement(uint32 ptime); // Met a jour le déplacement et déplace la créature si il faut

	void AIDoMelee(); // Attaque en mélée et suit le joueur
	void AddAttacker(Unit * pUnit); // Permet d'ajouter un attacker à la liste d'attacker de la créature
	void CalcAggressionAdd(Unit * pUnit, uint32 AggroCount); // Ajoute de l'aggro à la créature
	void CheckForTargets(); // Cherche une cible attaquable dans l'aggro list
	void DeleteAggressorList(); // Cleanup de la liste des attackers
	void RemoveFromAggressorList(Unit * pUnit); // Retire une Unit de la liste d'aggressor;
	void GeneratePeriodicAggression();

	void HandleEvent(AiEvents event,Unit * pUnit,uint32 misc);
	void AttackReaction(Unit * pUnit,uint32 damage);

	void _CalcDestinationAndMove(Unit * target, float dist);
	void UpdateMove();
	bool CanAttack();

	Unit* GetTarget() { return m_target; };
	std::map<Unit*,AttackerInfo*> GetTargets() { return m_targets; }

	// Loot
	Player * m_firstattacker; // Le loot appartiend donc au premier qui touche le mob

	LocationVector m_dest; // Point de destination pour le déplacement;
protected:

	Unit * m_Unit;

	// Position
	LocationVector m_spawn; // Point de spawn;
	LocationVector m_return; // Point de retour quand en mouvement
	uint8 m_returnregion; // region de retour;
	uint16 m_returnoffx,m_returnoffy; // Points de retour pour les offset;
	LocationVector m_next; // Prochaine position

	// Combat
	Unit* m_target; // Target en cours;
	boost::timer m_movetimer; // Timer pour l'update du déplacement

	// Unit , Tous les dommage subbit;
	std::map<Unit*,AttackerInfo*> m_targets;
	uint32 m_nextUpdateTarget; // Temps avant le prochain update;
	uint32 m_nextAttack; // Temps avant la prochaine attaque
	uint32 m_totalMoveTime;
	int m_outcount; // temps de out of combat avant que la créature décroche

	Mutex m_lock;
};


#endif