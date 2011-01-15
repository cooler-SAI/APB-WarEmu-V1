#include "StdAfx.h"

void ScriptedAI::DoMeleeAttack()
{
	if(m_creature->GetAIInterface())
		m_creature->GetAIInterface()->AIDoMelee();
}

void ScriptedAI::DoRangedAttack()
{

}

void ScriptedAI::Say(std::string Text)
{
	m_creature->SayText(Text);
}

void ScriptedAI::Yell(std::string Text)
{
	m_creature->ShoutText(Text);
}

void ScriptedAI::Cast(Unit* pTarget, uint32 SpellID)
{

}

void ScriptedAI::MoveTo(uint16 x, uint16 y, uint16 z, void (*DestinationReachedCallback)())
{
	m_dstcallback = DestinationReachedCallback;
	m_creature->MoveTo(x, y, z);
}

Unit* ScriptedAI::GetRandomTarget()
{
	if(m_creature->GetAIInterface())
	{
		int total = m_creature->GetAIInterface()->GetTargets().size();

		uint32 u_rand = RandomUInt(total);
		uint32 u_count = 0;

		uint32 u_alivecount = 0;

		for(std::map<Unit*,AttackerInfo*>::const_iterator itr = m_creature->GetAIInterface()->GetTargets().begin(); itr != m_creature->GetAIInterface()->GetTargets().end(); ++itr)
			if((*itr).first && (*itr).second && !(*itr).first->IsDead())
				if((*itr).first->GetZoneId() == m_creature->GetZoneId())
					u_alivecount++;

		if(u_alivecount < 2)
			return m_creature->GetTarget();

		for(std::map<Unit*,AttackerInfo*>::const_iterator itr = m_creature->GetAIInterface()->GetTargets().begin(); itr != m_creature->GetAIInterface()->GetTargets().end(); ++itr)
		{
			u_count++;

			if(u_count == u_rand)
			{
				if((*itr).first && (*itr).second && !(*itr).first->IsDead())
					if((*itr).first->GetZoneId() == m_creature->GetZoneId())
						return (*itr).first;
				else
					return GetRandomTarget();
			}
		}
	}

	return m_creature->GetTarget();
}

Unit* ScriptedAI::GetTopAggroTarget()
{
	if(m_creature->GetAIInterface())
	{
		std::map<uint32, Unit*> OrderedAggroList;

		for(std::map<Unit*,AttackerInfo*>::const_iterator itr = m_creature->GetAIInterface()->GetTargets().begin(); itr != m_creature->GetAIInterface()->GetTargets().end(); ++itr)
			if((*itr).second && (*itr).first && !(*itr).first->IsDead())
				if((*itr).first->GetZoneId() == m_creature->GetZoneId())
					OrderedAggroList[(*itr).second->AggroValue] = (*itr).first;

		Unit* pLastValue = NULL;

		for(std::map<uint32, Unit*>::const_iterator itr = OrderedAggroList.begin(); itr != OrderedAggroList.end(); ++itr)
			pLastValue = (*itr).second;

		return pLastValue;
	}

	return NULL;
}

Unit* ScriptedAI::GetLastAggroTarget()
{
	if(m_creature->GetAIInterface())
	{
		std::map<uint32, Unit*> OrderedAggroList;

		for(std::map<Unit*,AttackerInfo*>::const_iterator itr = m_creature->GetAIInterface()->GetTargets().begin(); itr != m_creature->GetAIInterface()->GetTargets().end(); ++itr)
			if((*itr).second && (*itr).first && !(*itr).first->IsDead())
				if((*itr).first->GetZoneId() == m_creature->GetZoneId())
					OrderedAggroList[(*itr).second->AggroValue] = (*itr).first;

		for(std::map<uint32, Unit*>::const_iterator itr = OrderedAggroList.begin(); itr != OrderedAggroList.end(); ++itr)
			return (*itr).second;
	}

	return NULL;
}

std::vector<Unit*> ScriptedAI::GetTargets()
{
	std::vector<Unit*> ret;

	if(m_creature->GetAIInterface())
		for(std::map<Unit*,AttackerInfo*>::const_iterator itr = m_creature->GetAIInterface()->GetTargets().begin(); itr != m_creature->GetAIInterface()->GetTargets().end(); ++itr)
			if((*itr).second && (*itr).first && !(*itr).first->IsDead())
				if((*itr).first->GetZoneId() == m_creature->GetZoneId())
					ret.push_back((*itr).first);

	return ret;
}

std::vector<Unit*> ScriptedAI::GetTargetsInArea(float Radius)
{
	std::vector<Unit*> ret;

	if(m_creature->GetAIInterface())
		for(std::map<Unit*,AttackerInfo*>::const_iterator itr = m_creature->GetAIInterface()->GetTargets().begin(); itr != m_creature->GetAIInterface()->GetTargets().end(); ++itr)
			if((*itr).second && (*itr).first && !(*itr).first->IsDead())
				if((*itr).first->GetZoneId() == m_creature->GetZoneId())
					if((*itr).first->GetDistance(m_creature) < Radius)
						ret.push_back((*itr).first);

	return ret;
}


