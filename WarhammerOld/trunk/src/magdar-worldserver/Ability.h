#ifndef ABILITY_H
#define ABILITY_H

class Object;
class Unit;
class Player;
class Creature;
class GameObject;
class AbilityTarget;
struct AbilityInfo;
struct AbilityPlayer;

#define MIN_COOLDOWN 1300 // 1.5 sec

struct AbilityInfo
{
	uint32 entry;	// Numéro de l'ability
	uint8 level;	// Niveau de l'ability
	GameData::AbilityType Type; // Type de l'ability

	uint32 minrange; // Min distance
	uint32 maxrange; // Max distance

	uint32 cooldown; // Temps de recharge
	uint32 casttime;	// Temps de cast
	uint32 targettype;	// Type de target , direct , AOE , buff , etc etc
	uint32 damage;		// Domage instant

	uint16 actionpoints;
};


class Ability
{
public:

	Ability(uint32 abilityid,uint8 level);
	Ability(AbilityInfo * info);
	void Init(Object * Caster);

	void ApplyPlayerAbility(AbilityPlayer * Patch);

	void GenerateTargets();
	void Cast(bool check=true);
	GameData::AbilityResult StartCast();
	void Cancel();

	GameData::AbilityResult CanCast();

	void Update(uint32 tdiff);
	void Delete() { canceled=true;delete this; }


	// Sender
	void SendStartingCast(Unit * target);
	void SendEndedCast(Unit * target);
	void SendSpellDamage(Unit * target,uint32 damage);

	AbilityInfo * GetInfo() { return m_info; }

	
	bool IsDone() { return done; }
	bool IsCanceled() { return canceled; }


	uint32 CalculAbilityDamage(Unit * target);

private:
	// Caster Pointer
	Object * m_objcaster;
	Unit * m_unitcaster;
	Player * m_plrcaster;
	Creature * m_creacaster;
	GameObject * m_gocaster;

	AbilityTarget * m_target; // Target pour l'ability
	AbilityInfo * m_info;	// Information sur le spell en cours
	AbilityPlayer * m_patch; // Information a ajouter au spell , différent celon le caster , null pour les creatures/go

	// Cast Timer
	uint32 m_endcast;

	bool canceled;
	bool done;
};

#endif