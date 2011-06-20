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
        static public CharactersMgr Characters;
        static public WorldMgr World;

        static void Main(string[] args)
        {
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);
            Log.Texte("", "          _____   _____ ", ConsoleColor.Cyan);
            Log.Texte("", "    /\\   |  __ \\ / ____|", ConsoleColor.Cyan);
            Log.Texte("", "   /  \\  | |__) | (___  ", ConsoleColor.Cyan);
            Log.Texte("", "  / /\\ \\ |  ___/ \\___ \\ ", ConsoleColor.Cyan);
            Log.Texte("", " / ____ \\| |     ____) |", ConsoleColor.Cyan);
            Log.Texte("", "/_/    \\_\\_|    |_____/ Rift", ConsoleColor.Cyan);
            Log.Texte("", "http://AllPrivateServer.com", ConsoleColor.DarkCyan);
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);

            // Loading all configs files
            ConfigMgr.LoadConfigs();
            Config = ConfigMgr.GetConfig<RealmConfig>();
            Config.RealmInfo.GenerateName();

            // Loading log level from file
            if (!Log.InitLog(Config.LogLevel,"Realm"))
                ConsoleMgr.WaitAndExit(2000);

            CharactersMgr.CharactersDB = DBManager.Start(Config.CharactersDB.Total(), ConnectionType.DATABASE_MYSQL, "Characters");
            if (CharactersMgr.CharactersDB == null)
                ConsoleMgr.WaitAndExit(2000);

            WorldMgr.WorldDB = DBManager.Start(Config.WorldDB.Total(), ConnectionType.DATABASE_MYSQL, "World");
            if (WorldMgr.WorldDB == null)
                ConsoleMgr.WaitAndExit(2000);

            PacketProcessor.RegisterDefinitions();

            // Starting Remote Client
            Client = new RpcClient("Realm-" + Config.RealmInfo.RealmId, Config.LocalRpcIP, 1);
            if (!Client.Start(Config.RpcServerIp, Config.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);

            World = Client.GetLocalObject<WorldMgr>();
            Accounts = Client.GetServerObject<AccountMgr>();
            Characters = Client.GetLocalObject<CharactersMgr>();

            // 1 : Loading WorldMgr
            World.Load();

            // 2 : Loading CharactersMgr
            CharactersMgr.Client = Client;
            CharactersMgr.MyRealm = Config.RealmInfo;

            // 3 : Loading AccountsMgr
            Accounts.RegisterRealm(Config.RealmInfo, Client.Info);
            
            ConsoleMgr.Start();
        }
    }
}
