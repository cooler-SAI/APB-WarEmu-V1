using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

using FrameWork;
using FrameWork.Logger;

using Common;

namespace FileServer
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Assembly.Load("Common");

            Log.Info("FileServer", "Démarrage...");

            // Initialisation des Config de log et Générales
            if (!EasyServer.InitLog("File", "Configs/FileLog.conf")
                || !EasyServer.InitConfig("Configs/File.xml", "File"))
                return;

            // Initialisation du Client Rpc pour le FileServer
            if (!EasyServer.InitRpcServer("FileServer",
                                            EasyServer.GetConfValue<string>("File", "FileServer", "Key"),
                                            EasyServer.GetConfValue<int>("File", "FileServer", "Port")))
                return;

            Log.Succes("FileServer", "Initialisation du serveur terminée.");
            EasyServer.StartConsole();
        }
    }
}
