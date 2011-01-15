using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;
using FrameWork.Logger;

using Common;

namespace LobbyServer
{
    class Program
    {
        static public byte[] Version;
        static public int Build = 0;

        static public CharacterMgr CharMgr;
        static public FileManager FileMgr;

        [STAThread]
        static void Main(string[] args)
        {
            Log.Info("LobbyServer", "Démarrage ...");

            // Initialisation des Config de log et Générales
            if (!EasyServer.InitLog("Lobby", "Configs/LobbyLog.conf")
                || !EasyServer.InitConfig("Configs/Lobby.xml", "Lobby"))
                return;

            // Initialisation du Client Rpc pour le FileServer
            if (!EasyServer.InitRpcClient(  "LobbyFile",
                                            EasyServer.GetConfValue<string>("Lobby", "FileServer", "Key"),
                                            EasyServer.GetConfValue<string>("Lobby", "FileServer", "Ip"),
                                            EasyServer.GetConfValue<int>("Lobby", "FileServer", "Port")))
                return;

            // Initialisation du Client Rpc pour le CharacterServer
            if (!EasyServer.InitRpcClient(  "LobbyCharacter",
                                            EasyServer.GetConfValue<string>("Lobby", "CharacterServer", "Key"),
                                            EasyServer.GetConfValue<string>("Lobby", "CharacterServer", "Ip"),
                                            EasyServer.GetConfValue<int>("Lobby", "CharacterServer", "Port")))
                return;

            // Initialisation du TcpManager pour les Clients
            if (!EasyServer.Listen<TcpServer>(EasyServer.GetConfValue<int>("Lobby", "ClientServer", "Port"), "ClientServer"))
                return;

            CharMgr = new CharacterMgr();
            FileMgr = new FileManager();

            DBAccount Acct = CharMgr.GetAccount(1);

            string[] sVersion = EasyServer.GetConfValue<string>("Lobby", "ClientServer", "Version").Split('.');
            Build = EasyServer.GetConfValue<int>("Lobby","ClientServer","Build");

            Version = new byte[sVersion.Length];

            for (int i = 0; i < Version.Length; ++i)
                Version[i] = byte.Parse(sVersion[i]);

            Log.Info("LobbyServer", "Version = " + Version[0] + "." + Version[1] + "." + Version[2]+" : Build = "+Build);


            Log.Succes("LobbyServer", "Initialisation du serveur terminée.");

            EasyServer.StartConsole();
        }
    }
}
