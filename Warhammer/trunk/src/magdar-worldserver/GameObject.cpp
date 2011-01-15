#include "StdAfx.h"

GameObject::GameObject(uint32 entry,const GameObjectSpawn * gos) : Unit()
{ 
	m_type = TYPEID_GAMEOBJECT;
	m_entry=entry;
	m_guid=gos->guid; 
	m_gos=gos;
	m_gon=NULL;
};
bool GameObject::Load()
{
	if(!m_gon)
		m_gon = objmgr.GetGameObjectProto(m_entry);

	if(!m_gon) 
		return false;


	m_worldposition.ChangeCoords(m_gos->WorldX , m_gos->WorldY , m_gos->WorldZ , m_gos->WorldO );

	m_name = m_gon->name;
	m_zone = m_gos->zone;
	m_region = objmgr.GetRegionByZone(m_zone);
	m_speed=100;
	m_model = m_gos->displayid;
	m_scale = m_gon->scale;
	m_health = 100;
	m_maxhealth = m_health;


	CalculOffset();
	CalculPos();

	return true;
}
void GameObject::Update(uint32 tdiff)
{
	Unit::Update(tdiff);
}
void GameObject::SendMeTo(Player * Plr)
{
	if(Plr == NULL) return;
	
	Log.Debug("Gameobject","Sending %s to %s",GetName().c_str(),Plr->GetName().c_str());
	uint16 responseSize = 42+GetName().length();
	Buffer* b = Plr->GetCom()->m_bufferPool->allocateBuffer(responseSize+3);
	b->write<uint16>(responseSize);
	b->write<uint8>(F_CREATE_STATIC);

	b->write<uint16>(m_oid);
	b->write<uint16>(0);
	b->write<uint16>(GetWorldO());
	b->write<uint16>(GetWorldZ());
	b->write<uint32>(GetWorldX());
	b->write<uint32>(GetWorldY());
	b->write<uint16>(m_gos->displayid);
	
	b->write<uint16>(m_gos->Unk[0]); // unk1
	b->write<uint16>(m_gos->Unk[1]); // unk2
	b->write<uint16>(m_gos->Unk[2]); // unk3
	b->write<uint8>(0); //
	b->write<uint16>(m_gos->Unk[3]); // unk4
	b->fill(0,5);
	b->write<uint16>(m_gos->Unk[4]); // unk5
	b->write<uint16>(m_gos->Unk[5]); // unk6

	b->write<uint32>(0);

	b->write<uint8>(GetName().length()+1);
	b->write(reinterpret_cast<const uint8*>(GetName().c_str()),GetName().length());
	
	b->write<uint8>(0);

	Plr->sendPacket(b);
}
Unit * GameObject::GetTarget()
{
	return m_aiInterface->GetTarget();
}
void GameObject::Save()
{
	Log.Debug("Gameobject","Save %u",m_gos->guid);

	stringstream ss;
	ss << "INSERT INTO gameobject_spawns (";
	ss << "guid,entry,zone,WorldX,WorldY,WorldZ,WorldO,";
	ss << "displayid,Unk1,Unk2,Unk3,Unk4,Unk5,Unk6)";
	ss << " VALUES (";

	ss << "'" << m_gos->guid << "',";
	ss << "'" << m_gos->entry << "',";
	ss << "'" << m_gos->zone << "',";
	ss << "'" << m_gos->WorldX << "',";
	ss << "'" << m_gos->WorldY << "',";
	ss << "'" << m_gos->WorldZ << "',";
	ss << "'" << m_gos->WorldO << "',";
	ss << "'" << m_gos->displayid << "'";

	for(int i=0;i<6;++i)
		ss << ",'" << m_gos->Unk[i] << "'";

	ss << ")";

	WorldDatabase.Execute(ss.str().c_str());
}