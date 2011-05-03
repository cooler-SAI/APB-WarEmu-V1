using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Rpc(true,System.Runtime.Remoting.WellKnownObjectMode.Singleton,1)]
    public class AccountMgr : RpcObject
    {
        static public MySQLObjectDatabase AccountDB;

        #region Accounts

        public Account GetAccount(long Id)
        {
            return AccountDB.SelectObject<Account>("Id=" + Id);
        }

        public Account GetAccount(string SessionKey)
        {
            return AccountDB.SelectObject<Account>("SessionKey='" + AccountDB.Escape(SessionKey) + "'");
        }

        public Account GetAccount(string Username, string Sha_Password)
        {
            return AccountDB.SelectObject<Account>("Username = '" + AccountDB.Escape(Username) + "' AND Sha_Password='" + AccountDB.Escape(Sha_Password) + "'");
        }

        #endregion
    }
}
