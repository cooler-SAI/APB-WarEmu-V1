using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using Common;

using FrameWork;
using FrameWork.Logger;

namespace LauncherServer
{
    class Program
    {
        static public AccountMgr AcctMgr = null;
        static public int Version = 1;
        static public string Message = "Invalide";
        static public FileInfo Info;
        static public string StrInfo;

        [STAThread]
        static void Main(string[] args)
        {
            Log.Info("LauncherServer", "Lancement...");

            AppDomain.CurrentDomain.Load("Common");
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(onError);

            if (!EasyServer.InitLog("Launcher", "Configs/Launcher.log"))
                return;

            if (!EasyServer.InitConfig("Configs/Launcher.xml", "Launcher"))
                return;

            if(!EasyServer.InitRpcClient("Launcher",
                EasyServer.GetConfValue<string>("Launcher","AccountCacher","Key"),
                EasyServer.GetConfValue<string>("Launcher","AccountCacher","Ip"),
                EasyServer.GetConfValue<int>("Launcher","AccountCacher","Port")))
                return;

            Version = EasyServer.GetConfValue<int>("Launcher", "LauncherServer", "Version");
            Message = EasyServer.GetConfValue<string>("Launcher", "LauncherServer", "Message");
            Info = new FileInfo("Configs/mythloginserviceconfig.xml");
            if (!Info.Exists)
            {
                Log.Error("Configs/mythloginserviceconfig.xml", "Fichier de configuration introuvable");
                return;
            }
            StrInfo = Info.OpenText().ReadToEnd();

            if(!EasyServer.Listen<TCPServer>(
                EasyServer.GetConfValue<int>("Launcher","LauncherServer","Port"),
                "Launcher"))
                return;

            AcctMgr = new AccountMgr();

            EasyServer.StartConsole();
        }

        static void onError(object sender, UnhandledExceptionEventArgs e)
        {
            Log.Error("onError", e.ExceptionObject.ToString());
        }
    }
}
