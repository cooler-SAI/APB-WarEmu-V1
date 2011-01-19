#include "StdAfx.h"

class VillagerQuest : public ScriptedQuest
{
public:

	VillagerQuest() : ScriptedQuest() {}

	void OnStart(Player * Plr,QuestAchievement * Ach) 
	{
		Log.Debug("VillagerQuest","OnStart");
	}
	void OnComplete(Player * Plr,QuestAchievement * Ach) 
	{
		Log.Debug("VillagerQuest","OnComplete");
	}
	void onCancel(Player *Plr,QuestAchievement * Ach) 
	{
		Log.Debug("VillagerQuest","onCancel");
	}
	void onSpeackTo(Player *Plr,Unit * speacked,QuestAchievement * Ach) 
	{
		Log.Debug("VillagerQuest","onSpeackTo");
	}
	void onUnitKill(Player *Plr,Unit * Killed,QuestAchievement * Ach) 
	{
		Log.Debug("VillagerQuest","onUnitKill");
	}
	void onItemPickup(Player *Plr,uint32 entry,QuestAchievement * Ach)
	{
		Log.Debug("VillagerQuest","onItemPickup");
	}
	void onGetReward(Player *Plr,QuestAchievement * Ach) 
	{
		Log.Debug("VillagerQuest","onGetReward");
	}

};

void RegisterVillager(ScriptMgr *Mgr)
{
	ScriptedQuest *VillagerScript = (ScriptedQuest*) new VillagerQuest();
	Mgr->RegisterQuestScript(30001, VillagerScript); // GrimmenHagen Marauder Torchbearer
}