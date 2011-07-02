using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;
using Common;


namespace MapServer
{
    class Program
    {
        static public MapConfig Config;
        static public RpcClient Client;

        static public Realm Rm = null;

        static public AccountMgr Accounts
        {
            get
            {
                return Characters.GetAccounts();
            }
        }
        static public CharactersMgr Characters
        {
            get
            {
                return Rm.GetObject<CharactersMgr>();
            }
        }
        static public WorldMgr World
        {
            get
            {
                return Rm.GetObject<WorldMgr>();
            }
        }
        static public MapMgr Maps
        {
            get
            {
                return Client.GetLocalObject<MapMgr>();
            }
        }

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
            Config = ConfigMgr.GetConfig<MapConfig>();

            // Loading log level from file
            if (!Log.InitLog(Config.LogLevel, "Map"))
                ConsoleMgr.WaitAndExit(2000);

            // Starting Remote Client
            Client = new RpcClient("Map-" + Config.ServerInfo.MapAdress, Config.ClientInfo.RpcLocalIp, 2);
            if (!Client.Start(Config.ClientInfo.RpcServerIp, Config.ClientInfo.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);

            // Listening Client
            if (!TCPManager.Listen<RiftServer>(Config.ServerInfo.MapPort, "CharacterServer"))
                ConsoleMgr.WaitAndExit(2000);

            MapMgr.Client = Client;
            MapMgr.MapInfo = Config.ServerInfo;
            MapMgr.MapInfo.RpcInfo = Client.Info;
            Rm = Client.GetServerObject<CharactersMgr>().GetRealm();

            Log.Success("Realm","Connected to : " + Rm.Name);

            World.RegisterMaps(Config.ServerInfo, Client.Info);

            ConsoleMgr.Start();
        }
    }
}
