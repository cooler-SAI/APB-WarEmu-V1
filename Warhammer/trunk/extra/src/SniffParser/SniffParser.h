#pragma once


struct PacketSniff
{
	string Name;
	int size;
	string content;

	string originalcontent;
};

// Testé les nom Unknow
// Testé les packet size a 0
class SniffParser
{
public:
	StructureLoader * m_Loader;
	PacketSniff * ActuPacket;
	int i;
	map<string,int> m_blockcounter;
	list<PacketSniff*> m_packets;
	void LoadSniff(StructureLoader * Loader);

	void ParseLigne(string & line);
	void SendCount();
	void StartParsing();
};