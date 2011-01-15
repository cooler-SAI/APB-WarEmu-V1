#include "StdAfx.h"

void Simple_DB_AI::LoadFromDB(uint32 CreatureID)
{
	QueryResult * Result = WorldDatabase.Query("SELECT * FROM creature_events WHERE entry = '%u'", CreatureID);

	if(Result)
	{
		Simple_DB_AI_Event* Evt = NULL;

		do
		{
			Evt = new Simple_DB_AI_Event;

			Evt->EventType = Result->Fetch()[1].GetUInt8();
			Evt->EventParam = Result->Fetch()[2].GetUInt8();
			Evt->EventActionType = Result->Fetch()[3].GetUInt8();

			Evt->EventData1 = Result->Fetch()[4].GetUInt32();
			Evt->EventData2 = Result->Fetch()[5].GetUInt32();
			Evt->EventData3 = Result->Fetch()[6].GetUInt32();
			Evt->EventData4 = Result->Fetch()[7].GetUInt32();
			Evt->EventData5 = Result->Fetch()[8].GetUInt32();
			Evt->EventData6 = Result->Fetch()[9].GetUInt32();

			Evt->StringData1 = std::string(Result->Fetch()[10].GetString());
			Evt->StringData2 = std::string(Result->Fetch()[11].GetString());
			Evt->StringData3 = std::string(Result->Fetch()[12].GetString());

			Evt->tTimer.restart();

			m_eventstack.push_back(Evt);
		}
		while(Result->NextRow());
	}
}

void Simple_DB_AI::DoAction(Simple_DB_AI_Event* Evt)
{
	switch(Evt->EventActionType)
	{
	case ACTION_CAST:
		if(Evt->EventData3 == TARGET_SELF)
			Cast(m_creature, Evt->EventData4);
		else if(Evt->EventData3 == TARGET_ENNEMY)
			Cast(m_creature, Evt->EventData4);

		break;

	case ACTION_SAY:
		Say(std::string(Evt->StringData1));
		break;
	}
}

void Simple_DB_AI::HandleEvent(Simple_DB_AI_Event* Evt)
{
	switch(Evt->EventParam)
	{
	case PARAM_DOMELEE:
		DoMeleeAttack();
		break;
	case PARAM_DORANGED:
		DoRangedAttack();
		break;
	case PARAM_TIMED:
		// Verifier si le timer est dépasser
		if(Evt->tTimer.elapsed() > (double)(Evt->EventData1 / 1000.0))
		{
			DoAction(Evt);
			Evt->tTimer.restart();
		}

		break;
	case PARAM_HEALTH_PERCENT:
		// 0 pour pourcentage de vie supérieur ou égal
		if(Evt->EventData1 == 0)
			if(m_creature->GetHealthPct() >= Evt->EventData2)
				DoAction(Evt);

		// 1 pour pourcentage de vie inférieur ou égal
		if(Evt->EventData1 == 1)
			if(m_creature->GetHealthPct() <= Evt->EventData2)
				DoAction(Evt);

		// 2 pour pourcentage de vie égal
		if(Evt->EventData1 == 2)
			if(m_creature->GetHealthPct() == Evt->EventData2)
				DoAction(Evt);

		break;

	case PARAM_MANA_PERCENT:

		break;

	case PARAM_ENNEMI_COUNT:

		break;

	case PARAM_TARGET_HEALTH_PERCENT:
		// 0 pour pourcentage de vie supérieur ou égal
		if(Evt->EventData1 == 0)
			if(m_creature->GetTarget()->GetHealthPct() >= Evt->EventData2)
				DoAction(Evt);

		// 1 pour pourcentage de vie inférieur ou égal
		if(Evt->EventData1 == 1)
			if(m_creature->GetTarget()->GetHealthPct() <= Evt->EventData2)
				DoAction(Evt);

		// 2 pour pourcentage de vie égal
		if(Evt->EventData1 == 2)
			if(m_creature->GetTarget()->GetHealthPct() == Evt->EventData2)
				DoAction(Evt);

		break;

	case PARAM_RANDOM:
		uint32 uRandValue = RandomUInt(100);

		if(uRandValue >= Evt->EventData3)
			DoAction(Evt);

		break;
	}
}

void Simple_DB_AI::OnUpdate()
{
	Simple_DB_AI_Event* Evt = NULL;

	for(std::vector<Simple_DB_AI_Event*>::iterator itr = m_eventstack.begin(); itr != m_eventstack.end(); ++itr)
	{
		Evt = (*itr);

		if(!Evt || Evt->EventType != EVENT_UPDATE)
			continue;

		HandleEvent(Evt);
	}
}

void Simple_DB_AI::OnEnterCombat()
{
	Simple_DB_AI_Event* Evt = NULL;

	for(std::vector<Simple_DB_AI_Event*>::iterator itr = m_eventstack.begin(); itr != m_eventstack.end(); ++itr)
	{
		Evt = (*itr);

		if(!Evt || Evt->EventType != EVENT_ENTERCOMBAT)
			continue;

		HandleEvent(Evt);
	}
}

void Simple_DB_AI::OnDamageTaken()
{
	Simple_DB_AI_Event* Evt = NULL;

	for(std::vector<Simple_DB_AI_Event*>::iterator itr = m_eventstack.begin(); itr != m_eventstack.end(); ++itr)
	{
		Evt = (*itr);

		if(!Evt || Evt->EventType != EVENT_DAMAGETAKEN)
			continue;

		HandleEvent(Evt);
	}
}

void Simple_DB_AI::OnLeaveCombat()
{
	Simple_DB_AI_Event* Evt = NULL;

	for(std::vector<Simple_DB_AI_Event*>::iterator itr = m_eventstack.begin(); itr != m_eventstack.end(); ++itr)
	{
		Evt = (*itr);

		if(!Evt || Evt->EventType != EVENT_LEAVECOMBAT)
			continue;

		HandleEvent(Evt);
	}
}

void Simple_DB_AI::OnDeath()
{
	Simple_DB_AI_Event* Evt = NULL;

	for(std::vector<Simple_DB_AI_Event*>::iterator itr = m_eventstack.begin(); itr != m_eventstack.end(); ++itr)
	{
		Evt = (*itr);

		if(!Evt || Evt->EventType != EVENT_DEATH)
			continue;

		HandleEvent(Evt);
	}
}

void Simple_DB_AI::OnKilledUnit()
{
	Simple_DB_AI_Event* Evt = NULL;

	for(std::vector<Simple_DB_AI_Event*>::iterator itr = m_eventstack.begin(); itr != m_eventstack.end(); ++itr)
	{
		Evt = (*itr);

		if(!Evt || Evt->EventType != EVENT_KILLEDUNIT)
			continue;

		HandleEvent(Evt);
	}
}