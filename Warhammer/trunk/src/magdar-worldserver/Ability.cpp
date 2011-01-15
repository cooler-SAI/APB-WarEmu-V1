#include "StdAfx.h"


using namespace GameData;

Ability::Ability(uint32 abilityid,uint8 level)
{
	m_info =  objmgr.GetAbilityInfo(abilityid,level);
	m_patch=NULL;
}
Ability::Ability(AbilityInfo * info)
{
	m_info = info;
	m_patch=NULL;
}
void Ability::Init(Object * Caster)
{
	if(!Caster) return;

	Log.Debug("Ability","Init Casting : Caster = %s",Caster->GetName().c_str());
	canceled=false;
	done=false;
	m_objcaster = NULL;
	m_unitcaster = NULL;
	m_plrcaster = NULL;
	m_creacaster = NULL;
	m_gocaster = NULL;

	m_objcaster = Caster;

	if(Caster->IsUnit())
	{
		m_unitcaster = Caster->GetUnit();

		if(Caster->IsPlayer())
			m_plrcaster = Caster->GetPlayer();
		else if(Caster->IsCreature())
			m_creacaster = Caster->GetCreature();
		else if(Caster->IsGameObject())
			m_gocaster = Caster->GetGameObject();
	}

	GenerateTargets();
}
void Ability::GenerateTargets()
{
	Log.Debug("Ability","GenerateTargets");
	m_target = new AbilityTarget();

	m_target->ClearTargets();

	if(m_plrcaster && m_plrcaster->GetTarget() && !m_plrcaster->GetTarget()->IsPlayer() && m_plrcaster->GetTargetType() == GameData::TARGETTYPES_TARGET_ENEMY)
		m_target->AddTarget(m_plrcaster->GetTarget());
}
void Ability::ApplyPlayerAbility(AbilityPlayer * Patch)
{
	Log.Debug("Ability","ApplyPlayerAbility");
	if(!Patch) return;

	m_patch = Patch;
}
void Ability::Cast(bool check)
{
	if(!m_info) return;

	Log.Debug("Ability","Cast");

	if(check)
	{
		GameData::AbilityResult Result = CanCast();

		if(Result != ABILITYRESULT_OK)
		{
			if(m_plrcaster)
				m_plrcaster->GetAbilityInterface()->SendCastResult(Result);
			canceled=true;
			return;
		}
	}

	m_target->ClearCount();
	Unit * target = NULL;
	do{
		target = m_target->GetTarget();
		if(!target) break;
		if(m_unitcaster)
		{
			uint32 damage = CalculAbilityDamage(target);
			SendStartingCast(target);
			SendSpellDamage(target,m_info->damage);
			m_unitcaster->DealDamage(target,m_info->damage,m_info);
		}

	}while(m_target->NextTarget());

	//SendEndedCast(target);
	done=true;
}
uint32 Ability::CalculAbilityDamage(Unit * target)
{
	if(!target) return 0;

	float str = m_unitcaster->GetStat(STATS_STRENGTH);

	float damage = m_info->damage;

	if(m_info->maxrange <= 5) // Melée ability , donc on ajoute le bonus de strenght
		damage += str/5.0f;

	return damage;
}
GameData::AbilityResult Ability::StartCast()
{
	if(!m_info) return ABILITYRESULT_ILLEGALTARGET;

	Log.Debug("Ability","Starting Casting %s",m_objcaster->GetName().c_str());
	GameData::AbilityResult Result = CanCast();

	if(Result != ABILITYRESULT_OK)
		return Result;

	if(!m_unitcaster->RemoveAP(m_info->actionpoints))
		return ABILITYRESULT_AP;

	if(m_info->casttime)
		m_endcast = getMSTime()+m_info->casttime;
	else m_endcast=0;

	return ABILITYRESULT_OK;
}
GameData::AbilityResult Ability::CanCast()
{
	Log.Debug("Ability","CanCast");
	if(!m_target) return ABILITYRESULT_ILLEGALTARGET;

	if(!m_target->HasTargets()) return ABILITYRESULT_ILLEGALTARGET;

	if(m_unitcaster->GetAP() < m_info->actionpoints) return ABILITYRESULT_AP;

	m_target->ClearCount();

	Unit * target = NULL;
	do{
		target = m_target->GetTarget();
		if(!target) break;

		if(m_unitcaster)
		{
			float distance = m_unitcaster->GetDistance(target);
			if(distance < m_info->minrange)
				return ABILITYRESULT_TOOCLOSE;

			if(distance > m_info->maxrange)
				return ABILITYRESULT_OUTOFRANGE;

			if(target->IsDead())
				return ABILITYRESULT_TARGET_DEAD;

			/*if(!m_unitcaster->isInFront(target)) // TODO : Fixer la rotation
				return ABILITYRESULT_OUT_OF_ARC;*/
		}

	}while(m_target->NextTarget());

	return ABILITYRESULT_OK;
}
void Ability::Update(uint32 tdiff)
{
	if(canceled || done)
		return;

	if(m_endcast <= getMSTime())
		Cast();
}
void Ability::Cancel()
{
	Log.Debug("Ability","Cancel");
	canceled=true;

	/*if(m_plrcaster)
	{
		m_plrcaster->GetAbilityInterface()->SendCastResult(ABILITYRESULT_INTERRUPTED);
		return;
	}*/
}
/*
{SERVER} Packet: (0x00DA) F_USE_ABILITY PacketSize = 20
|------------------------------------------------|----------------|
|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
|------------------------------------------------|----------------|
|00 00 1F DF 36 11 06 10 29 35 02 01 00 00 FF 2C |....6...)5.....,|
|07 00 00 00                                     |....            |
-------------------------------------------------------------------
*/

// Sender
void Ability::SendStartingCast(Unit * target)
{
	Log.Debug("Ability","SendStartingCast");

	uint16 responseSize = 10;
	Buffer *b = new Buffer(responseSize+3);

	b->write<uint16>(responseSize);
	b->write<uint8>(F_UPDATE_STATE);
	b->write<uint16>(m_unitcaster->GetOid());
	b->write<uint8>(0x1A);
	b->fill(0,7);
	m_unitcaster->dispachPacketInRange(b,true);
}
/*
{SERVER} Packet: (0x00D7) F_INIT_EFFECTS PacketSize = 11
|------------------------------------------------|----------------|
|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
|------------------------------------------------|----------------|
|01 01 00 00 36 11 08 00 2C 01 00                |....6...,..     |
-------------------------------------------------------------------

*/
void Ability::SendEndedCast(Unit * target)
{
	Log.Debug("Ability","SendEndedCast");

	uint16 responseSize = 20;
	Buffer *b = new Buffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_USE_ABILITY);
	b->write<uint16>(0);
	b->write<uint16>(m_info->entry);
	b->write<uint16>(m_unitcaster->GetOid());
	b->write<uint16>(0x0610);
	b->write<uint16>(target->GetOid());
	b->write<uint16>(0x0201);
	b->write<uint16>(0);
	b->write<uint32>(0xFF2C0700);
	b->write<uint16>(0);
	m_unitcaster->dispachPacketInRange(b,true);

	if(m_plrcaster)
	{
		Buffer *d = new Buffer(4+3);
		d->write<uint16>(4);
		d->write<uint8>(F_MAIL);
		d->write<uint32>(0);
		m_plrcaster->sendPacket(d);
	}
	
}
/*
{SERVER} Packet: (0x0014) F_HIT_PLAYER PacketSize = 8
|------------------------------------------------|----------------|
|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
|------------------------------------------------|----------------|
|36 11 26 63 00 AC 42 00                         |6.&c..B.        |
-------------------------------------------------------------------*/
void Ability::SendSpellDamage(Unit * target,uint32 damage)
{
	Log.Debug("Ability","SendSpellDamage");
	uint16 responseSize = 13;
	Buffer *b = new Buffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_CAST_PLAYER_EFFECT);
	b->write<uint16>(m_unitcaster->GetOid());
	b->write<uint16>(target->GetOid());
	b->write<uint16>(m_info->entry);
	b->write<uint8>(02);
	b->write<uint16>(0007);
	b->write<uint8>( (damage*2)-1); // AF = 175
	b->write<uint8>(0x01);
	b->write<uint16>(0x3E18);
	m_unitcaster->dispachPacketInRange(b,true);
}