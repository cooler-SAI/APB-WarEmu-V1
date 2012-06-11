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
using System.Reflection;

using FrameWork;

using Common;

namespace WorldServer
{
    class Program
    {
        static public CharacterMgr CharMgr;
        static public FileManager FileMgr;

        static public WorldServerConfig Config;

        static public RpcClient FileServerClient;
        static public RpcClient CharacterServerClient;

        static void Main(string[] args)
        {
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);
            Log.Texte("", "          _____   _____ ", ConsoleColor.Cyan);
            Log.Texte("", "    /\\   |  __ \\ / ____|", ConsoleColor.Cyan);
            Log.Texte("", "   /  \\  | |__) | (___  ", ConsoleColor.Cyan);
            Log.Texte("", "  / /\\ \\ |  ___/ \\___ \\ ", ConsoleColor.Cyan);
            Log.Texte("", " / ____ \\| |     ____) |", ConsoleColor.Cyan);
            Log.Texte("", "/_/    \\_\\_|    |_____/ APB-World", ConsoleColor.Cyan);
            Log.Texte("", "http://AllPrivateServer.com", ConsoleColor.DarkCyan);
            Log.Texte("", "-------------------------------", ConsoleColor.DarkBlue);

            Assembly.Load("Common");

            Log.Info("WorldServer", "Starting ...");

            ConfigMgr.LoadConfigs();
            Config = ConfigMgr.GetConfig<WorldServerConfig>();

            if (!Log.InitLog(Config.LogLevel, "WorldServer"))
                ConsoleMgr.WaitAndExit(2000);

            FileServerClient = new RpcClient("WorldServer-File-"+Config.WorldID, Config.FileServerRpc.RpcLocalIp, 0);
            if (!FileServerClient.Start(Config.FileServerRpc.RpcServerIp, Config.FileServerRpc.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);

            CharacterServerClient = new RpcClient("WorldServer-Char-" + Config.WorldID, Config.CharacterServerRpc.RpcLocalIp, 0);
            if (!CharacterServerClient.Start(Config.CharacterServerRpc.RpcServerIp, Config.CharacterServerRpc.RpcServerPort))
                ConsoleMgr.WaitAndExit(2000);

            if (!TCPManager.Listen<TcpServer>(Config.WorldServerPort, "World"))
                ConsoleMgr.WaitAndExit(2000);

            CharMgr = CharacterServerClient.GetServerObject<CharacterMgr>();
            FileMgr = FileServerClient.GetServerObject<FileManager>();

            CharMgr.RegisterWorld(CharacterServerClient.Info.RpcID,
                                    Config.WorldID,
                                    Config.WorldName,
                                    Config.WorldServerPort,
                                    Config.WorldServerIp);


            ConsoleMgr.Start();
        }
    }
}
