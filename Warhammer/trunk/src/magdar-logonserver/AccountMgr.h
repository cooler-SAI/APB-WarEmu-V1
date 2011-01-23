#ifndef _ACCOUNTMGR_H
#define _ACCOUNTMGR_H

struct Account
{
	uint32 AccountId;
	string Username;
	string Password;

	uint16 SessionKey;

	Account() { SessionKey = NULL;Permission=0; }
	~Account() { if(SessionKey) SessionKey=NULL; }

	void SetSessionKey(const uint16 key)
	{
		if(SessionKey == NULL)
			SessionKey = key;
	}

	uint8 Permission;

	bool locked;

	string KeyToken;
};

class AccountMgr : public Singleton < AccountMgr >
{
public:
	~AccountMgr();

	void AddAccount(Field* field);

	Account* GetAccount(string Name);
	Account* GetAccount(uint32 AccountID);

	bool CheckPass(std::string& username,std::string& hashpass,std::string& X1,std::string& K3);
	bool checkTokenLogin(const std::string& inUsername, const std::string& inHash, const std::string& K3);

	void UpdateAccount(Account * acct, Field * field);
	void ReloadAccounts(bool silent);
	void ReloadAccountsCallback();

	std::string makePassword(std::string& username, std::string& password)
	{
		std::string ToEncode = username + ":" + password;

		char cryptIN[256];
		char outHash[SHA256_DIGEST_LENGTH];

		memset(cryptIN,0,256);
		memcpy(&cryptIN,ToEncode.c_str(),ToEncode.length());
		EVP_Digest(cryptIN,ToEncode.length(),(uint8*)&outHash,NULL,EVP_sha256(),NULL);

		return Utils::byteArrayToAsciiString((unsigned char*)&outHash,SHA256_DIGEST_LENGTH);
	}

	__forceinline size_t GetCount() { return AccountDatabase.size(); }

	HM_NAMESPACE::hash_map<string, Account*> AccountDatabase;

	bool m_cleanpass;
private:
	Account* __GetAccount(string Name)
	{
		// this should already be uppercase!
		HM_NAMESPACE::hash_map<string, Account*>::iterator itr = AccountDatabase.find(Name);

		if(itr == AccountDatabase.end())	return NULL;
		else								return itr->second;
	}

protected:
	Mutex setBusy;
};

#define sAccountMgr AccountMgr::getSingleton()
#endif