#include "StdAfx.h"

AIInterface::AIInterface()
{
	m_Unit=NULL;
	m_spawn.ChangeCoords(0,0,0,0);
	m_return.ChangeCoords(0,0,0,0);
	m_dest.ChangeCoords(0,0,0,0);
	m_state=STATE_IDLE;
	m_target=NULL;
	m_targets.clear();
	m_nextUpdateTarget=0;
	m_nextAttack=0;
}
void AIInterface::HandleEvent(AiEvents event,Unit * pUnit,uint32 misc)
{
	if(m_Unit == NULL) return;

	Creature* pCreature = NULL;

	if(m_Unit->IsCreature())
		pCreature = m_Unit->GetCreature();

	switch(event)
	{
	case EVENT_ENTERCOMBAT:
		{
			if(pUnit == NULL)
				return;
			if(m_Unit->IsDead()) 
				return;

			m_state = STATE_ATTACKING;
			m_return.ChangeCoords(m_Unit->GetWorldX(),m_Unit->GetWorldY(),m_Unit->GetWorldZ(),m_Unit->GetWorldO());
			m_returnoffx=m_Unit->GetOffX();
			m_returnoffy=m_Unit->GetOffY();
			m_returnregion = m_Unit->GetRegion();
			m_target = pUnit;
			m_nextAttack=0;
			m_outcount=UNIXTIME+OUT_TIME;

			if(pUnit->IsPlayer())
				m_firstattacker = pUnit->GetPlayer();

			if(pCreature && pCreature->m_script)
				pCreature->m_script->OnEnterCombat();

		}break;
	case EVENT_DAMAGETAKEN:
		{
			if(pUnit == NULL)
				return;

			if(m_Unit->IsDead()) 
				return;

			pUnit->CombatStatus->OnDamageDealt( m_Unit );
			if(m_firstattacker == NULL && pUnit->IsPlayer())
				m_firstattacker = pUnit->GetPlayer();

			CalcAggressionAdd(pUnit, misc);

			if(pCreature && pCreature->m_script)
				pCreature->m_script->OnDamageTaken();

		}break;
	case EVENT_LEAVECOMBAT:
		{
			Log.Debug("AI","LeaveCombat");
			m_state = STATE_IDLE;
			m_target = NULL;
			m_nextAttack=0;
			m_firstattacker = NULL;
			m_outcount=0;

			m_Unit->CombatStatus->Vanished();
			if(!m_Unit->IsDead())
			{
				float pox = mapmgr.CalculPos(m_returnregion,m_return.x,true);
				float poy = mapmgr.CalculPos(m_returnregion,m_return.y,false);
				m_Unit->SetPosition(pox,poy,m_return.z*4,m_return.o*8,m_returnoffx,m_returnoffy,true);
			}

			if(pCreature && pCreature->m_script)
				pCreature->m_script->OnLeaveCombat();

		}break;
	};
}
void AIInterface::CalcAggressionAdd(Unit* pUnit, uint32 AggroCount)
{
	AttackerInfo* pAttacker = m_targets[pUnit];

	if(!pAttacker)
		return;

	pAttacker->AggroValue += AggroCount;
}
void AIInterface::DeleteAggressorList()
{
	m_lock.Acquire();
	for(std::map<Unit*,AttackerInfo*>::const_iterator itr = m_targets.begin(); itr != m_targets.end(); ++itr)
	{
		if((*itr).first && (*itr).second)
		{
			delete (*itr).second;
			m_targets[(*itr).first] = NULL;
		}
	}

	m_targets.clear();

	m_lock.Release();
}
void AIInterface::GeneratePeriodicAggression()
{
	for(std::map<Unit*,AttackerInfo*>::const_iterator itr = m_targets.begin(); itr != m_targets.end(); ++itr)
		if((*itr).first && !(*itr).first->IsDead() && (*itr).second)
			(*itr).second->AggroValue += 1; // Aggro par tick
}
void AIInterface::CheckForTargets()
{
	m_lock.Acquire();

	uint32 TopAggro = 0;
	Unit* TopAggroUnit = NULL;

	for(std::map<Unit*,AttackerInfo*>::const_iterator itr = m_targets.begin(); itr != m_targets.end(); ++itr)
	{
		if(!(*itr).first || (*itr).first->IsDead())
		{
			if((*itr).second)
			{
				// mettre le pointeur sur NULL ? bug psychologique la petit memleak a regler
				/*delete (*itr).second;
				(*itr).first*/
			}

			continue;
		}

		if((*itr).first && (*itr).second && !(*itr).first->IsDead())
		{
			if(((*itr).second->AggroValue + 1) > TopAggro)
			{
				TopAggroUnit = (*itr).first;
				TopAggro = (*itr).second->AggroValue;
			}
		}
	}

	m_target = TopAggroUnit;

	m_lock.Release();

	if(!m_target || ( m_target && m_target->IsDead() ) )
		HandleEvent(EVENT_LEAVECOMBAT,m_target,0);
}
void AIInterface::RemoveFromAggressorList(Unit * pUnit)
{
	m_lock.Acquire();

	if(pUnit)
	{
		if(m_targets[pUnit])
		{
			delete m_targets[pUnit];
			m_targets[pUnit] = NULL;
		}
	}

	m_lock.Release();
}

void AIInterface::AddAttacker(Unit * pUnit)
{
	// Si l'aggresseur n'est pas dans la liste des ennemis
	if(pUnit)
	{
		if(!m_targets[pUnit])
		{
			AttackerInfo* pAttacker = new AttackerInfo;
			pAttacker->AggroValue = 0;

			if(pUnit->IsPlayer())
				pAttacker->Class = pUnit->GetPlayer()->GetCareer();
			else
				pAttacker->Class = 0;

			pAttacker->Taunt = false;
			pAttacker->UserData = NULL;
			
			m_targets[pUnit] = pAttacker;
		}

		// Nécéssaire pour la target initiale
		CheckForTargets();
	}
}
void AIInterface::AttackReaction(Unit * pUnit,uint32 damage)
{
	AddAttacker(pUnit);

	if( m_state == STATE_IDLE ) // nouveau combat
		HandleEvent(EVENT_ENTERCOMBAT,pUnit,damage);
	else if( m_state == STATE_ATTACKING )
		HandleEvent(EVENT_DAMAGETAKEN,pUnit,damage);
}
void AIInterface::AIDoMelee()
{
	uint32 ptime = m_Unit->m_updatetime;

	// Ensuite on met a jour les mouvement si on est en waypoint ou si le target est loin
	_UpdateMovement(ptime);

	// Ensuite on update le combat pour attaquer si nessecaire;
	if( m_state == STATE_ATTACKING )
		_UpdateCombat(ptime);
}
void AIInterface::Update(uint32 ptime)
{
	m_Unit->m_updatetime = ptime;

	// On met a jour le target pour savoir si on doit passer en combat ou non
	if(m_state != STATE_ATTACKING)
	{
		_UpdateTarget(ptime);
	}

	Creature* pCreature = NULL;
	if(m_Unit->IsCreature())
		pCreature = m_Unit->GetCreature();

	if(pCreature && pCreature->m_script)
	{
		pCreature->m_script->OnUpdate();
	}
	else
	{
		AIDoMelee();
	}
}
void AIInterface::_UpdateMovement(uint32 ptime)
{
	if( m_state == STATE_ATTACKING)
	{
		if(!m_Unit->IsInWorld())
			return;

		/*uint16 pinX = mapmgr.CalculPin(m_returnregion,m_return.x,true);
		uint16 pinY = mapmgr.CalculPin(m_returnregion,m_return.y,false);
		if(m_Unit->GetDistance(pinX,pinY) > 150.f)
		{
			Log.Debug("Ai","Trop Loin du spawn (%u,%u)->(%u,%u) region=%u",pinX,pinY,m_Unit->CalcPinX(),m_Unit->CalcPinY(),m_returnregion);
			HandleEvent(EVENT_LEAVECOMBAT,NULL,0);
		}*/
	}
	else if( m_state == STATE_MOVEWP)
	{


	}
}
void AIInterface::_CalcDestinationAndMove(Unit * target, float dist)
{
	/*if(target && target->IsUnit() )
	{
		float ResX = target->GetX();
		float ResY = target->GetY();
		float ResZ = target->GetZ();

		float angle = m_Unit->calcAngle(m_Unit->GetX(), m_Unit->GetY(), ResX, ResY) * float(M_PI) / 180.0f;
		float x = dist * cosf(angle);
		float y = dist * sinf(angle);

		if(target->GetTypeId() == TYPEID_PLAYER && TO_PLAYER( target )->m_isMoving )
		{
			// cater for moving player vector based on orientation
			x -= cosf(target->GetO());
			y -= sinf(target->GetO());
		}

		m_next.ChangeCoords(ResX - x,ResY - y,ResZ);
	}
	else
	{
		target = NULL;
		m_next.x = m_Unit->GetX();
		m_next.y = m_Unit->GetY();
		m_next.z = m_Unit->GetZ();
	}

	float dx = m_next.x - m_Unit->GetX();
	float dy = m_next.y - m_Unit->GetY();
	if(dy != 0.0f)
	{
		float angle = atan2(dx, dy);
		m_Unit->SetOrientation(angle);
	}

	UpdateMove();*/
}
bool AIInterface::CanAttack()
{
	if(!m_target) return false;

	float distance = m_Unit->GetDistance(m_target);
	Creature * Crea = NULL;
	if(m_Unit->IsCreature())
		Crea = m_Unit->GetCreature();

	if( distance <= 5.0f || (Crea && Crea->GetCreatureProto()->ranged && distance < 30.0f) )
		return true;

	return false;
}
void AIInterface::UpdateMove()
{
	/*//this should NEVER be called directly !!!!!!
	//use MoveTo()
	float distance = m_Unit->GetDistance(m_next.x,m_next.y);
	
	// 100 mettre en  7 secondes;

	m_dest.x = m_next.x;
	m_dest.y = m_next.y;
	m_dest.z = m_next.z;
	
	m_next.x = m_next.y = m_next.z = 0;

	uint32 moveTime = distance/0.007;

	m_totalMoveTime = moveTime;

	if( m_Unit->IsUnit() )
	{
		CreaturePointer creature = TO_CREATURE(m_Unit);

		float angle = 0.0f;
		float c_reach =GetUnit()->GetFloatValue(UNIT_FIELD_COMBATREACH);

		//We don't want little movements here and there; 
		float DISTANCE_TO_SMALL_TO_WALK = c_reach - 1.0f <= 0.0f ? 1.0f : c_reach - 1.0f;

		// don't move if we're well within combat range; rooted can't move neither
		if( distance < DISTANCE_TO_SMALL_TO_WALK || (creature->proto && creature->proto->CanMove == LIMIT_ROOT ) )
			return; 

			// Calculate the angle to our next position

		float dx = (float)m_dest.x - m_Unit->GetX();
		float dy = (float)m_dest.y - m_Unit->GetY();
		/*if(dy != 0.0f)
		{
			angle = atan2(dy, dx);
			m_Unit->SetPosition(angle);
		}


	}
	SendMoveToPacket(m_dest.x, m_dest.y, m_dest.z, m_Unit->GetO(), moveTime + UNIT_MOVEMENT_INTERPOLATE_INTERVAL );

	m_timeToMove = moveTime;
	m_timeMoved = 0;
	if(m_moveTimer == 0)
		m_moveTimer =  UNIT_MOVEMENT_INTERPOLATE_INTERVAL; // update every few msecs

	//m_creatureState = MOVING;*/
}

void AIInterface::_UpdateCombat(uint32 ptime)
{
	CheckForTargets();

	if(m_target == NULL)
		return;

	Creature * Crea = NULL;
	if(m_Unit->IsCreature())
		Crea = m_Unit->GetCreature();

	if(m_Unit->IsDead() || m_target->IsDead())
	{
		DeleteAggressorList();
		HandleEvent(EVENT_LEAVECOMBAT,m_target,0);
		return;
	}

	if(!m_Unit->IsMoving() && !m_Unit->isInFront(m_target))
		m_Unit->setInFront(m_target);

	if(m_nextAttack >=ptime)
		m_nextAttack-=ptime;
	else m_nextAttack=0;

	if(m_nextAttack <=0)
	{
			// strike
		if( CanAttack() )
		{
			m_Unit->Strike(m_target);
			m_nextAttack = 2000;
			m_outcount=UNIXTIME+OUT_TIME;
			m_Unit->SetMoving(false);
		}
		else
		{
			Log.Debug("Ai","Trop Loin");
			m_Unit->SetMoving(true);
			uint16 o = m_Unit->GetWorldO();
			if(!m_Unit->isInFront(GetTarget()))
				o = m_Unit->GetFacing(GetTarget());

			m_dest.x = GetTarget()->CalcPinX();
			m_dest.y = GetTarget()->CalcPinY();
			m_dest.z = GetTarget()->GetWorldZ();
			m_dest.o = o;

			m_Unit->SetPosition(GetTarget()->GetX(),GetTarget()->GetY(),GetTarget()->GetZ(),o,GetTarget()->GetOffX(),GetTarget()->GetOffY());
			
			m_nextAttack = 500;

			if(m_outcount <= UNIXTIME) // stop combat
				RemoveFromAggressorList(m_target);


		}
	}		
}

void AIInterface::_UpdateTarget(uint32 ptime)
{
	if(m_nextUpdateTarget > ptime)
		m_nextUpdateTarget-=ptime;
	else m_nextUpdateTarget=0;


	if(m_nextUpdateTarget <= 0) // On update le target)
	{
		GeneratePeriodicAggression();
		m_nextUpdateTarget=TARGET_UPDATE_INTERVAL;
	}
}