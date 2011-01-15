// SniffParser.cpp : fichier projet principal.

#include "stdafx.h"

using namespace System;
SERVER_DECL Database* Database_World;

bool StarDB()
{
	string lhostname, lusername, lpassword, ldatabase;
	int lport = 3306;
	int ltype = 1;

	// Configure Logon Database...
	lhostname = "127.0.0.1";


	/*while(1)
	{
		uint32 nb;
		cout << "Entrer world position" << endl;
		cin >> nb;
		uint32 offx = nb >> 12;
		uint32 x = (nb-offx*4096)*2;

		cout << "OffX="<< offx <<" X="<<x<< endl;
	}*/
	cout << "User name ?" << endl;
	cin >> lusername;

	cout << "Password ?" << endl;
	cin >> lpassword;

	cout << "Db name ?" << endl;
	cin >> ldatabase;

	Database_World = Database::Create();

	// Initialize it
	if(!WorldDatabase.Initialize(lhostname.c_str(), (unsigned int)lport, lusername.c_str(),
		lpassword.c_str(), ldatabase.c_str(), 5,16384))
	{
		sLog.outError("sql: World database initialization failed. Exiting.");
		return false;
	}   

	return true;
}
int main(array<System::String ^> ^args)
{
	string b;
	// Nom , total , erreur
	map<string,map<int,int>> m_packets;
	list<Packet*> m_list;

	cout << "Connexion a la DB monde" << endl;
	if(!StarDB())
		return 0;

    int count = 0;
	while(1)
	{
		stringstream filename;
		filename<<"sniff"<<count<<".txt";
		cout << "Chargement de " << filename.str().c_str() << std::endl;
		ifstream fichier(filename.str().c_str());
		if(!fichier) break;

        std::string ligne; // variable contenant chaque ligne lue
        while ( std::getline( fichier, ligne ) )
        {
			Packet * pck = new Packet(ligne);
			if(!pck) continue;

			string Name = pck->GetOpcodeName();
			m_packets[Name][0]++;
			m_list.push_back(pck);
        }
		cout << "Fin du chargement..."<<endl;
		count++;
	}

	if(!count) return 0;

	cout << "Lancement du decodage..."<<endl;
	int i=1;

	for(list<Packet*>::iterator itr = m_list.begin();itr!=m_list.end();itr++)
	{
		//cout <<i << ":";
		if(!(*itr)->DecodePacket())
			m_packets[(*itr)->GetOpcodeName()][1]++;
		//cout << endl << endl;
		i++;
	}
	cout << "Fin du decodage..."<<endl;

	cout << "Assemblage des Npc Et Npc Text..."<<endl;

	for(list<Packet*>::iterator itr = m_list.begin();itr!=m_list.end();itr++)
	{
		if( (*itr)->m_opcode != 233) continue;
		if( !(*itr)->m_oid ) continue;

		for(list<Packet*>::iterator itrr = m_list.begin();itrr!=m_list.end();itrr++)
		{
			if( !(*itrr)->m_oid ) continue;

			if( (*itrr)->m_oid == (*itr)->m_oid )
			{
				(*itr)->_createInterract( (*itrr)->m_entry );
			}
		}
	}
	cout << "Fin de l'assemblage..."<<endl;


	cout << "==============TOTAL================="<<endl;
	for(map<string,map<int,int>>::iterator itr = m_packets.begin();itr!=m_packets.end();itr++)
		cout << itr->first << " : " << itr->second[0] << " / Non decoder :"<<itr->second[1] << std::endl;
	cout << "===================================="<<endl;

	cin >> b;

    return 0;
}
void OnCrash(bool ok)
{


}