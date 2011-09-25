#include "../../StdAfx.h"

class RavenScript : public ScriptedAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(RavenScript);
	RavenScript(Creature* c) : ScriptedAI(c)
	{

	}

	void OnUpdate()
	{
		DoMeleeAttack();
	}

	void OnEnterCombat()
	{
		Say("You will Die !!");
	}

	void OnDamageTaken()
	{

	}

	void OnLeaveCombat()
	{

	}

	void OnDeath()
	{
		Say("Ho my GOD ! i'm dead !");
	}

	void OnKilledUnit()
	{
		Say("Hahaha , naab !");
	}

};

void RegisterRaventScript(ScriptMgr *Mgr)
{
	Mgr->RegisterAiScript("RavenScript",&RavenScript::Create);
}