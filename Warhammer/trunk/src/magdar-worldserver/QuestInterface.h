#ifndef QUESTINTERFACE_H
#define QUESTINTERFACE_H

class Player;
struct QuestObjectif;
enum Quest_Objectif;

struct QuestAchievement
{
	QuestInfo * Quest;

	uint32 questid;
	bool done;
	
	vector<QuestObjectif*> m_acheved; // Listes des objectif achevés (par terminé)
	vector<const ItemProto*> m_tochoice; // Liste des items a choisir
	list<uint8> m_selected; // Liste des items sélectionné

	QuestObjectif * GetObjByGuid(uint32 guid); // Sélectionne un objectif avec son guid

	void CopyFromQuest(); // Copie tous les objectifs de la quète
	void CheckDone(); // Vérifi si tous les objectifs sont finis
};

class QuestInterface
{
public:

	void Init(Player * Plr);

	void AcceptQuest(uint32 questid);
	void RemoveQuest(uint32 questid);
	void SendDoneQuest(uint32 questid,Creature * Crea);
	void SelectReward(uint32 questid,uint8 num);

	void AddDoneQuest(uint32 entry)
	{ 
		if(!HasDoneQuest(entry)) 
		{
			m_lock.Acquire();
			m_questdone.push_back(entry);
			m_lock.Release();
		}
	}
	bool HasDoneQuest(uint32 entry)
	{
		bool ok=false;
		m_lock.Acquire();

		if(m_questdone.size())
			for(list<uint32>::iterator itr=m_questdone.begin();itr!=m_questdone.end();++itr)
				if( (*itr) == entry)
				{ok=true;break;}

		m_lock.Release();

		return ok;
	}

	bool HasQuest(uint32 questid);
	QuestAchievement * GetQuestAchievement(uint16  questid)
	{
		QuestAchievement * Ach=NULL;

		m_lock.Acquire();

		if(m_quests.size())
		{
			for(list<QuestAchievement*>::iterator itr= m_quests.begin();itr!=m_quests.end();++itr)
				if( (*itr)->questid == questid )
					{Ach=(*itr);break;}
		}

		m_lock.Release();

		return Ach;
	}

	bool HasAchievedQuest(uint32 questid)
	{
		m_lock.Acquire();
		if(m_quests.size())
		{
			for(list<QuestAchievement*>::iterator itr= m_quests.begin();itr!=m_quests.end();++itr)
				if( (*itr)->questid == questid )
				{
					m_lock.Release();
					return (*itr)->done;
				}
		}	
		m_lock.Release();
		return false;
	}

	void SendAllQuest();
	void DoneQuest(uint16 questid);

	void SendQuestStatus(QuestInfo * Info,GameData::QuestCompletion status);
	void SendQuest(QuestAchievement* Info);
	void SendQuestUpdate(uint16 questid,uint8 objectif,uint8 count,bool done);


	void Save();

	// HANDLER
	void OnSpeackToCreature(Creature * Crea);
	void OnKillCreature(Creature * Crea);

	void onHandleEvent(Quest_Objectif objectif,uint32 count,Unit * unit=NULL,ItemProto * Proto=NULL,Player * Plr=NULL);

private:

	Player * m_player; // Owner
	

	FastMutex m_lock;
	list<uint32> m_questdone;
	list<QuestAchievement*> m_quests;
};


#endif