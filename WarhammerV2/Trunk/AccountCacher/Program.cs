using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

using Common;

using FrameWork;
using FrameWork.Logger;
using FrameWork.Config;
using FrameWork.Database;

namespace AccountCacher
{
    class Program
    {
        static public AccountMgr AcctMgr = null;
        static public AccountConfigs Conf = null;
        static public MySQLObjectDatabase AccountDatabase = null;

        [STAThread]
        static void Main(string[] args)
        {
            Log.Info("AccountCacher", "Lancement");

            Assembly.Load("Common");
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(onError);

            if(!EasyServer.InitLog("AccountCacher","Configs/AccountCacher.log"))
                return;

            ConfigMgr.LoadConfigs();
            Conf = ConfigMgr.GetConfig<AccountConfigs>();

            AccountDatabase = DBManager.Start(Conf.AccountDatabase.Total(), ConnectionType.DATABASE_MYSQL, "Accounts");
            if (AccountDatabase == null)
                return;

            if (!EasyServer.InitRpcServer("AccountCacher", Conf.RpcServer))
                return;

            AcctMgr = new AccountMgr();
            AccountMgr.Database = AccountDatabase;
            AcctMgr.LoadRealms();

            EasyServer.StartConsole();
        }

        static void onError(object sender, UnhandledExceptionEventArgs e)
        {
            Log.Error("onError", e.ExceptionObject.ToString());
        }
    }
}
