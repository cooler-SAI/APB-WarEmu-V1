#include "StdAfx.h"

ZoneMgr::ZoneMgr(uint16 zone_id) : eventHolder(zone_id)
{
	Log.Debug("ZoneMgr","new Zone(%u)",zone_id);
	m_zoneid = zone_id;

	lastUnitUpdate = getMSTime();
	m_holder = &eventHolder;
	InactiveMoveTime = 0;
	mLoopCounter=0;
	thread_kill_only = false;
	thread_running = false;
	
	m_PlayerStorage.clear();
	activeCreatures.clear();
	activeGameObjects.clear();

	m_lastoid=3;
	m_Info = objmgr.GetZoneInfo(zone_id);

	m_height = hfmgr.GetImage(zone_id);

	m_influencelist = objmgr.GetInfluenceList(m_zoneid);
	m_pquests = questmgr.GeneratePQuest(this);
}

bool ZoneMgr::run()
{
	return Do();
}
const RegionOffset * ZoneMgr::GetBaseRegion() 
{ 
	if(m_Info)	return m_Info->Offset;
	else		return NULL;
}
bool ZoneMgr::Do()
{
#ifdef WIN32
	threadid=GetCurrentThreadId();
#endif

	thread_running = true;
	SetThreadName("Zone mgr - M%u|I%u",this->m_zoneid);
	ObjectSet::iterator i;
	uint32 last_exec=getMSTime();

	uint32 exec_time, exec_start;
#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif
	THREAD_TRY_EXECUTION2
	while(m_threadRunning)
	{
		exec_start=getMSTime();

		if(m_objectinsertpool.size())
		{
			m_objectinsertlock.Acquire();//<<<<<<<<<<<<<<<<
			for(i=m_objectinsertpool.begin();i!=m_objectinsertpool.end();++i)
			{
				if((*i)) (*i)->PushToWorld(this);
			}
			m_objectinsertpool.clear();
			m_objectinsertlock.Release();//>>>>>>>>>>>>>>>>
		}


		_PerformObjectDuties();

		last_exec=getMSTime();
		exec_time=last_exec-exec_start;
		if(exec_time<ZONE_UPDATE_PERIOD)
		{
			Sleep(ZONE_UPDATE_PERIOD-exec_time);
		}

	}
	THREAD_HANDLE_CRASH2
	thread_running = false;
	if(thread_kill_only)
		return false;

	return false;
}
ZoneMgr::~ZoneMgr()
{
	Log.Debug("ZoneMgr","~ZoneMgr()");
	// in case this goes feeefeee
	ZoneMgr * pThis = this;

	SetThreadName("thread_proc");//free the name

	sEventMgr.RemoveEvents(this);
	sEventMgr.RemoveEventHolder( m_zoneid ); 

	// Remove objects

	for(uint32 i=0;i<MAX_CELLS_POSITION;++i)
	{
		for(uint32 j=0;j<MAX_CELLS_POSITION;++j)
		{
			if(!m_cells[i][j]) continue;

			m_cells[i][j]->_unloadpending=false;
			m_cells[i][j]->RemoveObjects();
		}
	}

	m_PlayerStorage.clear();
	activeCreatures.clear();
	activeGameObjects.clear();

	Log.Notice("MapMgr", "ZoneMgr %u shut down." , m_zoneid);
}
void ZoneMgr::_PerformObjectDuties()
{
	++mLoopCounter;
	uint32 mstime = getMSTime();
	uint32 difftime = mstime - lastUnitUpdate;
	if(difftime > 500)
		difftime = 500;

	// Update our objects.

	Creature * ptr;
	__creature_iterator = activeCreatures.begin();
	for(; __creature_iterator != activeCreatures.end();)
	{
		ptr = *__creature_iterator;
		++__creature_iterator;

		ptr->Update(difftime);
	}

	// Update any events.
	eventHolder.Update(difftime);

	// Update players.
	Player * ptr4;
	__player_iterator = m_PlayerStorage.begin();
	for(; __player_iterator != m_PlayerStorage.end();)
	{
		ptr4 = __player_iterator->second;;
		++__player_iterator;

		ptr4->Update( difftime );
	}

	lastUnitUpdate = mstime;

	// Update gameobjects (every 2nd tick only).
	if( mLoopCounter % 2 )
	{
		difftime = mstime - lastGameobjectUpdate;

		GameObject* ptr5;
		__gameobject_iterator = activeGameObjects.begin();
		for(; __gameobject_iterator != activeGameObjects.end(); )
		{
			ptr5 = *__gameobject_iterator;
			++__gameobject_iterator;

			ptr5->Update( difftime );
		}

		lastGameobjectUpdate = mstime;
	}

	if(m_pquests.size())
	{
		for(__quest_iterator = m_pquests.begin();__quest_iterator!=m_pquests.end();++__quest_iterator)
			(*__quest_iterator)->Update( difftime );
	}

	_UpdateObjects();
}
void ZoneMgr::_UpdateObjects()
{
/*	//m_updateMutex.Release();

	Object* pObj;
	Player* pOwner;
	//std::set<ObjectPointer >::iterator it_start, it_end, itr;
	unordered_set<Player*  >::iterator it_start, it_end, itr;
	Player*  lplr;
	uint32 count = 0;
	
	//m_updateMutex.Acquire();
	UpdateQueue::iterator iter = _updates.begin();
	//m_updateMutex.Release();
	PUpdateQueue::iterator it, eit;

	for(; iter != _updates.end();)
	{
		pObj = *iter;
		++iter;
		if(!pObj) continue;

			if( pObj->IsInWorld() )
			{
				// players have to receive their own updates ;)
				if( pObj->GetType() == TYPEID_PLAYER )
				{
					// need to be different! ;)
					count = pObj->BuildValuesUpdateBlockForPlayer( &m_updateBuffer, TO_PLAYER( pObj ) );
					if( count )
					{
						TO_PLAYER( pObj )->PushUpdateData( &m_updateBuffer, count );
						m_updateBuffer.clear();
					}
				}

				if( pObj->IsUnit() && pObj->HasUpdateField( UNIT_FIELD_HEALTH ) )
					TO_UNIT( pObj )->EventHealthChangeSinceLastUpdate();

				// build the update
				count = pObj->BuildValuesUpdateBlockForPlayer( &m_updateBuffer, NULLPLR );

				if( count )
				{
					it_start = pObj->GetInRangePlayerSetBegin();
					it_end = pObj->GetInRangePlayerSetEnd();
					for(itr = it_start; itr != it_end;)
					{
						lplr = *itr;
						++itr;
						// Make sure that the target player can see us.
						if( lplr != NULL && lplr->GetTypeId() == TYPEID_PLAYER && lplr->IsVisible( pObj ) )
							lplr->PushUpdateData( &m_updateBuffer, count );
					}
					m_updateBuffer.clear();
				}
			}
		pObj->ClearUpdateMask();
	}
	//_updates.clear();
	//m_updateMutex.Release();
	
	// generate pending a9packets and send to clients.
	PlayerPointer plyr;
	for(it = _processQueue.begin(); it != _processQueue.end();)
	{
		plyr = *it;
		eit = it;
		++it;

		if( plyr == NULL )
			continue;

		_processQueue.erase(eit);
		if(plyr->GetMapMgr() == shared_from_this())
			plyr->ProcessPendingUpdates(&m_updateBuildBuffer, &m_compressionBuffer);
	}*/
}
CellMgr * ZoneMgr::Create(uint16 x,uint16 y)
{
	if(!x || !y) return NULL;
	if(x>998) x=998;
	if(y>998) y=998;

	CellMgr * Cell=NULL;
	Cell = m_cells[x][y];


	if(!Cell)
	{
		Cell = new CellMgr(x,y,this);
		m_cells[x][y] = Cell;
	}

	return Cell;
}
CellMgr * ZoneMgr::GetCell(uint16 x,uint16 y)
{
	if(x > MAX_CELLS_POSITION || y>MAX_CELLS_POSITION)
		return NULL;

	if(m_cells[x]) return m_cells[x][y];
	else return NULL;
}
void ZoneMgr::AddObject(Object * obj,bool newguid)
{
	if(newguid)
	{
		obj->SetNewOid( GenerateNewOid() );

		if(obj->IsPlayer())
		{
			Log.Debug("ZoneMgr","[%u] AddPlayer %s",m_zoneid,obj->GetName().c_str());
			obj->GetPlayer()->sendPlayerLoaded();
			if(obj->GetPlayer()->m_acceptmonster==false)
				return;
		}
	}
	m_objectinsertlock.Acquire();//<<<<<<<<<<<<
	m_objectinsertpool.insert(obj);
	m_objectinsertlock.Release();//>>>>>>>>>>>>
}
void ZoneMgr::RemoveObject(Object * obj, bool free_guid)
{
	/////////////
	// Assertions
	/////////////

	if(obj->IsPlayer())
	{
		Log.Debug("ZoneMgr","[%u] Removing Player %s",m_zoneid,obj->GetName().c_str());
		if(obj->GetPlayer()->GetPublicQuest())
			obj->GetPlayer()->GetPublicQuest()->RemovePlayer(obj->GetPlayer());
	}
	else if(obj->IsUnit())
		if(obj->GetUnit()->GetPublicQuest())
			obj->GetUnit()->SetPublicQuest(NULL);

	ASSERT(obj);
	ASSERT(obj->GetZoneId() == m_zoneid);
	//ASSERT(obj->GetPositionX() > _minX && obj->GetPositionX() < _maxX);
	//ASSERT(obj->GetPositionY() > _minY && obj->GetPositionY() < _maxY);

	obj->sendRemoved();

	M_object.Acquire();
	m_object[obj->GetOid()] = NULL;
	M_object.Release();

	//_updates.erase(obj);

	Player * plObj = (obj->GetType() == TYPEID_PLAYER) ? obj->GetPlayer() : NULL;

	///////////////////////////////////////
	// Remove object from all needed places
	///////////////////////////////////////
 
	switch(obj->GetType())
	{
		case TYPEID_PLAYER:
			// check iterator
			if( __player_iterator != m_PlayerStorage.end() && __player_iterator->second == obj->GetPlayer() )
				++__player_iterator;
			break;
	}

	if(obj->Active)
		obj->Deactivate(this);

	if(!obj->GetCellMgr())
	{
		obj->SetCellMgr(GetCell(obj->GetOffX(),obj->GetOffY()) );	
	}

	if(obj->GetCellMgr())
	{
		ASSERT(obj->GetCellMgr());
	
		// Remove object from cell
		obj->GetCellMgr()->RemoveObject(obj);
	
		// Unset object's cell
		obj->SetCellMgr(NULL);
	}

	// Clear any updates pending
	/*if(obj->GetType() == TYPEID_PLAYER)
	{
		_processQueue.erase(obj->GetPlayer());
		TO_PLAYER( obj )->ClearAllPendingUpdates();
	}*/
	
	// Remove object from all objects 'seeing' him
	for (Object::InRangeSet::iterator iter = obj->GetInRangeSetBegin();iter != obj->GetInRangeSetEnd(); ++iter)
	{
		if( (*iter) )
		{
			(*iter)->RemoveInRangeObject(obj);
		}
	}
	
	// Clear object's in-range set
	obj->ClearInRangeSet();

	if(plObj)
	{
		UpdateCellActivity(obj->GetOffX(), obj->GetOffY(), 2);

		m_PlayerStorage.erase( plObj->GetPlayerID() );
	}

	/*if(!HasPlayers() && !InactiveMoveTime && !forced_expire && GetMapInfo()->type != INSTANCE_NULL)
	{
		InactiveMoveTime = UNIXTIME + (MAPMGR_INACTIVE_MOVE_TIME * 60);	   // 5 mins -> move to inactive
	}*/
}
void ZoneMgr::PushObject(Object * obj)
{
	/////////////
	// Assertions
	/////////////
	ASSERT(obj);

	if(obj->IsPlayer())
	{
		Log.Debug("ZoneMgr","[%u] Pushing Player %s",m_zoneid,obj->GetName().c_str());
		sEventHook.ExecuteHook(EVENTS_PLAYER_ZONE_CHANGED,2,obj->GetPlayer(),GetZoneId());
	}
	else if(obj->IsGameObject())
	{
		Log.Debug("ZoneMgr","[%u] Pushing GameObject %s",m_zoneid,obj->GetName().c_str());
	}

	M_object.Acquire();
	m_object[obj->GetOid()] = obj;
	M_object.Release();

	Player * plObj = NULL;

	if( obj->IsPlayer() )
	{
		plObj = obj->GetPlayer();
		if(plObj == NULL)
		{
			Log.Error("MapMgr","Could not get a valid playerobject from object while trying to push to world");
			return;
		}
		ClientCom * Com = NULL;
		Com = plObj->GetCom();
		if(Com == NULL)
		{
			Log.Error("MapMgr","Could not get a valid session for player while trying to push to world");
			return;
		}
	}
	
	obj->ClearInRangeSet();

	///////////////////////
	// Get cell coordinates
	///////////////////////

	ASSERT(obj->GetZoneId() == m_zoneid);

	uint16 zx = obj->GetOffX();
	uint16 zy = obj->GetOffY();


	if(!zx || !zy)
		return;

	if(zx > MAX_CELLS_POSITION || zy>MAX_CELLS_POSITION)
		return;


	CellMgr *objCell = GetCell(zx,zy);
	if (!objCell)
	{
		if(plObj)
		Log.Error("ZoneMgr","Creating Own Cell");
		objCell = Create(zx,zy);
	}

	uint16 endX = zx+1;
	uint16 endY = zy+1;

	uint16 startX = zx-1;
	uint16 startY = zy-1;

	uint16 posX, posY;
	CellMgr *cell;
	CellMgr::ObjectSet::iterator iter;

	//////////////////////
	// Build in-range data
	//////////////////////

	if(plObj)
	Log.Error("ZoneMgr","Updating In Range Cell");
	for (posX = startX; posX <= endX; posX++ )
	{
		for (posY = startY; posY <= endY; posY++ )
		{
			cell = GetCell(posX, posY);
			if (cell)
			{
				UpdateInRangeSet(obj, plObj, cell);
			}
		}
	}

	//Add to the cell's object list
	objCell->AddObject(obj);

	obj->SetCellMgr(objCell);

	 //Add to the mapmanager's object list
	if(plObj)
	{

		Log.Error("ZoneMgr","UpdatingCell Activity");
		m_PlayerStorage[plObj->GetPlayerID()] = plObj;
		UpdateCellActivity(zx, zy, 2);
	}

	// Handle activation of that object.
	if(objCell->IsActive() && obj->CanActivate())
		obj->Activate(this);

	if(plObj && InactiveMoveTime && !forced_expire)
		InactiveMoveTime = 0;

	CheckInRangePq(obj);
}
bool ZoneMgr::_CellActive(uint16 x, uint16 y)
{
	if(x >= 998 || y >= 998)
		return false;

	uint16 endX = x + 1;
	uint16 endY = y + 1;
	uint16 startX = x - 1;
	uint16 startY = y - 1;
	uint16 posX, posY;

	CellMgr *objCell;

	for (posX = startX; posX <= endX; posX++ )
	{
		for (posY = startY; posY <= endY; posY++ )
		{
			objCell = GetCell(posX, posY);

			if (objCell)
			{
				if (objCell->HasPlayers() || objCell->IsForcedActive() )
				{
					return true;
				}
			}
		}
	}

	return false;
}
void ZoneMgr::UpdateCellActivity(uint16 x, uint16 y, int radius)
{
	CellSpawns * sp;
	uint16 endX = x + radius;
	uint16 endY = y + radius;
	uint16 startX = x - radius;
	uint16 startY = y - radius;
	uint16 posX, posY;

	CellMgr *objCell;

	for (posX = startX; posX <= endX; posX++ )
	{
		for (posY = startY; posY <= endY; posY++ )
		{
			objCell = GetCell(posX, posY);

			if (!objCell)
			{
				if(_CellActive(posX,posY))
				{
					objCell = Create(posX, posY);

					objCell->SetActivity(true);

					ASSERT(!objCell->IsLoaded());

					sp = mapmgr.GetSpawnsList(m_zoneid,posX, posY);
					if(sp) objCell->LoadObjects(sp);
				}
			}
			else
			{
				//Cell is now active
				if (_CellActive(posX, posY) && !objCell->IsActive())
				{
					objCell->SetActivity(true);

					if (!objCell->IsLoaded())
					{
						sp = mapmgr.GetSpawnsList(m_zoneid,posX, posY);
						if(sp) objCell->LoadObjects(sp);
					}
				}
				else if (!_CellActive(posX, posY) && objCell->IsActive())
				{
					objCell->SetActivity(false);
				}
			}
		}
	}

}
void ZoneMgr::UpdateInRangeSet( Object * obj, Player * plObj, CellMgr* cell )
{
	if( cell == NULL )
		return;

	Object * curObj;
	Player * plObj2;
	ObjectSet::iterator iter = cell->Begin();
	ObjectSet::iterator iter2 = cell->End();
	ObjectSet::iterator itr;
	float fRange;
	//bool cansee, isvisible;

	while(iter != iter2)
	{
		curObj = *iter;
		++iter;

		if( curObj == NULL )
			continue;

		if(curObj == obj)
			continue;

		fRange = MAX_VISIBILITY_RANGE;
		float range=0;
		if(!curObj->IsPlayer() && !obj->IsPlayer())
			fRange/=2;

		range = curObj->GetDistance(obj);

		if ( curObj != obj && (fRange == 0.0f || range <= fRange ) )
		{
			if( !obj->IsInRangeSet( curObj ) )
			{
				//Log.Debug("ZoneMgr","!obj->IsInRangeSet");

				// Object in range, add to set
				obj->AddInRangeObject( curObj );
				curObj->AddInRangeObject( obj );

				if( curObj->IsPlayer() )
				{
					plObj2 = curObj->GetPlayer();

					if( plObj2->CanSee( obj ) /*&& !plObj2->IsVisible( obj )*/ )
					{
						obj->SendMeTo(plObj2);
						obj->SendEquiped(plObj2);
						plObj2->AddVisibleObject(obj);
					}
				}

				if( plObj != NULL )
				{
					if( plObj->CanSee( curObj ) /*&& !plObj->IsVisible( curObj )*/ )
					{
							curObj->SendMeTo(plObj);
							curObj->SendEquiped(plObj);
							plObj->AddVisibleObject( curObj );
					}
				}
			}
			else
			{
				//Log.Debug("ZoneMgr"," is obj->IsInRangeSet");

				/*// Check visiblility
				if( curObj->IsPlayer() )
				{
					plObj2 = curObj->GetPlayer();
					cansee = plObj2->CanSee(obj);
					isvisible = plObj2->GetVisibility(obj, &itr);
					if(!cansee && isvisible)
					{
						//plObj2->PushOutOfRange(obj->GetNewGUID());
						plObj2->RemoveVisibleObject(itr);
					}
					else if(cansee && !isvisible)
					{
						obj->SendMeTo(plObj2);
						obj->SendEquiped(plObj2);
						plObj2->AddVisibleObject(obj);
					}
				}

				if( plObj )
				{
					cansee = plObj->CanSee( curObj );
					isvisible = plObj->GetVisibility( curObj, &itr );
					if(!cansee && isvisible)
					{
						//plObj->PushOutOfRange( curObj->GetNewGUID() );
						plObj->RemoveVisibleObject( itr );
					}
					else if(cansee && !isvisible)
					{
						curObj->SendMeTo(plObj);
						curObj->SendEquiped(plObj);
						plObj->AddVisibleObject( curObj );
					}
				}*/
			}
		}
	}
}
void ZoneMgr::CheckInRangeInfluence(Player *Plr)
{
	if(!Plr || !m_influencelist.size()) return;

	for(vector<InfluenceInfo*>::iterator itr=m_influencelist.begin();itr!=m_influencelist.end();++itr)
	{
		if(!(*itr) || !(*itr)->tok_entry) continue;
		if( (*itr)->realm != Plr->GetRealm()-1) continue;
		if( Plr->m_TokInterface->HasTok( (*itr)->tok_entry ) ) continue;

		float distance = Plr->GetDistance((*itr)->pinx,(*itr)->piny);
		if( distance < 300.0f)
		{
			if(!(*itr)->influenceid)
				Plr->m_InfluenceInterface->m_wait.push_back((*itr));

			Plr->m_TokInterface->AddTok( (*itr)->tok_entry );
			if( (*itr)->tok_explore )
				Plr->m_TokInterface->AddTok( (*itr)->tok_explore );
		}
	}
}
void ZoneMgr::CheckInRangeArea(Player *Plr)
{
	return; // TODO : déterminer comment fonctionne les areas !

	Log.Debug("ZoneMgr","CheckInrangeArea");
	if(!Plr) return;

	int realm = Plr->GetRealm()-1;

	AreaInfo * Choice = mapmgr.CheckAreaInfo(realm,m_zoneid,Plr->CalcPinX(),Plr->CalcPinY());

	if(!Choice) return;

	Log.Debug("ZoneMgr","Area Choisie = %i , Influence=%i",Choice->areaid,Choice->influenceid[realm]);
	if(Plr->GetArea() == Choice)
		return;

	Plr->SetArea(Choice);
}
void ZoneMgr::CheckInRangePq(Object *obj)
{
	if(!obj) return;
	if(!obj->IsUnit()) return;

	Player * pObj=NULL;
	Unit * unit=obj->GetUnit();

	if(obj->IsPlayer())
		pObj = obj->GetPlayer();

	if(unit->GetPublicQuest() && !unit->GetPublicQuest()->IsInrange(unit))
		if(pObj)	pObj->GetPublicQuest()->RemovePlayer(pObj);
		else		unit->SetPublicQuest(NULL);

	for(list<PQuest*>::iterator itr=m_pquests.begin();itr!=m_pquests.end();++itr)
	{
		if(!(*itr)) continue;
		if(unit->GetPublicQuest() == (*itr)) continue;
		if(obj->IsPlayer() && (*itr)->GetRealm() && (*itr)->GetRealm() != obj->GetPlayer()->GetRealm()) continue;

		if( (*itr)->IsInrange(obj) )
		{
			if(pObj) (*itr)->AddPlayer(pObj);
			else unit->SetPublicQuest( (*itr) );
		}
	}
}
void ZoneMgr::ChangeObjectLocation( Object * obj )
{
	// Items and containers are of no interest for us
	if( obj->GetZoneMgr() != this )
		return;

	Player * plObj;

	if( obj->IsPlayer() )
	{
		plObj = obj->GetPlayer();
		if(plObj && plObj->Trade) plObj->Trade->CheckDistance();
	}
	else
	{
		plObj = NULL;
	}

	Object * curObj;
	float fRange;

	///////////////////////////////////////
	// Update in-range data for old objects
	///////////////////////////////////////

	if(obj->HasInRangeObjects()) 
	{
		for (Object::InRangeSet::iterator iter = obj->GetInRangeSetBegin(), iter2; iter != obj->GetInRangeSetEnd();)
		{
			curObj = *iter;
			iter2 = iter++;

			if(curObj == NULL)
				continue;

			fRange = MAX_VISIBILITY_RANGE;
			float range = curObj->GetDistance(obj);
			if(!curObj->IsPlayer() && !obj->IsPlayer())
				fRange/=2;

			//If we have a update_distance, check if we are in range. 
			if( fRange > 0.0f && range >= fRange )
			{
	
				if( plObj )
					plObj->RemoveIfVisible(curObj);

				if( curObj->IsPlayer() )
					curObj->GetPlayer()->RemoveIfVisible(obj);

				curObj->RemoveInRangeObject(obj);

				if( obj->GetZoneMgr() != this )
				{
					/* Something removed us. */
					return;
				}

				obj->RemoveInRangeObject(iter2);
			}
		}
	}

	///////////////////////////
	// Get new cell coordinates
	///////////////////////////
	if(obj->GetZoneMgr() != this)
	{
		/* Something removed us. */
		return;
	}

	uint16 cellX = obj->GetOffX();
	uint32 cellY = obj->GetOffY();

	CellMgr *objCell = GetCell(cellX, cellY);
	CellMgr * pOldCell = obj->GetCellMgr();
	if (!objCell)
	{
		objCell = Create(cellX,cellY);
	}

	// If object moved cell
	if (objCell != pOldCell)
	{
		Log.Debug("Zone","Object Moved Cell %s",obj->GetName().c_str());

		//if(!objCell->IsActive() && !plObj && obj->Active)
			//obj->Deactivate(this);

		if(obj->GetCellMgr())
			obj->GetCellMgr()->RemoveObject(obj);
	
		objCell->AddObject(obj);
		obj->SetCellMgr(objCell);
	}


	//////////////////////////////////////
	// Update in-range set for new objects
	//////////////////////////////////////

	if(obj->GetType() == TYPEID_PLAYER)
	{
		// have to unlock/lock here to avoid a deadlock situation.
		UpdateCellActivity(cellX, cellY, 2);
		if( pOldCell != NULL )
		{
			// only do the second check if theres -/+ 2 difference
			if( abs( (int)cellX - (int)pOldCell->m_x ) > 2 ||
				abs( (int)cellY - (int)pOldCell->m_y ) > 2 )
			{
				UpdateCellActivity( pOldCell->m_x, pOldCell->m_y, 2 );
			}
		}
	}

	uint16 endX = cellX + 1 ;
	uint16 endY = cellY + 1 ;
	uint16 startX = cellX - 1;
	uint16 startY = cellY - 1;
	uint16 posX, posY;
	CellMgr *cell;
	CellMgr::ObjectSet::iterator iter;

	for (posX = startX; posX <= endX; ++posX )
	{
		for (posY = startY; posY <= endY; ++posY )
		{
			cell = GetCell(posX, posY);
			if (cell)
				UpdateInRangeSet(obj, plObj, cell);
		}
	}

	if(!plObj) // On ne check que les unités, les joueurs sont check dans le changement d'offset(sa bouge trop un joueur mdr)
		CheckInRangePq(obj);
}
void ZoneMgr::SendMessageToCellPlayers(Buffer *b,uint32 offx,uint32 offy,ClientCom * Com,bool self,uint32 cell_radius)
{
	uint32 cellX = offx;
	uint32 cellY = offy;
	uint32 endX = cellX + cell_radius;
	uint32 endY = cellY + cell_radius;
	uint32 startX = cellX - cell_radius;
	uint32 startY = cellY - cell_radius;

	uint32 posX, posY;
	CellMgr *cell;
	CellMgr::ObjectSet::iterator iter, iend;
	for (posX = startX; posX <= endX; ++posX )
	{
		for (posY = startY; posY <= endY; ++posY )
		{
			cell = GetCell(posX, posY);
			if (cell && cell->HasPlayers() )
			{
				iter = cell->Begin();
				iend = cell->End();
				for(; iter != iend; ++iter)
				{
					if((*iter)->IsPlayer())
					{
						Player * Plr = (*iter)->GetPlayer();
						if(Plr) 
						{
							if( (Com && !self && Plr->GetCom() != Com) || self)
							Plr->dispachPacket(b);
						}
					}
				}
			}
		}
	}
}
Creature * ZoneMgr::SpawnCreature(uint32 entry,uint32 wx,uint32 wy,uint32 wz,uint32 wo,bool addtoworld)
{
	if(!entry) return NULL;

	const CreatureProto * Proto = objmgr.GetCreatureProto(entry);
	CreatureSpawn * Sp = new CreatureSpawn();

	if(!Proto) return NULL;

	Sp->guid = objmgr.GenerateSpawnGuid();
	Sp->entry = entry;
	Sp->zone = m_zoneid;
	Sp->WorldX = wx;
	Sp->WorldY = wy;
	Sp->WorldZ = wz;
	Sp->WorldO = wo;
	Sp->bytes = Proto->bytes;
	Sp->icone = Proto->icone;
	Sp->Unk1 = Proto->Unk1;
	Sp->emote = Proto->emote;
	for(int i=0;i<6;++i)
		Sp->Unk[i] = Proto->Unk[i];

	Sp->title = Proto->title;
	Sp->flags = Proto->flags;

	Creature * Crea = CreateCreature(Sp);

	if(Crea) Crea->Load();

	if(Crea && addtoworld)
		Crea->AddToWorld(this,true);

	return Crea;
}
GameObject * ZoneMgr::SpawnGameObject(uint32 entry,uint32 wx,uint32 wy,uint32 wz,uint32 wo,bool addtoworld)
{
	if(!entry) return NULL;

	const GameObjectProto * Proto = objmgr.GetGameObjectProto(entry);
	GameObjectSpawn * Sp = new GameObjectSpawn();

	if(!Proto) return NULL;

	Sp->guid = objmgr.GenerateGoGuid();
	Sp->entry = entry;
	Sp->zone = m_zoneid;
	Sp->WorldX = wx;
	Sp->WorldY = wy;
	Sp->WorldZ = wz;
	Sp->WorldO = wo;
	Sp->displayid = Proto->displayid;
	for(int i=0;i<6;++i)
		Sp->Unk[i] = Proto->Unk[i];

	GameObject * Go = CreateGameObject(Sp);

	if(Go) Go->Load();

	if(Go && addtoworld)
		Go->AddToWorld(this,true);

	return Go;
}

Creature * ZoneMgr::CreateCreature(const CreatureSpawn * Sp)
{
	if(!Sp) return NULL;

	Creature * Crea = new Creature(Sp->entry,Sp);
	m_CreatureStorage.insert( make_pair< const uint32, Creature* >(Sp->guid, Crea));
	return Crea;
}
GameObject * ZoneMgr::CreateGameObject(const GameObjectSpawn * Gs)
{
	//Validate the entry
	if(!Gs) return NULL;
	GameObject * go = new GameObject(Gs->entry,Gs);
	m_GameObjectStorage.insert( make_pair<const uint32, GameObject* >(Gs->guid, go) );
	return go;
}
Object * ZoneMgr::GetObjectByOid(uint16 oid)
{
	Object * obj=NULL;
	M_object.Acquire();
	obj = m_object[oid];
	M_object.Release();
	return obj;
}

Player * ZoneMgr::GetPlayer(uint16 oid)
{
	Object * obj=GetObjectByOid(oid);
	if(!obj) return NULL;
	if(!obj->IsPlayer()) return NULL;

	return obj->GetPlayer();
}

Unit * ZoneMgr::GetUnit(uint16 oid)
{
	Object * obj=GetObjectByOid(oid);
	if(!obj) return NULL;
	if(!obj->IsUnit()) return NULL;

	return obj->GetUnit();
}
Creature * ZoneMgr::GetCreature(uint16 oid)
{
	Object * obj=GetObjectByOid(oid);
	if(!obj) return NULL;
	if(!obj->IsCreature()) return NULL;

	return obj->GetCreature();
}
int ZoneMgr::GetHeight(uint32 x,uint32 y)
{
	return HeightFieldMgr::GetHeight(m_height,x,y);
}
int ZoneMgr::GetHeight(Object * obj)
{
	return GetHeight(obj->CalcPinX(),obj->CalcPinY());
}