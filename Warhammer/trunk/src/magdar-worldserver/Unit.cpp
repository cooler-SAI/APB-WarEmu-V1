#include "StdAfx.h"

Unit::Unit() : Object()
{ 
	m_type = TYPEID_UNIT;
	m_speed=100;
	m_level=1;
	m_model=1;
	m_scale=50;
	m_sex=0;
	m_health=1;
	m_maxhealth=1;
	m_maxactionpoints = 250;
	m_actionpoints = 250;
	m_ismoving=false;
	m_Ranklevel=0;
	
	m_nextrespawn=0;
	m_nextHpRegen = HP_REGEN_TIME;
	m_nextApRegen = AP_REGEN_TIME;
	m_nextareaupdate=AREA_UPDATE_TIME;
	m_nextgraphical=0;

	m_aiInterface = new AIInterface();
	CombatStatus = new CombatManager();
	CombatStatus->SetUnit(this);
	m_pq=NULL;
	m_Area=NULL;
};

void Unit::Update( uint32 p_time )
{
	uint32 mstime = getMSTime();

	if(m_nextrespawn && m_nextrespawn <= UNIXTIME)
	{
		RezUnit();
	}

	if(!IsDead())
	{
        if( p_time >= m_nextHpRegen )
		    RegenerateHealth();
	    else
		    m_nextHpRegen -= p_time;

		if( p_time >= m_nextApRegen)
			RegenerateAP();
		else m_nextApRegen -= p_time;
	}
	
	if(m_nextareaupdate>p_time)
		m_nextareaupdate-=p_time;
	else
	{
		m_nextareaupdate=AREA_UPDATE_TIME;
		UpdateArea(p_time);
	}

}
void Unit::UpdateArea(uint32 tdiff)
{
	if(IsPlayer())
	{
		m_zoneMgr->CheckInRangeArea(GetPlayer());
		m_zoneMgr->CheckInRangeInfluence(GetPlayer());
	}
	m_zoneMgr->CheckInRangePq(this);
}

void Unit::RegenerateHealth()
{
	m_nextHpRegen = HP_REGEN_TIME;

	if(CombatStatus->IsInCombat())
		return;

	if(m_health == m_maxhealth)
		return;

	uint32 mhp = GetTotalHealth()/8;
	if(mhp+m_health > GetTotalHealth())
		m_health = GetTotalHealth();
	else m_health += mhp;

	if(IsPlayer())
		GetPlayer()->sendHealth();
	else if(IsCreature())
		GetCreature()->SendObjectState();

}
void Unit::RegenerateAP()
{
	m_nextApRegen = AP_REGEN_TIME;

	if(GetCurrentAbility())
	{
		return;
	}

	if(m_actionpoints < m_maxactionpoints) // No Regen When Casting
	{
		uint16 regen = 25; // 25 Regen + items Bonus

		if(m_actionpoints+regen > m_maxactionpoints)
			m_actionpoints = m_maxactionpoints;
		else m_actionpoints+=regen;

		if(IsPlayer())
			GetPlayer()->sendHealth();
		else if(IsCreature())
			GetCreature()->SendObjectState();
	}
}
Ability * Unit::GetCurrentAbility()
{
	if(IsPlayer())
		return GetPlayer()->GetAbilityInterface()->GetCurrentAbility();
	else return NULL;
}
bool Unit::RemoveAP(uint16 counts) 
{ 
	if(counts > m_actionpoints) 
		return false;
	else m_actionpoints-=counts;

	if(IsPlayer()) 
		GetPlayer()->sendHealth(); 

	return true; 
}
void Unit::Strike( Unit * pVictim ) // Calcul All damage info
{
	if(!pVictim) return;
	if(!pVictim->IsInWorld()) return;
	if(pVictim->IsDead()) return;

	uint16 dmg = 1;
	uint16 m_realdmg = 0;
	uint16 mitiged = 0;

	float m_special[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	// SPECIAL CASE
	float dmgreductionpct = 0;
	float dmgreduction = 0;

	// DAMAGE CALCULATION
	if(IsPlayer())
		dmg = GetPlayer()->CalculDamage();
	else if(IsCreature())
	{
		uint8 level = GetLevel();
		dmg=10+10*level+(m_Ranklevel*15);
	}

	// VICTIM STATS EFFEXT
	if(pVictim->IsPlayer())
	{
		// Toughness / 5 = Reduces damage by X DPS. 
		Player * pPlayer = pVictim->GetPlayer();
		if(!pPlayer) return;
		if(!pPlayer->IsInWorld()) return;

		float Tou = pPlayer->GetStat(GameData::STATS_TOUGHNESS);
		dmgreduction+=Tou/5;

		// Armor: <Percent Damage Reduction> = Armor / (Level * 1.1)
		float armor = pPlayer->GetItemInterface()->GetTotalArmor();
		dmgreductionpct += armor / (float(pPlayer->GetLevel()) * 1.1);

		// Block:  = [Block Rating of Shield / (Level * 7.5 + 50)] * 20 
		Item * itm = pPlayer->GetItemInterface()->GetItemInSlot(MAIN_GAUCHE);
		if(itm)
		{
			float block = itm->GetStat(GameData::BONUSTYPES_EBONUS_BLOCK);
			if(block) m_special[GameData::COMBATEVENT_BLOCK]+= (block / (pPlayer->GetLevel()*7.5+50)) * 20;
		}

		// Parry:  = [Weapon Skill / (Level * 7.5 + 50)] * 7.5 
		float Wsk = pPlayer->GetStat(GameData::STATS_WEAPONSKILL);
		m_special[GameData::COMBATEVENT_PARRY]+= (Wsk / (pPlayer->GetLevel()*7.5+50)) * 7.5;

		// Evade:  = [Initiative / (Level * 7.5 + 50)] * 7.5 
		float Ini = pPlayer->GetStat(GameData::STATS_INITIATIVE);
		m_special[GameData::COMBATEVENT_EVADE]+= (Ini / (pPlayer->GetLevel()*7.5+50)) * 7.5;

		// Disrupt:  = [Willpower / (Level * 7.5 + 50)] * 7.5 
		float Will = pPlayer->GetStat(GameData::STATS_WILLPOWER);
		m_special[GameData::COMBATEVENT_DISRUPT]+= (Will / (pPlayer->GetLevel()*7.5+50)) * 7.5;

		// Initiative: (Level * 7.5 + 50) / Initiative * .1 * 100 = +% to be critically hit. 
		m_special[GameData::COMBATEVENT_CRITICAL]+= ((pPlayer->GetLevel()*7.5+50)/Ini) * 10;
	}
	else 
	{
		m_special[GameData::COMBATEVENT_CRITICAL]+=(GetLevel()*2);
		dmgreductionpct+=25; // + 25% reduction
	}

	dmgreductionpct = min(75.0f,dmgreductionpct);

	if(dmg > dmgreduction)
	{
		dmg-=dmgreduction;
		m_realdmg = dmg-( (float(dmg)/float(100)) * dmgreductionpct);
	}
	else m_realdmg=5*GetLevel();

	mitiged = dmg-m_realdmg;

	Log.Debug("Unit","Strike ,%s dmg=%u,real=%u",GetName().c_str(),dmg,m_realdmg);

	GameData::CombatEvent Event;
	float chance = RandomFloat(100.0f);
	//vector<int> m_ok;
	int i=0;
	for(i=0;i<=10;++i)
	{
		if(chance<m_special[i])
			break;
	}

	/*if(m_ok.size())
	{
		uint32 s = m_ok.size();
		i = m_ok[RandomUInt(s-1)];
	}*/

	if(i>=10)
		Event=GameData::COMBATEVENT_HIT;
	else 
	{
		Event=GameData::CombatEvent(i);
		switch(Event)
		{
		case GameData::COMBATEVENT_CRITICAL:
			{
				m_realdmg+=(m_realdmg*50)/100;
			}break;
		case GameData::COMBATEVENT_BLOCK:
			{
				m_realdmg=0;
			}break;
		case GameData::COMBATEVENT_PARRY:
			{
				m_realdmg=0;
			}break;
		case GameData::COMBATEVENT_EVADE:
			{
				m_realdmg=0;
			}break;
		case GameData::COMBATEVENT_DISRUPT:
			{
				m_realdmg=0;
			}break;
		};
	}

	SendAttackState(pVictim,m_realdmg,mitiged,Event);
	DealDamage(pVictim,m_realdmg);
}
bool Unit::canReachWithAttack(Unit * pVictim)
{
	if( pVictim == NULL )
		return false;

	if( GetZoneMgr() != pVictim->GetZoneMgr() )
		return false;

	float distance = GetDistance(pVictim);
	return ( distance <= 5.0f );
}
void Unit::SendAttackState(Unit * Victim,uint16 damage,uint16 mitiged,GameData::CombatEvent Event,AbilityInfo * Info)
{
	if(!Victim)
		return;

	// Frappe
	{
		uint16 responseSize = 20;
		Buffer * b = new Buffer(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_USE_ABILITY);
		b->write<uint16>(0x00);
		b->write<uint16>(Info ? Info->entry : GetOid() );
		b->write<uint16>(GetOid());
		b->write<uint16>(Info ? 0x0610 : 0x00); // Spell Name , Ability.csv
		b->write<uint16>(Victim->GetOid());
		if(!Info)
		{
			b->write<uint8>(0x02);
			b->fill(0,9);
		}
		else
		{
			b->write<uint8>(0x06);
			b->write<uint8>(1);
			b->write<uint16>(0);
			b->write<uint32>(0x023F0C00);
			b->write<uint16>(0);
		}

		dispachPacketInRange(b,true);
		delete b;
	}

	// Dommage
	{
		uint16 responseSize = 12;
		Buffer * b = new Buffer(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_CAST_PLAYER_EFFECT);
		b->write<uint16>(GetOid());
		b->write<uint16>(Victim->GetOid());

		b->write<uint16>(Info ? Info->entry : 0x00);
		b->write<uint8>(Info ? 2 : 0x00);
		b->write<uint8>(Event);

		b->write<uint8>(Info ? 7 : 0x13); // Effect
		if(!Info)
		{
			b->write<uint8>( damage ? (damage*2)-1 : 0 );
			b->write<uint8>( uint8(mitiged*2) );
		}
		else
			b->write<uint16>(0x0799);

		b->write<uint8>(0x00);

		dispachPacketInRange(b,true);
		delete b;
	}

	if(Info)
	{
		uint16 responseSize = 10;
		Buffer * b = new Buffer(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_CAST_PLAYER_EFFECT);
		b->write<uint16>(GetOid());
		b->write<uint16>(Victim->GetOid());
		b->write<uint16>(Info->entry);
		b->write<uint8>(2);
		b->fill(0,3);
		dispachPacketInRange(b,true);
		delete b;
	}


}
void Unit::SetLootable(bool loot,Player * Plr)
{
	if(Plr == NULL) return;

	uint16 responseSize = 10;
	Buffer * b = new Buffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_UPDATE_STATE);
	b->write<uint16>(GetOid());
	b->write<uint8>(9);
	b->write<uint8>(loot);
	b->fill(0,6);

	if(!Plr->GetGroup())
		Plr->sendPacket(b,false);
	else 
		Plr->GetGroup()->DispachToGroup(b);

	if(!loot)
		sEventHook.ExecuteHook(EVENTS_INTERACT_LOOT_CLOSE,1,this);
}
void Unit::DealDamage(Unit * Victim,uint16 damage,AbilityInfo * Info)
{
	if(Victim==NULL || Victim->IsDead())
		return;

	Victim->GetAIInterface()->AttackReaction( this, damage );

	if(Victim->m_health <= damage) // Unit die
	{
		Victim->m_health=0;
		Victim->CombatStatus->Vanished();
	}
	else
	{
		Victim->m_health-=damage;
	}

	uint16 responseSize = 8;
	Buffer * b = new Buffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_HIT_PLAYER);
	b->write<uint16>(GetOid());
	b->write<uint16>(Victim->GetOid());
	b->write<uint8>(0x02);
	b->write<uint8>( Victim->GetTotalHealth() );
	b->write<uint8>( Victim->GetHealthPct() );
	b->write<uint8>(0x00);
	dispachPacketInRange(b,true);
	delete b;
	
	if(IsPlayer())
	{
		if(GetPublicQuest())
			GetPublicQuest()->PlayerDealDamage(GetPlayer(),Victim,damage);
	}

	if(Victim->m_health == 0)
	{
		Victim->SetUnitDeath(this);
		objmgr.CalculXpToGive(this,Victim);

		if(GetAIInterface())
			GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT,Victim,0);
	}

	if(Victim->IsPlayer())
		Victim->GetPlayer()->sendHealth();

	CombatStatus->OnDamageDealt( Victim );
}
void Unit::SetUnitDeath(Unit * killer)
{
	Log.Debug("Unit","SetUnitDeath %s",GetName().c_str());
	{
		uint16 responseSize = 12;
		Buffer * b = new Buffer(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_OBJECT_DEATH);
		b->write<uint16>(GetOid());

		b->write<uint8>(0x01); // Unk sometimes 05
		b->write<uint8>(0x00);
		b->write<uint16>(killer->GetOid());

		b->write<uint16>(0); // SpellKill ? 01 = true
		b->write<uint16>(0); // Unk
		b->write<uint16>(0); // Spell who kill unit

		killer->dispachPacketInRange(b,true);
		delete b;
	}

	Creature * cKiller = NULL;
	Player * pKiller = NULL;
	GameObject * goKiller = NULL;

	Creature * cVictim = NULL;
	Player * pVictim = NULL;

	if(killer->IsPlayer())
	{
		pKiller = killer->GetPlayer();

		if(GetPublicQuest())
			GetPublicQuest()->PlayerKillUnit(pKiller,this);
	}
	else if(killer->IsCreature())
	{
		cKiller = killer->GetCreature();

		if(cKiller->m_script)
			cKiller->m_script->OnKilledUnit();
	}
	else if(killer->IsGameObject())
	{
		goKiller = killer->GetGameObject();
	}

	if(IsCreature())	cVictim = GetCreature();
	else if(IsPlayer())	pVictim = GetPlayer();

	if(cVictim)
	{
		if(cVictim->m_script)
			cVictim->m_script->OnDeath();

		cVictim->AddByte(3);

		if(GetAIInterface() && GetAIInterface()->m_firstattacker)
			pKiller = GetAIInterface()->m_firstattacker;

		if(pKiller) // Send loot only if killer is Player or killer summoned by Player TODO
		{
			pKiller->GetQuestInterface()->onHandleEvent(QUEST_KILL_MOB,1,this,NULL,pKiller);
			
			if(pKiller->GetPublicQuest())
				pKiller->GetPublicQuest()->HandleEvent(QUEST_KILL_MOB,1,this,NULL,pKiller);

			objmgr.GenerateLoot(this,pKiller);
		}

		m_nextrespawn = UNIXTIME+GetCreature()->GetCreatureProto()->respawntime;
	}
	else if(pVictim)
	{
		pVictim->sendShowDialog(objmgr.RezTime,5);
		m_nextrespawn = objmgr.RezTime+UNIXTIME;

		sEventHook.ExecuteHook(EVENTS_PLAYER_DEATH,1,pVictim);

		if(pVictim->GetPublicQuest())
			pVictim->GetPublicQuest()->PlayerDie(pVictim,killer);
	}

	if(GetPublicQuest())
		GetPublicQuest()->UnitDie(this,killer);

	m_health=0;
}
void Unit::setInFront(Object * target) // not the best way to do it, though
{
	if(target==NULL)
		return;

	//angle the object has to face
	SetPosition(GetX(),GetY(),GetZ(),GetFacing(target),0,0,true);
}
void Unit::SendLootTo(Player * Plr,Struct_f_interract interract)
{
	if(Plr == NULL) return;

	Loot * Info = GetLoot();

	Log.Debug("Unit","SendLootTo");
	if(!Info || (Info && !Info->IsValid()) )
	{
		Log.Debug("Unit","IsNot Valid");
		SetLootable(false,Plr);
		delete Info;
	}
	else
	{	
		Info->HandleInteract(Plr,interract);
	}
}
void Unit::RemoveFromWorld()
{
	CombatStatus->OnRemoveFromWorld();

	Object::RemoveFromWorld();
}
void Unit::RezUnit()
{
	m_nextrespawn=0;

	if(IsCreature())
	{
		Creature * Crea = GetCreature();
		Crea->RemoveByte(3); // DEATH
		Crea->RemoveFromWorld();
		Crea->Load(); // reload all info
		Crea->AddToWorld(true);
	}
	else if(IsPlayer())
	{
		Player * Plr = GetPlayer();
		Plr->RezPlayer();
	}
}
bool Unit::HasQuestForPlayer(Player * Plr)
{
	if(Plr == NULL)
		return false;

	if(!HasQuests())
		return false;

	for(list<QuestInfo *>::iterator itr=QuestsBegin();itr!=QuestsEnd();++itr)
		if(questmgr.CanStartQuest(Plr,(*itr)))
			return true;

	return false;
}
bool Unit::HasQuestDoneForPlayer(Player * Plr)
{
	if(Plr == NULL)
		return false;

	if(!HasFinisher())
		return false;

	for(list<QuestInfo *>::iterator itr=QuestsFinishBegin();itr!=QuestsFinishEnd();++itr)
		if(Plr->GetQuestInterface()->HasAchievedQuest((*itr)->entry))
			return true;

	return false;
}
bool Unit::HasQuestToDoneForPlayer(Player * Plr)
{
	if(Plr == NULL)
		return false;

	if(!HasFinisher())
		return false;

	for(list<QuestInfo *>::iterator itr=QuestsFinishBegin();itr!=QuestsFinishEnd();++itr)
		if(Plr->GetQuestInterface()->HasQuest((*itr)->entry))
			return true;

	return false;
}
QuestInfo * Unit::GetQuest(uint32 questid)
{
	for(list<QuestInfo *>::iterator itr=QuestsBegin();itr!=QuestsEnd();++itr)
		if((*itr) && (*itr)->entry==questid)
			return (*itr);

	return NULL;
}