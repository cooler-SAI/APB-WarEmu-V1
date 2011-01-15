#pragma once

#define MAX_CUSTOM 5

/*
Liste des Commandes :

Get:uint:table:colone -> R�cup�re la valeur en int
GetString:1:table:colone:destination -> R�cup�re le string jusqu'a trouv� ce qu'il faut;
GetBoucle:1:table:colone: -> R�cup�re et incr�mente le nom avec le nombre;

Query -> Requ�te sur sql
Query:1:create -> Cr� une requ�te et la garde en m�moire
Query:1:dump -> Sort la requ�te dans un fichier sql et la supprime de la m�moire

Skip:uint: -> Saute le nombre d'uint demand�

Boucle:1:string:string: -> Boucle le nombre de foi ke la colone le dit:
	Boucle:1:traits_0: -> recherche la valeur de traits_0 en m�moire et fait la boucle
Boucle:2: -> Fin de la boucle: si on a atteind le bon nombre sa continue sinon sa reppart a Boucle:1:


Goto:1:string -> Supprime tout jusqu'a trouv� la valeur cherch�.

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