#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

class Object;
class Creature;
class Unit;
class Player;

#define LEAVE_TIME 5000 // Temps avant ke le joueur passe hors combat


// Combat Manager sert a enregistré toutes les informations durant un combat.
// Si le joueurs est attaqué , si il attaque

class CombatManager
{
	Unit* m_Unit;

	typedef set<uint16> AttackerMap;
	typedef set<uint16> HealedSet;		// Must Be Players!

	HealedSet m_healers;
	HealedSet m_healed;
	AttackerMap m_attackTargets;

	uint16 m_primaryAttackTarget;
	bool m_lastStatus;

public:
	AttackerMap m_attackers;

	CombatManager() : m_lastStatus(false), m_primaryAttackTarget(0) {}
	void AddAttackTarget(uint16 oid);						// this means we clicked attack, not actually striked yet, so they shouldn't be in combat.
	void ClearPrimaryAttackTarget();								// means we deselected the unit, stopped attacking it.

	void OnDamageDealt(Unit * pTarget);								// this is what puts the other person in combat.
	void WeHealed(Unit * pHealTarget);								// called when a player heals another player, regardless of combat state.

	void RemoveAttacker(Unit * pAttacker,uint16 oid);				// this means we stopped attacking them totally. could be because of deaggro, etc.
	void RemoveAttackTarget(Unit * pTarget);						// means our DoT expired.

	void UpdateFlag();												// detects if we have changed combat state (in/out), and applies the flag.

	bool IsInCombat();												// checks if we are in combat or not.

	void OnRemoveFromWorld();										// called when we are removed from world, kills all references to us.
	
	void Vanished()
	{
		ClearAttackers();
		ClearHealers();
	}

	uint16 GetPrimaryAttackTarget() { return m_primaryAttackTarget; }
	void SetUnit(Unit * p) { m_Unit = p; }
	void TryToClearAttackTargets();									// for pvp timeout
	void AttackersForgetHate();										// used right now for Feign Death so attackers go home

protected:
	bool InternalIsInCombat();										// called by UpdateFlag, do not call from anything else!
	bool IsAttacking(Unit * pTarget);								// internal function used to determine if we are still attacking target x.
	void AddAttacker(uint16 oid);									// internal function to add an attacker
	void RemoveHealed(Unit * pHealTarget);							// usually called only by updateflag

	void ClearHealers();											// this is called on instance change.
	void ClearAttackers();											// means we vanished, or died.
	void ClearMyHealers();
};

#endif