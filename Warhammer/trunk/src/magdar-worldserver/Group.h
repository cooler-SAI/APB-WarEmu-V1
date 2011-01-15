#ifndef GROUP_H
#define GROUP_H

#define UPDATE_GROUPE_TIME 2000;

class Player;

typedef std::list<Player*> GroupMembersSet;

struct GroupInvitation
{
	uint16 owner;
	uint16 invited;
	Player * Owner;
	Player * Invited;
	uint32 expire;

	GroupInvitation(uint16 own,uint16 inv,Player * Own,Player * Inv);
	void SendInvitation();
	void DeclineInvitation();
	void AcceptInvitation();
	void Destroy();
};

class Group
{
	public:
		Group();

		/************************************************************************/
		/* LEADER																*/
		/************************************************************************/
		Player * GetLeader() { return m_Leader; };
		void SetLeader(Player * Plr);
		bool IsLeader(Player * Plr) { return m_Leader == Plr; };

		/************************************************************************/
		/* MEMBERS																*/
		/************************************************************************/
		void AddMember(Player * Plr);
		void RemoveMember(Player * Plr);
		bool HasMember(uint32 playerid);
		bool IsFull() { return m_GroupMembers.size() >= 6; };
		GroupMembersSet::iterator GetGroupBegin() { return m_GroupMembers.begin(); }
		GroupMembersSet::iterator GetGroupEnd() { return m_GroupMembers.end(); }
		uint32 GetGroupMembers() { return m_GroupMembers.size(); }

		/************************************************************************/
		/* FUNCTION																*/
		/************************************************************************/
		void sendWhosLeader(Player * Plr); // Envoi le nom du leader du groupe;
		void sendNewMember(Player * Plr); // Envoi a tous le groupe le nouveau membre qui a rejoins;

		void sendMessageToGroup(Player * Sender,string message);
		void sendGroupBuild(Player * Plr);
		void sendNullGroup(Player * Plr);
		void DispachToGroup(Buffer * b);

		void Delete();

		void Update();
		void UpdatePlayerPosition(Player * Plr);

	private:
		Player * m_Leader;
		Mutex M_GroupMembers;
		GroupMembersSet m_GroupMembers;
};



#endif