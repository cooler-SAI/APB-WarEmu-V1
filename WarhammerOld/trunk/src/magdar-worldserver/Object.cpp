#include "StdAfx.h"

Object::Object() 
{ 
	m_type = TYPEID_OBJECT; 
	m_zoneMgr = NULL;
	Active=false;
	m_oid=0;

	m_name.clear();
	m_subname.clear();

	m_lastposition.ChangeCoords(0,0,0,0);
	m_worldposition.ChangeCoords(0,0,0,0);
	m_position.ChangeCoords(0,0,0,0);
	m_zone=0;
	m_region=0;
	m_zoneMgr=NULL;
	m_cellMgr=NULL;

	m_OffSetX=0;
	m_OffSetY=0;
	m_zone_x=0;
	m_zone_y=0;

	m_objectsInRange.clear();
	m_inRangePlayers.clear();
	m_oppFactsInRange.clear();

	m_basestats.clear();
	m_stats.clear();
}
Object::~Object() 
{ 

}
Player * Object::GetPlayer()
{ 
	return static_cast<Player*>( this );
};
Creature * Object::GetCreature()
{ 
	return static_cast<Creature*>( this );
};
Unit * Object::GetUnit()
{ 
	return static_cast<Unit*>( this );
};
GameObject * Object::GetGameObject()
{
	return static_cast<GameObject*>( this );
}
///////////////////////////////////////////////////
//			Position System
///////////////////////////////////////////////////
void Object::AddToWorld(ZoneMgr * Mgr,bool newguid) 
{
	if(IsInWorld())
		RemoveFromWorld();

	m_zone=Mgr->GetZoneId();
	m_zoneMgr = Mgr;

	Mgr->AddObject(this,newguid);
}

void Object::AddToWorld(bool newguid) 
{
	if(IsInWorld())
		RemoveFromWorld();

	ZoneMgr * Zone = mapmgr.GetZoneMgr(this);
	if(!Zone) return;

	m_zone=Zone->GetZoneId();
	m_zoneMgr = Zone;

	Zone->AddObject(this,newguid);
}
void Object::RemoveFromWorld()
{
	ZoneMgr * m = m_zoneMgr;
	m_zoneMgr = NULL;

	m->RemoveObject(this,true);

	// update our event holder
	event_Relocate();
}
void Object::PushToWorld(ZoneMgr * Zone)
{
	if(!Zone) return;

	Zone->PushObject(this);
   
	event_Relocate();
}
uint16 Object::CalcPinX()
{
	return mapmgr.CalculPin(GetRegion(),m_worldposition.x,true);
}
uint16 Object::CalcPinY()
{
	return mapmgr.CalculPin(GetRegion(),m_worldposition.y,false);
}
float Object::GetDistance(Object * obj)
{
	return GetDistance(float(obj->CalcPinX()),float(obj->CalcPinY()));
}
float Object::GetDistance(float x,float y)
{
	float mex = CalcPinX();
	float mey = CalcPinY();
	float youx = x;
	float youy = y;

	float distx = max(mex,youx) - min(mex,youx);
	float disty = max(mey,youy) - min(mey,youy);
	if(distx>=180)
	{
		distx-=180;
		distx/=12;
		distx+=5;
	}
	else
	{
		distx/=36;
	}
	if(disty>=180)
	{
		disty-=180;
		disty/=12;
		disty+=5;
	}
	else
	{
		disty/=36;
	}


	distx = distx*distx;
	disty = disty*disty;

	float result = sqrt(distx+disty);

    return result;
}
void Object::Activate(ZoneMgr * mgr)
{
	switch(GetType())
	{
	case TYPEID_CREATURE:
			mgr->activeCreatures.insert(GetCreature());
		break;
	case TYPEID_GAMEOBJECT:
			mgr->activeGameObjects.insert(GetGameObject());
		break;
	}

	Active = true;
}

void Object::Deactivate(ZoneMgr * mgr)
{
	switch(GetType())
	{
		case TYPEID_CREATURE:
				// check iterator
				if( mgr->__creature_iterator != mgr->activeCreatures.end() && (*mgr->__creature_iterator) == GetCreature() )
					++mgr->__creature_iterator;

				mgr->activeCreatures.erase(GetCreature());
			break;
		case TYPEID_GAMEOBJECT:
				// check iterator
				if( mgr->__gameobject_iterator != mgr->activeGameObjects.end() && (*mgr->__gameobject_iterator) == GetGameObject() )
					++mgr->__gameobject_iterator;

				mgr->activeGameObjects.erase(GetGameObject());
			break;
	}
	Active = false;
}
	bool Object::isInFront(Unit * target)
	{
		if(target==NULL)
			return false;

		int directrot = uint16(GetFacing(target));
		int orientation = GetO();

		int result = directrot-orientation;

		if(result <= 4000 && result >= -4000)
			return true;
		else return false;
		
		/*int dleft = int(directrot);
		int dright = int(directrot);

		int left=0;
		int right=0;

		// special case
		while(dleft != orientation && left < 3000)
		{
			++dleft;
			if(dleft>32768)
				dleft=0;

			++left;
		}

		while(dright != orientation && right < 3000)
		{
			--dright;
			if(dright<=0)
				dright=32768;

			++right;
		}

		if(right < 3000)
			return true;
		if(left < 3000)
			return true;
		
		return false;*/
	}

	bool Object::isInBack(Unit * target)
	{
		if(GetDistance(target) < 5.0f) return false; 

		// check if we are behind something ( is the object within a 180 degree slice of our negative y axis )

		double x = target->GetWorldX() - m_worldposition.x;
		double y = target->GetWorldY() - m_worldposition.y;

		double angle = atan2( y, x );
		angle = ( angle >= 0.0 ) ? angle : 2.0 * M_PI + angle;
		angle -= target->GetWorldO();

		while( angle > M_PI) angle -= 2.0 * M_PI;

		uint32 h = 0; 
		while(angle < -M_PI)
		{
			angle += 2.0 * M_PI;
			if(++h > 1000) return false;
		}

		// replace M_H_PI in the two lines below to reduce or increase angle

		double left = -1.0 * ( M_H_PI / 2.0 );
		double right = ( M_H_PI / 2.0 );

		return( ( angle >= left ) && ( angle <= right ) );
	}
	float Object::calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y )
{
	float dx = Position2X-Position1X;
	float dy = Position2Y-Position1Y;
	double angle=0.0f;

	// Calculate angle
	if (dx == 0.0)
	{
		if (dy == 0.0)
			angle = 0.0;
		else if (dy > 0.0)
			angle = M_PI * 0.5 /* / 2 */;
		else
			angle = M_PI * 3.0 * 0.5/* / 2 */;
	}
	else if (dy == 0.0)
	{
		if (dx > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if (dx < 0.0)
			angle = atanf(dy/dx) + M_PI;
		else if (dy < 0.0)
			angle = atanf(dy/dx) + (2*M_PI);
		else
			angle = atanf(dy/dx);
	}

	// Convert to degrees
	angle = angle * float(180 / M_PI);

	// Return
	return float(angle);
}

float Object::calcRadAngle( float Position1X, float Position1Y, float Position2X, float Position2Y )
{
	double dx = double(Position2X-Position1X);
	double dy = double(Position2Y-Position1Y);
	double angle=0.0;

	// Calculate angle
	if (dx == 0.0)
	{
		if (dy == 0.0)
			angle = 0.0;
		else if (dy > 0.0)
			angle = M_PI * 0.5/*/ 2.0*/;
		else
			angle = M_PI * 3.0 * 0.5/*/ 2.0*/;
	}
	else if (dy == 0.0)
	{
		if (dx > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if (dx < 0.0)
			angle = atan(dy/dx) + M_PI;
		else if (dy < 0.0)
			angle = atan(dy/dx) + (2*M_PI);
		else
			angle = atan(dy/dx);
	}

	// Return
	return float(angle);
}

float Object::getEasyAngle( float angle )
{
	while ( angle < 0 ) {
		angle = angle + 360;
	}
	while ( angle >= 360 ) {
		angle = angle - 360;
	}
	return angle;
}
void Object::ConvertWorldPos()
{
	m_worldposition.x = float(m_zone_x+(GetX()/2 & 0x00000FFF));
	m_worldposition.y = float(m_zone_y+(GetY()/2 & 0x00000FFF));

	if(GetZoneId() == 161) // WTF ....
		m_worldposition.z = float( ((GetZ()/2)+32768)/2 );
	else
		m_worldposition.z = float(GetZ()/4);

	m_worldposition.o = float(GetO()/8);
}
void Object::SetPosition(float x,float y,float z,float o,uint32 offx,uint32 offy,bool forceupdate)
{
	bool updateMap = forceupdate;

	float distance = GetDistance(m_lastposition.x,m_lastposition.y);
	if(distance > 2.0f)
		updateMap = true;

	m_position.ChangeCoords(x, y, z, o);

	if(offx)
	{
		m_OffSetX = offx;
		m_zone_x = m_OffSetX << 12;
		updateMap = true;
	}
	if(offy)
	{
		m_OffSetY = offy;
		m_zone_y = m_OffSetY << 12;
		updateMap = true;
	}

	ConvertWorldPos();

	if (IsInWorld() && updateMap)
	{
		m_lastposition.ChangeCoords(CalcPinX(),CalcPinY(),GetWorldZ(),o);
		m_zoneMgr->ChangeObjectLocation(this);
		if(IsCreature())
			GetCreature()->SendObjectState();
	}
}
float Object::GetFacing(uint16 x,uint16 y)
{
	float angle = calcAngle(GetX(),GetY(),x,y); 

	if( angle >= 90.0f)
		angle-=90;
	else if( angle < 90.0f)
		angle+=270;

	float orientation = angle*91.013f;

	return orientation;
}
float Object::GetFacing(Object * target)
{
	if(target == NULL)
		return 0;

	return GetFacing(target->GetX(),target->GetY());
}
void Object::CalculOffset() 
{
	m_OffSetX = mapmgr.CalculOffset(m_worldposition.x);
	m_OffSetY = mapmgr.CalculOffset(m_worldposition.y);
	m_zone_x = m_OffSetX << 12;
	m_zone_y = m_OffSetY << 12;
}

void Object::CalculPos() 
{
	m_position.x = mapmgr.CalculPos(GetRegion(),m_worldposition.x,true);
	m_position.y = mapmgr.CalculPos(GetRegion(),m_worldposition.y,false);
	m_position.z = m_worldposition.z*4;
	m_position.o = m_worldposition.o*8;
}
void Object::dispachPacketInRange(Buffer *b,bool self)
{
	if( HasInRangePlayers() )
	{
		unordered_set<Player*  >::iterator itr;
		for(itr=GetInRangePlayerSetBegin();itr!=GetInRangePlayerSetEnd();++itr)
		{
			if( !(*itr) ) continue;
			if( !(*itr)->IsInWorld() ) continue;
			(*itr)->dispachPacket(b);
		}
	}

	if( self && IsPlayer() )
		GetPlayer()->dispachPacket(b);
}
void Object::sendRemoved(Player * Plr)
{
	Buffer *b = new Buffer(7);
	b->write<uint16>(4);
	b->write<uint8>(F_REMOVE_PLAYER);
	b->write<uint16>(GetOid());
	b->write<uint16>(0);
	if(Plr==NULL)
		dispachPacketInRange(b);
	else Plr->sendPacket(b,false);
}