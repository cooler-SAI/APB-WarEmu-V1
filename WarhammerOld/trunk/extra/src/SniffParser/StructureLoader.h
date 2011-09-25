#pragma once

#define MAX_CUSTOM 5

/*
Liste des Commandes :

Get:uint:table:colone -> Récupère la valeur en int
GetString:1:table:colone:destination -> Récupère le string jusqu'a trouvé ce qu'il faut;
GetBoucle:1:table:colone: -> Récupère et incrémente le nom avec le nombre;

Query -> Requète sur sql
Query:1:create -> Cré une requète et la garde en mémoire
Query:1:dump -> Sort la requète dans un fichier sql et la supprime de la mémoire

Skip:uint: -> Saute le nombre d'uint demandé

Boucle:1:string:string: -> Boucle le nombre de foi ke la colone le dit:
	Boucle:1:traits_0: -> recherche la valeur de traits_0 en mémoire et fait la boucle
Boucle:2: -> Fin de la boucle: si on a atteind le bon nombre sa continue sinon sa reppart a Boucle:1:


Goto:1:string -> Supprime tout jusqu'a trouvé la valeur cherché.

Copy:1:source:destination:colone -> Copie la colone d'une table a une autre;


*/

struct InfoColone
{
	// Colone , Value;
	map<string,string> m_colone;
};

struct InfoLine
{
	string command;
	int value;

	string custom[MAX_CUSTOM];
};

struct LineStore
{
	list<InfoLine*> m_liste;
};


class StructureLoader
{
public:
	map<string,LineStore*> m_structures;
	string actublock;
	LineStore * ActuStore;
	FILE * fp;

	string GetVariable(string & ligne);
	void PrintLine(InfoLine * Info);
	void ParseLigne(string & ligne);
	void PrintBlock(string name);
	void LoadStructure(ifstream& fichier);

	bool HaveStructure(string nom)
	{
		map<string,LineStore*>::iterator itr = m_structures.find(nom);
		if( itr != m_structures.end() )
			return true;
		else return false;
	};

	void ParseSql(string &content,string name);
	int HexaToInt(int type,string &content);
	string GetHex(int type,string &content);
};