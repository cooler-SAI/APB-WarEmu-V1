#include "StdAfx.h"

void ClientCom::onInfluenceDetail()
{
	Log.Debug("ClientCom","onInfluenceDetail");
	m_readBuffer.Trace();

	uint16 influenceid;
	m_readBuffer >> influenceid;

	if(!m_Plr || !influenceid) return;

	InfluencePlayerInfo * Info = m_Plr->m_InfluenceInterface->AddInfluence(influenceid,0);

	m_Plr->m_InfluenceInterface->sendInfluenceDetail(Info);
	m_Plr->m_InfluenceInterface->sendInfluenceUpdated(Info,true);
}

void InfluencePlayerInfo::GenereateReward(Player *Plr)
{
	if(!Plr) return;
	if(!Info) return;

	for(int i=0;i<3;++i)
	{
		m_reward[i].clear();

		for(vector<ItemInfluence*>::iterator itr=Info->m_rewards[i].begin();itr!=Info->m_rewards[i].end();++itr)
		{
			if(!(*itr)->Proto) continue;

			if(m_reward[i].size() >=3)
				continue;

			uint32 playerskill = Plr->GetSkills();
			uint32 playercareer = 1 << (Plr->GetCareerLine()-1);
			uint8 race =  1 << (Plr->GetRace()-1);
			uint32 itmskill = (*itr)->Proto->skill << 1;

			if((*itr)->Proto->skill && (playerskill &= itmskill) != itmskill )
				continue;

			if((*itr)->Proto->career && !((*itr)->Proto->career & playercareer) )
				continue;

			if((*itr)->Proto->race && !((*itr)->Proto->race & race) )
				continue;


			m_reward[i].push_back( (*itr) );
		}
	}
}

void InfluenceInterface::LoadInfluences()
{
	if(m_influs.size()) return; // Téléportation pas de reload;

	QueryResult * Result = CharacterDatabase.Query("SELECT influenceid,influence,rewarded_0,rewarded_1,rewarded_2 FROM characters_influence WHERE character_id=%u",m_player->GetPlayerID());
	if(Result)
	{
		do{

			InfluencePlayerInfo * Info = new InfluencePlayerInfo;
			Info->influenceid = Result->Fetch()[0].GetUInt16();
			Info->influence = Result->Fetch()[1].GetUInt32();

			for(int i=0;i<3;++i)
				Info->rewarded[i] = Result->Fetch()[2+i].GetUInt32();

			Info->Info = objmgr.GetInfluenceInfo(Info->influenceid);
			Info->GenereateReward(m_player);
			m_influs[Info->influenceid] = Info;
		}while(Result->NextRow());
	}
	delete Result;

}
InfluencePlayerInfo * InfluenceInterface::AddInfluence(uint16 influenceid,uint32 counts,InfluenceInfo * IInfo)
{
	InfluencePlayerInfo * Info = GetInfluence(influenceid);
	if(!Info)
	{
		Info = new InfluencePlayerInfo;
		Info->influenceid = influenceid;
		Info->influence = counts;

		for(int i=0;i<3;++i)
			Info->rewarded[i]=0;

		if(IInfo)
			Info->Info = IInfo;
		else if(m_wait.size())
		{
			InfluenceInfo * wait = m_wait.front();
			m_wait.erase(m_wait.begin());

			Info->Info = wait;
			wait->influenceid = influenceid;

			WorldDatabase.Execute("UPDATE zone_chapters SET influenceid=%u WHERE chapterid=%u",influenceid,wait->chapterid);
		}
		else
			Info->Info = objmgr.GetInfluenceInfo(influenceid);

		Info->GenereateReward(m_player);
		m_influs[influenceid] = Info;

		CharacterDatabase.Execute("INSERT INTO characters_influence (character_id,influenceid,influence) VALUE (%u,%u,%u);",m_player->GetPlayerID(),Info->influenceid,counts);
	}

	if(!counts) return Info;

	if(!Info->Info) return NULL;

	if(Info->influence+counts > Info->Info->maxinflu)
		Info->influence = Info->Info->maxinflu;
	else Info->influence+=counts;

	sendInfluenceUpdated(Info);

	return Info;
}
bool InfluenceInterface::HasInfluence(uint16 influenceid)
{
	return GetInfluence(influenceid) != NULL;
}
InfluencePlayerInfo * InfluenceInterface::GetInfluence(uint16 influenceid)
{
	return m_influs[influenceid];
}
void InfluenceInterface::sendInfluenceInfo(InfluencePlayerInfo* Info)
{
	if(!Info) return;

	Log.Debug("InfluenceInterface","sendInfluenceInfo %u",Info->influence);
	uint16 responseSize = 16;
    Buffer* b = new Buffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_INFLUENCE_INFO);
    b->write<uint16>(2);
	b->write<uint16>(Info->influenceid);
	b->write<uint32>(Info->Info->maxinflu);
	b->write<uint16>(Info->influenceid);
	b->write<uint32>(Info->influence);
	b->write<uint16>(0);
	m_player->sendPacket(b);
}
void InfluenceInterface::sendInfluenceUpdated(InfluencePlayerInfo* Info,bool print)
{
	if(!Info) return;

	Log.Debug("InfluenceInterface","sendInfluenceUpdated %u",Info->influenceid);
	Buffer *b = new Buffer(12+3);
	b->write<uint16>(12);
	b->write<uint8>(F_INFLUENCE_UPDATE);
	b->write<uint16>(Info->influenceid);
	b->write<uint16>(0);
	b->write<uint32>(Info->influence);
	b->write<uint8>(print ? 2 : 1);
	b->fill(0,3);
	m_player->sendPacket(b);
}
void InfluenceInterface::sendInfluenceDetail(InfluencePlayerInfo* Info)
{
	if(!Info) return;

	Log.Debug("InfluenceInterface","sendInfluenceDetail %u",Info->influenceid);

	Buffer *b = new Buffer(0);
	b->write<uint16>(Info->influenceid);
	for(int i=0;i<3;++i)
	{
		bool first=true;
		for(vector<ItemInfluence*>::iterator itr=Info->m_reward[i].begin();itr!=Info->m_reward[i].end();++itr)
		{
			if(first)
			{
				b->write<uint32>( (*itr)->influence );
				b->write<uint8>(1);
				b->write<uint64>( 0x00000000000001F4 );
				b->write<uint8>(1);
				b->write<uint16>( Info->m_reward[i].size() );
				first=false;
			}
			
			sChat.BuildItemInfo(b,(*itr)->Proto);
		}
	}

	b->write<uint16>(0x3C00);

	Buffer *d = new Buffer(b->size()+3);
	d->write<uint16>(b->size());
	d->write<uint8>(F_INFLUENCE_DETAILS);
	d->write(b);
	m_player->sendPacket(d);
	delete b;
}