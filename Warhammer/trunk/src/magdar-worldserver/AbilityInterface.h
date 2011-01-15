#ifndef ABILITYINTERFACE_H
#define ABILITYINTERFACE_H

struct AbilityInfo;

class Player;
class Unit;
class Object;
class Creature;
class AbilityTarget;


// Structure contenant les information sur les abilité des joueurs;
// Lorsqu'un joueur cast une ability , cette structure sert a garder les données , comme le cooldown restant etc etc;
// Appeller par un Update , le cooldown se décrémente jusqu'a 0 pour pouvoir être ensuite caster
// Cette structure sert aussi a implémenter les informations de base, comme les abilités passive qui augmente les dmg ou diminue le casttime;

struct AbilityPlayer
{
	uint32 entry;
	uint8 level;

	AbilityInfo * Info;
	uint32 m_cooldown;
};

class AbilityInterface
{
public:

	void Init(Player * Plr);
	void Save();
	void UpdateLevel(uint8 level);

	void SendAllAbility();

	bool HasAbility(uint32 entry);
	bool HasAbility(AbilityInfo * Info);

	GameData::AbilityResult CanCast(uint32 entry);
	GameData::AbilityResult CanCast(AbilityPlayer * Info);

	bool IsCasting();

	AbilityPlayer * GetAbility(uint32 entry);
	AbilityPlayer * GetAbility(AbilityInfo * Info);

	void Update(uint32 tdiff); // Mettre a jour les spells en cours de cast;


	void StartCast(uint32 entry);
	void StartCast(AbilityPlayer * Ap);

	Ability * GetCurrentAbility() { return m_currentAbility; }
	void SetCurrentAbility(Ability * Ab);

	void SetAbilityTimer(AbilityPlayer * Ap);
	void SendAbilityTimer(uint32 entry);
	void SendCastResult(GameData::AbilityResult Result);

private:
	Player * m_player; // Owner de l'interface;

	vector<AbilityPlayer*> m_ability;

	Ability * m_currentAbility;

	uint32 m_globalcooldown;
};


#endif