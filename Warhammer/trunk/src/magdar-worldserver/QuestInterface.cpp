#include "StdAfx.h"

void QuestAchievement::CopyFromQuest()
{
	if(!Quest) return;

	questid = Quest->entry;
	for(vector<QuestObjectif*>::iterator itr=Quest->m_objectif.begin();itr!=Quest->m_objectif.end();++itr)
	{
		QuestObjectif * no = new QuestObjectif;
		no->objectif =	(*itr)->objectif;
		no->ItemProto = (*itr)->ItemProto;
		no->CreaSpawn = (*itr)->CreaSpawn;
		no->CreaProto = (*itr)->CreaProto;
		no->done =		false;
		no->num =		(*itr)->num;
		no->count =		0;
		no->m_base =	(*itr);
		no->Objective =	(*itr)->Objective;
		no->guid = (*itr)->guid;


		m_acheved.push_back(no);
	}
}
void QuestAchievement::CheckDone()
{
	if(m_acheved.size())
	for(vector<QuestObjectif*>::iterator itr = m_acheved.begin();itr!=m_acheved.end();++itr)
		if(!(*itr)->done)
			return;

	done=true;
}
QuestObjectif * QuestAchievement::GetObjByGuid(uint32 guid)
{
	if(m_acheved.size())
	for(vector<QuestObjectif*>::iterator itr = m_acheved.begin();itr!=m_acheved.end();++itr)
		if((*itr)->guid == guid)
			return (*itr);

	return NULL;
}
void QuestInterface::Init(Player * Plr)
{
	if(Plr == NULL) return;

	m_player = Plr;
	uint32 counts=0;
	Log.Debug("QuestInterface","Loading QuestAchievement For %u",Plr->GetPlayerID());
	QueryResult * Result = CharacterDatabase.Query("SELECT questid,objectif,done FROM characters_quest WHERE character_id=%u",Plr->GetPlayerID());
	if(Result)
	{
		do{
			uint32 questid = Result->Fetch()[0].GetUInt32();
			QuestInfo * Info = questmgr.GetQuestInfo(questid);

			if(!Info) continue;
			counts++;

			if(Result->Fetch()[2].GetBool())
			{
				AddDoneQuest(questid);
				continue; // Don"t load quest who are done
			}

			QuestAchievement * Ach = new QuestAchievement;
			Ach->Quest = Info;
			Ach->CopyFromQuest();
			Ach->m_tochoice = questmgr.GetItemChoice(m_player,Info);

			string Objectif = Result->Fetch()[1].GetString();
			
			//[guid,count],[guid,count]
			int a = Objectif.find_first_of("[");
			int b = Objectif.find_first_of("]");
			int c = Objectif.find_first_of(",");

			while(a != string::npos && b != string::npos && c != string::npos)
			{
				if( a < c && b > c)
				{
					uint32 guid = atoi(Objectif.substr(a+1,c).c_str());
					uint32 count = atoi(Objectif.substr(c+1,b).c_str());
					Log.Debug("QuestInterface","Guid = %u, count = %u",guid,count);
					QuestObjectif * Obj = NULL;
					Obj = Ach->GetObjByGuid(guid);
					if(Obj)
						Obj->count = count;
					Obj->CheckDone();
				}
				Objectif.erase(0,b);

				if(Objectif.size() > 1)
					Objectif.erase(0,1); // ,[

				a = Objectif.find_first_of("[");
				b = Objectif.find_first_of("]");
				c = Objectif.find_first_of(",");
			}

			Ach->CheckDone();
			m_quests.push_back(Ach);

		}while(Result->NextRow());
	}
	delete Result;
	Log.Debug("QuestInterface","Loaded %u QuestAchievement For %u",counts,Plr->GetPlayerID());
}
void QuestInterface::SendAllQuest()
{
	Log.Debug("QuestInterface","Sending All QuestAchievement For %u",m_player->GetPlayerID());
	m_lock.Acquire();

	Buffer *d = new Buffer(0);

	d->write<uint8>(m_quests.size());
	if(m_quests.size())
	{
		for(list<QuestAchievement*>::iterator itr= m_quests.begin();itr!=m_quests.end();++itr)
			if( (*itr) )
			{
				d->write<uint16>((*itr)->questid);
				d->write<uint16>((*itr)->Quest->name.length());
				d->write(reinterpret_cast<const uint8*>((*itr)->Quest->name.c_str()),(*itr)->Quest->name.length());
				d->write<uint8>(0);
			}
	}

	m_lock.Release();
	uint16 responseSize = d->size();
    Buffer* b = m_player->Allocate(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_QUEST_LIST);
	b->write(d);
    m_player->sendPacket(b);
	delete d;

}
void QuestInterface::Save()
{
	Log.Debug("QuestInterface","Saving QuestAchievement For %u",m_player->GetPlayerID());
	m_lock.Acquire();

	if(m_quests.size())
		for(list<QuestAchievement*>::iterator itr=m_quests.begin();itr!=m_quests.end();++itr)
		{
			if(!(*itr)) continue;

			stringstream ss;
			uint32 questid = (*itr)->questid;
			for(vector<QuestObjectif*>::iterator itt=(*itr)->m_acheved.begin();itt!=(*itr)->m_acheved.end();++itt)
			{
				if(ss.str().size())
				 ss << ",";

				ss << "[" << (*itt)->guid << "," << uint32((*itt)->count) << "]";
			}
			CharacterDatabase.Execute("REPLACE INTO characters_quest(character_id,questid,objectif,done) VALUE ('%u','%u','%s',0)",m_player->GetPlayerID(),questid,ss.str().c_str());
		}

	if(m_questdone.size())
		for(list<uint32>::iterator itr=m_questdone.begin();itr!=m_questdone.end();++itr)
		{
			CharacterDatabase.Execute("REPLACE INTO characters_quest(character_id,questid,done) VALUE ('%u','%u',1)",m_player->GetPlayerID(),(*itr));
		}

	m_lock.Release();

}
bool QuestInterface::HasQuest(uint32 questid)
{
	m_lock.Acquire();
	if(m_quests.size())
	{
		for(list<QuestAchievement*>::iterator itr= m_quests.begin();itr!=m_quests.end();++itr)
			if( (*itr)->questid == questid )
			{m_lock.Release();return true;};
	}
	m_lock.Release();
	return false;
}
void QuestInterface::AcceptQuest(uint32 questid)
{
	if(!m_player)
		return;

	Log.Debug("QuestInterface","[%u] Player Accept Quest %u",m_player->GetPlayerID(),questid);
	
	if(HasQuest(questid))
		return;
	
	QuestInfo * Info = questmgr.GetQuestInfo(questid);
	if(!Info) return;

	QuestAchievement * Ach = new QuestAchievement;
	Ach->questid = questid;
	Ach->Quest = Info;
	Ach->done = false;
	Ach->CopyFromQuest();
	Ach->m_tochoice = questmgr.GetItemChoice(m_player,Info);

	m_lock.Acquire();
	m_quests.push_back(Ach);
	m_lock.Release();

	SendQuestStatus(Info,GameData::QUESTCOMPLETION_OFFER);

	if(Ach->Quest && Ach->Quest->m_script)
		Ach->Quest->m_script->OnStart(m_player,Ach);

	Ach->CheckDone();
}
void QuestInterface::SelectReward(uint32 questid,uint8 num)
{
	if(!num) return;

	QuestAchievement * Ach = GetQuestAchievement(questid);
	if(!Ach) return;
	if(!Ach->done) return;

	if(num)
		--num;

	Log.Debug("Quest","SELECT = %u",uint32(num));

	if(num >= Ach->m_tochoice.size() && Ach->m_tochoice.size())
		num = Ach->m_tochoice.size()-1;

	Ach->m_selected.push_back(num);
}
void QuestInterface::DoneQuest(uint16 questid)
{
	if(!m_player) return;

	QuestAchievement * Ach = GetQuestAchievement(questid);
	if(!Ach) return;
	if(!Ach->done) return;

	Log.Debug("Quest","QST 1 %u - %u",Ach->m_selected.size(),Ach->m_tochoice.size());
	if( (Ach->m_selected.size() > Ach->m_tochoice.size()) || (Ach->m_tochoice.size() && Ach->m_selected.size() < 1) )
	{
		Ach->m_selected.clear();
		return;
	}

	uint16 freeslot = m_player->GetItemInterface()->GetFreeSlot();
	Log.Debug("Quest","QST 2");
	if(freeslot < Ach->m_selected.size())
	{
		m_player->sendLocalizedString("",TEXT_OVERAGE_CANT_SALVAGE);
		return;
	}

	Log.Debug("Quest","QST 3");
	if(Ach->Quest && Ach->Quest->m_script)
		Ach->Quest->m_script->onGetReward(m_player,Ach);


	Log.Debug("Quest","QST 4");
	for(list<uint8>::iterator itr=Ach->m_selected.begin();itr != Ach->m_selected.end();++itr)
	{
		Log.Debug("Quest","QST 4 = %u",uint32((*itr)));
		if( (*itr) >= Ach->m_tochoice.size())
			(*itr) = Ach->m_tochoice.size()-1;

		const ItemProto * Proto = Ach->m_tochoice[(*itr)];

		if(!Proto) continue;
		else m_player->GetItemInterface()->CreateItem(Proto);
	}

	m_player->GiveXp(Ach->Quest->m_xp);
	m_player->AddWealth(Ach->Quest->m_gold);

	AddDoneQuest(Ach->Quest->entry);
	SendQuestStatus(Ach->Quest,GameData::QUESTCOMPLETION_DONE);

	m_lock.Acquire();
	m_quests.remove(Ach);
	m_lock.Release();

	delete Ach;
}
void QuestInterface::SendDoneQuest(uint32 questid,Creature * Crea)
{
	if(!m_player) return;
	if(Crea == NULL) return;
	
	QuestAchievement * Ach = GetQuestAchievement(questid);

	if(!Ach) return;
	if(!Ach->done) return;

	Buffer * b= new Buffer(0);
	b->write<uint8>(3);
	b->write<uint8>(0);
	b->write<uint16>(Ach->questid);
	b->write<uint16>(0);
	b->write<uint16>(Crea->GetOid());
	b->write<uint16>(0);
	b->write<uint16>(m_player->GetOid());
	questmgr.BuildPreQuest(b,Ach->Quest,false);
	b->write<uint8>(Ach->m_tochoice.size() ? 1 : 0); // Nombre d'item a choisir
	b->write<uint8>(0);
	b->write<uint8>(Ach->m_tochoice.size()); // Nombre d'item
	if(Ach->m_tochoice.size())
	{
		for(vector<const ItemProto*>::iterator itr=Ach->m_tochoice.begin();itr!=Ach->m_tochoice.end();++itr)
			sChat.BuildItemInfo(b,(*itr));
	}
	Buffer *d = m_player->Allocate(b->size()+3);
	d->write<uint16>(b->size());
	d->write<uint8>(F_INTERACT_RESPONSE);
	d->write(b);
	m_player->sendPacket(d);
	delete b;
}
void QuestInterface::RemoveQuest(uint32 questid)
{
	if(!m_player)
		return;

	Log.Debug("QuestInterface","[%u] Player Remove Quest %u",m_player->GetPlayerID(),questid);

	QuestAchievement * Arch = GetQuestAchievement(questid);
	if(Arch)
	{
		if(Arch->Quest && Arch->Quest->m_script)
			Arch->Quest->m_script->onCancel(m_player,Arch);

		SendQuestStatus(Arch->Quest,GameData::QUESTCOMPLETION_ABANDONED);

		m_lock.Acquire();
		CharacterDatabase.Execute("DELETE FROM characters_quest WHERE questid=%u",Arch->questid);
		m_quests.remove(Arch);
		m_lock.Release();

		delete Arch;
	}
}
void QuestInterface::SendQuestStatus(QuestInfo * Info,GameData::QuestCompletion status)
{
	if(!m_player) return;
	if(!Info) return;

	Buffer *b = m_player->Allocate(10+Info->name.length()+3);
	b->write<uint16>(10+Info->name.size());
	b->write<uint8>(F_QUEST_LIST_UPDATE);
	b->write<uint16>(Info->entry);

	if(status == GameData::QUESTCOMPLETION_ABANDONED || status == GameData::QUESTCOMPLETION_DONE)
		b->write<uint8>(0);
	else 
		b->write<uint8>(1);

	b->write<uint8>( status == GameData::QUESTCOMPLETION_DONE ? 1 : 0);

	b->write<uint32>(0x0000FFFF);
	b->writeBytePascalString(Info->name);
	b->write<uint8>(0);
	m_player->sendPacket(b);
}
void QuestInterface::SendQuest(QuestAchievement* Info)
{
	if(!Info) return;

	QuestInfo * Quest = Info->Quest;

	Buffer *b = new Buffer(0);
	b->write<uint16>(Quest->entry);
	b->write<uint8>(0);
	questmgr.BuildPreQuest(b,Quest,true);
	b->write<uint8>(Info->m_tochoice.size() ? 1 : 0); // Nombre d'item a choisir
	b->write<uint8>(0); // ici on met le togive
	b->write<uint8>(Info->m_tochoice.size()); // Nombre d'item
	if(Info->m_tochoice.size())
	{
		for(vector<const ItemProto*>::iterator itr=Info->m_tochoice.begin();itr!=Info->m_tochoice.end();++itr)
			sChat.BuildItemInfo(b,(*itr));
	}
	b->write<uint16>(Info->m_acheved.size()); // ObjectifCount


	QuestObjectif * Ob=NULL;
	QuestObjectif * Obact=NULL;
	if(Info->m_acheved.size())
	for(vector<QuestObjectif*>::iterator itr = Info->m_acheved.begin();itr!=Info->m_acheved.end();++itr)
	{
		Obact = (*itr);
		Ob = (*itr)->m_base;
		if( !Obact || !Ob) continue;

		b->write<uint8>(Obact->count);
		b->write<uint8>(Ob->count);
		b->write<uint16>(0);
		b->writeBytePascalString(Ob->Objective);
	}
	b->write<uint16>(1);
	b->writeBytePascalString(Quest->name);
	string td = "Return to your giver";
	b->writeBytePascalString(td);

	b->write<uint16>(0x006A);
	b->write<uint16>(0x046D);
	b->write<uint16>(0x4D9E);
	b->write<uint16>(0xCB65);
	b->fill(0,18);

	Buffer *d = m_player->Allocate(b->size()+3);
	d->write<uint16>(b->size());
	d->write<uint8>(F_QUEST_INFO);
	d->write(b);
	m_player->sendPacket(d);
	delete b;
}
void QuestInterface::SendQuestUpdate(uint16 questid,uint8 objectif,uint8 count,bool done)
{
	if(!m_player) return;

	Log.Debug("QuestInterface","SendQuest Updated %u=%u",objectif,count);
	Buffer *b = m_player->Allocate(7+3);
	b->write<uint16>(7);
	b->write<uint8>(F_QUEST_UPDATE);
	b->write<uint16>(questid);
	b->write<uint8>(done);
	b->write<uint8>(objectif+1);
	b->write<uint8>(count);
	b->write<uint16>(0);
	m_player->sendPacket(b);
}
void QuestInterface::onHandleEvent(Quest_Objectif objectif,uint32 count,Unit * unit,ItemProto * Proto,Player * Plr)
{
	m_lock.Acquire();

	for(list<QuestAchievement*>::iterator itr = m_quests.begin();itr!=m_quests.end();++itr)
	{
		if(!(*itr)) continue;

		for(vector<QuestObjectif*>::iterator itt = (*itr)->m_acheved.begin();itt!=(*itr)->m_acheved.end();++itt)
		{
			if(!(*itt)) continue;
			if((*itt)->done) continue;

			if((*itt)->objectif == objectif)
			{
				if(objectif == QUEST_SPEACK_TO || objectif == QUEST_KILL_MOB)
				{
					if(!unit || !unit->IsCreature())
						continue;

					if( !(*itt)->CreaProto || (*itt)->CreaProto != unit->GetCreature()->GetCreatureProto())
						continue;

					if((*itr)->Quest->m_script)
						if(objectif == QUEST_SPEACK_TO)
							(*itr)->Quest->m_script->onSpeackTo(Plr,unit,(*itr));
						else 
							(*itr)->Quest->m_script->onUnitKill(Plr,unit,(*itr));
				}
				else if(objectif == QUEST_USE_GO)
				{

				}
				else if(objectif == QUEST_GET_ITEMS)
				{
					if(!Proto || !(*itt)->ItemProto || (*itt)->ItemProto != Proto)
						continue;

					if((*itr)->Quest->m_script)
						(*itr)->Quest->m_script->onItemPickup(Plr,Proto->entry,(*itr));
				}
				else if(objectif == QUEST_KILL_PLAYERS)
				{
						if((*itr)->Quest->m_script)
							(*itr)->Quest->m_script->onUnitKill(Plr,unit,(*itr));	
				}

				(*itt)->count+=count;
				(*itt)->CheckDone();
				(*itr)->CheckDone();
				SendQuestUpdate((*itr)->questid,(*itt)->num,(*itt)->count,(*itr)->done);
			}
		}
	}

	m_lock.Release();
}