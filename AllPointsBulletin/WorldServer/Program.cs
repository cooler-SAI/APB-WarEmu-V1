using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

using FrameWork;
using FrameWork.Logger;

using Common;

namespace WorldServer
{
    class Program
    {
        static public CharacterMgr CharMgr;
        static public FileManager FileMgr;

        static public string WorldName;
        static public int WorldId;

        static void Main(string[] args)
        {
            Assembly.Load("Common");
            Log.Info("WorldServer", "Démarrage ...");

            // Initialisation des Config de log et Générales
            if (!EasyServer.InitLog("World", "Configs/WorldLog.conf")
                || !EasyServer.InitConfig("Configs/World.xml", "World"))
                return;

            // Initialisation du Client Rpc pour le FileServer
            if (!EasyServer.InitRpcClient("WorldFile",
                                            EasyServer.GetConfValue<string>("World", "FileServer", "Key"),
                                            EasyServer.GetConfValue<string>("World", "FileServer", "Ip"),
                                            EasyServer.GetConfValue<int>("World", "FileServer", "Port")))
                return;

            // Initialisation du Client Rpc pour le CharacterServer
            if (!EasyServer.InitRpcClient("WorldChar",
                                            EasyServer.GetConfValue<string>("World", "CharacterServer", "Key"),
                                            EasyServer.GetConfValue<string>("World", "CharacterServer", "Ip"),
                                            EasyServer.GetConfValue<int>("World", "CharacterServer", "Port")))
                return;

            // Initialisation du TcpManager pour les Clients
            if (!EasyServer.Listen<TcpServer>(EasyServer.GetConfValue<int>("World", "ClientServer", "Port"), "ClientServer"))
                return;

            CharMgr = new CharacterMgr();
            FileMgr = new FileManager();

            WorldName = EasyServer.GetConfValue<string>("World", "ClientServer", "Name");
            WorldId = EasyServer.GetConfValue<int>("World", "ClientServer", "Id");

            CharMgr.RegisterWorld(EasyServer.GetRpcClientId("WorldChar"),
                                    WorldId,
                                    WorldName,
                                    EasyServer.GetConfValue<int>("World", "ClientServer", "Port"),
                                    EasyServer.GetConfValue<string>("World", "ClientServer", "Ip"));


            EasyServer.StartConsole();
        }
    }
}
