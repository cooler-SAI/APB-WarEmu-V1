using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

using FrameWork;
using FrameWork.Logger;

using Common;

namespace CharacterServer
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Assembly.Load("Common");

            Log.Info("CharacterServer", "Démarrage...");

            // Initialisation des Config de log et Générales
            if (!EasyServer.InitLog("Char", "Configs/CharLog.conf")
                || !EasyServer.InitConfig("Configs/Char.xml", "Char"))
                return;

            // Initialisation du Client Rpc pour le FileServer
            if (!EasyServer.InitRpcServer("Character",
                                            EasyServer.GetConfValue<string>("Char", "CharacterServer", "Key"),
                                            EasyServer.GetConfValue<int>("Char", "CharacterServer", "Port")))
                return;

            // Initialisation de la Base de donnée
            if (!EasyServer.InitMysqlDB("Configs/CharDB.xml", "CharDB"))
                return;

            CharacterMgr.Database = EasyServer.GetDatabase("CharDB");

            Log.Succes("CharacterServer", "Initialisation du serveur terminée.");
            EasyServer.StartConsole();
        }
    }
}
