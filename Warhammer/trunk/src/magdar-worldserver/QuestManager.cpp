#include "StdAfx.h"

initialiseSingleton(QuestMgr);

QuestMgr::QuestMgr()
{
	LoadQuests();
	LoadQuestsObjectives();
	LoadQuestsStarter();
	LoadQuestsFinisher();

	LoadPQuests();
}
void QuestMgr::LoadQuests()
{
	Log.Debug("QuestMgr","Loading Quests ...");
	
	uint32 errors=0;
	QueryResult * Result = WorldDatabase.Query("SELECT `entry`,`Name`,`Type`,`Level`,`Desc`,`Particular`,`Xp`,`Gold`,`Given`,`Choice`,`PrevQuest` FROM quests");
	if(Result)
	{
		barGoLink bar(Result->GetRowCount());

		do{
			++bar;

			QuestInfo * Info = new QuestInfo;
			Info->entry = Result->Fetch()[0].GetUInt32();
			Info->name = Result->Fetch()[1].GetString();
			Info->type = Result->Fetch()[2].GetUInt32();
			Info->level = Result->Fetch()[3].GetUInt32();
			Info->description = Result->Fetch()[4].GetString();
			Info->particular = Result->Fetch()[5].GetString();
			Info->m_xp = Result->Fetch()[6].GetUInt32();
			Info->m_gold = Result->Fetch()[7].GetUInt32();
			Info->prevquest = Result->Fetch()[8].GetUInt32();
			Info->obj_count = 0;

			string Give = Result->Fetch()[8].GetString();

			//[206003,1],[2006002,10]
			int a = Give.find_first_of("[");
			int b = Give.find_first_of("]");
			int c = Give.find_first_of(",");

			while(a != string::npos && b != string::npos && c != string::npos)
			{
				if( a < c && b > c)
				{
					uint32 entry = atoi(Give.substr(a+1,c).c_str());
					uint32 count = atoi(Give.substr(c+1,b).c_str());

					const ItemProto * proto = objmgr.GetItemProto(entry);

					if(proto)
						Info->m_given.push_back(proto);
					else errors++;
				}
				Give.erase(0,b);

				if(Give.size() > 1)
					Give.erase(0,1); // ,[

				a = Give.find_first_of("[");
				b = Give.find_first_of("]");
				c = Give.find_first_of(",");
			}

			string Choice = Result->Fetch()[9].GetString();
			a = Choice.find_first_of("[");
			b = Choice.find_first_of("]");
			c = Choice.find_first_of(",");
			while(a != string::npos && b != string::npos && c != string::npos)
			{
				if( a < c && b > c)
				{
					uint32 entry = atoi(Choice.substr(a+1,c).c_str());
					uint32 count = atoi(Choice.substr(c+1,b).c_str());

					const ItemProto * proto = objmgr.GetItemProto(entry);

					if(proto)
						Info->m_choice.push_back(proto);
					else errors++;
				}

				Choice.erase(0,b);

				if(Choice.size() > 1)
					Choice.erase(0,2); // ,[

				a = Choice.find_first_of("[");
				b = Choice.find_first_of("]");
				c = Choice.find_first_of(",");
			}

			m_quests[Info->entry] = Info;
			
		}while(Result->NextRow());
	}
	delete Result;

	Log.Success("QuestMgr","Loaded %u Quest. . .",m_quests.size());
	if(errors)
		Log.Error("QuestMgr","%u Quests Errors",errors);
}
void QuestMgr::LoadQuestsObjectives()
{
	Log.Debug("QuestMgr","Loading Quests Objectives ...");

	QueryResult * Result = WorldDatabase.Query("SELECT guid,entry,type,count,objective,obj1,obj2,obj3 FROM quests_objectives");
	uint32 count=0;
	if(Result)
	{
		QuestInfo * Info = NULL;
		do{
			uint32 guid = Result->Fetch()[0].GetUInt32();
			uint32 entry = Result->Fetch()[1].GetUInt32();

			Info = m_quests[entry];

			if(!Info) continue;

			uint32 type = Result->Fetch()[2].GetUInt32();
			uint32 ocount = Result->Fetch()[3].GetUInt32();
			string Objectif = Result->Fetch()[4].GetString();
			string obj1 = Result->Fetch()[5].GetString();

			uint32 obj = atoi(obj1.c_str());

			QuestObjectif * Ob = new QuestObjectif;
			Ob->guid = guid;
			Ob->objectif = Quest_Objectif(type);
			Ob->count = ocount;
			Ob->Objective = Objectif;
			count++;

			GererateObjectif(Ob,obj);

			Ob->num = Info->m_objectif.size();
			Info->m_objectif.push_back(Ob);
			Info->obj_count++;
		}while(Result->NextRow());
	}
	delete Result;

	Log.Success("QuestMgr","Loaded %u Objectives",count);
}
void QuestMgr::LoadQuestsStarter()
{
	Log.Debug("QuestMgr","Loading QuestStarter ...");
	uint32 counts=0;
	QueryResult * Result = WorldDatabase.Query("SELECT entry,creatureId FROM quests_starter");
	if(Result)
	{
		do{
			uint32 entry = Result->Fetch()[0].GetUInt32();
			uint32 crea = Result->Fetch()[1].GetUInt32();

			QuestInfo * Info = m_quests[entry];

			if(Info)
			{
				m_starter[crea].push_back( Info );
				counts++;
			}

		}while(Result->NextRow());
	}
	delete Result;

	Log.Success("QuestMgr","Loaded %u QuestStarter. . .",counts);
}
void QuestMgr::LoadQuestsFinisher()
{
	Log.Debug("QuestMgr","Loading QuestFinisher ...");
	uint32 counts=0;
	QueryResult * Result = WorldDatabase.Query("SELECT entry,creatureId FROM quests_finisher");
	if(Result)
	{
		do{
			uint32 entry = Result->Fetch()[0].GetUInt32();
			uint32 crea = Result->Fetch()[1].GetUInt32();

			QuestInfo * Info = m_quests[entry];

			if(Info)
			{
				m_finisher[crea].push_back( Info );
				counts++;
			}

		}while(Result->NextRow());
	}
	delete Result;

	Log.Success("QuestMgr","Loaded %u QuestFinisher. . .",counts);
}
void QuestMgr::LoadPQuests()
{
	Log.Debug("QuestMgr","Loading Public Quests ...");
	uint32 counts=0;
	QueryResult * Result = WorldDatabase.Query("SELECT entry,Name,Type,Level,zone,pinx,piny,chapterid,tok_discovered,tok_unlocked FROM publicquests");
	if(Result)
	{
		do{
			uint32 entry =	Result->Fetch()[0].GetUInt32();
			string name =	Result->Fetch()[1].GetString();
			uint32 Type =	Result->Fetch()[2].GetUInt32();
			uint32 Level =	Result->Fetch()[3].GetUInt32();
			uint16 zone =	Result->Fetch()[4].GetUInt32();
			uint32 pinx =	Result->Fetch()[5].GetUInt32();
			uint32 piny =	Result->Fetch()[6].GetUInt32();
			uint32 chapterid = Result->Fetch()[7].GetUInt32();
			uint32 tok_discovered= Result->Fetch()[8].GetUInt32();
			uint32 tok_unlocked= Result->Fetch()[9].GetUInt32();

			PQInfo * QInfo = new PQInfo;
			QInfo->entry =	entry;
			QInfo->name =	name;
			QInfo->realm =	Type;
			QInfo->level =	Level;
			QInfo->zoneid = zone;
			QInfo->pinx =	pinx;
			QInfo->piny =	piny;
			QInfo->Info =	objmgr.GetInfluenceInfo(chapterid);
			QInfo->tok_discovered = tok_discovered;
			QInfo->tok_unlocked	=	tok_unlocked;

			QueryResult * Obj = WorldDatabase.Query("SELECT `guid`,`stage`,`type`,`objective`,`count`,`description`,`obj`,`tok_completed` FROM `publicquests_objectives` WHERE entry=%u ORDER BY `guid`",entry);
			if(Obj)
			{
				PQStageInfo * laststage=NULL;
				do{
					uint32 guid =		Obj->Fetch()[0].GetUInt32();
					string stage =		Obj->Fetch()[1].GetString();
					uint32 type =		Obj->Fetch()[2].GetUInt32();
					string Objective =	Obj->Fetch()[3].GetString();
					uint32 ocount =		Obj->Fetch()[4].GetUInt32();
					string Description= Obj->Fetch()[5].GetString();
					string ObjId =		Obj->Fetch()[6].GetString();
					uint32 tok_completed=	Obj->Fetch()[7].GetUInt32();
					uint32 objid = atoi(ObjId.c_str());

					QuestObjectif * obj =	new QuestObjectif(); // Objectif a atteindre
					obj->guid =			guid;
					obj->Objective =	Objective;
					obj->objectif =		Quest_Objectif(type);
					obj->count =		ocount;
					obj->done =			false;
					uint32 objentry = atoi(ObjId.c_str());
					GererateObjectif(obj,objid);
			
					if(!laststage || (laststage && laststage->stageName != stage) )
					{
						PQStageInfo * Stage= new PQStageInfo;
						Stage->description = Description;
						Stage->entry = QInfo->entry;
						Stage->stageName = stage;
						Stage->tok_completed = tok_completed;
						QInfo->m_stages.push_back(Stage);
						laststage = Stage;
					}
					laststage->AddObjectif(obj);
				}while(Obj->NextRow());
			}
			delete Obj;

			m_pquests[zone].push_back(QInfo);
			++counts;
		}while(Result->NextRow());
	}
	delete Result;

	Log.Success("QuestMgr","Loaded %u Public Quests",counts);

}
bool QuestMgr::CanStartQuest(Player * Plr,QuestInfo * Info)
{
	if(Plr == NULL) return false;
	if(!Info) return false;

	if(Info->type && Info->type != Plr->GetRealm())
		return false;

	if(Info->level && Info->level > Plr->GetLevel())
		return false;

	if(Plr->GetQuestInterface()->HasDoneQuest(Info->entry))
		return false;

	if(Info->prevquest && !Plr->GetQuestInterface()->HasDoneQuest(Info->prevquest))
		return false;

	if(Plr->GetQuestInterface()->HasQuest(Info->entry))
		return false;

	return true;
}
void ClientCom::onQuest()
{
	if(!m_Plr) return;
	if(!m_Plr->IsInWorld()) return;

	uint16 questid,status,unk2,unk4,creaoid;
	uint8 unk3,unk31;

	m_readBuffer >> questid >> status >> unk2 >> unk3 >> unk31 >> unk4 >> creaoid;

	Creature * Crea = m_Plr->GetZoneMgr()->GetCreature(creaoid);

	switch(status)
	{
	case 1:	// show Quest
			if(Crea)
				questmgr.BuildQuestForPlayer(questid,Crea,m_Plr);
		break;

	case 2: // Accept Quest
			m_Plr->GetQuestInterface()->AcceptQuest(questid);
		break;

	case 3: // Quest Done
			m_Plr->GetQuestInterface()->DoneQuest(questid);
		break;

	case 4: // Quest Done Info
			m_Plr->GetQuestInterface()->SendDoneQuest(questid,Crea);
		break;
	case 5: // Select Quest Reward
			m_Plr->GetQuestInterface()->SelectReward(questid,unk31);
		break;

	};
}
void ClientCom::onRequestQuest()
{
	if(!m_Plr) return;

	uint16 questid;
	uint8 status;

	m_readBuffer >> questid >> status;

	switch(status)
	{
	case 0: // Quest Accepted
		{
			QuestAchievement * Ach = m_Plr->GetQuestInterface()->GetQuestAchievement(questid);
			m_Plr->GetQuestInterface()->SendQuest(Ach);
		}
		break;
	case 1: // Lose quest
		{
			m_Plr->GetQuestInterface()->RemoveQuest(questid);
		}
		break;
	case 2: // send to group
		break;

	};
}
void QuestMgr::BuildQuestForPlayer(uint32 questid,Creature * Crea,Player * Plr)
{
	if(!questid || Plr == NULL || Crea == NULL) return;
	
	if(!Crea->HasQuests() || !Crea->NumOfQuests())
		return;

	if(Plr->GetQuestInterface()->HasQuest(questid))
		return;

	QuestInfo * Info = Crea->GetQuest(questid);
	if(!Info)
		return;

	Buffer *b = new Buffer(0);

	b->write<uint8>(1);
	b->write<uint8>(1);
	b->write<uint16>(Info->entry);
	b->write<uint16>(0);
	b->write<uint16>(Crea->GetOid());
	b->write<uint16>(0);
	b->write<uint16>(Plr->GetOid());
	b->write<uint16>(0);

	BuildQuestInfo(b,Info,Plr);

	Buffer *d = Plr->Allocate(b->size()+3);
	d->write<uint16>(b->size());
	d->write<uint8>(F_INTERACT_RESPONSE);
	d->write(b);

	Plr->sendPacket(d);
	delete b;
}
void QuestMgr::BuildPreQuest(Buffer *b,QuestInfo *Info,bool particular)
{
	if(!Info) return;

	b->writeBytePascalString(Info->name);
	b->write<uint16>(Info->description.length());
	b->write(reinterpret_cast<const uint8*>(Info->description.c_str()),Info->description.length());
	if(particular)
	{
		b->write<uint16>(Info->particular.length());
		b->write(reinterpret_cast<const uint8*>(Info->particular.c_str()),Info->particular.length());
	}
	b->write<uint8>(1);
	b->write<uint32>(Info->m_gold);
	b->write<uint32>(Info->m_xp);
}
void QuestMgr::BuildInterractQuest(Creature * Crea,Player * Plr)
{
	if(Crea == NULL || Plr == NULL) return;

	// Check Quest Finishier
	QuestInfo * Info = NULL;
	QuestAchievement * Ach = NULL;

	list<QuestAchievement*> m_acheved; // Liste des quètes terminée pour récompence
	list<QuestInfo*> m_toget; // Liste des quètes a prendre a ce pnj

	// On Cherche les quètes pouvant être finies
	if(Crea->HasFinisher() && Crea->NumOfFinisher())
	{
		for(list<QuestInfo*>::iterator itr = Crea->QuestsFinishBegin();itr != Crea->QuestsFinishEnd();++itr)
		{
			if(!(*itr)) continue;

			Ach = Plr->GetQuestInterface()->GetQuestAchievement( (*itr)->entry );
			if(Ach && Ach->done)
				m_acheved.push_back(Ach);
		}
	}

	// On Cherche les quètes pouvant être commencées
	if(Crea->HasQuests() && Crea->NumOfQuests())
	{
		for(list<QuestInfo *>::iterator itr = Crea->QuestsBegin();itr != Crea->QuestsEnd();++itr)
		{
			if(!(*itr)) continue;
			if(!questmgr.CanStartQuest(Plr,(*itr))) continue;
			
			m_toget.push_back((*itr));
		}
	}

	if(!m_toget.size() && !m_acheved.size())
		return;

	Log.Debug("QuestMgr","Sending QUEST !");
	string text = "";
	Buffer *b = new Buffer(0);
	b->write<uint8>(0);
	b->write<uint16>(Crea->GetOid());
	b->fill(0,3);
	b->write<uint8>(0x60);
	b->fill(0,4);
	b->write<uint16>(Plr->GetOid());

	b->write<uint8>(m_toget.size());
	for(list<QuestInfo*>::iterator itr=m_toget.begin();itr!=m_toget.end();++itr)
	{
		if(!text.size())
			text = (*itr)->particular;

		b->write<uint8>(0);
		b->write<uint16>((*itr)->entry);
		b->write<uint16>(0);
		b->writeBytePascalString((*itr)->name);
	}

	b->write<uint8>(m_acheved.size());
	for(list<QuestAchievement*>::iterator itr=m_acheved.begin();itr!=m_acheved.end();++itr)
	{
		if(!text.size())
			text = (*itr)->Quest->particular;

		b->write<uint8>(0);
		b->write<uint16>((*itr)->Quest->entry);
		b->writeBytePascalString((*itr)->Quest->name);
	}

	b->write<uint8>(0);
	b->writeBytePascalString(text);
	b->write<uint8>(0);

	Buffer * d = Plr->Allocate(b->size()+3);
	d->write<uint16>(b->size());
	d->write<uint8>(F_INTERACT_RESPONSE);
	d->write(b);
	Plr->sendPacket(d);
	delete b;
}
vector<const ItemProto*> QuestMgr::GetItemChoice(Player * Plr,QuestInfo * Info)
{
	vector<const ItemProto*> m_list;
	if(Plr == NULL) return m_list;
	if(!Info) return m_list;

	uint32 playercareer = 1 << (Plr->GetCareerLine()-1);
	uint8 race =  1 << (Plr->GetRace()-1);

	for(vector<const ItemProto*>::iterator itr = Info->m_choice.begin();itr != Info->m_choice.end();++itr)
	{
		if( !(*itr) ) continue;

		if( ((*itr)->career && !((*itr)->career & playercareer)) || ((*itr)->race && !((*itr)->race & race))) {}
		else m_list.push_back( (*itr) );
	}

	return m_list;

}
void QuestMgr::BuildQuestInfo(Buffer *b,QuestInfo *Info,Player * Plr)
{
	if(!Info) return;

	BuildPreQuest(b,Info,true);

	vector<const ItemProto*> m_list = GetItemChoice(Plr,Info);

	b->write<uint8>(m_list.size() ? 1 : 0); // Nombre d'item a choisir
	b->write<uint8>(0);
	b->write<uint8>(m_list.size()); // Nombre d'item
	if(m_list.size())
	{
		for(vector<const ItemProto*>::iterator itr=m_list.begin();itr!=m_list.end();++itr)
			sChat.BuildItemInfo(b,(*itr));
	}

	b->write<uint8>(Info->m_objectif.size()); // ObjectifCount
	QuestObjectif * Ob=NULL;
	if(Info->m_objectif.size())
	for(vector<QuestObjectif*>::iterator itr = Info->m_objectif.begin();itr!=Info->m_objectif.end();++itr)
	{
		Ob = (*itr);

		if(!Ob) continue;

		b->write<uint8>(Ob->count);
		b->writeBytePascalString(Ob->Objective);
	}

	b->write<uint8>(0);
}
void QuestMgr::GererateObjectif(QuestObjectif *obj,uint32 objentry)
{
	if(!obj) return;

	switch(obj->objectif)
	{
	case QUEST_KILL_PLAYERS:
		if(obj->Objective.size() < 1)
			obj->Objective = "Enemy Players";
		break;
	case QUEST_SPEACK_TO:
	case QUEST_KILL_MOB:
		obj->CreaProto = objmgr.GetCreatureProto(objentry);

		if(obj->Objective.size() < 1 && obj->CreaProto)
			obj->Objective = obj->CreaProto->name;
		break;
	case QUEST_USE_GO:
		
		break;
	case QUEST_GET_ITEMS:
		obj->ItemProto = objmgr.GetItemProto(objentry);
		break;
	};
}
list<PQuest*> QuestMgr::GeneratePQuest(ZoneMgr * Zone)
{
	list<PQuest*> m_list;
	if(!Zone) return m_list;

	for(list<PQInfo*>::iterator itr = m_pquests[Zone->GetZoneId()].begin();itr!=m_pquests[Zone->GetZoneId()].end();++itr)
	{
		if(!(*itr)) continue;

		PQuest * Quest = new PQuest((*itr),Zone);
		m_list.push_back(Quest);
	}
	return m_list;
}
PQInfo * QuestMgr::GetPQInfo(uint32 questid,uint16 zoneid)
{
	if(zoneid)
	{
		for(list<PQInfo*>::iterator itr=m_pquests[zoneid].begin();itr!=m_pquests[zoneid].end();++itr)
			if((*itr)->entry == questid)
				return (*itr);
	}
	else
	{
		for(unordered_map<uint16,list<PQInfo*> >::iterator itr=m_pquests.begin();itr!=m_pquests.end();++itr)
		{
			for(list<PQInfo*>::iterator itt=itr->second.begin();itt!=itr->second.end();++itt)
				if((*itt)->entry == questid)
					return (*itt);
		}
	}
	return NULL;
}