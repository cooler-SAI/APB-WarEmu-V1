#include "LogonStdAfx.h"

initialiseSingleton(AccountMgr);

AccountMgr::~AccountMgr()
{
	for(HM_NAMESPACE::hash_map<string,Account*>::iterator itr = AccountDatabase.begin(); itr != AccountDatabase.end(); ++itr)
		delete itr->second;
}

Account* AccountMgr::GetAccount(string Name)
{
		setBusy.Acquire();
		Account * pAccount = NULL;

		HM_NAMESPACE::hash_map<string, Account*>::iterator itr = AccountDatabase.find(Name);

		if(itr == AccountDatabase.end())	pAccount = NULL;
		else								pAccount = itr->second;

		setBusy.Release();
		return pAccount;
}
Account* AccountMgr::GetAccount(uint32 AccountID)
{
		setBusy.Acquire();
		Account * pAccount = NULL;

		HM_NAMESPACE::hash_map<string, Account*>::iterator itr = AccountDatabase.begin();
		for(;itr != AccountDatabase.end();itr++)
		{
			if( itr->second )
				if(itr->second->AccountId == AccountID)
				{
					pAccount = itr->second;
					break;
				}

		}
		setBusy.Release();

		return pAccount;
}
void AccountMgr::ReloadAccounts(bool silent)
{
	setBusy.Acquire();
	if(!silent) sLog.outString("[AccountMgr] Accounts Reloading...");

	// Load *all* accounts.
	QueryResult * result = sLogonSQL->Query("SELECT id,username,password,password_crypt,permission_level,locked FROM accounts");
	Field * field;
	string AccountName;
	set<string> account_list;
	Account * acct;

	if(result)
	{
		do 
		{
			field = result->Fetch();
			AccountName = field[1].GetString();

			// transform to uppercase

			//Use private __GetAccount, for locks
			acct = __GetAccount(AccountName);
			if(acct == 0)
			{
				// New account.
				AddAccount(field);
			}
			else
			{
				// Update the account with possible changed details.
				UpdateAccount(acct, field);
			}

			// add to our "known" list
			account_list.insert(AccountName);

		} while(result->NextRow());

		delete result;
	}

	// check for any purged/deleted accounts
	HM_NAMESPACE::hash_map<string, Account*>::iterator itr = AccountDatabase.begin();
	HM_NAMESPACE::hash_map<string, Account*>::iterator it2;

	for(; itr != AccountDatabase.end();)
	{
		it2 = itr;
		++itr;

		if(account_list.find(it2->first) == account_list.end())
		{
			delete it2->second;
			AccountDatabase.erase(it2);
		}
		else
		{
			it2->second->Username = it2->first;
		}
	}

	if(!silent) sLog.outString("[AccountMgr] %u Accounts loaded.", AccountDatabase.size());
	setBusy.Release();
}

void AccountMgr::AddAccount(Field* field)
{
	Account * acct = new Account;
	
	acct->AccountId		= field[0].GetUInt32();
	string Username     = field[1].GetString();
	string PassNC		= field[2].GetString();
	string Password	    = field[3].GetString();
	uint8 permission	= field[4].GetUInt8();
	

	if(PassNC.size())
	{
		if(!Password.size())
		{
			Password = makePassword(Username,PassNC);
			sLogonSQL->Execute("UPDATE accounts SET password_crypt='%s' WHERE id='%u'",Password.c_str(),acct->AccountId);
		}

		if(m_cleanpass)
		{
			PassNC.clear();
			sLogonSQL->Execute("UPDATE accounts SET password='' WHERE id='%u'",acct->AccountId);
		}
	}

	acct->Username				= Username;
	acct->Password				= Password;
	acct->Permission			= permission;
	acct->locked				= field[5].GetBool();

	// Convert username/password to uppercase. this is needed ;)

	AccountDatabase[Username] = acct;

	sLog.outString("[AccountMgr] Account '%s' Loaded !",Username.c_str());
}

void AccountMgr::UpdateAccount(Account * acct, Field * field)
{
	uint32 id = field[0].GetUInt32();
	string Username     = field[1].GetString();
	string PassNC	    = field[2].GetString();
	string Password	    = field[3].GetString();
	uint8 permission	= field[4].GetUInt8();

	if(PassNC.size())
	{
		if(!Password.size())
		{
			Password = makePassword(Username,PassNC);
			sLogonSQL->Execute("UPDATE accounts SET password_crypt='%s' WHERE id='%u'",Password.c_str(),acct->AccountId);
		}

		if(m_cleanpass)
		{
			PassNC.clear();
			sLogonSQL->Execute("UPDATE accounts SET password='' WHERE id='%u'",acct->AccountId);
		}
	}

	if(id != acct->AccountId)
	{
		//printf("Account %u `%s` is a duplicate.\n", id, acct->Username.c_str());
		sLog.outColor(TYELLOW, " >> deleting duplicate account %u [%s]...", id, Username.c_str());
		sLog.outColor(TNORMAL, "\n");
		sLogonSQL->Execute("DELETE FROM accounts WHERE id=%u", id);
		return;
	}

	acct->AccountId				= field[0].GetUInt32();
	acct->Username				= Username;
	acct->Password				= Password;
	acct->Permission			= permission;
	acct->locked				= field[5].GetBool();

	// Convert username/password to uppercase. this is needed ;)
}

void AccountMgr::ReloadAccountsCallback()
{
	ReloadAccounts(true);
}
bool AccountMgr::CheckPass(std::string& username,std::string& hashpass,std::string& X1,std::string& K3)
{
	Log.Debug("AcccountMgr","CheckPass [%s]",username.c_str());
	Account * Acc = GetAccount(username);
	if(!Acc) 
		return false;

	if( Acc->Password.size() != 64)
		return false;

	std::string tmp = Utils::makeHash(X1,Utils::makeSalt(username),Acc->Password);
    std::string result = Utils::makeHash(K3,X1,tmp);

	Log.Debug("AcccountMgr","CheckPass [%s] result=%s / hash=%s",username.c_str(),result.c_str(),hashpass.c_str());
	if (result == hashpass)
	{
		Acc->KeyToken = X1+tmp;
		return true;
	}
	else 
		return false;

}
bool AccountMgr::checkTokenLogin(const std::string& inUsername, const std::string& inHash, const std::string& K3)
{
	return true;
}