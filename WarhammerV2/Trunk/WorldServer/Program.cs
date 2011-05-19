using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Reflection;

using Common;

using FrameWork;
using FrameWork.Logger;
using FrameWork.Database;
using FrameWork.Config;

namespace WorldServer
{
    class Program
    {
        static public WorldConfigs Conf = null;
        static public AccountMgr AcctMgr = null;
        static public MySQLObjectDatabase WorldDatabase = null;
        static public MySQLObjectDatabase CharacterDatabase = null;

        static public Realm Rm;

        [STAThread]
        static void Main(string[] args)
        {
            Log.Info("WorldServer", "Lancement...");

            Assembly.Load("Common");
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(onError);
            Console.CancelKeyPress += new ConsoleCancelEventHandler(OnClose);

            if (!EasyServer.InitLog("WorldServer", "Configs/WorldServer.log"))
                return;

            ConfigMgr.LoadConfigs();
            Conf = ConfigMgr.GetConfig<WorldConfigs>();
            

            if (!EasyServer.InitRpcClient("WorldAccount",Conf.RpcAccountClient))
                return;

            CharacterDatabase = DBManager.Start(Conf.CharacterDatabase.Total(), ConnectionType.DATABASE_MYSQL, "Characters");
            if (CharacterDatabase == null)
                return;
            CharMgr.Database = CharacterDatabase;

            WorldDatabase = DBManager.Start(Conf.WorldDatabase.Total(), ConnectionType.DATABASE_MYSQL, "World");
            if (WorldDatabase == null)
                return;

            AcctMgr = new AccountMgr();

            Rm = Program.AcctMgr.GetRealm(Conf.RealmId);

            if (Rm == null)
            {
                Log.Error("WorldServer", "Royaume (" + Conf.RealmId + ") introuvable");
                return;
            }

            int Start = Environment.TickCount;

            LoaderMgr.InitLoad(WorldMgr.LoadItem_Infos);

            LoaderMgr.InitLoad(WorldMgr.LoadCreatureProto);
            LoaderMgr.InitLoad(WorldMgr.LoadCreatureSpawns);
            LoaderMgr.InitLoad(WorldMgr.LoadCreatureItems);
            LoaderMgr.InitLoad(WorldMgr.LoadCreatureVendors);
            LoaderMgr.InitLoad(WorldMgr.LoadCreatureLoots);

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
            int End = Environment.TickCount;

            Log.Info("Loader", "Chargement de la database en : " + (End - Start) + "ms");

            if (!EasyServer.Listen<TCPServer>(Rm.Port,"World"))
                return;

            Program.AcctMgr.UpdateRealm(EasyServer.GetRpcClientId("WorldAccount"), Rm.RealmId);

            EasyServer.StartConsole();
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
