#include "StdAfx.h"

using namespace GameData;

void ClientCom::onDoAbility()
{
	if(!m_Plr) return;

	uint32 unk,unk2,unk3=0;
	uint16 spellid;
	m_readBuffer >> unk >> unk2 >> unk3 >> spellid;
	m_Plr->GetAbilityInterface()->StartCast(spellid);
}

void ClientCom::onInterrupt()
{
	if(m_Plr && m_Plr->GetCurrentAbility())
		m_Plr->GetCurrentAbility()->Cancel();
}

void AbilityInterface::Init(Player * Plr)
{
	if(!Plr) return;

	m_player = Plr;
	m_currentAbility = NULL;
	m_globalcooldown=0;
	m_ability.clear();

	Log.Debug("AbilityInterface","Loading Spell For %u",Plr->GetPlayerID());
	QueryResult * Result = CharacterDatabase.Query("SELECT spellid,level FROM characters_spells WHERE character_id=%u",Plr->GetPlayerID());
	if(Result)
	{
		AbilityInfo * Info = NULL;
		do{
			uint32 id = Result->Fetch()[0].GetUInt32();
			uint8 level = Result->Fetch()[1].GetUInt32();

			Info = objmgr.GetAbilityInfo(id,level);

			if(!Info) continue;

			AbilityPlayer * AP = new AbilityPlayer;

			AP->entry = id;
			AP->level = level;
			AP->m_cooldown = 0;
			AP->Info = Info;

			m_ability.push_back(AP);

		}while(Result->NextRow());
	}
	delete Result;
}
void AbilityInterface::Save()
{
	Log.Debug("AbilityInterface","Saving Spell For %u",m_player->GetPlayerID());
	for(int i=0;i<m_ability.size();++i)
	{
		AbilityPlayer * Ap = m_ability[i];

		if(!Ap) continue;

		CharacterDatabase.Execute("REPLACE INTO characters_spells (character_id,spellid,level) VALUE ('%u','%u','%u')",m_player->GetPlayerID(),Ap->entry,Ap->level);
	}
}
void AbilityInterface::UpdateLevel(uint8 level)
{
	AbilityPlayer * Ap = NULL;
	AbilityInfo * NewInfo = NULL;
	for(int i=0;i<m_ability.size();++i)
	{
		Ap = m_ability[i];
		if(!Ap) continue;
		NewInfo = objmgr.GetAbilityInfo(Ap->entry,level);
		if(NewInfo)
		{
			Ap->level = level;
			Ap->Info = NewInfo;
		}
	}
}
void AbilityInterface::SendAllAbility()
{
	uint16 responseSize = 4+(m_ability.size()*3); // take care of this hard-coded value!
	Buffer* b = m_player->Allocate(responseSize+3);
	b->write<uint16>(responseSize); // take care of this hard-coded value!
	b->write<uint8>(F_CHARACTER_INFO);
	b->write<uint8>(1); //Actions
	b->write<uint8>(m_ability.size()); //Number of abilities in packet
	b->write<uint16>(0x0300); //Actions

	AbilityPlayer * Ap = NULL;

	for(int i=0;i<m_ability.size();++i)
	{
		Ap = m_ability[i];
		if(!Ap) continue;
		b->write<uint16>(Ap->entry);
		b->write<uint8>(Ap->level);
	}
	m_player->sendPacket(b);
}

void AbilityInterface::StartCast(uint32 entry)
{
	Log.Debug("AbilityInterface","%u Starting Cast %u",m_player->GetPlayerID(),entry);
	
	AbilityPlayer * Ap = GetAbility(entry);
	StartCast(Ap);
}
void AbilityInterface::StartCast(AbilityPlayer * Ap)
{
	GameData::AbilityResult Result = CanCast(Ap);

	if(Result != 0)
	{
		SendCastResult(Result);
		return;
	}

	// On Annule l'abilité uniquement pour en caster une autre
	if(m_currentAbility)
		if( m_currentAbility->GetInfo() != Ap->Info)
			m_currentAbility->Cancel();
		else if( !m_currentAbility->IsDone() && !m_currentAbility->IsCanceled() )
			return;

	Ability * Ab = new Ability(Ap->Info);
	Ab->Init(m_player);
	Ab->ApplyPlayerAbility(Ap);
	Result = Ab->StartCast();

	if(Result == 0)
	{
		m_player->m_nextApRegen=AP_REGEN_TIME*2;
		SetCurrentAbility(Ab);
		SetAbilityTimer(Ap);
	}
	else
		SendCastResult(Result);

}
void AbilityInterface::SendCastResult(GameData::AbilityResult Result)
{
	Log.Debug("AbilityInterface","SendCastResult %u",Result);


}
void AbilityInterface::SetAbilityTimer(AbilityPlayer * Ap)
{
	Ap->m_cooldown = getMSTime();
	Ap->m_cooldown += Ap->Info->cooldown;

	m_globalcooldown = getMSTime();
	m_globalcooldown += MIN_COOLDOWN;

	SendAbilityTimer(Ap->entry);
}
void AbilityInterface::SendAbilityTimer(uint32 entry)
{
	uint16 responseSize = 12;
	Buffer * b = new Buffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_SET_ABILITY_TIMER);
	b->write<uint16>(entry);
	b->fill(0,10);
	m_player->sendPacket(b);
}
GameData::AbilityResult AbilityInterface::CanCast(uint32 entry)
{
	AbilityPlayer * Ap = GetAbility(entry);
	return CanCast(Ap);
}
GameData::AbilityResult AbilityInterface::CanCast(AbilityPlayer * Info)
{
	if(!Info) return ABILITYRESULT_DISABLED;

	if(Info->m_cooldown > getMSTime()) return ABILITYRESULT_NOTREADY;
	if(m_globalcooldown > getMSTime()) return ABILITYRESULT_NOTREADY;

	if(m_currentAbility && m_currentAbility->GetInfo() == Info->Info && !m_currentAbility->IsDone() && m_currentAbility->IsCanceled() ) return ABILITYRESULT_QUEUED;

	if(Info->Info->actionpoints > m_player->GetAP()) return ABILITYRESULT_AP;

	return ABILITYRESULT_OK;
}
AbilityPlayer * AbilityInterface::GetAbility(uint32 entry)
{
	size_t size = m_ability.size();

	AbilityPlayer* AP = NULL;
	for(size_t t = 0; t < size; ++t)
	{
		AP = m_ability[t];

		if(AP && AP->Info && AP->Info->entry == entry)
			return AP;
	}
	return NULL;
}
AbilityPlayer * AbilityInterface::GetAbility(AbilityInfo * Info)
{
	if(Info)
		return GetAbility(Info->entry);
	else return NULL;
}
void AbilityInterface::SetCurrentAbility(Ability * Ab)
{
	Ability * Last = m_currentAbility;

	m_currentAbility = NULL;
	if(Last)
		Last->Delete();

	if(Ab && !Ab->IsDone() && !Ab->IsCanceled())
		m_currentAbility = Ab;
	else m_currentAbility = NULL;
}
void AbilityInterface::Update(uint32 tdiff)
{
	if(m_currentAbility)
	{
		if(m_currentAbility->IsDone() || m_currentAbility->IsCanceled())
			SetCurrentAbility(NULL);
		else m_currentAbility->Update(tdiff);
	}
}