#include "StdAfx.h"

CellMgr::CellMgr(uint16 x,uint16 y,ZoneMgr * Zone)
{
	m_zone = Zone;
	_active = false;
	_loaded = false;
	_playerCount = 0;
	_unloadpending=false;
	_objects.clear();
	 m_x=x;
	 m_y=y;
}
void CellMgr::AddObject(Object* obj)
{
	if(obj->IsPlayer())
		++_playerCount;

	_objects.insert(obj);
}

void CellMgr::RemoveObject(Object* obj)
{
	if(obj->IsPlayer())
		--_playerCount;

	_objects.erase(obj);
}
void CellMgr::LoadObjects(CellSpawns * sp)
{
	Log.Debug("CellMgr","[%u][%u] %u Creatures/%u Gameobject",m_x,m_y,sp->CreatureSpawns.size(),sp->GOSpawns.size());
	_loaded = true;

	if(sp->CreatureSpawns.size())//got creatures
	{
		Creature * c;
		for(CreatureSpawnList::iterator i=sp->CreatureSpawns.begin();i!=sp->CreatureSpawns.end();++i)
		{
			if(!(*i)) continue;

				c= m_zone->CreateCreature((*i));
				if(c==NULL) continue;

				if(c->Load()) c->AddToWorld(m_zone,true);
				else delete c;
		}
	}
	
	if(sp->GOSpawns.size())//got GOs
	{
		GameObject * go;
		for(GOSpawnList::iterator i=sp->GOSpawns.begin();i!=sp->GOSpawns.end();++i)
		{
			if(!(*i)) continue;

			go = m_zone->CreateGameObject((*i));
			if(go == NULL) continue;

			if(go->Load()) go->AddToWorld(m_zone,true);
			else delete go;
		}
	}
}
void CellMgr::SetActivity(bool state)
{	
	if(!_active && state)
	{
		// Move all objects to active set.
		for(ObjectSet::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
		{
			if(!(*itr)->Active && (*itr)->CanActivate())
				(*itr)->Activate(m_zone);
		}

		//if(_unloadpending)
			//CancelPendingUnload();
	} else if(_active && !state)
	{
		// Move all objects from active set.
		for(ObjectSet::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
		{
			if((*itr)->Active)
				(*itr)->Deactivate(m_zone);
		}

		/*if(sWorld.map_unload_time && !_unloadpending)
			QueueUnloadPending();

		if (_mapmgr->IsCollisionEnabled())
			CollideInterface.DeactivateTile(_mapmgr->GetMapId(), _x/8, _y/8);*/
	}

	_active = state; 

}
void CellMgr::RemoveObjects()
{
	ObjectSet::iterator itr;
	//uint32 ltime = getMSTime();

	/* delete objects in pending respawn state */
	//Object * pObject;
	/*for(itr = _respawnObjects.begin(); itr != _respawnObjects.end(); ++itr)
	{
		pObject = *itr;
		if(!pObject)
			continue;
		
		switch(pObject->GetTypeId())
		{
		case TYPEID_UNIT: {
				if( pObject->IsVehicle() )
				{
					_mapmgr->_reusable_guids_vehicle.push_back( pObject->GetUIdFromGUID() );
					TO_VEHICLE(pObject)->m_respawnCell=NULL;
					TO_VEHICLE(pObject)->Destructor();
					pObject = NULLOBJ;
				}
				else if( !pObject->IsPet() )
				{
					_mapmgr->_reusable_guids_creature.push_back( pObject->GetUIdFromGUID() );
					TO_CREATURE(pObject)->m_respawnCell=NULL;
					TO_CREATURE(pObject)->Destructor();
					pObject = NULLOBJ;
				}
			}break;

		case TYPEID_GAMEOBJECT: {
			TO_GAMEOBJECT(pObject)->m_respawnCell=NULL;
			TO_GAMEOBJECT(pObject)->Destructor();
			pObject = NULLOBJ;
			}break;
		default:
			pObject->Destructor();
			pObject = NULLOBJ;
			break;

		}
	}
	_respawnObjects.clear();
	*/
	//This time it's simpler! We just remove everything :)
	Object * obj; //do this outside the loop!
	for(itr = _objects.begin(); itr != _objects.end();)
	{
		obj = (*itr);
		itr++;

		if(!obj)
			continue;

		if( _unloadpending )
		{
			/*if(obj->GetTypeFromGUID() == HIGHGUID_TYPE_TRANSPORTER)
				continue;

			if(obj->GetTypeId()==TYPEID_CORPSE && obj->GetUInt32Value(CORPSE_FIELD_OWNER) != 0)
				continue;

			if(!obj->m_loadedFromDB)
				continue;*/
		}

		if( obj->Active ) obj->Deactivate( m_zone );
		if( obj->IsInWorld() ) obj->RemoveFromWorld();

		delete obj;
		obj = NULL;
	}

	_playerCount = 0;
	_loaded = false;
}