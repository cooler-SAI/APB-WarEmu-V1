#include "StdAfx.h"

void ClientCom::onPlayerVoiceOver() // Affichage des nombres de joueurs sur la zone
{
	uint16 unk,zone=0;
	m_readBuffer >> unk >> zone;

	Log.Error("ClientCom","onPlayerVoiceOver %u",zone);
	ZoneMgr * Zone = mapmgr.GetZoneMgr(zone);
	if(!Zone) return;

	if(Zone->m_pquests.size())
	{
		uint16 responseSize = 11+(6*Zone->m_pquests.size());
		Buffer *b = new Buffer(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_OBJECTIVE_UPDATE);
		b->write<uint32>(0);
		b->write<uint8>(6);
		b->write<uint32>(zone);
		b->write<uint16>(Zone->m_pquests.size());

		for(list<PQuest*>::iterator itr = Zone->m_pquests.begin();itr!=Zone->m_pquests.end();++itr)
		{
			b->write<uint32>( (*itr)->GetEntry() );
			b->write<uint16>( (*itr)->GetPlayerCounts() );
		}

		m_Plr->sendPacket(b);
	}
}

void PQStageInfo::AddObjectif(QuestObjectif* Obj)
{
	Obj->num = this->Objectifs.size();
	Obj->done=false;
	Objectifs.push_back(Obj);
}
void PQStageInfo::Reset()
{
	for(vector<QuestObjectif*>::iterator itr=Objectifs.begin();itr!=Objectifs.end();++itr)
	{
		(*itr)->count=0;
		(*itr)->done=false;
	}
}
void PQStageInfo::CheckDone()
{
	done=true;
	for(vector<QuestObjectif*>::iterator itr=Objectifs.begin();itr!=Objectifs.end();++itr)
	{
		(*itr)->CheckDone();
		if(done && !(*itr)->done)
			done=false;
	}
}

PQuest::PQuest(PQInfo *Info,ZoneMgr * Zone) : 
  m_info(Info), m_zoneMgr(Zone),m_currentstage(NULL),m_script(NULL),m_started(false),m_stagenum(0),m_endtime(0)
  {
	  // Comme il ne faut pas que les différentes quètes instancié utilise le même pointeur d'objectif
	  // On recré de nouveaux pointeur contenant les objectif mais séparément

	  m_stages.clear();
	  if(m_info && m_info->m_stages.size())
	  {
		  for(vector<PQStageInfo*>::iterator itr=m_info->m_stages.begin();itr!=m_info->m_stages.end();++itr)
		  {
				PQStageInfo* Info = new PQStageInfo;
				Info->description = (*itr)->description;
				Info->entry =		(*itr)->entry;
				Info->stageName =	(*itr)->stageName;
				Info->tok_completed = (*itr)->tok_completed;
				Info->done = false;


				for(vector<QuestObjectif*>::iterator itt=(*itr)->Objectifs.begin();itt!=(*itr)->Objectifs.end();++itt)
				{
					QuestObjectif * Ob = new QuestObjectif;
					Ob->m_base = (*itt);
					Ob->CopyFromBase();
					Info->Objectifs.push_back(Ob);
				}

				m_stages.push_back(Info);
		  }
	  }

	  m_script = scriptmgr.GetPQScript(m_info->entry,this);
  }

void PQuest::Start()
{
	// Si l'instance est démaré et qu'elle ne doit pas être reset on return
	if(m_started && !m_endtime) return;

	Log.Debug("PQuest","Starting Quest %s",GetName().c_str());

	m_started=true;
	m_stagenum=0;
	m_endtime=0;

	if(!m_stages.size())
		return;

	if(m_currentstage)
		m_currentstage->Reset();

	m_currentstage = m_stages[m_stagenum];

	if(m_currentstage)
		m_currentstage->Reset();

	if(m_script)
		m_script->onStart();
}
void PQuest::NextStage()
{
	if( m_endtime ) return; // Pas de nextstage avant d'avoir reset

	Log.Debug("PQuest","Stage %s done : next Stage",m_currentstage->stageName.c_str());

	++m_stagenum;

	InfluenceInfo * Info = GetInfluenceInfo(); // On récupère l'influence info pour l'ajouter aux joueurs présents

	PQStageInfo * ancienstage=m_currentstage; // On stoque le stage actuel pour ajouter les tok aux joueurs
	m_currentstage=NULL;						// On Supprimme le stage actuel

	bool toend=false;
	

	if(m_stagenum < m_stages.size()) // Si le numéro de stage est inférieur au nombre de stages total
	{
		m_currentstage = m_stages[m_stagenum];		// On passe le stage suivant au stage actuel
		if(m_currentstage) m_currentstage->Reset();
	}
	else toend=true;
	

	M_players.Acquire();
	for(map<uint32,Player*>::iterator itr=m_players.begin();itr!=m_players.end();++itr)
	{
		if(!itr->second) continue;

		if(Info) itr->second->m_InfluenceInterface->AddInfluence(Info->influenceid,200,Info);
		if(ancienstage) itr->second->m_TokInterface->AddTok(ancienstage->tok_completed);

		sendPQuest(itr->second); // On envoi les informations du stage actuel
	}
	M_players.Release();

	if(toend) // Si c'est la fin , alors on appel END , qui relancera le stage au bout de 2 minutes
	{
		End(); // Fin des stage , compte a rebour de reset;
		return;
	}

	if(m_script) 
		m_script->onNextStage();
}
void PQuest::End()
{
	Log.Debug("PQuest","End");

	m_endtime=UNIXTIME+TIME_PQ_RESET;
	m_currentstage = NULL;

	// TODO : générer les récompenses !

	if(m_script)
		m_script->onEnd();
}
void PQuest::Update(uint32 tdiff)
{
	if(m_endtime && m_endtime<=UNIXTIME)
		Start();

	if(m_script)
		m_script->Update(tdiff);
}
void PQuest::AddPlayer(Player * Plr)
{
	if(!Plr) return;

	if(HasPlayer(Plr)) return;

	Log.Debug("PQuest","AddingPlayer %s To Quest %s",Plr->GetName().c_str(),GetName().c_str());

	if(!m_started)
		Start();

	if(Plr->GetPublicQuest())
		Plr->GetPublicQuest()->RemovePlayer(Plr);

	PQScore* score = new PQScore;
	score->character_id	= Plr->GetPlayerID();
	score->killcount	= 0;
	score->healcounts	= 0;
	score->bonus		= 0;
	score->damages		= 0;
	score->participe	= true;

	M_players.Acquire();
	m_players[Plr->GetPlayerID()]	= Plr;
	m_scores[Plr->GetPlayerID()]	= score;
	M_players.Release();

	{
		// 09 00 02 00 00 07 D0 00 00 08 34
		Buffer *b = new Buffer(11+3);
		b->write<uint16>(11);
		b->write<uint8>(F_INTERACT_RESPONSE);
		b->write<uint8>(9);
		b->write<uint16>(2);
		b->write<uint16>(0);
		b->write<uint16>(0x07D0);
		b->write<uint16>(0);
		b->write<uint16>(0x0834);
		Plr->sendPacket(b);
	}

	InfluencePlayerInfo * Ifp = Plr->m_InfluenceInterface->GetInfluence(GetInfluenceInfo()->influenceid);
	Plr->m_InfluenceInterface->sendInfluenceUpdated(Ifp,true);

	Plr->SetPublicQuest( this );

	Plr->m_TokInterface->AddTok(m_info->tok_discovered);
	Plr->m_TokInterface->AddTok(m_info->tok_unlocked);

	sendPQuest(Plr);

	if(m_script)
		m_script->onAddPlayer(Plr);
}
void PQuest::RemovePlayer(Player * Plr)
{
	if(!Plr) return;

	Log.Debug("PQuest","RemovePlayer %s",Plr->GetName().c_str());

	PQScore * Score = GetPQScore(Plr->GetPlayerID());

	M_players.Acquire();
	m_players.erase(Plr->GetPlayerID());
	m_scores.erase(Plr->GetPlayerID());
	if(Score) delete Score;
	M_players.Release();

	if(m_script)
		m_script->onRemovePlayer(Plr);

	Plr->SetPublicQuest( NULL );

	sendLeave(Plr);
}
bool PQuest::HasPlayer(Player *Plr)
{
	if(!Plr) return false;

	M_players.Acquire();
	bool has = (m_players.find(Plr->GetPlayerID()) != m_players.end());
	M_players.Release();

	return has;
}
bool PQuest::IsInrange(Object *Obj)
{
	if(!Obj) return false;

	if(Obj->GetZoneMgr() != m_zoneMgr)
		return false;

	if( !GetPinX() || !GetPinY() ) return false;

	float distance = Obj->GetDistance(GetPinX(),GetPinY());

	if(distance > 200.0f) // TODO : vérifier ke chaque quète n'a pas un range différent 
		return false;
	else return true;
}
uint16 PQuest::GetPlayerCounts()
{
	M_players.Acquire();
	uint16 counts = m_players.size();
	M_players.Release();

	return counts;
}
void PQuest::HandleEvent(Quest_Objectif objectif,uint32 count,Unit * unit,ItemProto * Proto,Player * Plr)
{
	// On Compte l'objectif
	if(!m_currentstage)  return;

	Log.Debug("PQuest","HandleEvent %u",objectif);
	if(m_currentstage->Objectifs.size())
	{
		for(vector<QuestObjectif*>::iterator itr=m_currentstage->Objectifs.begin();itr!=m_currentstage->Objectifs.end();++itr)
		{
			if((*itr)->done) continue;
			if((*itr)->objectif != objectif) continue;

				switch(objectif)
				{
				case QUEST_UNKNOWN:
					continue;
					break;

				case QUEST_KILL_MOB:
				case QUEST_SPEACK_TO:

					if(!unit || !unit->IsCreature())
						continue;

					if( !(*itr)->CreaProto || (*itr)->CreaProto != unit->GetCreature()->GetCreatureProto())
						continue;

					break;

				case QUEST_USE_GO:
					continue;
					break;

				case QUEST_GET_ITEMS:

					if(!Proto || !(*itr)->ItemProto || (*itr)->ItemProto != Proto)
						continue;

					break;

				case QUEST_KILL_PLAYERS:
					continue;
					break;
				};

				(*itr)->count+=count;

				if(Plr)
					Plr->m_InfluenceInterface->AddInfluence(GetInfluenceInfo()->influenceid,100,GetInfluenceInfo());

				M_players.Acquire();
				for(map<uint32,Player*>::iterator itp=m_players.begin();itp!=m_players.end();++itp)
				{
					if(!itp->second) continue;
					sendPQuestUpdate(itp->second,(*itr)->guid,(*itr)->count);
					stringstream ss;
					ss << (*itr)->Objective << " " << uint32((*itr)->count) << "/" << uint32((*itr)->m_base->count);
					itp->second->sendLocalizedString(ss.str(),TEXT_NEW_PUBLIC_QUEST);
				}
				M_players.Release();
		}

		m_currentstage->CheckDone();
		if(m_currentstage->done)
			NextStage();
	}
}
void PQuest::PlayerKillUnit(Player *Plr,Unit *unit)
{
	if(!Plr || !unit) return;

	Log.Debug("PQuest","PlayerKillUnit %s",Plr->GetName().c_str());

	PQScore * Score = GetPQScore(Plr->GetPlayerID());
	if(Score) Score->killcount++;

	if(m_script)
		m_script->onPlayerKillUnit(Plr,unit);
}
void PQuest::PlayerDie(Player *Plr,Unit *Killer)
{
	if(!Plr || !Killer) return;

	Log.Debug("PQuest","PlayerDie %s",Plr->GetName().c_str());
	if(m_script)
		m_script->onPlayerDie(Plr,Killer);
}
void PQuest::PlayerDealHeal(Player *Plr,Unit *Healed,uint32 count)
{
	if(!Plr || !Healed) return;

	PQScore * Score = GetPQScore(Plr->GetPlayerID());
	if(Score) Score->healcounts+=count;

	if(m_script)
		m_script->onPlayerDealHeal(Plr,Healed,count);

}
void PQuest::PlayerDealDamage(Player *Plr,Unit *target,uint32 counts)
{
	if(!Plr || !target) return;

	Log.Debug("PQuest","PlayerDealDamage %s %u",Plr->GetName().c_str(),counts);
	PQScore * Score = GetPQScore(Plr->GetPlayerID());
	if(Score) Score->damages+=counts;

	if(m_script)
		m_script->onPlayerDealDamage(Plr,target,counts);
}
void PQuest::UnitDie(Unit *unit,Unit *Killer)
{
	if(!unit || !Killer) return;

	Log.Debug("PQuest","UnitDie %s ",unit->GetName().c_str());

	if(m_script)
		m_script->onUnitDie(unit,Killer);
}
void PQuest::sendPQuest(Player *Plr)
{
	if(!Plr) return;

	Log.Debug("PublicQuest","Sending ActuStage to %s",Plr->GetName().c_str());

	if(!m_currentstage) return;

	Buffer *b = new Buffer(0);

	b->write<uint32>(GetEntry());
	b->write<uint8>(0);
	b->write<uint8>(1);
	b->write<uint8>(1);
	b->write<uint16>(0);
	b->writeBytePascalString(GetName());

	b->write<uint8>(m_currentstage->Objectifs.size());

	for(vector<QuestObjectif*>::iterator itr=m_currentstage->Objectifs.begin();itr != m_currentstage->Objectifs.end();++itr)
	{
		Log.Debug("QuestStage","Objective = %s",(*itr)->Objective.c_str());

		b->write<uint16>(0); // sep
		b->write<uint16>( (*itr)->guid ); // Objectif ID
		b->write<uint8>(0);
		b->write<uint8>(1);
		b->write<uint8>(0);
		b->write<uint16>((*itr)->m_base->count); // Nombre de kill A faire ?
		b->write<uint16>((*itr)->count); // Nombre actuel
		b->write<uint8>(0);
		b->writeBytePascalString((*itr)->Objective);
		b->write<uint8>(1);
		
	}
	b->write<uint8>(0);
	b->writeBytePascalString(m_currentstage->stageName);
	b->write<uint8>(0);

	b->writeBytePascalString(m_currentstage->description);

	b->write<uint16>(0);

	b->write<uint16>(0); // Temps pour faire la quète (s)
	b->write<uint16>(0);
	b->write<uint16>(0); // Temps restant
	b->write<uint16>(0);
	b->write<uint8>(0); // Taille du nom de quelque chose ^^
	b->write<uint16>(GetInfluenceInfo()->influenceid);
	b->write<uint16>(0);
	b->write<uint8>(0);
	b->Trace();

	/*uint8 data[173] =
	{
		0x00,0x00,0x00,0xA5,0x00,0x01,0x01,0x00,0x00,0x13,0x52,0x61,0x76,0x65,0x6E,0x20,
		0x48,0x6F,0x73,0x74,0x20,0x56,0x61,0x6E,0x67,0x75,0x61,0x72,0x64,0x01,0x00,0x00,
		0x03,0x4A,0x00,0x01,0x00,0x00,0x19,0x00,0x15,0x00,0x14,0x52,0x61,0x76,0x65,0x6E,
		0x20,0x48,0x6F,0x73,0x74,0x20,0x4D,0x61,0x72,0x61,0x75,0x64,0x65,0x72,0x73,0x01,
		0x00,0x07,0x53,0x74,0x61,0x67,0x65,0x20,0x49,0x00,0x52,0x4B,0x69,0x6C,0x6C,0x20,
		0x74,0x68,0x65,0x20,0x52,0x61,0x76,0x65,0x6E,0x20,0x48,0x6F,0x73,0x74,0x20,0x4D,
		0x61,0x72,0x61,0x75,0x64,0x65,0x72,0x73,0x20,0x74,0x65,0x72,0x72,0x6F,0x72,0x69,
		0x7A,0x69,0x6E,0x67,0x20,0x74,0x68,0x65,0x20,0x70,0x65,0x61,0x73,0x61,0x6E,0x74,
		0x73,0x20,0x61,0x6E,0x64,0x20,0x70,0x69,0x6C,0x6C,0x61,0x67,0x69,0x6E,0x67,0x20,
		0x74,0x68,0x65,0x20,0x66,0x61,0x72,0x6D,0x6C,0x61,0x6E,0x64,0x2E,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x00,0x00,0x00
	};

	b->write(data,173);*/

	Buffer *d = new Buffer(b->size()+3);
	d->write<uint16>(b->size());
	d->write<uint8>(F_OBJECTIVE_INFO);
	d->write(b);
	Plr->sendPacket(d);
	delete b;
}
void PQuest::sendPQuestUpdate(Player *Plr,uint32 guid,uint8 count)
{
	if(!Plr) return;
	Log.Debug("PQuest","SendPQuestUpdate guid=%u , count=%u",guid,count);

	Buffer *b = new Buffer(13+3);
	b->write<uint16>(13);
	b->write<uint8>(F_OBJECTIVE_UPDATE);
	b->write<uint32>(GetEntry());
	b->write<uint8>(1);
	b->write<uint8>(1);
	b->write<uint32>(guid);
	b->write<uint16>(0);
	b->write<uint8>(count);
	Plr->sendPacket(b);
}
void PQuest::sendLeave(Player *Plr)
{
	if(!Plr) return;

	Buffer *b = new Buffer(8+3);
	b->write<uint16>(8);
	b->write<uint8>(F_OBJECTIVE_INFO);
	b->write<uint32>(GetEntry());
	b->write<uint32>(0);
	Plr->sendPacket(b);
}