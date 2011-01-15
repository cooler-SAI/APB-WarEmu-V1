#include "StdAfx.h"

initialiseSingleton(ObjectMgr);

/*void TestEvent(uint32 ParamCount,va_list args)
{
	Log.Debug("Test","Testing Event list=%u",ParamCount);

	Unit * unit = va_arg(args, Unit* );
	bool ok = va_arg(args, bool );

	if(unit)
		Log.Debug("Test","Unit name = %s , bool =%u",unit->GetName().c_str(),ok);

}*/

bool ItemLoot::Generate()
{
	if( Rand(m_chance * objmgr.loot_rate) )
		return true;
	else return false;
}


ObjectMgr::ObjectMgr()
{
	m_starttime=uint32(UNIXTIME);
	registered=	false; // true si le serveur est enregistré sur le logon
	opened=		false; // true si le port du jeu est ouvert;
	loaded =	false;
	m_discotimeforgm=	true;
	m_cleandb=			false;

	///////////////////////////////////////
	//	World Info
	///////////////////////////////////////

	uint32 starttime = getMSTime();
	Log.Notice("ObjectMgr","");
	LoadRealmConfig();	
	Log.Notice("ObjectMgr","");
	LoadMaxGuid();
	Log.Notice("ObjectMgr","");
	LoadRenowLevel();
	Log.Notice("ObjectMgr","");
	LoadXpInfo();
	Log.Notice("ObjectMgr","");
	LoadTokInfo();
	Log.Notice("ObjectMgr","");
	mapmgr.LoadAreaInfo();
	Log.Notice("ObjectMgr","");
	LoadRegionOffset();
	Log.Notice("ObjectMgr","");
	LoadZoneInfo();
	Log.Notice("ObjectMgr","");
	LoadItemProto();
	Log.Notice("ObjectMgr","");
	LoadCharacterInfo();
	Log.Notice("ObjectMgr","");
	LoadCharacterInfoLevel();
	Log.Notice("ObjectMgr","");
	LoadKeep();
	Log.Notice("ObjectMgr","");
	LoadCreatureProto();
	Log.Notice("ObjectMgr","");
	LoadCreatureSpawn();
	Log.Notice("ObjectMgr","");
	LoadCreatureVendors();
	Log.Notice("ObjectMgr","");
	LoadGameObjectName();
	Log.Notice("ObjectMgr","");
	LoadGameObjectSpawn();
	Log.Notice("ObjectMgr","");
	LoadCreatureItems();
	Log.Notice("ObjectMgr","");
	LoadCellSpawns();
	Log.Notice("ObjectMgr","");
	LoadCreatureLoot();
	Log.Notice("ObjectMgr","");
	LoadAbilityInfo();
	Log.Notice("ObjectMgr","");
	LoadInfluence();
	Log.Notice("ObjectMgr","");
	new QuestMgr();
	Log.Notice("ObjectMgr","");

	CleanDbErrors();
	scriptmgr.LoadScript();

	uint32 endtime = getMSTime();
	Log.Info("ObjMgr","Database Loaded in : %u ms",endtime-starttime);
	loaded = true;
}
ObjectMgr::~ObjectMgr()
{
	sCreatureProto.~SQLStorage();

}
void ObjectMgr::LoadMaxGuid()
{
	QueryResult * Result = CharacterDatabase.Query("SELECT MAX(guid) FROM characters_items");
	if(Result) itemguid = Result->Fetch()[0].GetUInt32();
	else itemguid=1;
	delete Result;

	Log.Success("ObjectMgr","Max item guid = %u",itemguid);

	Result = CharacterDatabase.Query("SELECT MAX(character_id) FROM characters");
	if(Result) characterguid = Result->Fetch()[0].GetUInt32();
	else characterguid=1;
	delete Result;

	Log.Success("ObjectMgr","Max characters guid = %u",characterguid);

	Result = WorldDatabase.Query("SELECT MAX(guid) FROM creature_spawns");
	if(Result) spawnguid = Result->Fetch()[0].GetUInt32();
	else spawnguid=1;
	delete Result;
	Log.Success("ObjectMgr","Max Spawn guid = %u",spawnguid);

	Result = WorldDatabase.Query("SELECT MAX(guid) FROM gameobject_spawns");
	if(Result) goguid = Result->Fetch()[0].GetUInt32();
	else goguid=1;
	delete Result;
	Log.Success("ObjectMgr","Max Go guid = %u",goguid);

	Result = CharacterDatabase.Query("SELECT MAX(guid) FROM characters_mails");
	if(Result) mailguid = Result->Fetch()[0].GetUInt32();
	else mailguid=1;
	delete Result;
	Log.Success("ObjectMgr","Max Mail guid = %u",goguid);
}
void ObjectMgr::LoadCreatureProto()
{
	Log.Debug("ObjectMgr","Loading Creature Proto ...");
	sCreatureProto.Load();
	Log.Success("ObjectMgr","Loaded %u CreatureProto. . .",sCreatureProto.RecordCount);
}
void ObjectMgr::LoadCreatureSpawn()
{
	Log.Debug("ObjectMgr","Loading Creatures Spawns ...");
	sCreatureSpawn.Load();
	Log.Success("ObjectMgr","Loaded %u CreatureSpawn. . .",sCreatureSpawn.RecordCount);
}
void ObjectMgr::LoadCreatureItems()
{
	Log.Debug("ObjectMgr","Loading Creature equipements...");

	// For reload case
    for (CacheCreatureItems::iterator itr = m_mCacheCreatureItem.begin(); itr != m_mCacheCreatureItem.end(); ++itr)
        itr->second.Clear();
    m_mCacheCreatureItem.clear();

    QueryResult *result = WorldDatabase.Query("SELECT entry,slot_item,model_item,effect FROM creature_items");
    if( !result )
    {
        barGoLink bar( 1 );

        ++bar;

        sLog.outString("");
        sLog.outError(">> Loaded `creature_items`, table is empty!");
        return;
    }

    barGoLink bar( result->GetRowCount() );

    uint32 count = 0;
    do
    {
        ++bar;
        Field* fields = result->Fetch();

        uint32 guid			= fields[0].GetUInt32();
        uint32 slot			= fields[1].GetUInt32();
        uint32 model		= fields[2].GetUInt32();
		uint32 effect		= fields[3].GetUInt32();


        CreatureEquipement& vList = m_mCacheCreatureItem[guid];

        vList.AddItem(slot,model,effect);
        ++count;

    } while (result->NextRow());
    delete result;

    Log.Success("ObjectMgr","Loaded %u Equipement ",count);

}
void ObjectMgr::LoadGameObjectName()
{
	Log.Debug("ObjectMgr","Loading GameObjects names ...");
	sGameObjectProto.Load();
	Log.Success("ObjectMgr","Loaded %u GameObjectNames. . .",sGameObjectProto.RecordCount);
}
void ObjectMgr::LoadGameObjectSpawn()
{
	Log.Debug("ObjectMgr","Loading GameObjects spanws ...");
	sGameObjectSpawn.Load();
	Log.Success("ObjectMgr","Loaded %u GameObjectSpawns. . .",sGameObjectSpawn.RecordCount);
}
void ObjectMgr::LoadCreatureLoot()
{
	Log.Debug("ObjectMgr","Loading Creature loots...");
	// For reload case
    for (CacheCreatureLoot::iterator itr = m_mCacheCreatureLoot.begin(); itr != m_mCacheCreatureLoot.end(); ++itr)
        itr->second.Clear();
    m_mCacheCreatureLoot.clear();

    QueryResult *result = WorldDatabase.Query("SELECT entry,itemid,loot_percent,loot_type FROM loots");
    if( !result )
    {
        barGoLink bar( 1 );

        ++bar;

        sLog.outString("");
        sLog.outError(">> Loaded `loots`, table is empty!");
        return;
    }

    barGoLink bar( result->GetRowCount() );
	FILE * fp = fopen("entryerror.txt","ab+");
    uint32 count = 0;
    do
    {
        ++bar;
        Field* fields = result->Fetch();

        uint32 entry		= fields[0].GetUInt32();
        uint32 itemid		= fields[1].GetUInt32();
		float loot_percent	= fields[2].GetFloat();
		uint32 loot_type	= fields[3].GetUInt32();

		const ItemProto * proto = GetItemProto(itemid);
		if(proto == NULL)
		{
			Log.Error("ObjMgr","[%u] Invalid Item Loot entry %u",entry,itemid);
			stringstream ss;
			ss << "DELETE FROM loots WHERE itemid="<<itemid;
			m_errors["World"].push_back(ss.str());
			fprintf(fp,"%u \n",itemid);
			continue;
		}

		if(loot_percent < 0.1f)  loot_percent=0.1f;
		if(loot_percent > 100.0f) loot_percent=100.0f;

        CreatureLoot& vList = m_mCacheCreatureLoot[entry];

        vList.AddLoot(proto,loot_percent,loot_type);
        ++count;

    } while (result->NextRow());
    delete result;
	fclose(fp);
    Log.Success("ObjectMgr","Loaded %u Creatures Loots ",count );
}
void ObjectMgr::LoadCellSpawns()
{
	Log.Debug("ObjectMgr","Assembling Cell CreatureSpawns ...");

	{
	if(sCreatureSpawn.MaxEntry)
	{
		barGoLink bar(sCreatureSpawn.MaxEntry);
		++bar;

		for(uint32 i = 1; i < sCreatureSpawn.MaxEntry; ++i)
		{
			++bar;

			const CreatureSpawn * Info = sCreatureSpawn.LookupEntry<CreatureSpawn>(i);
			if(!Info) continue;

			if(!Info->WorldX || !Info->WorldY)
			{
				Log.Error("ObjMgr","Invalid Spawn position  : Guid(%u)",Info->guid);
				stringstream ss;
				ss << "DELETE FROM creature_spawns WHERE guid="<<Info->guid;
				m_errors["World"].push_back(ss.str());
				continue;
			}

			uint16 offx = mapmgr.CalculOffset( Info->WorldX );
			uint16 offy = mapmgr.CalculOffset( Info->WorldY );

			if(!Info->zone)
			{
				uint16 zone = objmgr.GetNewZoneID(Info->WorldX,Info->WorldY);

				if(zone)
					WorldDatabase.Execute("UPDATE creature_spawns SET zone=%u WHERE guid=%u",zone,Info->guid);
			}

			if(offx>999 || offy>999 || !offx || !offy)
			{
				Log.Error("ObjMgr","Invalid Spawn offset : Guid(%u) [%u][%u]",Info->guid,offx,offy);
				stringstream ss;
				ss << "DELETE FROM creature_spawns WHERE guid="<<Info->guid;
				m_errors["World"].push_back(ss.str());
				continue;
			}

			bool ok=true;
			CellSpawns * CP = mapmgr.GetSpawnsList(Info->zone,offx,offy);
			if(!CP)
			{
				CP = new CellSpawns;
				ok = false;
			}

			CP->CreatureSpawns.push_back(Info);
			if(!ok)
				mapmgr.SetSpawnsList(Info->zone,offx,offy,CP);
		}
	}
	else
	{
		barGoLink bar(1);
		++bar;
	}

	}
	Log.Notice("ObjectMgr","");
	Log.Debug("ObjectMgr","Assembling Cell GameobjectSpawns ...");

	{
	if(sGameObjectSpawn.MaxEntry)
	{
		barGoLink bar(sGameObjectSpawn.MaxEntry);
		++bar;
		for(uint32 i = 1; i < sGameObjectSpawn.MaxEntry; ++i)
		{
			++bar;

			const GameObjectSpawn * Info = sGameObjectSpawn.LookupEntry<GameObjectSpawn>(i);
			if(!Info) continue;

			if(!Info->WorldX || !Info->WorldY)
			{
				Log.Error("ObjMgr","Invalid Spawn position  : Guid(%u)",Info->guid);
				stringstream ss;
				ss << "DELETE FROM gameobject_spawns WHERE guid="<<Info->guid;
				m_errors["World"].push_back(ss.str());
				continue;
			}

			uint16 offx = mapmgr.CalculOffset( Info->WorldX );
			uint16 offy = mapmgr.CalculOffset( Info->WorldY );

			if(offx>999 || offy>999 || !offx || !offy)
			{
				Log.Error("ObjMgr","Invalid Spawn offset : Guid(%u) [%u][%u]",Info->guid,offx,offy);
				stringstream ss;
				ss << "DELETE FROM gameobject_spawns WHERE guid="<<Info->guid;
				m_errors["World"].push_back(ss.str());
				continue;
			}

			bool ok=true;
			CellSpawns * CP = mapmgr.GetSpawnsList(Info->zone,offx,offy);
			if(!CP)
			{
				CP = new CellSpawns;
				ok = false;
			}

			CP->GOSpawns.push_back(Info);
			if(!ok)
				mapmgr.SetSpawnsList(Info->zone,offx,offy,CP);
		}
	}
	else
	{
		barGoLink bar(1);
		++bar;
	}

	}

}
void ObjectMgr::LoadKeep()
{
    ///- Fill the keeps list
    Keep stdKeep;
    for (int i=1; i<0x1F; ++i)
        m_keeps.push_back(stdKeep);

    /// \todo Get the keep data from the database.

    ///- Fill the objective list
    Objective stdObjective;
    for (int i=0; i<7071; ++i)
        m_objectives.push_back(stdObjective);

    /// \todo Get the objective data from the database.

    ///- Fill the city list
    City altdorf(GameData::CITYID_EMPIRE, GameData::REALM_ORDER);
    City inevitable(GameData::CITYID_CHAOS, GameData::REALM_DESTRUCTION);
    m_cities.push_back(altdorf);
    m_cities.push_back(inevitable);
}
void ObjectMgr::LoadRealmConfig()
{
	Log.Debug("ObjectMgr","Loading Realm configuration.");

	m_realm = new Realm;

	Config.MainConfig.GetInt("RealmInfo","RealmID",&m_realm->RealmID);
	Config.MainConfig.GetString("RealmInfo","Name",&m_realm->Name);
	Config.MainConfig.GetString("RealmInfo","Language",&m_realm->Language);
	Config.MainConfig.GetString("RealmInfo","Transfer",&m_realm->Transfert);
	Config.MainConfig.GetString("RealmInfo","Legacy",&m_realm->Legacy);
	Config.MainConfig.GetString("RealmInfo","Region",&m_realm->Region);
	Config.MainConfig.GetString("RealmInfo","OpenRVR",&m_realm->OpenRVR);
	Config.MainConfig.GetString("RealmInfo","RP",&m_realm->RP);

	Config.MainConfig.GetString("RealmInfo","Adresse",&m_realm->Adresse);
	Config.MainConfig.GetString("RealmInfo","Port",&m_realm->Port);

	Log.Debug("ObjectMgr","Realm [%s] : [%s:%s]-[%s] ",m_realm->Name.c_str(),m_realm->Adresse.c_str(),m_realm->Port.c_str(),m_realm->Language.c_str());

	CharacterDatabase.Execute("DELETE FROM characters_items WHERE character_id!=0 AND character_id NOT IN (SELECT character_id FROM characters)");

	Config.MainConfig.GetString("GameInfo","RealmMessage",&m_Welcome);
	Config.MainConfig.GetInt("GameInfo","MaxPlayer",&m_maxplayer);
	Config.MainConfig.GetBool("GameInfo","DiscoTimeForGm",&m_discotimeforgm);
	Config.MainConfig.GetBool("GameInfo","CleanDB",&m_cleandb);
	Config.MainConfig.GetString("GameInfo","ClientVersion",&ClientVersion);
	Config.MainConfig.GetInt("GameInfo","RezTime",&RezTime);
	Config.MainConfig.GetInt("GameInfo","SaveTime",&m_savetime);

	Config.MainConfig.GetInt("RateInfo","Xp",&xp_rate);
	Config.MainConfig.GetInt("RateInfo","Loot",&loot_rate);
	Config.MainConfig.GetInt("RateInfo","Gold",&gold_rate);

	if(xp_rate==0) xp_rate=1;
	if(loot_rate==0) loot_rate=1;
	if(gold_rate==0) gold_rate=1;

	CharacterDatabase.Execute("UPDATE characters SET online=0");
}
void ObjectMgr::LoadAbilityInfo()
{
	Log.Debug("ObjectMgr","Loading Ability Info...");

	uint32 counts=0;

	QueryResult * Result = WorldDatabase.Query("SELECT entry,level,abilitytype,minrange,maxrange,cooldown,targettype,damage,casttime,actionpoints FROM abilityinfo");
	if(Result)
	{
		do{
			AbilityInfo * Info = new AbilityInfo;
			Info->entry =		Result->Fetch()[0].GetUInt32();
			Info->level =		Result->Fetch()[1].GetUInt32();
			Info->Type =		GameData::AbilityType(Result->Fetch()[2].GetUInt32());
			Info->minrange =	Result->Fetch()[3].GetUInt32();
			Info->maxrange =	Result->Fetch()[4].GetUInt32();
			Info->cooldown =	Result->Fetch()[5].GetUInt32();
			Info->targettype =	Result->Fetch()[6].GetUInt32();
			Info->damage =		Result->Fetch()[7].GetUInt32();
			Info->casttime =	Result->Fetch()[8].GetUInt32();
			Info->actionpoints =Result->Fetch()[9].GetUInt32();

			m_abilityinfo[Info->entry][Info->level] = Info;
			counts++;

		}while(Result->NextRow());

	}
	delete Result;

	Log.Success("ObjectMgr","Loaded %u Ability Info",counts);
}
void ObjectMgr::LoadInfluence()
{
	Log.Debug("ObjectMgr","Loading InfluenceInfo. . .");

	uint32 counts=0;
	QueryResult * Result = WorldDatabase.Query("SELECT chapterid,zoneid,name,npc,influenceid,race,number,pinx,piny,tok_entry,tok_explore FROM zone_chapters");
	if(Result)
	{
		do{
			uint32 chapterid =	Result->Fetch()[0].GetUInt32();
			uint16 zoneid =		Result->Fetch()[1].GetUInt32();
			string name =		Result->Fetch()[2].GetString();
			uint32 npcid =		Result->Fetch()[3].GetUInt32();
			uint32 influenceid = Result->Fetch()[4].GetUInt32();
			string race=		Result->Fetch()[5].GetString();
			uint32 number=		Result->Fetch()[6].GetUInt32();
			uint32 pinx=		Result->Fetch()[7].GetUInt32();
			uint32 piny=		Result->Fetch()[8].GetUInt32();
			uint32 tok_entry=	Result->Fetch()[9].GetUInt32();
			uint32 tok_explore=	Result->Fetch()[10].GetUInt32();

			InfluenceInfo * Info = new InfluenceInfo();
			Info->chapterid =	chapterid;
			Info->zoneid =		zoneid;
			Info->name =		name;
			Info->npc =			this->GetCreatureProto(npcid);
			Info->influenceid = influenceid;
			Info->maxinflu =	0;
			Info->race			=race;
			Info->number		=number;
			Info->pinx			=pinx;
			Info->piny			=piny;
			Info->tok_entry		=tok_entry;
			Info->tok_explore	=tok_explore;

			Info->realm=0;
			if(race == "Dwarf" || race == "High Elf" || race == "Empire")
				Info->realm=0;
			else Info->realm=1;

			/*if(!Info->influenceid)
			{
				AreaInfo * Area = mapmgr.CheckAreaInfo(Info->realm,zoneid,pinx,piny);
				if(Area)
				{
					Info->influenceid = Area->influenceid[Info->realm];
					WorldDatabase.Execute("UPDATE zone_chapters SET influenceid=%u WHERE chapterid=%u",Info->influenceid,Info->chapterid);
				}
			}*/
			
			QueryResult * Items = WorldDatabase.Query("SELECT itemid,influence,influencetier FROM zone_chapters_reward WHERE chapterid=%u",Info->chapterid);
			if(Items)
			{
				do{
					uint32 itemid =		Items->Fetch()[0].GetUInt32();
					uint32 influence =	Items->Fetch()[1].GetUInt32();
					uint32 tier =		Items->Fetch()[2].GetUInt32();

					ItemInfluence * Item = new ItemInfluence;
					Item->influence =	influence;
					Item->Proto =		this->GetItemProto(itemid);
					Item->tier =		tier-1;

					if(!Item->Proto) continue;
					if(Item->tier >= 3) continue;

					Info->m_rewards[Item->tier].push_back(Item);

					if(Info->maxinflu < influence)
						Info->maxinflu = influence;

				}while(Items->NextRow());
			}
			delete Items;

			InfoInfluence.insert( make_pair( chapterid, Info) );

			++counts;
		}while(Result->NextRow());

	}
	delete Result;

	Log.Success("ObjectMgr","Loaded %u Influence Info. . .",counts);
}
void ObjectMgr::LoadCharacterInfo()
{
	Log.Debug("ObjectMgr","Loading CharacterInfo. . .");

	M_characterinfo.Acquire();
	m_characterinfo.clear();

	QueryResult * Result = WorldDatabase.Query("SELECT * FROM characterinfo");

	uint32 counts=0;
	if(Result)
	{
		barGoLink bar(Result->GetRowCount());
		do{
			++bar;

			Characterinfo * Info = new Characterinfo;

			Info->careerline =	Result->Fetch()[0].GetUInt32();
			Info->career =		Result->Fetch()[1].GetUInt32();
			Info->career_name = Result->Fetch()[2].GetString();
			Info->realm =		Result->Fetch()[3].GetUInt32();
			Info->region =		Result->Fetch()[4].GetUInt32();

			Info->zone =		Result->Fetch()[5].GetInt16();
			Info->pos_x =		Result->Fetch()[6].GetUInt32();
			Info->pos_y =		Result->Fetch()[7].GetUInt32();
			Info->pos_z =		Result->Fetch()[8].GetUInt32();
			Info->heading =		Result->Fetch()[9].GetUInt32();
			Info->rallypo =		Result->Fetch()[10].GetUInt32();
			Info->skills =		Result->Fetch()[11].GetUInt32();

			LoadCharacterInfoItems(Info);
			LoadCharacterInfoStats(Info);
			LoadCharacterInfoSpells(Info);

			m_characterinfo[Info->career] = Info;
		counts++;
		}while(Result->NextRow());
	}
	else 
		Log.Error("ObjectMgr","Can't Load CharacterInfo! Characters creation disabled !");
	
	M_characterinfo.Release();

	Log.Success("ObjectMgr","Loaded %u CharacterInfo. . .",counts);
	delete Result;
}
void ObjectMgr::LoadCharacterInfoLevel()
{
	Log.Debug("ObjectMgr","Loading Level info...");
	barGoLink bar(25);

	uint32 counts=0;
	for(int i=0;i<25;++i)
	{
		++bar;
		uint8 lastlevel=0;
		QueryResult * Result = WorldDatabase.Query("SELECT level,stat_id,stat_value FROM characterinfo_levelstats WHERE careerline=%u ORDER BY `level`",i);
		if(Result)
		{
			LevelInfo * Info = NULL;
			do{
				uint8 level =	Result->Fetch()[0].GetUInt32();
				uint16 careerline = i;
				uint16 type =	Result->Fetch()[1].GetUInt32();
				uint16 value =	Result->Fetch()[2].GetUInt32();

				if(lastlevel!=level)
				{
					counts++;
					if(Info)
						InfoLevel.AddLevelInfo(Info);
					Info = new LevelInfo;
					lastlevel=level;
					Info->careerline = careerline;
					Info->level = level;
				}

				Info->stats[type] = value;
			}while(Result->NextRow());
		}
		delete Result;
	}
	Log.Success("ObjectMgr","Loaded %u Level info",counts);
}
void ObjectMgr::LoadCharacterInfoItems(Characterinfo * Info)
{
	QueryResult * Result = WorldDatabase.Query("SELECT entry,slot_id,counts FROM characterinfo_items WHERE careerline=%u",Info->careerline);
	if(Result)
	{
		do{
			Characterinfo_item * item = new Characterinfo_item;
			item->entry = Result->Fetch()[0].GetUInt32();
			item->slot = Result->Fetch()[1].GetUInt32();
			item->count = Result->Fetch()[2].GetUInt32();
			Info->m_items[item->slot] = item;
		}while(Result->NextRow());
	}
	delete Result;
}
void ObjectMgr::LoadCharacterInfoStats(Characterinfo * Info)
{
	QueryResult * Result = WorldDatabase.Query("SELECT type_id,type_lv FROM characterinfo_stats WHERE career_id=%u",Info->career);
	if(Result)
	{
		do{
			Info->m_stats[Result->Fetch()[0].GetUInt8()] = Result->Fetch()[1].GetUInt16();
		}while(Result->NextRow());
	}
	delete Result;
}
void ObjectMgr::LoadCharacterInfoSpells(Characterinfo * Info)
{
	QueryResult * Result = WorldDatabase.Query("SELECT spellid FROM characterinfo_spells WHERE career_id=%u",Info->careerline);
	if(Result)
	{
		do{
			Info->m_spells.push_back( Result->Fetch()[0].GetUInt16() );
		}while(Result->NextRow());
	}
	delete Result;
}
void ObjectMgr::LoadItemProto()
{
	/*stringstream ss;
	ss << "SELECT entry";
	for(int i=1;i<13;++i)
	{
		ss << ",stat"<<i<<"_type,stat"<<i<<"_value";
	}
	ss << " FROM items";

	QueryResult * Result = WorldDatabase.Query(ss.str().c_str());
	if(Result)
	{
		do{
			uint32 entry = Result->Fetch()[0].GetUInt32();
			for(int i=1;i<24;)
			{
				uint32 type = Result->Fetch()[i].GetUInt32();
				uint32 value = Result->Fetch()[i+1].GetUInt32();

				if(type == 26)
				{
					uint32 tt = i/2;
					tt+=1;

					stringstream sd;
					sd << "UPDATE items SET stat"<<tt<<"_type=0,stat"<<tt<<"_value=0,armor="<<value<<" WHERE entry="<<entry;
					//Log.Error("Objmgr",sd.str().c_str());
					WorldDatabase.Execute(sd.str().c_str());
				}
				i+=2;
			}

		}while(Result->NextRow());
	}*/
	Log.Debug("ObjectMgr","Loading ItemProto. . .");
	sItemsTable.Load();
	Log.Success("ObjectMgr","Loaded %u ItemProto !",sItemsTable.RecordCount);
}
void ObjectMgr::LoadXpInfo()
{
	Log.Debug("ObjectMgr","Loading Xp Info. . .");
	sXpTable.Load();
	Log.Success("ObjectMgr","Loaded %u Xp Info. . .",sXpTable.RecordCount );
}
void ObjectMgr::LoadTokInfo()
{
	Log.Debug("ObjectMgr","Loading Tok Info. . .");
	sTokTable.Load();
	Log.Success("ObjectMgr","Loaded %u Tok Info. . .",sTokTable.RecordCount );
}
void ObjectMgr::LoadRegionOffset()
{
	Log.Debug("ObjectMgr","Loading Region Table. . .");
	sRegionOffset.Load();
	Log.Success("ObjectMgr","Loaded %u Region Info. . .",sRegionOffset.RecordCount );
}
void ObjectMgr::LoadZoneInfo()
{
	Log.Debug("ObjectMgr","Loading ZoneInfo Table. . .");

	uint32 counts=0;

	M_xpinfo.Acquire();
	QueryResult * Result = WorldDatabase.Query("SELECT * FROM zone_info");
	if(Result)
	{
		barGoLink bar(Result->GetRowCount());

		do{
			++bar;

			ZoneInfo * Info = new ZoneInfo;
			Info->zoneid =		Result->Fetch()[0].GetUInt32();
			Info->zonename =	Result->Fetch()[1].GetString();
			Info->minlevel =	Result->Fetch()[2].GetUInt32();
			Info->maxlevel =	Result->Fetch()[3].GetUInt32();
			Info->type =		Result->Fetch()[4].GetUInt32();
			Info->tier =		Result->Fetch()[5].GetUInt32();
			Info->price =		Result->Fetch()[6].GetUInt32();
			Info->region =		Result->Fetch()[7].GetUInt32();

			Info->minwx =		Result->Fetch()[8].GetUInt32();
			Info->minwy =		Result->Fetch()[9].GetUInt32();
			
			QueryResult * Tp = WorldDatabase.Query("SELECT * FROM zone_taxi WHERE zoneid=%u",Info->zoneid);
			if(Tp)
			{
				do{
					uint8 realmid = Tp->Fetch()[1].GetUInt32();
					if(realmid > 2) continue;

					uint32 x = Tp->Fetch()[2].GetUInt32();
					uint32 y = Tp->Fetch()[3].GetUInt32();
					uint32 z = Tp->Fetch()[4].GetUInt32();
					uint32 o = Tp->Fetch()[5].GetUInt32();
					Info->m_tp[realmid].x = float(x);
					Info->m_tp[realmid].y = float(y);
					Info->m_tp[realmid].z = float(z);
					Info->m_tp[realmid].o = float(o);

				}while(Tp->NextRow());
			}
			delete Tp;

			if(Info->region)
				Info->Offset = GetRegionOffset(Info->region);

			m_zoneinfo[Info->zoneid] = Info;

			counts++;
		}while(Result->NextRow());
	}
	M_xpinfo.Release();
	delete Result;
	Log.Success("ObjectMgr","Loaded %u Zone Info. . .",counts);
}
void ObjectMgr::LoadRenowLevel()
{
	Log.Debug("ObjectMgr","Loading Renow Table. . .");
	sRenownTable.Load();
	Log.Success("ObjectMgr","Loaded %u Renowns Info !",sRenownTable.RecordCount);

}
void ObjectMgr::LoadCreatureVendors()
{
	// For reload case
    for (CacheVendorItemMap::iterator itr = m_mCacheVendorItemMap.begin(); itr != m_mCacheVendorItemMap.end(); ++itr)
        itr->second.Clear();
    m_mCacheVendorItemMap.clear();

    std::set<uint32> skip_vendors;

	Log.Debug("ObjectMgr","Loading Vendors Table. . .");

    QueryResult *result = WorldDatabase.Query("SELECT entry,item_id,cost,reqitems FROM creature_vendors");
    if( !result )
    {
        barGoLink bar( 1 );

        ++bar;

        sLog.outString("");
        sLog.outError(">> Loaded `creature_vendors`, table is empty!");
        return;
    }

    barGoLink bar( result->GetRowCount() );

    uint32 count = 0;
    do
    {
        ++bar;
        Field* fields = result->Fetch();

        uint32 entry        = fields[0].GetUInt32();
        uint32 item_id      = fields[1].GetUInt32();
        uint32 cost		    = fields[2].GetUInt32();
		string reqitems		= fields[3].GetString();

		const ItemProto * Proto = GetItemProto(item_id);
		if(Proto == NULL)
		{
			Log.Error("ObjMgr","[%u] Vendors : Selling invalid item = %u",entry,item_id);
			stringstream ss;
			ss << "DELETE FROM creature_vendors WHERE item_id="<<item_id<<" AND entry="<<entry;
			m_errors["World"].push_back(ss.str());
			continue;
		}
		if(Proto->sellprice > cost && reqitems.size() < 1)
		{
			//Log.Warning("ObjMgr","[%u] Vendors : SellPrice > cost, hack fix for %u",entry,item_id);
			cost = Proto->sellprice;
		}


        VendorItemData& vList = m_mCacheVendorItemMap[entry];

        vList.AddItem(Proto,cost,reqitems);
        ++count;

    } while (result->NextRow());
    delete result;
	Log.Success("ObjectMgr","Loaded %u Vendors. . .",count);
}
//
//
//
//
PlayerInfo * ObjectMgr::GetPlayerInfo(uint32 characterid,bool all)
{
	PlayerInfo * Info;
	QueryResult * Result = CharacterDatabase.Query("SELECT * FROM characters WHERE character_id = %u",characterid);
	if(Result)
	{
			Info = new PlayerInfo;

			Info->character_id =	Result->Fetch()[0].GetUInt32();
			Info->account =			Result->Fetch()[1].GetUInt32();
			Info->slot =			Result->Fetch()[2].GetUInt32();
			Info->name =			Result->Fetch()[3].GetString();
			Info->model =			Result->Fetch()[4].GetUInt8();
			Info->level =			Result->Fetch()[5].GetUInt32();
			Info->career =			Result->Fetch()[6].GetUInt32();
			Info->careerline =		Result->Fetch()[7].GetUInt32();
			Info->realm =			Result->Fetch()[8].GetUInt32();
			Info->heldleft =		Result->Fetch()[9].GetUInt32();
			Info->race =			Result->Fetch()[10].GetUInt32();
			Info->last_time_played = Result->Fetch()[11].GetUInt32();

			for(uint32 i=0;i<8;++i)
				Info->traits[i] =	Result->Fetch()[12+i].GetUInt32();

			Info->wealth =			Result->Fetch()[20].GetUInt32();
			Info->speed =			Result->Fetch()[21].GetUInt32();
			Info->zone =			Result->Fetch()[22].GetInt16();
			Info->region =			Result->Fetch()[23].GetUInt32();
			
			Info->Position.x  = Result->Fetch()[24].GetFloat();
			Info->Position.y  = Result->Fetch()[25].GetFloat();
			Info->Position.z  = Result->Fetch()[26].GetFloat();
			Info->Position.o  = Result->Fetch()[27].GetFloat();

			Info->xp =				Result->Fetch()[28].GetUInt32();
			Info->restxp =			Result->Fetch()[29].GetUInt32();
			Info->renown =			Result->Fetch()[30].GetUInt32();
			Info->renownrank =		Result->Fetch()[31].GetUInt32();
			Info->rallyPoint =		Result->Fetch()[32].GetUInt32();
			Info->playedTime =		Result->Fetch()[33].GetUInt32();
			Info->health =			Result->Fetch()[34].GetUInt32();
			Info->maxhealth =		Result->Fetch()[35].GetUInt32();
			Info->actionpoints =	Result->Fetch()[36].GetUInt32();
			Info->maxactionpoints = Result->Fetch()[37].GetUInt32();
			Info->sex =				Result->Fetch()[38].GetUInt32();
			Info->m_skills =		Result->Fetch()[39].GetUInt32();


			QueryResult * Items = CharacterDatabase.Query("SELECT entry,slot_id FROM characters_items WHERE character_id=%u",Info->character_id);
			if(Items)
			{
				do{
					Info->m_items[ Items->Fetch()[1].GetUInt32() ] = Items->Fetch()[0].GetUInt32();
				}while(Items->NextRow());

			}
			delete Items;


			if(all)
			{
				QueryResult * Stats = CharacterDatabase.Query("SELECT type_id,type_lv FROM characters_stats WHERE character_id=%u",Info->character_id);
				if(Stats)
				{
					do{
						Info->m_stats[ Stats->Fetch()[0].GetUInt8() ] = Stats->Fetch()[1].GetUInt16();
					}while(Stats->NextRow());

				}
				delete Stats;

				CharacterDatabase.Execute("DELETE FROM characters_friends WHERE friend_id NOT IN (SELECT character_id FROM characters)");
				CharacterDatabase.Execute("DELETE FROM characters_friends WHERE character_id NOT IN (SELECT character_id FROM characters)");

				QueryResult * Friend = CharacterDatabase.Query("SELECT friend_id FROM characters_friends WHERE character_id=%u",Info->character_id);
				if(Friend)
				{
					do{
						Info->friends.insert( Friend->Fetch()[0].GetUInt32() );
					}while(Friend->NextRow());

				}
				delete Friend;

				QueryResult * HasFriend = CharacterDatabase.Query("SELECT character_id FROM characters_friends WHERE friend_id=%u",Info->character_id);
				if(HasFriend)
				{
					do{
						Info->hasfriends.insert( HasFriend->Fetch()[0].GetUInt32() );
					}while(HasFriend->NextRow());

				}
				delete HasFriend;


				QueryResult * ZoneAcces = CharacterDatabase.Query("SELECT zoneid FROM characters_zonesacces WHERE character_id=%u",Info->character_id);
				if(ZoneAcces)
				{
					M_zoneinfo.Acquire();
					do{
						uint16 zone = ZoneAcces->Fetch()[0].GetUInt16();
						if(!zone) continue;
						if(!m_zoneinfo[zone]) continue;

						Info->m_zonesacces.push_back(m_zoneinfo[zone]);

					}while(ZoneAcces->NextRow());
					M_zoneinfo.Release();

				}
				delete ZoneAcces;
			}

			Info->m_player = GetPlayer(Info->character_id);
			Log.Success("ObjectMgr","Character %s Loaded. . .",Info->name.c_str());
	}
	delete Result;
	return Info;
}
list<PlayerInfo*> ObjectMgr::GetPlayerInfoList(uint32 accountid)
{
	list<PlayerInfo*> m_list;
	QueryResult * Result = CharacterDatabase.Query("SELECT character_id FROM characters WHERE account=%u",accountid);
	if(Result)
	{
		Log.Success("ObjectMgr","Loading Account N°%u Character(s). . .",accountid);
		do{
			m_list.push_back( GetPlayerInfo( Result->Fetch()[0].GetUInt32() ) );
		}while( Result->NextRow() );
	}
	delete Result;

	return m_list;
}
GameData::Realm ObjectMgr::GetRealmAccount(uint32 accountid)
{
	QueryResult * Result = CharacterDatabase.Query("SELECT realm FROM characters WHERE account=%u LIMIT 0,1",accountid);
	if(Result)
	{
		return GameData::Realm(Result->Fetch()[0].GetUInt32());
	}
	delete Result;
	return GameData::REALM_NONE;
}
PlayerInfo * ObjectMgr::GetPlayerInfoBySlot(uint32 accountid,uint32 slot)
{
	PlayerInfo * Info;
	QueryResult * Result = CharacterDatabase.Query("SELECT character_id FROM characters WHERE account=%u AND slot=%u",accountid,slot);
	if(Result) Info = GetPlayerInfo( Result->Fetch()[0].GetUInt32() );
	delete Result;
	return Info;

}
uint32 ObjectMgr::GetRenownToLevel(uint8 rank)
{
	uint32 tolevel=0;
	const RenownsLevel * rr = sRenownTable.LookupEntry<RenownsLevel>(rank);
	if( rr ) tolevel = rr->RenownToLevel;
	return tolevel;
}
void ObjectMgr::RemoveCharacerBySlot(uint32 account,uint32 slot)
{
	if(!account) return;

	Log.Debug("ObjectMgr","Deleting Account[%u]-Slot[%u]",account,slot);
	QueryResult * Result = CharacterDatabase.Query("SELECT character_id FROM characters WHERE account=%u AND slot=%u ",account,slot);
	if(Result)
	{
		uint32 id = Result->Fetch()[0].GetUInt32();
		if(!id) return;

		Player * Plr = GetPlayer(id);
		if(Plr) DeletePlayer(Plr);

		CharacterDatabase.Execute("DELETE FROM characters_stats  WHERE character_id=%u",id);
		CharacterDatabase.Execute("DELETE FROM characters_spells WHERE character_id=%u",id);
		CharacterDatabase.Execute("DELETE FROM characters_friends WHERE character_id=%u",id);
		CharacterDatabase.Execute("DELETE FROM characters_friends WHERE friend_id=%u",id);
		CharacterDatabase.Execute("DELETE FROM characters WHERE account=%u AND slot=%u",account,slot);
	}
	delete Result;
}
void ObjectMgr::AddAccount(Account * Acct)
{
	M_accountname.Acquire();
	m_accountname[Acct->username] = Acct;
	M_accountname.Release();

	Log.Success("ObjectMgr","Registering Account : [%u]-[%s]. . .",Acct->AccountId,Acct->username.c_str());
}
uint32 ObjectMgr::GetAccountID(string name)
{
	uint32 id=0;
	M_accountname.Acquire();
	if(m_accountname[name])
		id=m_accountname[name]->AccountId;
	M_accountname.Release();

	return id;
}
uint8 ObjectMgr::GetAccountPermission(string name)
{
	uint8 level=0;
	M_accountname.Acquire();
	if(m_accountname[name])
		level=m_accountname[name]->Permission;
	M_accountname.Release();

	return level;
}
Characterinfo * ObjectMgr::GetCharacterInfo(uint32 career)
{
	M_characterinfo.Acquire();
	Characterinfo * Info = m_characterinfo[career];
	M_characterinfo.Release();

	return Info;
}
//
//
//
//
Player * ObjectMgr::CreatePlayer(uint32 character_id,ClientCom * Com)
{
	if(!character_id) return NULL;

	Player * Already = GetPlayerByAccount(Com->m_accountID);
	if(Already)
		DeletePlayer(Already);

	Player * Plr = new Player(Com);
	if( Plr->Load(character_id) )
	{
		M_players.Acquire();
		m_players[character_id] = Plr;
		M_players.Release();

		return Plr;
	}
	return NULL;
}
Player * ObjectMgr::GetPlayer(uint32 character_id)
{
		Player * Plr=NULL;
		M_players.Acquire();
		Plr = m_players[character_id];
		M_players.Release();

	return Plr;
}
Player * ObjectMgr::GetPlayerByAccount(uint32 acctid)
{
	Player * Plr=NULL;

	M_players.Acquire();
	for(map<uint32,Player*>::iterator itr = m_players.begin();itr != m_players.end();++itr)
	{
		if( itr->second != NULL)
			if(itr->second->GetCom() && itr->second->GetCom()->m_accountID == acctid)
			{
				Plr = itr->second;
				break;
			}
	}
	M_players.Release();

	return Plr;
}
Player * ObjectMgr::GetPlayerByName(string name)
{
	Player * Plr=NULL;
	name=Utils::strToLowerCase(name);

	M_players.Acquire();
	for(map<uint32,Player*>::iterator itr = m_players.begin();itr != m_players.end();++itr)
	{
		if( itr->second != NULL)
			if(Utils::strToLowerCase(itr->second->GetName()).compare(name) == 0)
			{
				Plr = itr->second;
				break;
			}
	}
	M_players.Release();

	return Plr;
}
Player * ObjectMgr::GetPlayerByOid(uint16 oid)
{
	Player * Plr=NULL;

	M_players.Acquire();
	for(map<uint32,Player*>::iterator itr = m_players.begin();itr != m_players.end();++itr)
	{
		if( itr->second != NULL)
			if(itr->second->GetOid() == oid)
			{
				Plr = itr->second;
				break;
			}
	}
	M_players.Release();
	return Plr;
}
void ObjectMgr::PlayerLeave(Player * Plr)
{
	if( Plr == NULL ) return;

	Plr->sendLeave();
}
void ObjectMgr::DeletePlayer(Player * Plr)
{
	if(Plr == NULL) return;

	Log.Debug("ObjectMgr"," Deleting player %s",Plr->GetName().c_str());
	CharacterDatabase.Execute("UPDATE characters SET online=0 WHERE character_id=%u",Plr->GetPlayerID());

	M_players.Acquire();
	m_players.erase( Plr->GetPlayerID() );
	M_players.Release();

	if(Plr->IsInWorld())
		Plr->RemoveFromWorld();

	if(Plr->GetPublicQuest())
		Plr->GetPublicQuest()->RemovePlayer(Plr);

	if(Plr->m_interract)
		if(Plr->m_interract->GetLoot())
			Plr->m_interract->GetLoot()->RemovePlayerToLoot(Plr->GetPlayerID());

	Plr->Save();

	if(Plr->GetGroup() != NULL)
		Plr->SetGroup(NULL);

	if(Plr->Trade)
		Plr->Trade->Destroy();


	Plr->Social_SetOffline();

	if(!Plr->m_leaved)
	{
		Plr->m_decostage = 0;
		Plr->sendLeave();
	}

	if(Plr->GetCom())	
		Plr->GetCom()->m_Plr = NULL;


	sEventMgr.RemoveEvents(Plr);
	delete Plr;
}
void ObjectMgr::DistributePacketToAll(Buffer * b)
{
	M_players.Acquire();
	for(map<uint32,Player*>::iterator itr = m_players.begin();itr != m_players.end();++itr)
	{
		Log.Debug("ObjectMgr","DistributePacketToAll()");

		if( itr->second )
			itr->second->dispachPacket(b);
	}
	M_players.Release();
}
void ObjectMgr::SendChatMessage(Player * Sender,string text,ChatLogFilters type)
{
	if(!Sender) return;

	M_players.Acquire();
	for(map<uint32,Player*>::iterator itr = m_players.begin();itr != m_players.end();++itr)
	{
		if( itr->second)
			if(Sender)
				itr->second->sendStringMessage(Sender->GetOid(),Sender->GetName(),text,type);
			else
				itr->second->sendStringMessage(0,"Server",text,type);
	}
	M_players.Release();
}
void ObjectMgr::RemoveAllPlayer()
{
	uint32 counts=0;

	list<Player*> players;

	M_players.Acquire();
	for(map<uint32,Player*>::iterator itr =  m_players.begin(); itr!=m_players.end() ;++itr)
	{
		if(itr->second && itr->second->GetCom())
		{
			players.push_back(itr->second);
			counts++;
		}
	}
	M_players.Release();

	for(list<Player*>::iterator itr =  players.begin(); itr!=players.end() ;++itr)
	{
		if((*itr))
			DeletePlayer((*itr));
	}


	Log.Debug("ObjectMgr","Removed %u Players online.",counts);
}
void ObjectMgr::SaveAllPlayers()
{
	map<uint32,Player*>::iterator itr;
	M_players.Acquire();
	for(itr=m_players.begin();itr!=m_players.end();++itr)
	{
		if(!itr->second) continue;

		itr->second->Save();
	}
	M_players.Release();
}
uint32 ObjectMgr::GenerateHealth(Unit * unit)
{
	if(!unit) return 0;

	uint8 level = unit->GetLevel();
	uint32 health = 1;

	health = (52*(level+1))+(level*2.5);

	if(unit->m_Ranklevel>0) health += (unit->m_Ranklevel*(5.85*level*52));

	return health;
}
void ObjectMgr::GenerateLoot(Unit * unit,Player * Plr)
{
	if(unit == NULL || Plr == NULL)
		return;

	if(unit->IsCreature())
	{
		Creature * crea = unit->GetCreature();

		Loot * loot = new Loot(unit);
		loot->GenerateLoot();

		loot->m_looter = Plr;
		loot->m_group = Plr->GetGroup();

		if( loot->IsValid() )
		{
			crea->SetLoot(loot);
			crea->SetLootable(true,Plr);
		} else delete loot;
	}
}

void ObjectMgr::SendPlayerOnline(Player * Plr,string name,string guildname,uint8 career,uint16 zoneid,uint8 minlvl,uint8 maxlvl)
{
	if(Plr == NULL) return;

	vector<Buffer*> m_buffers;
	uint16 responseSize=4;

	Buffer *b = new Buffer(0);
	uint8 count=0;

	map<uint32,Player*>::iterator itr;

	M_players.Acquire();
	for(itr=m_players.begin();itr!=m_players.end() && count < 30;++itr)
	{
		if(!itr->second) continue;
		if(itr->second == Plr) continue;

		if(name.size() && Utils::strToLowerCase(itr->second->GetName()).find( name ) == string::npos)
			continue;

		if(career && itr->second->GetCareer() != career)
			continue;

		if(zoneid != 255 && itr->second->GetZoneId() != zoneid)
			continue;

		if(itr->second->GetLevel() < minlvl || itr->second->GetLevel() > maxlvl)
			continue;

		sChat.BuildSocialInfo(b,itr->second,true);
		count++;
	}
	M_players.Release();

	Buffer * d= Plr->Allocate(b->size()+5+3);
	d->write<uint16>(b->size()+5);
	d->write<uint8>(F_SOCIAL_NETWORK);

	d->write<uint16>(0);
	d->write<uint8>(4);
	d->write<uint8>(count);
	d->write(b);
	d->write<uint8>(01);
	d->Trace();
	Plr->sendPacket(d);

	delete b;
}
void ObjectMgr::CalculXpToGive(Unit * attacker,Unit * victim)
{
	if(attacker==NULL || victim==NULL)
		return;

	if(victim->GetAIInterface() && victim->GetAIInterface()->m_firstattacker)
		attacker = victim->GetAIInterface()->m_firstattacker;

	if(attacker==NULL)
		return;

	if(!attacker->IsPlayer())
		return;

	bool pvp = false;

	if(attacker->IsPlayer() && victim->IsPlayer())
		pvp = true;

	if(pvp) return; // comment for xp Pvp

	float xp = 0;
	
	float atlevel = attacker->GetLevel();
	float vilevel = victim->GetLevel();

	// 4vs1
	xp+=vilevel*60;

	if(atlevel > vilevel)
		xp/=(atlevel-vilevel);

	xp+=(victim->m_Ranklevel*20);

	xp*=xp_rate;

	Log.Debug("ObjMgr","CalculXPToGive %f",xp);
	if(attacker->IsPlayer())
	{
		Player * Plr = attacker->GetPlayer();
		if(Plr->GetGroup())
		{
			for(GroupMembersSet::iterator itr = Plr->GetGroup()->GetGroupBegin();itr != Plr->GetGroup()->GetGroupEnd();++itr)
				// Le joueur ne recoit de l'experience que si il est dans la même zone et à distance de reception d'xp
				if((*itr)->GetZoneId() == Plr->GetZoneId())
					if((*itr)->GetDistance(Plr) < (float)MAX_VISIBILITY_RANGE) // 300.0f max_xp_distance à ajouter en tant que config ?
						(*itr)->GiveXp(xp / Plr->GetGroup()->GetGroupMembers());
		}
		else Plr->GiveXp(xp);
	}
}
GameData::InteractType ObjectMgr::GetInterractType(uint32 title)
{
	if(!title)
		return GameData::INTERACTTYPE_IDLE_CHAT;

	GameData::InteractType type;

	switch(title)
	{
		case 1:
		case 2:
		case 3:
		case 40:
			{
				type=GameData::INTERACTTYPE_TRAINER;
			}break;

		case 18:
		case 131:
			{
				type=GameData::INTERACTTYPE_FLIGHT_MASTER;
			}break;

		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 31:
		case 34:
		case 115:
		case 116:
		case 117:
		case 118:
		case 119:
		case 120:
		case 121:
		case 122:
		case 125:
		case 126:
		case 127:
		case 128:
		case 129:
		case 130:
		case 135:
			{
				type=GameData::INTERACTTYPE_DYEMERCHANT;
			}break;
		default:
		{
			type=GameData::INTERACTTYPE_BINDER;
		}break;
	};

	return type;
	/*
1	Entraîneur
2	Entraîneur de carrière
3	Entraînement renommée
4	Apothicaire
5	Boucher
6	Cultivateur
7	Récupérateur
8	Pilleur
9	Sorcier occulte
10	Marchand
11	Marchand d’armures
12	Marchand d’armes
13	Marchand de camp
14	Marchand d’armes de siège
15	Marchand d’artisanat
16	Marchand d’équipement de renommée
17	Maître du ralliement
18	Maître des vols
19	Greffier de guildes
20	Guérisseur
21	Postier
22	Banquier
23	Commissaire-priseur
24	Garde
25	Garde de guerre
26	Garde du corps
27	Mercenaire
28	Sergent
29	Seigneur de la forteresse
30	Seigneur de la citadelle
31	Vendeur de montures
32	Collectionneur
33	Maître des écuries
34	Intendant
35	Héraut
36	Greffier des noms
37	Gardien du coffre
38	Bibliothécaire
39	Agent immobilier
40	Entraîneur renommée apprenti
41	Maître de l’évènement live
42	Garde du Nordland XI
43	Garde de la Festenplatz
44	Garde du Sanctuaire de la moisson
45	Garde du Lagon perdu
46	Garde de la Pierre Grise
47	Garde du Monastère
48	Garde de la Crypte
49	Garde de Kinshel
50	Garde de Verentane
51	Garde de Feiten
52	Garde de la Taverne
53	Garde d’Hallenfurt
54	Garde de la Carrière
55	Garde du Guet du Reik
56	Garde du Marteau runique
57	Garde de Schwenderhalle
58	Garde de l’Armurerie de Kurlov
59	Garde de la Place des martyrs 
60	Garde du Cimetière
61	Garde d’Ortel von Zaris
62	Garde du Sanctuaire du temps
63	Garde de l’Elu éternel
64	Garde de Moultépines
65	Garde de Thaugamond
66	Garde du Poste de guet
67	Garde de l’Avant-poste
68	Garde de la Mine de pierre
69	Garde de la Batterie d’artillerie
70	Garde du Phare
71	Garde du Cuirassé
72	Garde d’Alcadizaar
73	Garde de l’Armurerie gobeline
74	Garde de Karagaz
75	Garde du Champ de tir d’artillerie
76	Garde de la Chute de Furrig
77	Garde de la Brasserie
78	Garde des Chutes d’Eau-sévère
79	Garde du Foyer-glacé
80	Garde de la Jonction du Gromril
81	Garde de Dolgrund
82	Garde des Armes du Destin
83	Garde de Gromril Kruk
84	Garde de la Crête
85	Garde de Karak Palik
86	Garde de Bonnet-de-Fou
87	Garde des Enclos des bêtes
88	Garde du Pic Pourrissant
89	Garde du Moulin des lance-rocs
90	Garde de Lorendyth
91	Garde de l’Autel de Khaine
92	Garde de l’Eclat du Chagrin
93	Garde de Sorgueflamme
94	Garde du Camp de Siège
95	Garde de la Cime de l’ombre 
96	Garde des Ecuries du Pilleur
97	Garde de l’Aiguille d’Ellyrion
98	Garde des Koupeurs d’bois
99	Garde du Débarcadère de la vierge
100	Garde de la Cime de Teclis
101	Garde de Sari’ Daroir
102	Garde de Vent glacial
103	Garde de Bel-Korhadris
104	Garde du Sanctuaire
105	Garde du Camp de siège 
106	Garde du Souvenir de Milaith
107	Garde de Deuil de feu
108	Garde de Pelgorath 
109	Garde de la Cime de Garde de feu
110	Garde du Baroud de Senlathain
111	Garde du Val de Sarathanan
112	Garde du Sanctuaire du Conquérant
113	Garde de la Caserne
114	Archiviste de Tactique de livre
115	Archiviste de trophées de livres
116	Marchand d’équipement basique de renommée
117	Marchand d’équipement vétéran de renommée
118	Marchand d’équipement de renommée avancé
119	Marchand d’équipement de renommée d’élite
120	Marchand d’amélioration
121	Marchand de bois
122	Marchand d’étendards
123	Ritualiste guérisseur
124	Adepte draineur de vie
125	Quartier-maître de siège
126	Quartier-maître recrue
127	Quartier-maître éclaireur
128	Quartier-maître soldat
129	Quartier-maître officier
130	Quartier-maître royal
131	Quartier-maître de l’expédition
132	Forgeron
133	Archiviste d’accessoires de livres
134	Archiviste de symboles de livres
135	Marchand de talismans
136	Les armées de l’Ordre
137	Les armées de la Destruction

	*/
}