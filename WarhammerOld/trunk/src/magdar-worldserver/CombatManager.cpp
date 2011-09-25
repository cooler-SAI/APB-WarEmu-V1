#include "StdAfx.h"

void CombatManager::ClearMyHealers()
{
	// this is where we check all our healers
	HealedSet::iterator i;
	Player * pt=NULL;
	for(i = m_healers.begin(); i != m_healers.end(); ++i)
	{
		pt = m_Unit->GetZoneMgr()->GetPlayer( *i );

		if(pt != NULL)
			pt->CombatStatus->RemoveHealed(m_Unit);
	}

	m_healers.clear();
}

void CombatManager::WeHealed(Unit * pHealTarget)
{
	if(pHealTarget == NULL)
		return;

	if(!pHealTarget->IsPlayer() || !m_Unit->IsPlayer() || pHealTarget == m_Unit)
		return;

	if(pHealTarget->CombatStatus->IsInCombat())
	{
		m_healed.insert(pHealTarget->GetOid());
		pHealTarget->CombatStatus->m_healers.insert(m_Unit->GetOid());
	}

	UpdateFlag();
}

void CombatManager::RemoveHealed(Unit * pHealTarget)
{
	if(pHealTarget == NULL)
		return;

	m_healed.erase(pHealTarget->GetOid());
	UpdateFlag();
}

void CombatManager::UpdateFlag()
{
	bool n_status = InternalIsInCombat();
	if(n_status != m_lastStatus)
	{
		m_lastStatus = n_status;
		if(n_status)
		{
			//printf(I64FMT" is now in combat.\n", m_Unit->GetGUID());
			Log.Success("CombatMgr","%u is now in combat.",m_Unit->GetOid());
			//m_Unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_COMBAT);
			//if(!m_Unit->hasStateFlag(UF_ATTACKING)) m_Unit->addStateFlag(UF_ATTACKING);
		}
		else
		{
			Log.Success("CombatMgr","%u is no longer in combat.",m_Unit->GetOid());
			//printf(I64FMT" is no longer in combat.\n", m_Unit->GetGUID());
			//m_Unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_COMBAT);
			//if(m_Unit->hasStateFlag(UF_ATTACKING)) m_Unit->clearStateFlag(UF_ATTACKING);

			// remove any of our healers from combat too, if they are able to be.
			//ClearMyHealers();

			//if( m_Unit->IsPlayer() )
				//TO_PLAYER(m_Unit)->UpdatePotionCooldown();
		}
	}
}

bool CombatManager::InternalIsInCombat()
{
	if(m_healed.size() > 0)
		return true;

	if(m_attackTargets.size() > 0)
		return true;

	if(m_attackers.size() > 0)
		return true;

	return false;
}

void CombatManager::AddAttackTarget(uint16 oid)
{
	if(oid == m_Unit->GetOid())
		return;

	Log.Success("CombatMgr","%u AddAttackTarget(%u)",oid,m_Unit->GetOid());
	m_attackTargets.insert(oid);
	//printf("Adding attack target "I64FMT" to "I64FMT"\n", guid, m_Unit->GetGUID());
	if(m_Unit->IsPlayer() &&
		m_primaryAttackTarget != oid)			// players can only have one attack target.
	{
		if(m_primaryAttackTarget)
			ClearPrimaryAttackTarget();

		m_primaryAttackTarget = oid;
	}

	UpdateFlag();
}

void CombatManager::ClearPrimaryAttackTarget()
{
	//printf("ClearPrimaryAttackTarget for "I64FMT"\n", m_Unit->GetGUID());
	if(m_primaryAttackTarget != 0)
	{
		Unit * pt = m_Unit->GetZoneMgr()->GetUnit(m_primaryAttackTarget);
		if(pt != NULL)
		{
			// remove from their attacker set. (if we have no longer got any DoT's, etc)
			if(!IsAttacking(pt))
			{
				pt->CombatStatus->RemoveAttacker(m_Unit, m_Unit->GetOid());
				m_attackTargets.erase(m_primaryAttackTarget);
			}

			m_primaryAttackTarget = 0;
		}
		else
		{
			m_attackTargets.erase(m_primaryAttackTarget);
			m_primaryAttackTarget = 0;
		}
	}

	UpdateFlag();
}

bool CombatManager::IsAttacking(Unit * pTarget)
{
	// check the target for any of our DoT's.
	/*for(uint32 i = MAX_NEGATIVE_AURAS_EXTEDED_START; i < MAX_NEGATIVE_AURAS_EXTEDED_END; ++i)
		if(pTarget->m_auras[i] != NULL)
			if(m_Unit->GetGUID() == pTarget->m_auras[i]->m_casterGuid && pTarget->m_auras[i]->IsCombatStateAffecting())
				return true;*/

	// place any additional checks here
	return false;
}

void CombatManager::RemoveAttackTarget(Unit * pTarget)
{
	// called on aura remove, etc.
	AttackerMap::iterator itr = m_attackTargets.find(pTarget->GetOid());
	if(itr == m_attackTargets.end())
		return;

	if(!IsAttacking(pTarget))
	{
		//printf("Removing attack target "I64FMT" on "I64FMT"\n", pTarget->GetGUID(), m_Unit->GetGUID());
		m_attackTargets.erase(itr);
		if(m_primaryAttackTarget == pTarget->GetOid())
			m_primaryAttackTarget = 0;

		UpdateFlag();
	}
	/*else
		printf("Cannot remove attack target "I64FMT" from "I64FMT"\n", pTarget->GetGUID(), m_Unit->GetGUID());*/
}

void CombatManager::RemoveAttacker(Unit * pAttacker,uint16 oid)
{
	AttackerMap::iterator itr = m_attackers.find(oid);
	if(itr == m_attackers.end())
		return;

	if( (!pAttacker) || (!pAttacker->CombatStatus->IsAttacking(m_Unit)) )
	{
		//printf("Removing attacker "I64FMT" from "I64FMT"\n", guid, m_Unit->GetGUID());
		m_attackers.erase(itr);
		UpdateFlag();
	}
	/*else
	{
		printf("Cannot remove attacker "I64FMT" from "I64FMT"\n", guid, m_Unit->GetGUID());
	}*/
}

void CombatManager::OnDamageDealt( Unit * pTarget )
{
	// we added an aura, or dealt some damage to a target. they need to have us as an attacker, and they need to be our attack target if not.
	//printf("OnDamageDealt to "I64FMT" from "I64FMT"\n", pTarget->GetGUID(), m_Unit->GetGUID());
	if( pTarget == m_Unit )
		return;

	//no need to be in combat if dead
	if( pTarget->IsDead() || m_Unit->IsDead() )
		return;

	AttackerMap::iterator itr = m_attackTargets.find( pTarget->GetOid() );
	if( itr == m_attackTargets.end() )
		AddAttackTarget(pTarget->GetOid());

	itr = pTarget->CombatStatus->m_attackers.find( m_Unit->GetOid() );
	if( itr == pTarget->CombatStatus->m_attackers.end() )
		pTarget->CombatStatus->AddAttacker( m_Unit->GetOid() );

	// update the timeout
	//m_Unit->CombatStatusHandler_ResetPvPTimeout();
}

void CombatManager::AddAttacker(uint16 oid)
{
	m_attackers.insert(oid);
	UpdateFlag();
}

void CombatManager::ClearAttackers()
{
	// this is a FORCED function, only use when the reference will be destroyed.
	AttackerMap::iterator itr = m_attackTargets.begin();
	Unit * pt;
	for(; itr != m_attackTargets.end(); ++itr)
	{
		pt = m_Unit->GetZoneMgr()->GetUnit(*itr);
		if(pt)
		{
			pt->CombatStatus->m_attackers.erase(m_Unit->GetOid());
			pt->CombatStatus->UpdateFlag();
		}
	}

	for(itr = m_attackers.begin(); itr != m_attackers.end(); ++itr)
	{
		pt = m_Unit->GetZoneMgr()->GetUnit(*itr);
		if(pt)
		{
			pt->CombatStatus->m_attackTargets.erase(m_Unit->GetOid());
			pt->CombatStatus->UpdateFlag();
		}
	}

	m_attackers.clear();
	m_attackTargets.clear();
	m_primaryAttackTarget = 0;
	UpdateFlag();
}

void CombatManager::ClearHealers()
{

	if( m_Unit->GetZoneMgr() == NULL )
		return; //wierd crash bug

	HealedSet::iterator itr = m_healed.begin();
	Player * pt=NULL;
	for(; itr != m_healed.end(); ++itr)
	{
		pt = m_Unit->GetZoneMgr()->GetPlayer(*itr);
		if(pt)
		{
			pt->CombatStatus->m_healers.erase(m_Unit->GetOid());
			pt->CombatStatus->UpdateFlag();
		}
	}

	for(itr = m_healers.begin(); itr != m_healers.end(); ++itr)
	{
		pt = m_Unit->GetZoneMgr()->GetPlayer(*itr);
		if(pt)
		{
			pt->CombatStatus->m_healed.erase(m_Unit->GetOid());
			pt->CombatStatus->UpdateFlag();
		}
	}

	m_healed.clear();
	m_healers.clear();
	UpdateFlag();
}

void CombatManager::OnRemoveFromWorld()
{
	ClearAttackers();
	ClearHealers();
}

bool CombatManager::IsInCombat()
{
	// If the unit doesn't exist - OR - the unit exists but is not in world
	if( !m_Unit || ( m_Unit && !m_Unit->IsInWorld() ) )
		return false;

	switch (m_Unit->GetType())
	{
		case TYPEID_CREATURE:
		case TYPEID_GAMEOBJECT:
		{
			/*if (m_Unit->IsPet() && ((Pet*)m_Unit)->GetPetAction() == PET_ACTION_ATTACK)
				return true;
			else if (m_Unit->IsPet())
				return m_lastStatus;
			else
			*/

			if(m_Unit->GetAIInterface())
			{
				if(m_Unit->GetAIInterface()->GetTarget()==NULL)
					return false;
				else return true;
			}
			else return false;

		} break;
		case TYPEID_PLAYER:
		{
			/*if (((Player*)m_Unit)->GetSummon() != NULL && ((Player*)m_Unit)->GetSummon()->GetPetOwner() == m_Unit && ((Player*)m_Unit)->GetSummon()->CombatStatus.IsInCombat())
				return true;*/

			return m_lastStatus;
		} break;
		default: return false;
	}
	return false;
}