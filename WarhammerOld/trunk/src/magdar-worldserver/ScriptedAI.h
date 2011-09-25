#ifndef SCRIPTEDAI_H
#define SCRIPTEDAI_H

class Object;
class Creature;
class Unit;
class Player;
class ScriptedAI;

struct ScriptedObject
{
	ScriptedAI* m_AI;
	std::string m_ScriptName;
};

typedef void(*destcallback)();

class SERVER_DECL ScriptedAI
{
public:
	ScriptedAI(Creature* c) : m_creature(c) {}

	virtual void OnUpdate() = 0;
	virtual void OnEnterCombat() = 0;
	virtual void OnDamageTaken() = 0;
	virtual void OnLeaveCombat() = 0;
	virtual void OnDeath() = 0;
	virtual void OnKilledUnit() = 0;

	void DoMeleeAttack();
	void DoRangedAttack();

	void Say(std::string Text);
	void Yell(std::string Text);
	void Cast(Unit* pTarget, uint32 SpellID);
	void MoveTo(uint16 x, uint16 y, uint16 z, destcallback DestinationReachedCallback);

	Unit* GetRandomTarget();
	Unit* GetTopAggroTarget();
	Unit* GetLastAggroTarget();

	std::vector<Unit*> GetTargets();
	std::vector<Unit*> GetTargetsInArea(float Radius);

protected:
	Creature* m_creature;
	destcallback m_dstcallback;
};


#endif
