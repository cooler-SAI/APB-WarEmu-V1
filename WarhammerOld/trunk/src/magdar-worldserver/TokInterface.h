#ifndef TOKINTERFACE_H
#define TOKINTERFACE_H

class Player;

struct TokPlayerInfo
{
	uint32 toktype;
	uint16 tokid;
	uint8 checked;
};

class TokInterface
{
public:
	TokInterface(Player *Plr) : m_player(Plr) {}
	void LoadToks();
	bool HasTok(uint16 entry);
	void AddTok(uint16 entry);

	// Sender
	void sendTok(TokPlayerInfo* Info);

private:
	Player * m_player;

	Mutex M_toks;
	map<uint16,TokPlayerInfo*> m_toks;

};

#endif