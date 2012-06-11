/*
 * Copyright (C) 2011 APS
 *	http://AllPrivateServer.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

using Common;

namespace LobbyServer
{
    class Program
    {
        static public byte[] Version;
        static public int Build = 0;

        static public CharacterMgr CharMgr;
        static public FileManager FileMgr;

        static public LobbyServerConfig Config;
        static public RpcClient FileServerClient;
        static public RpcClient CharacterServerClient;

        [STAThread]
        static void Main(string[] args)
        {
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);
            Log.Texte("", "          _____   _____ ", ConsoleColor.Cyan);
            Log.Texte("", "    /\\   |  __ \\ / ____|", ConsoleColor.Cyan);
            Log.Texte("", "   /  \\  | |__) | (___  ", ConsoleColor.Cyan);
            Log.Texte("", "  / /\\ \\ |  ___/ \\___ \\ ", ConsoleColor.Cyan);
            Log.Texte("", " / ____ \\| |     ____) |", ConsoleColor.Cyan);
            Log.Texte("", "/_/    \\_\\_|    |_____/ APB-Lobby", ConsoleColor.Cyan);
            Log.Texte("", "http://AllPrivateServer.com", ConsoleColor.DarkCyan);
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);

            Log.Info("LobbyServer", "Starting ...");

            ConfigMgr.LoadConfigs();
            Config = ConfigMgr.GetConfig<LobbyServerConfig>();

            if (!Log.InitLog(Config.LogLevel, "LobbyServer"))
                ConsoleMgr.WaitAndExit(2000);

            FileServerClient = new RpcClient("LobbyServer-File", Config.FileServerRpc.RpcLocalIp, 1);
            if (!FileServerClient.Start(Config.FileServerRpc.RpcServerIp, Config.FileServerRpc.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);

            CharacterServerClient = new RpcClient("LobbyServer-Char", Config.CharacterServerRpc.RpcLocalIp, 0);
            if (!CharacterServerClient.Start(Config.CharacterServerRpc.RpcServerIp, Config.CharacterServerRpc.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);

            if (!TCPManager.Listen<TcpServer>(Config.ClientServerPort, "Lobby"))
                ConsoleMgr.WaitAndExit(2000);

            CharMgr = CharacterServerClient.GetServerObject<CharacterMgr>();
            FileMgr = FileServerClient.GetServerObject<FileManager>();

            DBAccount Acct = CharMgr.GetAccount(1);

            string[] sVersion = Config.ClientVersion.Split('.');
            Build = Config.ClientBuild;

            Version = new byte[sVersion.Length];

            for (int i = 0; i < Version.Length; ++i)
                Version[i] = byte.Parse(sVersion[i]);

            Log.Info("LobbyServer", "Version = " + Version[0] + "." + Version[1] + "." + Version[2]+" : Build = "+Build);


            Log.Success("LobbyServer", "Server loaded.");

            ConsoleMgr.Start();
        }
    }
}
