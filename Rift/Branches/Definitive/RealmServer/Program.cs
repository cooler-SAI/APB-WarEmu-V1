using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;
using Common;

namespace RealmServer
{
    class Program
    {
        static public RealmConfig Config;
        static public RpcClient Client;

        static public AccountMgr Accounts;

        static void Main(string[] args)
        {
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);
            Log.Texte("", ",---.o", ConsoleColor.Cyan);
            Log.Texte("", "`---..,---.,---.,---.,---.", ConsoleColor.Cyan);
            Log.Texte("", "    |||---'|   ||   |,---|", ConsoleColor.Cyan);
            Log.Texte("", "`---'``---'`   '`   '`---^ Core", ConsoleColor.Cyan);
            Log.Texte("", "http://siennacore.com", ConsoleColor.Blue);
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);

            // Loading all configs files
            ConfigMgr.LoadConfigs();
            Config = ConfigMgr.GetConfig<RealmConfig>();
            Config.RealmInfo.GenerateName();

            // Loading log level from file
            if (!Log.InitLog(Config.LogLevel,"Realm"))
                ConsoleMgr.WaitAndExit(2000);

            // Starting Remote Client
            Client = new RpcClient("Realm-" + Config.RealmInfo.RealmId, Config.LocalRpcIP, 1);
            if (!Client.Start(Config.RpcServerIp, Config.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);

            Accounts = Client.GetServerObject<AccountMgr>();
            Accounts.RegisterRealm(Config.RealmInfo, Client.Info);

            CharacterMgr.Client = Client;
            CharacterMgr.MyRealm = Config.RealmInfo;

            ConsoleMgr.Start();
        }
    }
}
