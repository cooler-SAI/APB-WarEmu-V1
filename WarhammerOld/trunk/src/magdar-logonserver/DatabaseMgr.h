#ifndef _DATABASEMGR_H
#define _DATABASEMGR_H

class DatabaseMgr : public Singleton < DatabaseMgr >
{
public:
	Database * Logon;

private:

};

#define sLogonSQL DatabaseMgr::getSingleton().Logon

#endif