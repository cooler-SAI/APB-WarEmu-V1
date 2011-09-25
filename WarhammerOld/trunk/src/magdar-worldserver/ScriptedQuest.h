#ifndef SCRIPTEDQUEST_H
#define SCRIPTEDQUEST_H

struct QuestAchievement;
class Player;
class Unit;

class SERVER_DECL ScriptedQuest
{
public:
	ScriptedQuest() {}

	virtual void OnStart(Player * Plr,QuestAchievement * Ach) {}
	virtual void OnComplete(Player * Plr,QuestAchievement * Ach) {}
	virtual void onCancel(Player *Plr,QuestAchievement * Ach) {}

	// UseGo + SpeackTo
	virtual void onSpeackTo(Player *Plr,Unit * speacked,QuestAchievement * Ach) {}
	// Kill Player + Kill Mob
	virtual void onUnitKill(Player *Plr,Unit * Killed,QuestAchievement * Ach) {}
	virtual void onItemPickup(Player *Plr,uint32 entry,QuestAchievement * Ach) {}


	virtual void onGetReward(Player *Plr,QuestAchievement * Ach) {}
};


#endif