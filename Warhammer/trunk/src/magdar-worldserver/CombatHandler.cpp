#include "StdAfx.h"

void ClientCom::onPlayerInfo() // New Target
{
	if( !m_Plr) return;
	if( !m_Plr->IsInWorld()) return;

	uint16 unk;
	uint16 toid;
	uint8 unk2,faction;
	m_readBuffer >> unk >> toid >> unk2 >> faction;

	if(!toid)
	{
		m_Plr->SetTarget( NULL );
		m_Plr->SetTargetType(GameData::TARGETTYPES_TARGET_NONE);
		m_Plr->EventAttackStop();
		return;
	}


	Object * obj = m_Plr->GetZoneMgr()->GetObjectByOid(toid);
	if( obj && m_Plr && obj->IsUnit() )
	{
		m_Plr->SetTarget( static_cast<Unit*>(obj) );
		m_Plr->SetTargetType(GameData::TargetTypes(faction));
		m_Plr->EventAttackStop();

		if(obj == m_Plr)
			sEventHook.ExecuteHook(EVENTS_TARGET_SELF,1,m_Plr);
		else sEventHook.ExecuteHook(EVENTS_PLAYER_TARGET_UPDATED,2,m_Plr,obj);
	}
}

void ClientCom::onAttack()
{
	if(!m_Plr) return;

	Unit * target = m_Plr->GetTarget();
	if(target==NULL)
	{
		//Log.Error("Player","NO TARGET");
		m_Plr->EventAttackStop();
		return;
	}

	if(m_Plr->m_TargetType != GameData::TARGETTYPES_TARGET_ENEMY || target->IsPlayer() || (target->IsUnit() && target->GetUnit()->IsDead()))
	{
		//Log.Error("Player","TARGET NO ATTACKABLE");
		m_Plr->EventAttackStop();
		return;
	}

	m_Plr->EventAttackStart();
}

