#pragma once

SERVER_DECL extern Database* Database_World;

#define WorldDatabase (*Database_World)

struct Packet
{
	public:
	Packet(string b);
	
	int GetOpcode() { return m_opcode; };
	int GetSize() { return m_size; };
	string GetData() { return data; };
	string GetOpcodeName() { return opcodeName; };


	void DecodeHeader();
	bool DecodePacket();

	int m_opcode;
	int m_size;
	int m_oid;
	int m_entry;

	string data;
	string opcodeName;

	string m_name; // F_CREATE_MONSTER - STATIC
	string m_text; // F_INTERRACT_REPONSE

	bool _decodeCreature();
	bool _decodeInterract();
	bool _decodeStatic();

	void _createInterract(int entry);

	string GetValue(int size)
	{
		size*=2;

		if( m_cursor+size > data.size())
			return "0";

		string value = data.substr(m_cursor,size);
		m_cursor+=size;
		return value;
	};

	int GetInt(int size)
	{
		return strtoul(GetValue(size).c_str(),0,16) ;
	};

	void SkipValue(int size)
	{
		size*=2;
		m_cursor+=size;
	}

	int GetSize(string chaine)
	{
		if( chaine.size()+m_cursor > data.size())
			return 0;

		string tofind = data.substr(m_cursor,data.length());

		int i = tofind.find(chaine);
		if( i != string::npos )
		{
			return (i+chaine.length())/2;
		}

		return 0;
	}

	char hexToAscii(char first, char second)
	{
		char hex[5], *stop;
		hex[0] = '0';
		hex[1] = 'x';
		hex[2] = first;
		hex[3] = second;
		hex[4] = 0;
		return strtol(hex, &stop, 16);
	}


	// Table , colone , valeur
	map<string, map<string,string> > m_req;
	
	void AddSqlInfo(string table,string colone,string value)
	{
		m_req[table][colone] = value;
	}
	
	int m_cursor;
};