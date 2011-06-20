using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;
using Common;

namespace CharacterServer
{
    class Program
    {
        static public CharacterConfig Config;
        static public RpcServer Server;

        static public AccountMgr AcctMgr;

        [STAThread()]
        static void Main(string[] args)
        {
            Log.Texte("", "-------------------------------",    ConsoleColor.DarkBlue);
            Log.Texte("", "          _____   _____ ",           ConsoleColor.Cyan);
            Log.Texte("", "    /\\   |  __ \\ / ____|",         ConsoleColor.Cyan);
            Log.Texte("", "   /  \\  | |__) | (___  ",          ConsoleColor.Cyan);
            Log.Texte("", "  / /\\ \\ |  ___/ \\___ \\ ",       ConsoleColor.Cyan);
            Log.Texte("", " / ____ \\| |     ____) |",          ConsoleColor.Cyan);
            Log.Texte("", "/_/    \\_\\_|    |_____/ Rift",     ConsoleColor.Cyan);
            Log.Texte("", "http://AllPrivateServer.com",        ConsoleColor.DarkCyan);
            Log.Texte("", "-------------------------------",    ConsoleColor.DarkBlue);

            // Loading all configs files
            ConfigMgr.LoadConfigs();
            Config = ConfigMgr.GetConfig<CharacterConfig>();

            // Loading log level from file
            if (!Log.InitLog(Config.LogLevel,"Character"))
                ConsoleMgr.WaitAndExit(2000);

            AccountMgr.AccountDB = DBManager.Start(Config.AccountDB.Total(), ConnectionType.DATABASE_MYSQL, "Accounts");
            if (AccountMgr.AccountDB == null)
                ConsoleMgr.WaitAndExit(2000);

            // Starting Remote Server
            Server = new RpcServer(Config.RpcClientStartingPort, 1);
            if (!Server.Start(Config.RpcIP, Config.RpcPort))
                ConsoleMgr.WaitAndExit(2000);

            // Starting Accounts Manager
            AcctMgr = Server.GetLocalObject<AccountMgr>();
            if(AcctMgr == null)
                ConsoleMgr.WaitAndExit(2000);

            AcctMgr.LoadRealms();

            // Listening Client
            if (!TCPManager.Listen<RiftServer>(Config.CharacterServerPort, "CharacterServer"))
                ConsoleMgr.WaitAndExit(2000);

            ConsoleMgr.Start();
        }
    }
}
