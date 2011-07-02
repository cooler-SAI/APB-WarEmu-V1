using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Rpc(false, System.Runtime.Remoting.WellKnownObjectMode.Singleton, 2)]
    public class MapMgr : RpcObject
    {
        static public MapServerInfo MapInfo;
        static public RpcClient Client;

        public override void OnServerConnected()
        {
            if (MapInfo != null)
                Client.GetServerObject<WorldMgr>().RegisterMaps(MapInfo, Client.Info);
        }

        public Dictionary<string, long> Connecting = new Dictionary<string, long>();

        public Realm GetRealm()
        {
            return Client.GetServerObject<CharactersMgr>().GetRealm();
        }

        public Account GetAccount(string Email)
        {
            return GetRealm().GetObject<AccountMgr>().GetAccountByEmail(Email);
        }

        public void RegisterConnecting(string Email, long CharacterID)
        {
            Email = Email.ToLower();

            Log.Success("MapMgr", "Registering email : " + Email + ", Character = " + CharacterID);

            if (Connecting.ContainsKey(Email))
                Connecting[Email] = CharacterID;
            else
                Connecting.Add(Email, CharacterID);
        }

        public long GetConnecting(string Email)
        {
            Email = Email.ToLower();

            Log.Success("MapMgr", "GetConnecting : " + Email);

            if (Connecting.ContainsKey(Email))
                return Connecting[Email];
            return 0;
        }
    }
}
