#include "StdAfx.h"

void ClientCom::onRequestChar()
{
	

	uint16 operation;

    m_readBuffer >> operation;
    // 1 more (null byte) in buffer
	Log.Debug("ClientCom","onRequestChar() %04X",operation);

	m_realm = objmgr.GetRealmAccount(m_accountID);

    switch (operation)
    {
    case 0x2D58:
        {
            uint16 responseSize = opcodeSize[F_REQUEST_CHAR_ERROR][SC];
            Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
            b->write<uint16>(responseSize);
            b->write<uint8>(F_REQUEST_CHAR_ERROR);
            b->write<uint8>(m_realm);
            writePacket(b);
        }break;
    case 0x2D53:
        {
			list<PlayerInfo*> m_list = objmgr.GetPlayerInfoList( m_accountID );
            uint16 responseSize = opcodeSize[F_REQUEST_CHAR_RESPONSE][SC];
            Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
            b->write<uint16>(responseSize);
            b->write<uint8>(F_REQUEST_CHAR_RESPONSE);

            b->write(reinterpret_cast<const uint8*>(m_username.c_str()),m_username.size());
            b->fill(0,21-m_username.size());
            b->write<uint8>(m_realm);
            b->write<uint16>(m_list.size()); //??
			//24
            uint8 fillSlot=0;


			if(m_list.size() > 0)
			{
				for(list<PlayerInfo*>::iterator itr=m_list.begin();itr != m_list.end();++itr)
				{
					PlayerInfo * Char = (*itr);

					if( !Char ) continue;

					// Skip empty slots
					while (fillSlot != Char->slot)
					{
						b->fill(0x00,280);
						fillSlot++;
					}

					b->write(reinterpret_cast<const uint8*>( Char->name.c_str() ), Char->name.length() );
					b->fill(0,48-Char->name.length());
					//48
					b->write<uint8>(Char->level); //level
					b->write<uint8>(Char->career); //career
					b->write<uint8>(Char->realm); //realm
					b->write<uint8>(Char->sex); //sex
					b->write<uint8>(Char->model); //model
					b->write<uint16>(Char->zone); //zone
					b->fill(0,5);
					//60

					// Armor Model
					for(uint16 i=14;i<30;++i)
					{
						uint16 modelid = 0;
						if( Char->m_items[i] )
						{
							const ItemProto * Proto = objmgr.GetItemProto(Char->m_items[i]);
							if(Proto) 
								modelid = Proto->modelid;
						}
						// in reverse order
						b->write<uint8>(modelid &0xFF);
						b->write<uint8>(modelid >>8);
						b->fill(0,6); ///\todo there is still stuf in it to be discovered
					}
					//188
					b->fill(0,6);
					//194

					b->fill(0,6);
					b->write<uint16>(0xFF00);

					b->fill(0,6);
					b->write<uint16>(0xFF00);

					b->fill(0,6);
					b->write<uint16>(0xFF00);

					b->fill(0,6);
					b->write<uint16>(0xFF00);

					b->fill(0,6);
					b->write<uint16>(0xFF00);

					// Weapons Model
					for(uint16 i=10;i<13;++i)
					{
						uint16 modelid = 0;
						if( Char->m_items[i] )
						{
							const ItemProto * Proto = objmgr.GetItemProto(Char->m_items[i]);
							if(Proto) modelid = Proto->modelid;
						}
						b->write<uint16>(0x0000);
						// in reverse order
						b->write<uint8>(modelid&0xFF);
						b->write<uint8>(modelid>>8);
					}
					//246
					b->fill(0,10);
					//256
					b->write<uint16>(0xFF00);
					//258
					b->write<uint8>(0x00);
					b->write<uint8>(Char->race); //race
					//260
					b->fill(0,2);
					//263
					b->write(Char->traits,8);
					//271
					b->fill(0,10);
					//275
					fillSlot++;
				}
			}

            //Fill empty slots
            while (fillSlot < 10)
            {
                b->fill(0x00,280);
                fillSlot++;
            }

            //        uint8 characterBytes1[272] = {0x41,0x65,0x6C,0x69,0x75,0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x40,0x02,0x00,0x18,0x00,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD9,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCD,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x7B,0x07,0x00,0x00,0x74,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x07,0x00,0x00,0x00,0x80,0x84,0x04,0x16,0x06,0x00,0x00,0x00,0x00};
            //        b->write(characterBytes1,272);
            //        b->fill(0x00,272*9); /// \todo here, get the character list on the server

            writePacket(b);
            break;
    }
    default:
        {
			Log.Warning("ClientCom","[ID: %u] Unknown operation received: %u",m_connectionID,operation);
            break;
        }
    }
}
void ClientCom::onRequestCharTemplates()
{
    /// \todo understand F_REQUEST_CHAR_TEMPLATES
    uint16 responseSize = opcodeSize[F_REQUEST_CHAR_TEMPLATES][SC];
    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_REQUEST_CHAR_TEMPLATES);

    uint8 data[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; // seen 0x40 as well
    b->write(data,0x11);
    writePacket(b);

}
void ClientCom::onCreateCharacter()
{
    uint8 slot, race ,career, sex, model;
    uint8 traits[8];
    uint16 nameSize;
    char* characterName = NULL;

    m_readBuffer >> slot >> race >> career >> sex >> model;
	m_readBuffer >> nameSize;
	m_readBuffer.skip(2);
    m_readBuffer.read(traits,8);

	m_readBuffer.skip(7);
    characterName = (char*)malloc(nameSize);
    m_readBuffer.read(reinterpret_cast<uint8*>(characterName),nameSize);
    
    std::string strTraits = Utils::byteArrayToAsciiString(traits,5);

	Characterinfo * Info = objmgr.GetCharacterInfo(career);

    if (Info != NULL)
    {
        stringstream ssSQL;
		ssSQL << "INSERT INTO `characters` (character_id,account,slot,name,model,zone,level,career,careerline,realm,heldleft,race,";
		ssSQL << "traits_0,traits_1,traits_2,traits_3,traits_4,traits_5,traits_6,traits_7,";
		ssSQL << "wealth,speed,region,posX,posY,posZ,posHeading,xp,restxp,renown,renownrank,rallypoint,playedtime,";
		ssSQL << "health,maxhealth,actionpoints,maxactionpoints,sex,skills) VALUES('";

		uint32 character_id = objmgr.GenerateCharacterGuid();
		ssSQL << character_id << "','";
		ssSQL << m_accountID << "','";
		ssSQL << (uint32) slot << "','";
		ssSQL << characterName << "','";
		ssSQL << (uint32) model << "','";
		ssSQL << Info->zone << "','";
		ssSQL << DEFAULT_PLAYER_LEVEL << "','";
		ssSQL << (uint32) career << "','";
		ssSQL << (uint32)Info->careerline << "','";
		ssSQL << (uint32)Info->realm << "','";
		ssSQL << "0','";
		ssSQL << (uint32) race << "','";

		ssSQL << (uint32) traits[0] << "','";
		ssSQL << (uint32) traits[1] << "','";
		ssSQL << (uint32) traits[2] << "','";
		ssSQL << (uint32) traits[3] << "','";
		ssSQL << (uint32) traits[4] << "','";
		ssSQL << (uint32) traits[5] << "','";
		ssSQL << (uint32) traits[6] << "','";
		ssSQL << (uint32) traits[7] << "','";

		ssSQL << "0','";
		ssSQL << DEFAULT_PLAYER_SPEED << "','";
		ssSQL << Info->region << "','";
		ssSQL << Info->pos_x << "','";
		ssSQL << Info->pos_y << "','";
		ssSQL << (uint32)Info->pos_z << "','";
		ssSQL << (uint32)Info->heading << "','";

		ssSQL << "0','";
		ssSQL << "0','";
		ssSQL << "0','";
		ssSQL << "0','";
		ssSQL << (uint32)Info->rallypo << "','";
		ssSQL << "0','";
		ssSQL << Info->m_stats[GameData::STATS_TOUGHNESS]*10 << "','";
		ssSQL << Info->m_stats[GameData::STATS_TOUGHNESS]*10 << "','";
		ssSQL << "250','";
		ssSQL << "250','";
		ssSQL << (uint32) sex << "','";
		ssSQL << Info->skills << "')";

		CharacterDatabase.WaitExecute(ssSQL.str().c_str());

		for(map<uint16,Characterinfo_item*>::iterator itr = Info->m_items.begin(); itr != Info->m_items.end();++itr)
		{
				stringstream ddSQL;
				ddSQL << "INSERT INTO characters_items (guid,character_id,entry,slot_id,counts) VALUES (";
				ddSQL << objmgr.GenerateItemGuid() << ",";
				ddSQL << character_id << ",";
				ddSQL << itr->second->entry << ",";
				ddSQL << itr->second->slot << ",";
				ddSQL << itr->second->count << ")";
				CharacterDatabase.WaitExecute(ddSQL.str().c_str());
		}

		for(map<uint32,float>::iterator itr = Info->m_stats.begin(); itr != Info->m_stats.end();++itr)
		{
				stringstream ddSQL;
				ddSQL << "INSERT INTO characters_stats (character_id,type_id,type_lv) VALUES (";
				ddSQL << character_id << ",";
				ddSQL << itr->first << ",";
				ddSQL << itr->second << ")";
				CharacterDatabase.WaitExecute(ddSQL.str().c_str());
		}

		for(list<uint16>::iterator itr = Info->m_spells.begin(); itr != Info->m_spells.end();++itr)
		{
				stringstream ddSQL;
				ddSQL << "INSERT INTO characters_spells (character_id,spellid) VALUES (";
				ddSQL << character_id << ",";
				ddSQL << (*itr) << ")";
				CharacterDatabase.WaitExecute(ddSQL.str().c_str());
		}


        uint16 responseSize = opcodeSize[F_SEND_CHARACTER_RESPONSE][SC];
        Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
        b->write<uint16>(responseSize);
        b->write<uint8>(F_SEND_CHARACTER_RESPONSE);
        b->write(reinterpret_cast<const uint8*>(m_username.c_str()),m_username.length());
        b->fill(0,20-m_username.length());
        b->fill(0,4);
        writePacket(b);

		sEventHook.ExecuteHook(EVENTS_CREATE_CHARACTER,2,this,m_username);
    }
}
void ClientCom::onDeleteCharacter()
{
	 // Fixed size packet lenght already checked.

    uint8 slotid;
    m_readBuffer >> slotid;

	objmgr.RemoveCharacerBySlot(m_accountID,slotid);

    uint16 responseSize = opcodeSize[F_SEND_CHARACTER_RESPONSE][SC];
    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_SEND_CHARACTER_RESPONSE);
    b->write(reinterpret_cast<const uint8*>(m_username.c_str()),m_username.length());
    b->fill(0,20-m_username.length());
    b->fill(0,4);
    writePacket(b);

	sEventHook.ExecuteHook(EVENTS_DELETE_CHARACTER,1,uint32(slotid));
}
void ClientCom::onDumpArenasLarge()
{
    //Fixed buffer size already checked.

    uint8 characterSlot;
    m_readBuffer >> characterSlot;
    // 1 more (null) bytes in buffer

    const char* fromPort="38699"; /// \todo decide what to do with these port numbers
    const char* toPort="38700";

    uint16 responseSize = opcodeSize[F_WORLD_ENTER][SC];
    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(F_WORLD_ENTER);
    b->write<uint16>(0x0608); ///\todo Unknown!
    b->fill(0,20);
    b->write((const uint8*)fromPort,5);
    b->write((const uint8*)toPort,5);

    std::string localIPAddress;
    boost::asio::ip::tcp::resolver resolver(m_ioservice);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;
    /*while (iter != end)
    {
        if (iter->endpoint().address().to_string().compare("127.0.0.1") != 0)
        {
            localIPAddress = iter->endpoint().address().to_string();
            break;
        }
        ++iter;
    }*/

    b->write(reinterpret_cast<const uint8*>(localIPAddress.c_str()),localIPAddress.length());
    b->fill(0,20-localIPAddress.length());
    writePacket(b);
}
void ClientCom::onOpenGame()
{
    uint16 responseSize = opcodeSize[S_GAME_OPENED][SC];
    Buffer* b = m_bufferPool->allocateBuffer(responseSize+3);
    b->write<uint16>(responseSize);
    b->write<uint8>(S_GAME_OPENED);
    b->write<uint8>(0);
    writePacket(b);
}
void ClientCom::onInitPlayer()
{
	Log.Debug("ClientCom","Initialising Player [%s]",m_characterName.c_str());

	if(!m_Plr)
	{
		QueryResult * Result = CharacterDatabase.Query("SELECT character_id FROM characters WHERE name='%s'",m_characterName.c_str());
		if(Result)
		{
			m_userStage = INITIALIZING;

			uint32 characterid = Result->Fetch()[0].GetUInt32();
			Player * Plr = objmgr.CreatePlayer(characterid,this);
			if(Plr) m_Plr = Plr;
		}
		delete Result;
	}

	if(m_Plr)
	{
		m_userZone=m_Plr->GetZoneId();
		sendServerInfo();
		sendRealmBonus();
		m_Plr->sendLoad();
		return;
	}
}