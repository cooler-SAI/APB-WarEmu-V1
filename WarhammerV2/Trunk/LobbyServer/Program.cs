using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;

using FrameWork;
using FrameWork.Logger;
using FrameWork.Config;

namespace LobbyServer
{
    class Program
    {
        static public AccountMgr AcctMgr = null;
        static public LobbyConfigs Config = null;

        static void Main(string[] args)
        {
            Log.Info("LobbyServer", "Lancement...");

            AppDomain.CurrentDomain.Load("Common");
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(onError);

            try
            {
                if (!EasyServer.InitLog("Lobby", "Configs/Lobby.log"))
                    return;

                ConfigMgr.LoadConfigs();
                Config = ConfigMgr.GetConfig<LobbyConfigs>();

                if (!EasyServer.InitRpcClient("LobbyAccount",Config.AccountCacher.Key,Config.AccountCacher.Ip,Config.AccountCacher.Port))
                    return;

                if (!EasyServer.Listen<TCPServer>(Config.ClientPort,"Lobby"))
                    return;

                AcctMgr = new AccountMgr();
            }
            catch (Exception e)
            {
                Log.Error("Erreur", e.ToString());
            }

            EasyServer.StartConsole();
        }

        static void onError(object sender, UnhandledExceptionEventArgs e)
        {
            Log.Error("onError", e.ExceptionObject.ToString());
        }
    }
}
