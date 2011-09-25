#ifndef AI_GENERICCREATURE_H
#define AI_GENERICCREATURE_H

class Generic_Creature_AI : public ScriptedAI
{
public:
	Generic_Creature_AI(Creature* c) : ScriptedAI(c) {}

	void OnUpdate()
	{
		DoMeleeAttack();
	}

	void OnEnterCombat()
	{

	}

	void OnDamageTaken()
	{

	}

	void OnLeaveCombat()
	{

	}

	void OnDeath()
	{

	}

	void OnKilledUnit()
	{

	}
};
/*
void Get_GenericCreatureAI(Creature* c)
{
	c->m_script = (ScriptedAI*)(new Generic_Creature_AI(c));
}

void AddAI_GenericCreature(ScriptMgr *mgr)
{
	mgr->RegisterScript("generic_creature", &Get_GenericCreatureAI);
}
*/
#endif
