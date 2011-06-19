using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using FrameWork;
using Common;

namespace RealmServer
{
    class Program
    {
        static public RealmConfig Config;
        static public RpcClient Client;

        static public AccountMgr Accounts;
        static public CharacterMgr Characters;
        static public WorldMgr World;

        static public CacheData[] Data;
        static public CacheTemplate[] Templates;

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

            CharacterMgr.CharactersDB = DBManager.Start(Config.CharactersDB.Total(), ConnectionType.DATABASE_MYSQL, "Characters");
            if (CharacterMgr.CharactersDB == null)
                ConsoleMgr.WaitAndExit(2000);

            WorldMgr.WorldDB = DBManager.Start(Config.WorldDB.Total(), ConnectionType.DATABASE_MYSQL, "World");
            if (WorldMgr.WorldDB == null)
                ConsoleMgr.WaitAndExit(2000);

            PacketProcessor.RegisterDefinitions();

            // Starting Remote Client
            Client = new RpcClient("Realm-" + Config.RealmInfo.RealmId, Config.LocalRpcIP, 1);
            if (!Client.Start(Config.RpcServerIp, Config.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);

            Client.GetLocalObject<WorldMgr>().LoadCache();
            Accounts = Client.GetServerObject<AccountMgr>();
            Accounts.RegisterRealm(Config.RealmInfo, Client.Info);

            Characters = Client.GetLocalObject<CharacterMgr>();

            CharacterMgr.Client = Client;
            CharacterMgr.MyRealm = Config.RealmInfo;
            
            ConsoleMgr.Start();
        }
    }
}
