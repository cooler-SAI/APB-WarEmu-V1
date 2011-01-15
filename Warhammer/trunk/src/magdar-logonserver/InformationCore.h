#ifndef InformationCore_H
#define InformationCore_H

class AuthClient;
struct Account;

class InformationCore : public Singleton<InformationCore>
{
public:
	// RealmID , Realm *
	std::map<uint32,Realm*> m_realms;

	void AddCharInfo(uint32 accountid,list<CharInfo*> m_list)
	{
		M_charinfo.Acquire();
		m_charinfo[accountid] = m_list;
		M_charinfo.Release();	
	};
	void SendCharList(uint32 sequence,AuthClient * Com);
public:

	void AddRealm(Realm * realm);

	void SetRealm(Realm * realm);

	void RemoveRealm(Realm * realm);

	void SendAccountToRealm(Account * Acct);
	void SendRealm(Buffer* b);

	uint32 GetRealmCount();

	string m_password;

	Mutex M_charinfo;
	map<uint32,list<CharInfo*>> m_charinfo;

	void AddClient(AuthClient* Client)
	{
		m_clients.push_back(Client);
	};
	void RemoveClient(AuthClient* Client)
	{
		m_clients.remove(Client);
	};

	list<AuthClient*> m_clients;

};

#define sInfoCore InformationCore::getSingleton()

#endif