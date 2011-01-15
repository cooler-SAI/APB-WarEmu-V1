#include "../../StdAfx.h"

class RavenPQuest : public PQScript
{
public:

	ADD_PQUEST_FACTORY_FUNCTION(RavenPQuest);
	RavenPQuest(PQuest * Quest) : PQScript(Quest) 
	{
		
	}

	void onStart() 
	{
		Log.Info("PQScript","Raven Host Vanguard Starting !!");
	}
	void onNextStage() 
	{
		Log.Info("PQScript","Raven Host Vanguard Next Stage !!");
	}
	void onEnd() 
	{
		Log.Info("PQScript","Raven Host Vanguard End Stage !!");
	}


	void onAddPlayer(Player * Plr) 
	{
	
	}
	void onRemovePlayer(Player * Plr)
	{
	
	}

	void onPlayerKillUnit(Player *Plr,Unit *unit) 
	{
	
	}
	void onPlayerDie(Player *Plr,Unit *Killer) 
	{
	
	}
	void onPlayerDealHeal(Player *Plr,Unit *Healed,uint32 count) 
	{
	
	}
	void onPlayerDealDamage(Player *Plr,Unit *target,uint32 counts) 
	{
	
	}
	void onUnitDie(Unit *unit,Unit *Killer) 
	{
	
	}

	void onEvent()
	{
	
	}
	void onUpdate(uint32 tdiff) 
	{
	
	}
};
void RegisterRaventHostScript(ScriptMgr *Mgr)
{
	Mgr->RegisterPQuestScript(165,&RavenPQuest::Create);  // Raven Host Vanguard
}