#include "StdAfx.h"

void Player::onMovement(uint16 X,uint16 Y,uint16 Z,uint16 O)
{
	if( X == 0 || Y == 0 || Z == 0 ) return;

	if(m_firstmovement)
	{
		Social_SendFriends();
		Social_SetOnline();
		m_firstmovement=false;
	}

	if(!m_acceptmonster)
	{
		m_acceptmonster=true;
		PushToWorld(GetZoneMgr());
	}

	if(IsMoving())
	{
		if( IsDisconnecting() )
		{
			m_decostage=20000;
			sEventMgr.RemoveEvents(this,EVENTS_EXIT_GAME);
			sendLocalizedString(string(""),TEXT_CANCELLED_LOGOUT);
			m_disconecting=false;
		}
	}

	if(m_teleporting)
	{
		m_teleporting=false;
		return;
	}

	SetPosition(X,Y,Z,O);

	sEventHook.ExecuteHook(EVENTS_PLAYER_POSITION_UPDATED,1,this);
}
void Player::onPlayerMovementOffset(uint16 offsetX,uint16 offsetY)
{
	if(offsetX > 998)
		offsetX = 998;
	if(offsetY > 998)
		offsetY = 998;

	m_OffSetX = offsetX;
	m_OffSetY = offsetY;

    m_zone_x = m_OffSetX << 12;
    m_zone_y = m_OffSetY << 12;

	ConvertWorldPos();

	if( IsInWorld() )
	{
		ZoneInfo * Info = m_zoneMgr->GetZoneInfo();
		if(Info)
		{	
			if( (m_zone_x < Info->minwx || m_zone_x >= Info->minwx+65536) || (m_zone_y < Info->minwy || m_zone_y >= Info->minwy+65536))
			{
				uint16 zoneid = objmgr.GetNewZoneID(m_zone_x,m_zone_y);
				if(zoneid)
				{
					RemoveFromWorld();
					SetZoneId(zoneid);
					AddToWorld();
				}
			}
		}
		m_zoneMgr->ChangeObjectLocation(this);
	}
}
void ClientCom::onPlayerState2() // Moving
{
	if( !m_Plr ) return;
	// Player move , so , it's not teleporting or it's not first movement

	if( !m_Plr->IsInWorld() )
		return;

	//m_readBuffer.Trace();

	m_readBuffer.pushReaderPos();

	if(!m_Plr->m_teleporting && m_Plr->HasInRangePlayers() )
	{		
		uint16 responseSize = m_readBuffer.remains()+1;
		Buffer* b = new Buffer(responseSize+3);
		b->write<uint16>(responseSize);
		b->write<uint8>(F_PLAYER_STATE2);
		b->write(&m_readBuffer,false);
		b->write<uint8>(0);
		m_Plr->dispachPacketInRange(b);
	}

	m_readBuffer.popReaderPos();

	if(m_expectedSize == 8 ) return;

	uint32 size = m_readBuffer.size();

	if(size < 30) return;

	uint16 playerKeyFromPacket, rot, pos_x, pos_y, pos_z=0;
	uint8 movement[4];
	uint8 unk1, unk2=0;

	m_readBuffer >> playerKeyFromPacket;
	m_readBuffer.read(movement,4);
	m_readBuffer >> rot >> pos_x >> pos_y >> unk1 >> pos_z >> unk2;

	SwapByte::Swap(playerKeyFromPacket);
	SwapByte::Swap(rot);
	SwapByte::Swap(pos_x);
	SwapByte::Swap(pos_y);
	SwapByte::Swap(pos_z);

	if(movement[2] > 50) // En combat
	{
		rot/=2;
		pos_z/=2;

		pos_x = mapmgr.CalculPosInCombat(m_Plr->GetZoneMgr()->GetBaseRegion(),pos_x,m_Plr->GetOffX(),true);
		pos_y = mapmgr.CalculPosInCombat(m_Plr->GetZoneMgr()->GetBaseRegion(),pos_y,m_Plr->GetOffY(),false);
	}

	if(movement[0] != 254)	m_Plr->SetMoving(true);
	else					m_Plr->SetMoving(false);

	m_Plr->onMovement(pos_x,pos_y,pos_z,rot);
}
void ClientCom::onDumpStatics() // Changing Offset
{
    uint32 unk1;
    uint16 unk2, unk3, offsetX, offsetY;
    m_readBuffer >> unk1 >> unk2 >> offsetX >> unk3 >> offsetY;

	if(m_Plr && ( m_Plr->GetOffX() != offsetX || m_Plr->GetOffY() != offsetY) )
		m_Plr->onPlayerMovementOffset(offsetX,offsetY);

}
void ClientCom::onJumpZone()
{
	uint32 info;
	m_readBuffer >> info;
	Log.Error("CLientCom","On Jump Zone %X",info);
}
void ClientCom::onInterractQueue()
{
	Log.Error("CLientCom","onInterractQueue");
}
void ClientCom::onFlight()
{
	Log.Debug("ClientCom","onFlight");

	if(m_Plr == NULL) return;

	uint16 toid,status=0;
	m_readBuffer >> toid >> status;

	if(status == 0x0014) // teleport
	{
		if(!m_Plr->m_interract) return;
		if(m_Plr->m_interract->GetOid() != toid) return;	

		uint16 number=0;
		m_readBuffer >> number;

		Log.Debug("ClientCom","onFlight Accepted number=%u",number);

		if(!number) return;
		if(number > m_Plr->GetZoneAccesCount()) return;

		uint32 counts=1;
		ZoneInfo * Info = NULL;
		for(map<uint16,ZoneInfo*>::iterator itr=m_Plr->m_zoneacces.begin();itr!=m_Plr->m_zoneacces.end();++itr)
		{
			if(counts==number)
			{
				Info=itr->second;
				break;
			}
			counts++;
		}
		if(!Info) return;

		if(!m_Plr->RemoveWealth(Info->price))
		{
			// Vous n'avez pas asser d'argent pour acheter
			m_Plr->sendLocalizedString(string(""),TEXT_MERCHANT_INSUFFICIENT_MONEY_TO_BUY);
			return;
		}

		m_Plr->SafeTeleport(Info);
	}
}