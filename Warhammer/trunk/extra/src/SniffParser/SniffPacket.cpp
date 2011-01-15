#include "stdafx.h"

static int lastzone=0;

struct SpawnInfo
{
	int zoneid;
	int entry;
	int wx;
	int wy;
};

tr1::unordered_map<int, list<SpawnInfo*> > m_spawns;

//On vérifi si ya le même spawn sur +- 180 coord;
bool HasProxSpawn(SpawnInfo* Info)
{
	if(!Info) return true;

	list<SpawnInfo*>::iterator itr = m_spawns[Info->zoneid].begin();

	for(;itr != m_spawns[Info->zoneid].end();++itr)
	{
		if((*itr)->entry == Info->entry)
		{
			int diffx =  max(Info->wx,(*itr)->wx) - min(Info->wx,(*itr)->wx);
			int diffy = max(Info->wy,(*itr)->wy) - min(Info->wy,(*itr)->wy);

			if(diffy <= 180 && diffx <= 180)
				return true;
		}
	}

	return false;

};

Packet::Packet(string b)
{
	m_opcode=0;
	m_size=0;
	m_cursor=0;
	data = b;

	if(b.size())
		DecodeHeader();
}
void Packet::DecodeHeader()
{
	// [<S_PONG>(0x81)<20>]001930F34B48B263000000000000015D00000000
	
	// suppression des espace

	int z = data.find(' ');
	while( z != string::npos )
	{
		data.replace(z,1,"");
		z = data.find(' ');
	}

	string header;
	int i = data.find_first_of(']');
	header = data.substr(0,i+1);
	data.erase(0,i+1);

	int startname = header.find_first_of('<');
	int endname = header.find_first_of('>');
	
	if( startname != string::npos && endname != string::npos )
	{
		opcodeName = header.substr(startname+1,endname-2);
		header.erase(0,endname+1);
	}

	int startopcode = header.find_first_of('(');
	int endopcode = header.find_first_of(')');

	if( startopcode != string::npos && endopcode != string::npos )
	{
		string op = header.substr(startopcode+1,endopcode-1);
		op.erase(0,2);
		m_opcode = strtoul(op.c_str(),0,16);
		header.erase(0,endopcode+1);
	}

	int startsize= header.find_first_of('<');
	int endsize = header.find_first_of('>');

	if( startsize != string::npos && endsize != string::npos )
	{
		m_size = atoi(header.substr(startsize+1,endsize-1).c_str());
		header.erase(0,endname+1);
	}
}
bool Packet::DecodePacket()
{
	if( m_opcode == 114 ) // F_CREATE_MONSTER
		return _decodeCreature();
	else if( m_opcode == 233) // F_INTERACT_RESPONSE
		return _decodeInterract();
	else if( m_opcode == 113) // STATIC
		return _decodeStatic();

	return false;
}
bool Packet::_decodeInterract()
{
	SkipValue(1);
	m_oid = GetInt(2);
	SkipValue(2);
	int type = GetInt(2);
	int textsize = GetInt(2);

	string text = GetValue( textsize );
	if( text.size() < 2 )
		return false;

		//cout << "== NPC TEXT ==== ";

		stringstream nm;
		for(int i=0;i<text.size();i+=2)
		{
			char c = hexToAscii(text[i],text[i+1]);

			if(c == '\'') nm << "\\'";
			else nm << c;
		}

	m_text = nm.str();
	//cout << nm.str() << endl ;

	return true;
}
void Packet::_createInterract(int entry)
{
	if(!entry)
		return;

	m_entry = entry;

	if( m_text.size() < 1)
		return;

	QueryResult * Res = WorldDatabase.Query("SELECT text1,text2,text3 FROM creature_text WHERE entry='%u'",m_entry);

	if( Res )
	{
		string t1 = Res->Fetch()[0].GetString();
		string t2 = Res->Fetch()[1].GetString();
		string t3 = Res->Fetch()[2].GetString();

		delete Res;

		if(m_text.find( t1 ) != string::npos)
			return;
		if( m_text.find( t2 ) != string::npos)
			return;
		if( m_text.find( t3 ) != string::npos)
			return;

		if( t1.size() < 1 )
		{
			WorldDatabase.WaitExecute("UPDATE creature_text SET text1='%s' WHERE entry='%u'",m_text.c_str(),m_entry);
			return;
		}
		else if( t2.size() < 1 )
		{
			WorldDatabase.WaitExecute("UPDATE creature_text SET text2='%s' WHERE entry='%u'",m_text.c_str(),m_entry);
			return;
		}
		else if( t3.size() < 1 )
		{
			WorldDatabase.WaitExecute("UPDATE creature_text SET text3='%s' WHERE entry='%u'",m_text.c_str(),m_entry);
			return;
		}
	}
	else
	{
		WorldDatabase.WaitExecute("INSERT INTO creature_text (entry,text1,text2,text3) VALUE ('%u','%s','','')",m_entry,m_text.c_str());
	}
}
 bool Packet::_decodeStatic()
 {
	m_oid = GetInt(2);

	SkipValue(2);

	int WorldO = GetInt(2);
	int WorldZ = GetInt(2);
	int WorldX = GetInt(4);
	int WorldY = GetInt(4);

	int DisplayId = GetInt(2);
	int unk1 = GetInt(2);
	int unk2 = GetInt(2);
	int unk3 = GetInt(2);
	SkipValue(1);
	int unk4 = GetInt(2);
	SkipValue(5);
	int unk5 = GetInt(2);
	int unk6 = GetInt(2);
	SkipValue(4);
	int NameSize = GetInt(1);

	string name = GetValue( NameSize );
	if( name.size() < 2 )
		return false;

		//cout << "== GAMEOBJECT ==== ";

		stringstream nm;
		for(int i=0;i<name.size();i+=2)
		{
			char c = hexToAscii(name[i],name[i+1]);
			if( c=='^') break;

			if(c == '\'') nm << "\\'";
			else nm << c;
		}

		//cout << nm.str().c_str();

		if( nm.str().size() < 3)
			return false;

		m_name = nm.str();

		//cout << "========" << m_size;
		//cout << endl;

		m_entry=1;

		QueryResult * Existe = WorldDatabase.Query("SELECT entry FROM gameobject_proto WHERE name='%s'",m_name.c_str());
		if( (Existe && Existe->Fetch()[0].GetUInt32() == 0) || !Existe)
		{
			QueryResult * Entry = WorldDatabase.Query("SELECT MAX(entry) FROM gameobject_proto");
			if(Entry) m_entry = Entry->Fetch()[0].GetUInt32()+1;

			//cout << "Nouvel entry = " << m_entry << endl;
			stringstream sql;

			sql << "INSERT INTO gameobject_proto (";
			sql << "`entry`,`name`,`displayid`,`Unk1`,`Unk2`,`Unk3`,`Unk4`,`Unk5`,`Unk6`) VALUES('";

			sql << m_entry << "','";
			sql << m_name.c_str() << "','";
			sql << DisplayId << "','";
			sql << unk1 << "','";
			sql << unk2 << "','";
			sql << unk3 << "','";
			sql << unk4 << "','";
			sql << unk5 << "','";
			sql << unk6 << "')";

			delete Existe;
			bool ok = WorldDatabase.WaitExecute(sql.str().c_str());
			if(!ok) return false;
		}
		else
			m_entry = Existe->Fetch()[0].GetUInt32();

		int maxguid = 1;
		QueryResult * Max = WorldDatabase.Query("SELECT MAX(guid) FROM gameobject_spawns");
		if(Max) maxguid = Max->Fetch()[0].GetUInt32()+1;
		delete Max;
		stringstream dd;

		dd << "INSERT INTO gameobject_spawns (";

		dd << "`guid`,";
		dd << "`entry`,";
		dd << "`zone`,";
		dd << "`WorldX`,";
		dd << "`WorldY`,";
		dd << "`WorldZ`,";
		dd << "`WorldO`,";
		dd << "`displayid`,";
		dd << "`Unk1`,";
		dd << "`Unk2`,";
		dd << "`Unk3`,";
		dd << "`Unk4`,";
		dd << "`Unk5`,";
		dd << "`Unk6`)";
		dd << " VALUES ('";

		dd << maxguid << "','";
		dd << m_entry << "','";
		dd << lastzone << "','";
		dd << WorldX << "','";
		dd << WorldY << "','";
		dd << WorldZ << "','";
		dd << WorldO << "','";
		dd << DisplayId << "','";
		dd << unk1 << "','";
		dd << unk2 << "','";
		dd << unk3 << "','";
		dd << unk4 << "','";
		dd << unk5 << "','";
		dd << unk6 << "')";

		bool ok = WorldDatabase.WaitExecute(dd.str().c_str());
		if(!ok) return false;

	return true;
 }
bool Packet::_decodeCreature()
{

		m_oid = GetInt(2);

		SkipValue(2);

		int WorldO = GetInt(2);
		int WorldZ = GetInt(2);
		int WorldX = GetInt(4);
		int WorldY = GetInt(4);

		if( !WorldX || !WorldY || !WorldZ)
			return false;

		int offX = WorldX >> 12;
		int offY = WorldY >> 12;

		if(!offX || offX>=999)
			return false;

		if(!offY || offY>=999)
			return false;

		SkipValue(2);

		int DisplayId = GetInt(2);
		int Scale = GetInt(1);
		int Level = GetInt(1);
		int Faction = GetInt(1);

		if( !Scale || !DisplayId || !Level)
			return false;

		SkipValue(4);

		int Emote = GetInt(1); // 01 / 02 / 04

		SkipValue(2);

		int Unk1 = GetInt(1); // 01 - 04

		SkipValue(1);

		// 03 E8
		int Unk2 = GetInt(2);

		int Unk3 = GetInt(2);
		int Unk4 = GetInt(2);

		// 34 8E
		int Unk5 = GetInt(2);

		// 00 02 ...
		int Unk6 = GetInt(2);

		// 00 00
		int Title = GetInt(2);
		
		int size = 0;
		int BytesCount = GetInt(1); // 01

		if( BytesCount > 5) 
		{
			cout << "Erreur , Trop de bytes" << endl;
			return false;
		}

		stringstream Bytes;
		for(int i=0;i<BytesCount;i++)
		{
			Bytes << GetInt(1); // 19
			Bytes << ";";
		}

		SkipValue(1); // 00

		size = GetSize("00");
		string name = GetValue( size );
		if( name.size() < 2 )
		{
			cout << "Erreur packet nom="<<name<<endl;
			return false;
		}

		name.erase(name.size()-2,name.size());

		//cout << "== NPC ==== ";

		stringstream nm;
		for(int i=0;i<name.size();i+=2)
		{
			char c = hexToAscii(name[i],name[i+1]);
			if( c=='^') break;
			if(c == '\'')
				nm << "\\'";
			else
				nm << c;
		}

		//cout << nm.str().c_str();

		if( nm.str().size() < 3)
			return false;

		m_name = nm.str();

		//cout << "========" << m_size;
		//cout << endl;

		size = GetSize("010A");
		SkipValue(size);
		SkipValue(1);
		SkipValue(2);

		int Icone = GetInt(1);
		int Unk = GetInt(1);

		SkipValue(1);
		string Flags = GetValue(8);
		//cout << "Flags = " << Flags.c_str() << ">" << endl;
		int Vie = GetInt(1);

		SkipValue(1);

		int ZoneId = GetInt(1);

		lastzone = ZoneId;
		//cout << "<ZoneId="<< ZoneId <<"><Vie="<<Vie<<"\%>";

		bool item=false;
		for(int i=data.size()-1;i>data.size()-6;i--)
		{
			if(data[i] != '0')
				item=true;
		}

		m_entry =1;

		// Vérifier si le proto existe
		QueryResult * Existe = WorldDatabase.Query("SELECT entry,displayid1,displayid2 FROM creature_proto WHERE name='%s'",m_name.c_str());
		if( !Existe )
		{
			return false;
			QueryResult * Entry = WorldDatabase.Query("SELECT MAX(entry) FROM creature_proto");
			if(Entry) 
			{
				m_entry = Entry->Fetch()[0].GetUInt32();
				m_entry++;
			}

			stringstream ss;
			ss << "INSERT INTO creature_proto (";
			ss << "`entry`,";
			ss << "`name`,";
			ss << "`displayid1`,";
			ss << "`displayid2`,";
			ss << "`minscale`,";
			ss << "`maxscale`,";
			ss << "`minlevel`,";
			ss << "`maxlevel`,";
			ss << "`faction`,";
			ss << "`bytes`,";
			ss << "`ico`,";
			ss << "`Unk`,";
			ss << "`emote`,";
			ss << "`Unk1`,";
			ss << "`Unk2`,";
			ss << "`Unk3`,";
			ss << "`Unk4`,";
			ss << "`Unk5`,";
			ss << "`Unk6`,";
			ss << "`title`,";
			ss << "`Flags`) ";
			ss << "VALUES ('";

			ss << m_entry << "','";
			ss << m_name.c_str() << "','";
			ss << DisplayId << "','";
			ss << DisplayId << "','";
			ss << Scale << "','";
			ss << Scale << "','";
			ss << Level << "','";
			ss << Level << "','";
			ss << Faction << "','";
			ss << Bytes.str().c_str() << "','";
			ss << Icone << "','";
			ss << Unk << "','";
			ss << Emote << "','";
			ss << Unk1 << "','";
			ss << Unk2 << "','";
			ss << Unk3 << "','";
			ss << Unk4 << "','";
			ss << Unk5 << "','";
			ss << Unk6 << "','";
			ss << Title << "','";
			ss << Flags.c_str() << "');";

			delete Existe;
			bool ok = WorldDatabase.WaitExecute(ss.str().c_str());
			if(!ok) return false;
		}
		else // Existe , on met a jours les info variables
		{
			m_entry = Existe->Fetch()[0].GetUInt32();

			if(Existe->Fetch()[1].GetUInt32() && Existe->Fetch()[2].GetUInt32())
			{
				//cout << "EXISTE §§!!!!!!!!!§§" << endl;
				if( DisplayId != 0)
				{
					WorldDatabase.WaitExecute("UPDATE creature_proto SET displayid1 = %i WHERE displayid1 != %i AND entry='%u'",DisplayId,DisplayId,m_entry);
					WorldDatabase.WaitExecute("UPDATE creature_proto SET displayid2 = %i WHERE displayid2 != %i AND entry='%u'",DisplayId,DisplayId,m_entry);
				}
				if( Scale != 0)
				{
					WorldDatabase.WaitExecute("UPDATE creature_proto SET minscale = %i WHERE minscale > %i AND entry='%u'",Scale,Scale,m_entry);
					WorldDatabase.WaitExecute("UPDATE creature_proto SET maxscale = %i WHERE maxscale < %i AND entry='%u'",Scale,Scale,m_entry);
				}
				if( Level != 0)
				{
					WorldDatabase.WaitExecute("UPDATE creature_proto SET minlevel = %i WHERE minlevel > %i AND entry='%u'",Level,Level,m_entry);
					WorldDatabase.WaitExecute("UPDATE creature_proto SET maxlevel = %i WHERE maxlevel < %i AND entry='%u'",Level,Level,m_entry);
				}
			}
			else // existe mai sans aucune info
			{

				stringstream ss;
				ss << "UPDATE creature_proto SET ";

				ss << "`displayid1`='";
				ss << DisplayId << "',";

				ss << "`displayid2`='";
				ss << DisplayId << "',";

				ss << "`minscale`='";
				ss << Scale << "',";

				ss << "`maxscale`='";
				ss << Scale << "',";

				ss << "`minlevel`='";
				ss << Level << "',";

				ss << "`maxlevel`='";
				ss << Level << "',";

				ss << "`faction`='";
				ss << Faction << "',";

				ss << "`bytes`='";
				ss << Bytes.str().c_str() << "',";

				ss << "`ico`='";
				ss << Icone << "',";

				ss << "`Unk`='";
				ss << Unk << "',";

				ss << "`emote`='";
				ss << Emote << "',";

				ss << "`Unk1`='";
				ss << Unk1 << "',";

				ss << "`Unk2`='";
				ss << Unk2 << "',";

				ss << "`Unk3`='";
				ss << Unk3 << "',";

				ss << "`Unk4`='";
				ss << Unk4 << "',";

				ss << "`Unk5`='";
				ss << Unk5 << "',";

				ss << "`Unk6`='";
				ss << Unk6 << "',";

				ss << "`title`='";
				ss << Title << "',";

				ss << "`Flags`='";
				ss << Flags.c_str() << "' ";

				ss << "WHERE `entry`='" << m_entry << "';";

				bool ok = WorldDatabase.WaitExecute(ss.str().c_str());

			}
			delete Existe;
		}	

		SpawnInfo * Info = new SpawnInfo;
		Info->zoneid = ZoneId;
		Info->entry = m_entry;
		Info->wx = WorldX;
		Info->wy = WorldY;
		int maxguid = 1;


		if( !HasProxSpawn(Info) )
		{

			m_spawns[ZoneId].push_back(Info);

			QueryResult * Max = WorldDatabase.Query("SELECT MAX(guid) FROM creature_spawns");
			if(Max)
			{
				maxguid = Max->Fetch()[0].GetUInt32();
				maxguid++;
			}
			delete Max;
			stringstream dd;

			dd << "INSERT INTO creature_spawns (";

			dd << "`guid`,";
			dd << "`entry`,";
			dd << "`zone`,";
			dd << "`WorldX`,";
			dd << "`WorldY`,";
			dd << "`WorldZ`,";
			dd << "`WorldO`,";
			dd << "`bytes`,";
			dd << "`ico`,";
			dd << "`Unk`,";
			dd << "`emote`,";
			dd << "`Unk1`,";
			dd << "`Unk2`,";
			dd << "`Unk3`,";
			dd << "`Unk4`,";
			dd << "`Unk5`,";
			dd << "`Unk6`,";
			dd << "`title`,";
			dd << "`Flags`) ";
			dd << "VALUES ('";

			dd << maxguid << "','";
			dd << m_entry << "','";
			dd << ZoneId << "','";
			dd << WorldX << "','";
			dd << WorldY << "','";
			dd << WorldZ << "','";
			dd << WorldO << "','";
			dd << Bytes.str().c_str() << "','";
			dd << Icone << "','";
			dd << Unk << "','";
			dd << Emote << "','";
			dd << Unk1 << "','";
			dd << Unk2 << "','";
			dd << Unk3 << "','";
			dd << Unk4 << "','";
			dd << Unk5 << "','";
			dd << Unk6 << "','";
			dd << Title << "','";
			dd << Flags.c_str() << "');";

			bool ok = WorldDatabase.WaitExecute(dd.str().c_str());
			if(!ok) return false;
		}
		else return false;

		if(!item) return true;

		SkipValue(8);

		size=0;

		for(int i=10;i<30 && !size;i++)
		{
			for(int z=0;z<2 && !size;z++)
			{
				char srh[6];
				_snprintf(srh, 6, "%02X%04X",z,i);
				stringstream ss;
				for(int d=0;d<6;d++)
					ss << srh[d];

				size = GetSize(ss.str());
			}
		}

		if(!size)		
			return true;

		size-=4;

		SkipValue(size);

		int ItemCount = GetInt(1);

		if(!ItemCount) 
			return true;

		//cout << "<Items " << ItemCount << ">";

		for(int i=0;i<ItemCount;i++)
		{
			int effectcount = GetInt(1);

			int slot = GetInt(2);
			int modelid = GetInt(2);

			if(!slot || !modelid)
				continue;

			if(slot==10 || slot==11)
			{
				QueryResult * Quer = WorldDatabase.Query("SELECT type FROM items WHERE modelid=%u LIMIT 0,1",modelid);
				if(Quer)
				{
					uint32 type = Quer->Fetch()[0].GetUInt32();
					
					if(type == 15 || type == 9 || type ==7 )
						WorldDatabase.WaitExecute("UPDATE creature_proto SET rangedattack='1' WHERE entry=%u",m_entry);
				}
				delete Quer;
			}

			int m_effect = 0;

			for(int z=0;z<effectcount;z++)
				m_effect = GetInt(4);

			WorldDatabase.WaitExecute("INSERT INTO creature_items (entry,slot_item,model_item,effect) VALUES ('%u','%u','%u','%u')",m_entry,slot,modelid,m_effect);
		}

		return true;
}
