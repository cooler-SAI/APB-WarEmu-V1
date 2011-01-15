#ifndef PQSCRIPT_H
#define PQSCRIPT_H

class Unit;
class Player;
class PQuest;

class PQScript
{
public:
	PQScript(PQuest * Quest) :
		m_quest(Quest),m_eventtime(0),m_nextevent(0)
	{
	
	} 

	virtual void onStart() {}
	virtual void onNextStage() {}
	virtual void onEnd() {}

	// SCRIPT
	virtual void onAddPlayer(Player * Plr) {}
	virtual void onRemovePlayer(Player * Plr) {}

	virtual void onPlayerKillUnit(Player *Plr,Unit *unit) {}
	virtual void onPlayerDie(Player *Plr,Unit *Killer) {}
	virtual void onPlayerDealHeal(Player *Plr,Unit *Healed,uint32 count) {}
	virtual void onPlayerDealDamage(Player *Plr,Unit *target,uint32 counts) {}
	virtual void onUnitDie(Unit *unit,Unit *Killer) {}

	virtual void onEvent() {} // Pour enregistrer un Event;
	virtual void onUpdate(uint32 tdiff) {} // Update régulié

	void Update(uint32 tdiff);

private:
	PQuest * m_quest;

	uint32 m_eventtime;
	uint32 m_nextevent;

	void RegisterEvent(uint32 time);
	void ModifyEventTime(uint32 time);
	void StopEvent();
};

#endif