#include "stdafx.h"

void SniffParser::LoadSniff(StructureLoader * Loader)
{
	m_Loader = Loader;
	int count=0;
	while(1)
	{
		stringstream filename;
		filename<<"sniff";
		filename<<count;
		filename<<".txt";
		ifstream fichier(filename.str().c_str());
		if(!fichier)
			break;

        std::string ligne; // variable contenant chaque ligne lue
        while ( std::getline( fichier, ligne ) )
        {
			ParseLigne(ligne);
        }
		cout << "Sortie du Fichier !!"<<endl;
		count++;
	}
	// 0 Fichiers ouvert
	if(!count) return;
	StartParsing();
	SendCount();

	for(list<PacketSniff*>::iterator itr = m_packets.begin();itr != m_packets.end() ; itr ++)
	{
		stringstream dd;
		dd << "ordre/" << (*itr)->Name << ".txt";
		FILE * fp = fopen(dd.str().c_str(),"ab+");
			if(fp) 
			{
				stringstream ss;ss << "Nouveau Packet : Nom=[" << (*itr)->Name << "] Taille=" << (*itr)->size << "\r\n";
				fprintf(fp,"%s", ss.str().c_str() );
				fprintf(fp,"|------------------------------------------------|----------------|\r\n" );
				fprintf(fp,"|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|\r\n" );
				fprintf(fp,"|------------------------------------------------|----------------|\r\n" );
				fprintf(fp,"%s", (*itr)->originalcontent.c_str() );
				fprintf(fp,"\r\n \r\n" );
				fclose(fp);
			}
	}
}
void SniffParser::ParseLigne(string & ligne)
{
	// Nouveau block ?
	if(ligne.find("RC4 key") != string::npos) return;
	int a = ligne.find("{SERVER}");

	if(a != string::npos )
	{
		string PacketName;
		int PacketSize=0;
		ligne.erase(0,26);
		a = ligne.find_first_of(' ');
		PacketName = ligne.substr(0,a);
		ligne.erase(0,a+13);
		PacketSize = atoi(ligne.c_str());
		ActuPacket = NULL;
		m_blockcounter[PacketName]++;

		//On Vérifi la taille et le nom
		if( PacketName.compare("UNKNOWN") == 0 || PacketSize==0 )
			return;

		cout << "Nouveau Packet : Nom=" << PacketName << " Taille=" << PacketSize << std::endl;

		PacketSniff * Packet = new PacketSniff;
		Packet->Name = PacketName;
		Packet->size = PacketSize;
		ActuPacket = Packet;
		i=0;
		return;
	}

	i++;
	// 4 Lignes qui servent a rien;
	if( i < 4 || ActuPacket==NULL) return;

	// On supprime tous les FFFFF si yen a;
	int ff = ligne.find("FFFFFF");
	while(ff != string::npos)
	{
		ligne.erase(ff,ff+6);
		ff = ligne.find("FFFFFF");
	}

	ActuPacket->originalcontent+=ligne;
	ActuPacket->originalcontent+="\r\n";
	ligne.erase(0,1);

	int x,y;

	x = ligne.find_first_of('|');
	y = ligne.find_last_of('|');

	if( x != string::npos && y != string::npos)
	{
		ligne.erase(x,y);

		// On Supprime tous les espaces
		ff = ligne.find(" ");
		while(ff != string::npos)
		{
			ligne.erase(ff,1);
			ff = ligne.find(" ");
		}
		ActuPacket->content+=ligne;
		//cout << ligne << endl;
	}
	else
	{
		i=0;
		m_packets.push_back(ActuPacket);
		ActuPacket = NULL;
	}
}
void SniffParser::SendCount()
{
	for(map<string,int>::iterator itr = m_blockcounter.begin();itr!=m_blockcounter.end();itr++)
		cout << itr->first << " : " << itr->second << std::endl;
}
void SniffParser::StartParsing()
{
	for(list<PacketSniff*>::iterator itr = m_packets.begin();itr != m_packets.end() ; itr ++)
	{
			m_Loader->ParseSql( (*itr)->content, (*itr)->Name);
	}
	
	
}