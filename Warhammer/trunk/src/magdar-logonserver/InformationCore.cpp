#include "LogonStdAfx.h"

initialiseSingleton(InformationCore);

void InformationCore::SetRealm(Realm * realm)
{
	if( !realm ) return;

	if(realm->connected)
		Log.Success("InformationCore","Realm <Online> : [%s] [%s:%s]",realm->Name.c_str(),realm->Adresse.c_str(),realm->Port.c_str());
	else 
		Log.Info("InformationCore","Realm <Offline> : [%s] [%s:%s]",realm->Name.c_str(),realm->Adresse.c_str(),realm->Port.c_str());

	m_realms[realm->RealmID] = realm;
}
void InformationCore::RemoveRealm(Realm * realm)
{
	m_realms.erase(realm->RealmID);
	realm = NULL;
}
void InformationCore::SendAccountToRealm(Account * Acct)
{
	if( m_charinfo[ Acct->AccountId ].size() > 0)
		return;

	map<uint32,Realm*>::iterator itr;
	for(itr = m_realms.begin(); itr != m_realms.end(); itr++)
	{
		if( itr->second && itr->second->Auth && itr->second->Auth->b_connected)
			itr->second->Auth->SendAccountId(Acct);
	}
}
void InformationCore::SendRealm(Buffer* b)
{
	if( m_realms.size() < 1 ) return;

	map<uint32,Realm*>::iterator itr;

	b->write<uint32>( m_realms.size() ); // Nombre de realm a envoyer

	for(itr = m_realms.begin(); itr != m_realms.end(); itr++)
	{
		Realm * m_realm = itr->second;

		if( !m_realm ) continue;

		b->write<uint8>(m_realm->RealmID);		//current server number
		b->write<uint8>(m_realm->connected);
		b->write<uint32>(0x00000001);
		b->write<uint8>(m_realm->RealmID);		//current server number
		b->write<uint8>(m_realm->RealmID);		//current server number

		string tmpStr = "["+m_realm->Language+"] " + m_realm->Name;
		b->write(tmpStr);
		b->write<uint32>(19); // UNK

		b->write(string("setting.allow_trials"));
		b->write(string("1"));

		b->write(string("setting.charxferavailable"));
		b->write(m_realm->Transfert);

		b->write(string("setting.language"));
		b->write(m_realm->Language);

		b->write(string("setting.legacy"));
		b->write(m_realm->Legacy);

		b->write(string("setting.manualbonus.realm.destruction"));
		b->write(string("0"));

		b->write(string("setting.manualbonus.realm.order"));
		b->write(string("0"));

		b->write(string("setting.name"));
		b->write(m_realm->Name);

		b->write(string("setting.net.address"));
		b->write(m_realm->Adresse);

		b->write(string("setting.net.port"));
		b->write(m_realm->Port);

		b->write(string("setting.redirect"));
		b->write(string("0"));

		b->write(string("setting.region"));
		b->write(m_realm->Region);

		b->write(string("setting.retired"));
		b->write(string("0"));

		b->write(string("status.queue.Destruction.waiting"));
		b->write(string("0"));

		b->write(string("status.queue.Order.waiting"));
		b->write(string("0"));

		b->write(string("status.realm.destruction.density"));
		b->write(string("0"));

		b->write(string("status.realm.order.density"));
		b->write(string("0"));

		b->write(string("status.servertype.openrvr"));
		b->write(m_realm->OpenRVR);

		b->write(string("status.servertype.rp"));
		b->write(m_realm->RP);
	   
		b->write(string("status.status"));
		b->write(string("0"));

	}
}
uint32 InformationCore::GetRealmCount()
{
	if(m_realms.size() > 0 )
		return m_realms.size();
	else return 0;
}
void InformationCore::SendCharList(uint32 sequence,AuthClient * Com)
{
	if( m_charinfo[ Com->GetAccountID() ].size() > 0 )
	{
		Log.Debug("InformationCore","Sended %u Character(s) to world for account %u",m_charinfo[ Com->GetAccountID() ].size(),Com->GetAccountID());
		uint32 size = m_charinfo[Com->GetAccountID()].size() + 8;
		Buffer* b = Com->m_bufferPool->allocateBuffer(size);

		b->write<uint32>(sequence);
		b->write<uint16>(0x0000);

		b->write<uint16>( m_charinfo[Com->GetAccountID()].size() );

		for( list<CharInfo*>::iterator itr = m_charinfo[Com->GetAccountID()].begin(); itr != m_charinfo[Com->GetAccountID()].end() ; itr++)
		{
			CharInfo * Info = (*itr);
			if(!Info) continue;

			std::ostringstream oss;
			oss << "<SummaryData version=\"11\" blockTypeId=\"1\">";
			oss << "<Name>";
			oss <<  Info->name;
			oss << "</Name>";
			oss << "<Imagenum>";
			oss << Info->model;
			oss << "</Imagenum>";
			oss << "<Region>";
			oss << Info->zone;
			oss << "</Region>";
			oss << "<WornList capacity=\"1\">"; //: note voluntarily cut. Goes to 15 on commercial server
			oss << "<Worn index=\"1\">" << Info->equipped_1 << "</Worn>";
			oss << "</WornList>";
			oss << "<ReadiedList capacity=\"5\">";
			oss << "<Readied index=\"0\">1916</Readied>"; //we still need to figure out what is readied.
			oss << "</ReadiedList>";
			oss << "<Level>";
			oss << Info->level;
			oss << "</Level>";
			oss << "<CharClass>";
			oss << Info->career;
			oss << "</CharClass>";
			oss << "<Realm>";
			oss << Info->realm;
			oss << "</Realm>";
			oss << "<HeldLeft>";
			oss << Info->heldleft;
			oss << "</HeldLeft>";
			oss << "<Race>";
			oss << Info->race;
			oss << "</Race>";
			oss << "<FeatureHigh>2147483648</FeatureHigh>"; //??
			oss << "<FeatureLow>1495468152</FeatureLow>";
			oss << "<RevisionId>97769940</RevisionId>";
			oss << "</SummaryData>";

			string str;
			str.assign( oss.str() );
			b->write<uint16>( Info->ServerID );			//Server ID for next character
			b->write<uint32>( Info->character_id );		//Character ID (found later in F_GUILD_DATA and S_PLAYER_INITTED)
			b->write<uint64>( Info->last_time_played);			//Time (since epoch) that character was last played
			b->write( str );
		}
		Com->writePacket(b,SMSG_GET_CHARACTER_SUMMARIES_RESPONSE);
	}
	else
	{
		uint32 size = 8;
		Buffer* b = Com->m_bufferPool->allocateBuffer(size);
		b->write<uint32>(sequence);
		b->write<uint16>(0x0000);
		b->write<uint16>(0);		    //number of Characters
		Com->writePacket(b,SMSG_GET_CHARACTER_SUMMARIES_RESPONSE);
	}

}