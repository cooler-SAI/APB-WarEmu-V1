using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Reflection;

using Common;
using FrameWork;

namespace WorldServer
{
    class Program
    {
        static public WorldConfigs Config = null;
        static public RpcClient Client = null;
        static public AccountMgr AcctMgr
        {
            get
            {
                return Client.GetServerObject<AccountMgr>();
            }
        }
        static public TCPServer Server;
        static public Realm Rm;

        [STAThread]
        static void Main(string[] args)
        {
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(onError);
            Console.CancelKeyPress += new ConsoleCancelEventHandler(OnClose);

            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);
            Log.Texte("", "          _____   _____ ", ConsoleColor.Cyan);
            Log.Texte("", "    /\\   |  __ \\ / ____|", ConsoleColor.Cyan);
            Log.Texte("", "   /  \\  | |__) | (___  ", ConsoleColor.Cyan);
            Log.Texte("", "  / /\\ \\ |  ___/ \\___ \\ ", ConsoleColor.Cyan);
            Log.Texte("", " / ____ \\| |     ____) |", ConsoleColor.Cyan);
            Log.Texte("", "/_/    \\_\\_|    |_____/ Warhammer", ConsoleColor.Cyan);
            Log.Texte("", "http://AllPrivateServer.com", ConsoleColor.DarkCyan);
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);

            // Loading all configs files
            ConfigMgr.LoadConfigs();
            Config = ConfigMgr.GetConfig<WorldConfigs>();

            // Loading log level from file
            if (!Log.InitLog(Config.LogLevel, "WorldServer"))
                ConsoleMgr.WaitAndExit(2000);

            CharMgr.Database = DBManager.Start(Config.CharacterDatabase.Total(), ConnectionType.DATABASE_MYSQL, "Characters");
            if (CharMgr.Database == null)
                ConsoleMgr.WaitAndExit(2000);

            WorldMgr.Database = DBManager.Start(Config.WorldDatabase.Total(), ConnectionType.DATABASE_MYSQL, "World");
            if (WorldMgr.Database == null)
                ConsoleMgr.WaitAndExit(2000);

            Client = new RpcClient("WorldServer-" + Config.RealmId, Config.AccountCacherInfo.RpcLocalIp, 1);
            if (!Client.Start(Config.AccountCacherInfo.RpcServerIp, Config.AccountCacherInfo.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);
            

            Rm = Program.AcctMgr.GetRealm(Config.RealmId);

            if (Rm == null)
            {
                Log.Error("WorldServer", "Realm (" + Config.RealmId + ") not found");
                return;
            }

            long Start = TCPManager.GetTimeStampMS();

            LoaderMgr.InitLoad(WorldMgr.LoadItem_Info);

            LoaderMgr.InitLoad(WorldMgr.LoadCreatureProto);
            LoaderMgr.InitLoad(WorldMgr.LoadCreatureSpawns);
            LoaderMgr.InitLoad(WorldMgr.LoadCreatureItems);

            LoaderMgr.InitLoad(WorldMgr.LoadZone_Info);
            LoaderMgr.InitLoad(WorldMgr.LoadXp_Info);
            LoaderMgr.InitLoad(WorldMgr.LoadRenown_Info);
            LoaderMgr.InitLoad(WorldMgr.LoadTok_Infos);
            LoaderMgr.InitLoad(WorldMgr.LoadChapter_Infos);

            LoaderMgr.InitLoad(WorldMgr.LoadQuests);
            LoaderMgr.InitLoad(WorldMgr.LoadQuestsObjectives);

            LoaderMgr.InitLoad(CharMgr.LoadCharacterInfo);
            LoaderMgr.InitLoad(CharMgr.LoadCharacterInfoItems);
            LoaderMgr.InitLoad(CharMgr.LoadCharacterInfoStats);
            LoaderMgr.InitLoad(CharMgr.LoadCharacters);
            LoaderMgr.InitLoad(CharMgr.LoadItems);

            LoaderMgr.Wait();
            WorldMgr.LoadRelation();

            long End = TCPManager.GetTimeStampMS();

            Log.Info("Loader", "Database loaded in : " + (End - Start) + "ms");

            if (!TCPManager.Listen<TCPServer>(Rm.Port, "World"))
                ConsoleMgr.WaitAndExit(2000);

            Server = TCPManager.GetTcp<TCPServer>("World");

            AcctMgr.UpdateRealm(Client.Info, Rm.RealmId);

            ConsoleMgr.Start();
        }

        static void onError(object sender, UnhandledExceptionEventArgs e)
        {
            Log.Error("onError", e.ExceptionObject.ToString());
        }

        static public void OnClose(object obj,ConsoleCancelEventArgs Args)
        {
            Log.Info("Fermeture", "Fermeture du serveur");

            WorldMgr.Stop();
            Player.Stop();
        }
    }
}
