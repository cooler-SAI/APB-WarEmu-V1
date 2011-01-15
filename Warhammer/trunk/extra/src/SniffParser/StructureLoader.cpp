#include "StdAfx.h"

char *TextToHex( char *text, int len) 
{
	if (!text) return 0;

	char *buffer = new char[2*len]+1;
	if (!buffer) return 0;

	for (int i=0; i<len; i++)
	sprintf( buffer+2*i, "%02x", (unsigned int) text[i]);

	return buffer;
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

string StringToAscii(string hex)
{
	int a = hex.size();
	stringstream ss;
	for(int i=0;i<a;i++)
	{
		char t = hexToAscii(hex[0],hex[1]);
		ss << t;
		hex.erase(0,2);
		i++;
	}
return ss.str();
}

string StructureLoader::GetVariable(string & ligne)
{
	int a = ligne.find_first_of(':');

	string txt;

	if( a != string::npos )
	{
		txt = ligne.substr(0,a);
		ligne.erase(0,a+1);
	}
	return txt;
}
void StructureLoader::PrintLine(InfoLine * Info)
{
	cout << " Command :[" << Info->command << "] - Value :[" << Info->value << "]";

	for(int z=0;z<MAX_CUSTOM;z++)
		if( Info->custom[z].size() > 0)
			cout << " Custom" << z << " :[" << Info->custom[z]<<"] | ";

	cout << endl;

}
void StructureLoader::ParseLigne(string & ligne)
{
	int a = ligne.find_first_of('<');
	int c = ligne.find_first_of('>');

	if(c != string::npos )
	{
		m_structures.insert( make_pair(actublock,ActuStore));
		std::cout << "Fin du Block : " << actublock << std::endl;
		actublock.clear();
		return;
	}

	if( a != string::npos ) // Début d'un nouveau systeme
	{
		ActuStore = new LineStore;
		actublock = ligne.substr(a+1,ligne.size()-1);
		std::cout << "Nouveau Block : " << actublock << std::endl;
		return;

	}		//Si il n'y a pas de débu mais qu'il y a un block actuel
	
	if( actublock.size() )
	{
		InfoLine * Info = new InfoLine;
		Info->command = GetVariable(ligne);
		Info->value = atoi(GetVariable(ligne).c_str());

		for(int z=0;z<MAX_CUSTOM;z++)
			Info->custom[z] = GetVariable(ligne);

		//PrintLine(Info);
		ActuStore->m_liste.push_back(Info);
	}
}
void StructureLoader::PrintBlock(string name)
{
	map<string,LineStore*>::iterator itr = m_structures.find(name);
	if( itr != m_structures.end() )
	{
		for(list<InfoLine*>::iterator itrr = itr->second->m_liste.begin();itrr!= itr->second->m_liste.end();itrr++)
		{
			PrintLine((*itrr));
		}
	}
	else
	{
		cout << "Block Introuvable !" << std::endl;
	}

}
void StructureLoader::LoadStructure(ifstream& fichier)
{
        std::string ligne; // variable contenant chaque ligne lue
		fp = fopen("sql.txt","ab+");
		if(!fp)
		{
			cout << "sql.txt introuvable";
			return;
		}
        while ( std::getline( fichier, ligne ) )
        {
			if(ligne.size() > 0)
			ParseLigne(ligne);
        }
}
void StructureLoader::ParseSql(string &content,string name)
{
	if(!fp) return;

	map<string,LineStore*>::iterator itr = m_structures.find(name);
	if( itr != m_structures.end() )
	{
		cout << "Decodage Du Packet : " << name << endl;
		int next_count=0;
		int counter=0;
		bool ignore=false;
		string next_table;
		stringstream enregistrement;

		// table,infocolone;
		map<string,InfoColone*> m_req;
		// Pour les boucle pointeur sur le retour et l'actuel;
		list<InfoLine*>::iterator  ret,actu;
		for(list<InfoLine*>::iterator itrr = itr->second->m_liste.begin();itrr!= itr->second->m_liste.end();itrr++)
		{
			InfoLine * Info = (*itrr);
			if(!Info || ignore) continue;
			//cout << "Commande = " << Info->command << " ["<<Info->value<<"] "<< endl;
			if(Info->command == "Query") // Information sur les requètes
			{
				if(Info->custom[0] == "create") // Cré une nouvelle requète
				{
					InfoColone * col = new InfoColone;
					m_req[Info->custom[1]] = col;
					//cout << "Creation d'une novuelle requète pour ["<<Info->custom[1]<<"]" << endl;
				}
				else if(Info->custom[0] == "dump") // Exporte en sql la requète
				{
					if(m_req[Info->custom[1]] == NULL) continue;
					InfoColone * col = m_req[Info->custom[1]];

					stringstream req;
					req << "INSERT INTO "<< Info->custom[1] << " (";

					bool ok=true;
					for( map<string,string>::iterator itt=col->m_colone.begin();itt!=col->m_colone.end();itt++)
					{
						if(!ok) req << ",";
						req <<itt->first;

						ok=false;
					}

					req << ") VALUE(";
					ok = true;
					bool coded=false;
					for( map<string,string>::iterator itt=col->m_colone.begin();itt!=col->m_colone.end();itt++)
					{

						if( itt->second[0] == '@') // Requète mysql
						{	
							if(!coded && !ok) req << "'";
							if(!ok) req << ",";
							coded = true;
							req << itt->second.c_str();
						}
						else
						{
							if(!coded) req << "'";
							// '1','2,@mob,
							if(!ok) req << ",'";
							req << itt->second.c_str();
							coded=false;
						}
						ok =false;
					}
					if( !coded ) req << "'";
					req << ");";
					m_req[Info->custom[1]] = NULL;
					//cout << "Dump :" << req.str() <<endl;

					//fprintf(fp,req.str().c_str());
					//fprintf(fp," \r\n");
					enregistrement << req.str() << " \r\n";
				}
				else if(Info->custom[0] == "execute") // Ajoute une requète au fichié;
				{
					//fprintf(fp,Info->custom[1].c_str());
					//fprintf(fp," \r\n");
					enregistrement << Info->custom[1] << " \r\n";
				}
			}
			else if(Info->command == "Skip") // Zap le nombre désiré
			{
				if(content.size() <=0 ) continue; // plus rien a lire on zap tout;
				if(Info->value < 8 ) continue; // Valeur incorrect
				content.erase(0,Info->value/4);
			}
			else if(Info->command == "Boucle")
			{
				if(Info->value == 1) // Création de la boucle;
				{
					// Cherche la table;
					if( m_req[Info->custom[0]] == NULL ) continue;
					if( !m_req[Info->custom[0]]->m_colone[Info->custom[1]].size() ) continue;

					next_count = atoi( m_req[Info->custom[0]]->m_colone[ Info->custom[1] ].c_str() );
					ret = itrr;
					//cout << "Creation d'une boucle : x" << next_count <<endl;
				}
				else if(Info->value == 2) // Retour a la boucle;
				{
					// Si on a fait le nombre de boucle qu'il fallait
					if( next_count-1 <= counter || next_count > 100)  // Problème de lecture
					{
						next_count=0;
						counter=0;
						continue;
					}
					counter++;
					itrr = ret;
					//cout << "Retour a la boucle : "<< counter <<endl;
				}
			}
			else if(Info->command == "Get" )
			{
							if(content.size() <=0 ) continue; // plus rien a lire on zap tout;
				if( m_req[Info->custom[0]] == NULL ) continue;

				if(Info->value < 8 ) continue;
				stringstream ss;
				int a = HexaToInt(Info->value,content);
				ss << a;
				m_req[Info->custom[0]]->m_colone.insert( make_pair(Info->custom[1],ss.str()) );
				//cout << "Get : ["<<Info->custom[0]<<"]["<<Info->custom[1]<<"] = "<<ss.str()<<endl;
			}
			else if(Info->command == "GetBoucle" )
			{
							if(content.size() <=0 ) continue; // plus rien a lire on zap tout;
				if( m_req[Info->custom[0]] == NULL ) continue;

				if(Info->value < 8 ) continue;

				stringstream ss;
				int a = HexaToInt(Info->value,content);
				ss << a;
				stringstream name;
				name << Info->custom[1];
				name<<counter+2;
				m_req[Info->custom[0]]->m_colone.insert( make_pair(name.str(),ss.str()) );
				//cout << "GetBoucle : ["<<Info->custom[0]<<"]["<<name.str()<<"] = "<<ss.str()<<endl;
			}
			else if(Info->command == "GetString") // On récupère le string;
			{
							if(content.size() <=0 ) continue; // plus rien a lire on zap tout;
				if( m_req[Info->custom[0]] == NULL ) continue;

				stringstream nom;

				int a = content.find(Info->custom[2]);
				if( a != string::npos)
				{
					string hex = content.substr(0,a);
					content.erase(0,a);
					nom << StringToAscii(hex);

				m_req[Info->custom[0]]->m_colone.insert( make_pair(Info->custom[1],nom.str()) );
				//cout << "GetString : ["<<Info->custom[0]<<"]["<<Info->custom[1]<<"] = " << nom.str() <<endl;
				}
			}
			else if( Info->command == "Goto" ) // Supprime tout jusqu'a la valeur choisi
			{
							if(content.size() <=0 ) continue; // plus rien a lire on zap tout;
				int a = content.find(Info->custom[0]);
				if( a != string::npos)
					content.erase(0,a);

				//cout << "[Goto] -> [" << Info->custom[0] << "]" << endl;
			}
			else if( Info->command == "Copy" ) // Supprime tout jusqu'a la valeur choisi
			{
				if( m_req[Info->custom[0]] == NULL ) continue;
				if( m_req[Info->custom[1]] == NULL ) continue;

				string value = m_req[Info->custom[0]]->m_colone[ Info->custom[2] ];
				m_req[Info->custom[1]]->m_colone[Info->custom[2]] = value;
			}
			else if( Info->command == "Add") // Ajoute une colone avec la valeur qu'on veut
			{
				if( m_req[Info->custom[0]] == NULL ) continue;
				m_req[Info->custom[0]]->m_colone.insert( make_pair(Info->custom[1],Info->custom[2]) );
			}
			else if( Info->command == "If")
			{
				string tt;
				if( m_req[Info->custom[0]]!= NULL )
				{
					if( Info->custom[1] == "NULL" )
					{
						cout << "NULL" <<" \n\r";
						std::cin >> tt;

						if( m_req[Info->custom[0]]->m_colone[Info->custom[1]].size() <= 0)
						{
							ignore = true;
							m_req.clear();
							cout << "Name==NULL \n\r";
							std::cin >> tt;
						}

					}
					else
					{
						if(m_req[Info->custom[0]]->m_colone[Info->custom[1]].c_str() == Info->custom[1])
						{

						}

					}

				}
			}
		}
		fprintf(fp,enregistrement.str().c_str());
		cout << "Fin du decodage : " << name << endl;
	}
}
int StructureLoader::HexaToInt(int type,string &content)
{
	if( type <= 64 && type!= 0) // Uint8 - 64
	{
		type/=4;
		string hex = content.substr(0,type);
		content.erase(0,type);
		 std::istringstream iss(hex.c_str());
		 int n;
		 iss >> std::hex >> n;
		 return n;
	}
	else return 0; // string
}
string StructureLoader::GetHex(int type,string &content)
{
	string hex;
	if( type <= 64 && type!= 0) // Uint8 - 64
	{
		type/=4;
		hex = content.substr(0,type);
		content.erase(0,type);
		return hex;
	}
	else return hex; // string
}
