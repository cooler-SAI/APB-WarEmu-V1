#ifndef INFLUENCEINTERFACE_H
#define INFLUENCEINTERFACE_H

class Player;

struct ItemInfluence;
struct InfluenceInfo;
struct AreaInfo;


struct InfluencePlayerInfo
{
	uint16 influenceid; // Numéro de l'influence
	uint32 influence; // Nombre d'influence;

	uint32 rewarded[3]; // Item déja pris;

	vector<ItemInfluence*> m_reward[3]; // Items a récupérer

	InfluenceInfo * Info; // Information sur le chapitre;

	void GenereateReward(Player * Plr);
};

class InfluenceInterface
{
public:
	InfluenceInterface(Player * Plr) : m_player(Plr) {}
	void LoadInfluences();

	InfluencePlayerInfo * AddInfluence(uint16 influenceid,uint32 counts,InfluenceInfo * IInfo=NULL);
	bool HasInfluence(uint16 influenceid);
	InfluencePlayerInfo * GetInfluence(uint16 influenceid);

	void sendInfluenceInfo(InfluencePlayerInfo* Info);
	void sendInfluenceUpdated(InfluencePlayerInfo* Info,bool print=false);
	void sendInfluenceDetail(InfluencePlayerInfo* Info);


	vector<InfluenceInfo*> m_wait;
private:
	Player * m_player;

	map<uint16,InfluencePlayerInfo*> m_influs;
};

#endif