using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Rpc(false, System.Runtime.Remoting.WellKnownObjectMode.Singleton, 1)]
    public class CharacterMgr : RpcObject
    {
        static public MySQLObjectDatabase CharactersDB;

        #region Realm

        static public Realm MyRealm;
        static public RpcClient Client;

        public override void OnServerConnected()
        {
            if(MyRealm != null && Client != null)
                Client.GetServerObject<AccountMgr>().RegisterRealm(MyRealm, Client.Info);
        }


        #endregion

        #region Characters

        #endregion
    }
}
